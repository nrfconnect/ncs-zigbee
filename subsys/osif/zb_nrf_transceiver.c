/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include <nrf_802154.h>
#include <nrf_802154_const.h>
#include <nrf_802154_types.h>
#include <zboss_api.h>
#include <zb_macll.h>
#include <zb_transceiver.h>
#include "zb_nrf_platform.h"

#if !defined NRF_802154_FRAME_TIMESTAMP_ENABLED || \
	!NRF_802154_FRAME_TIMESTAMP_ENABLED
#warning Must define NRF_802154_FRAME_TIMESTAMP_ENABLED!
#endif

LOG_MODULE_DECLARE(zboss_osif, CONFIG_ZBOSS_OSIF_LOG_LEVEL);

enum zb_radio_state {
	ZB_RADIO_STATE_SLEEP,
	ZB_RADIO_STATE_RECEIVE,
	ZB_RADIO_STATE_TRANSMIT,
};

struct zboss_rx_frame {
	void *fifo_reserved;
	uint8_t *psdu;
	int8_t power;
	uint8_t lqi;
	uint64_t time;
	bool ack_fpb;
};

struct nrf5_data {
	enum zb_radio_state state;
	
	int8_t tx_power;
	uint8_t channel;
	bool promiscuous;

	struct {
		struct zboss_rx_frame frames[CONFIG_NRF_802154_RX_BUFFERS + 1];
		struct k_fifo fifo;
		bool last_frame_ack_fpb;
	} rx;

	struct {
		uint8_t *psdu;
	} tx;

	struct {
		uint32_t time_us;
		int8_t value;
	} energy_detection;
	
	struct k_sem rssi_wait;
};

static struct nrf5_data nrf5_data;

static int nrf_802154_radio_init(void)
{
	k_fifo_init(&nrf5_data.rx.fifo);
	k_sem_init(&nrf5_data.rssi_wait, 0, 1);
	
	nrf_802154_init();
	
	nrf5_data.state = ZB_RADIO_STATE_SLEEP;

	LOG_INF("802.15.4 radio driver initialized");
	
	return 0;
}

SYS_INIT(nrf_802154_radio_init, POST_KERNEL, CONFIG_ZBOSS_RADIO_INIT_PRIORITY);

void zb_trans_hw_init(void)
{
	LOG_INF(">>> zb_trans_hw_init() - configuring driver PIBs from ZBOSS thread");
	
	nrf_802154_auto_ack_set(true);
	nrf_802154_src_addr_matching_method_set(NRF_802154_SRC_ADDR_MATCH_ZIGBEE);
	
	LOG_INF(">>> zb_trans_hw_init() DONE");
}

void zb_trans_set_pan_id(zb_uint16_t pan_id)
{
	LOG_DBG("%s: 0x%x", __func__, pan_id);
	nrf_802154_pan_id_set((zb_uint8_t *)(&pan_id));
}

void zb_trans_set_long_addr(zb_ieee_addr_t long_addr)
{
	LOG_DBG("%s: 0x%llx", __func__, (uint64_t)*long_addr);
	nrf_802154_extended_address_set(long_addr);
}

void zb_trans_set_short_addr(zb_uint16_t addr)
{
	LOG_DBG("%s: 0x%x", __func__, addr);
	nrf_802154_short_address_set((uint8_t *)(&addr));
}

static int zboss_energy_detection_start(uint32_t time_us)
{
	nrf5_data.energy_detection.time_us = time_us;
	
	if (!nrf_802154_energy_detection(time_us)) {
		return -EBUSY;
	}
	
	return 0;
}

void zb_trans_start_get_rssi(zb_uint8_t scan_duration_bi)
{
	int err;
	uint32_t time_us = ZB_TIME_BEACON_INTERVAL_TO_USEC(scan_duration_bi);

	LOG_DBG("%s: %d us", __func__, time_us);

	err = zboss_energy_detection_start(time_us);
	
	while (err != 0) {
		LOG_DBG("Energy detection start failed, retrying");
		k_usleep(500);
		err = zboss_energy_detection_start(time_us);
	}
}

void zb_trans_get_rssi(zb_uint8_t *rssi_value_p)
{
	LOG_DBG("%s", __func__);

	/* Blocking implementation: wait for energy detection to complete.
	 * The semaphore is signaled by nrf_802154_energy_detected() callback
	 * or by nrf_802154_energy_detection_failed() after retry attempt.
	 */
	k_sem_take(&nrf5_data.rssi_wait, K_FOREVER);
	*rssi_value_p = (uint8_t)nrf5_data.energy_detection.value;
	LOG_DBG("Energy detected: %d", *rssi_value_p);
}

zb_ret_t zb_trans_set_channel(zb_uint8_t channel_number)
{
	LOG_DBG("%s: %d", __func__, channel_number);
	nrf_802154_channel_set(channel_number);
	return RET_OK;
}

void zb_trans_set_tx_power(zb_int8_t power)
{
	LOG_DBG("%s: %d", __func__, power);
	nrf_802154_tx_power_set(power);
}

void zb_trans_get_tx_power(zb_int8_t *power)
{
	*power = (zb_int8_t)nrf_802154_tx_power_get();
	LOG_DBG("%s: %d", __func__, *power);
}

void zb_trans_set_pan_coord(zb_bool_t enabled)
{
	LOG_DBG("%s: %d", __func__, enabled);
	nrf_802154_pan_coord_set((bool)enabled);
}

void zb_trans_set_auto_ack(zb_bool_t enabled)
{
	LOG_DBG("%s: %d", __func__, enabled);
	nrf_802154_auto_ack_set((bool)enabled);
}

void zb_trans_set_promiscuous_mode(zb_bool_t enabled)
{
	LOG_DBG("%s: %d", __func__, enabled);
	nrf_802154_promiscuous_set((bool)enabled);
}

void zb_trans_enter_receive(void)
{
	LOG_DBG("%s", __func__);
	(void)nrf_802154_receive();
	nrf5_data.state = ZB_RADIO_STATE_RECEIVE;
}

void zb_trans_enter_sleep(void)
{
	LOG_DBG("%s", __func__);
	(void)nrf_802154_sleep_if_idle();
	nrf5_data.state = ZB_RADIO_STATE_SLEEP;
}

zb_bool_t zb_trans_is_receiving(void)
{
	zb_bool_t is_receiv = (nrf5_data.state == ZB_RADIO_STATE_RECEIVE) ? ZB_TRUE : ZB_FALSE;
	LOG_DBG("%s: %d", __func__, is_receiv);
	return is_receiv;
}

zb_bool_t zb_trans_is_active(void)
{
	zb_bool_t is_active = (nrf5_data.state != ZB_RADIO_STATE_SLEEP) ? ZB_TRUE : ZB_FALSE;
	LOG_DBG("%s: %d", __func__, is_active);
	return is_active;
}

zb_bool_t zb_trans_transmit(zb_uint8_t wait_type, zb_time_t tx_at,
			    zb_uint8_t *tx_buf, zb_uint8_t current_channel)
{
	LOG_DBG("%s: channel %d", __func__, current_channel);
	nrf_802154_tx_error_t result;
	nrf_802154_capabilities_t caps = nrf_802154_capabilities_get();

#ifndef ZB_ENABLE_ZGP_DIRECT
	ARG_UNUSED(tx_at);
	ARG_UNUSED(current_channel);
#endif

	nrf5_data.state = ZB_RADIO_STATE_TRANSMIT;

	switch (wait_type) {
	case ZB_MAC_TX_WAIT_CSMACA:
		if (caps & NRF_802154_CAPABILITY_CSMA) {
			nrf_802154_transmit_csma_ca_metadata_t csma_metadata = {
				.frame_props = {
					.is_secured = false,
					.dynamic_data_is_set = false,
				},
			};
			result = nrf_802154_transmit_csma_ca_raw(tx_buf, &csma_metadata);
		} else {
			nrf_802154_transmit_metadata_t cca_metadata = {
				.frame_props = {
					.is_secured = false,
					.dynamic_data_is_set = false,
				},
				.cca = true,
			};
			result = nrf_802154_transmit_raw(tx_buf, &cca_metadata);
		}
		break;

#ifdef ZB_ENABLE_ZGP_DIRECT
	case ZB_MAC_TX_WAIT_ZGP:
		if (!(caps & NRF_802154_CAPABILITY_DELAYED_TX)) {
			LOG_ERR("NRF_802154_CAPABILITY_DELAYED_TX not supported");
			nrf5_data.state = ZB_RADIO_STATE_RECEIVE;
			return ZB_FALSE;
		}
		nrf_802154_transmit_at_metadata_t at_metadata = {
			.frame_props = {
				.is_secured = false,
				.dynamic_data_is_set = false,
			},
			.cca = true,
		};
		result = nrf_802154_transmit_raw_at(tx_buf, tx_at, &at_metadata);
		break;
#endif

	case ZB_MAC_TX_WAIT_NONE:
	{
		nrf_802154_transmit_metadata_t tx_metadata = {
			.frame_props = {
				.is_secured = false,
				.dynamic_data_is_set = false,
			},
			.cca = false,
		};
		result = nrf_802154_transmit_raw(tx_buf, &tx_metadata);
		break;
	}

	default:
		LOG_ERR("Invalid wait_type: %d", wait_type);
		ZB_ASSERT(0);
		nrf5_data.state = ZB_RADIO_STATE_RECEIVE;
		return ZB_FALSE;
	}

	return (result == NRF_802154_TX_ERROR_NONE) ? ZB_TRUE : ZB_FALSE;
}

void zb_trans_buffer_free(zb_uint8_t *buf)
{
	LOG_DBG("%s", __func__);
	nrf_802154_buffer_free_raw(buf);
}

zb_bool_t zb_trans_set_pending_bit(zb_uint8_t *addr, zb_bool_t value, zb_bool_t extended)
{
	LOG_DBG("%s: value=%d", __func__, value);

	if (!value) {
		return (zb_bool_t)nrf_802154_pending_bit_for_addr_set(
			(const uint8_t *)addr, (bool)extended);
	} else {
		return (zb_bool_t)nrf_802154_pending_bit_for_addr_clear(
			(const uint8_t *)addr, (bool)extended);
	}
}

void zb_trans_src_match_tbl_drop(void)
{
	LOG_DBG("%s", __func__);
	nrf_802154_pending_bit_for_addr_reset(false);
	nrf_802154_pending_bit_for_addr_reset(true);
}

zb_time_t osif_sub_trans_timer(zb_time_t t2, zb_time_t t1)
{
	return ZB_TIME_SUBTRACT(t2, t1);
}

zb_bool_t zb_trans_rx_pending(void)
{
	return k_fifo_is_empty(&nrf5_data.rx.fifo) ? ZB_FALSE : ZB_TRUE;
}

zb_uint8_t zb_trans_get_next_packet(zb_bufid_t buf)
{
	LOG_DBG("%s", __func__);
	zb_uint8_t *data_ptr;
	zb_uint8_t length = 0;

	if (!buf) {
		return 0;
	}

	struct zboss_rx_frame *rx_frame = k_fifo_get(&nrf5_data.rx.fifo, K_NO_WAIT);
	if (!rx_frame) {
		return 0;
	}

	length = rx_frame->psdu[0];
	data_ptr = zb_buf_initial_alloc(buf, length);
	ZB_MEMCPY(data_ptr, (void const *)(rx_frame->psdu + 1), length);

	zb_macll_metadata_t *metadata = ZB_MACLL_GET_METADATA(buf);
	metadata->lqi = rx_frame->lqi;
	metadata->power = rx_frame->power;
	
	*ZB_BUF_GET_PARAM(buf, zb_time_t) = (zb_time_t)rx_frame->time;
	zb_macll_set_received_data_status(buf, rx_frame->ack_fpb);

	nrf_802154_buffer_free_raw(rx_frame->psdu);
	rx_frame->psdu = NULL;

	return 1;
}

zb_ret_t zb_trans_cca(void)
{
	bool cca_result = nrf_802154_cca();
	return cca_result ? RET_OK : RET_BUSY;
}

/* nRF 802.15.4 driver callbacks - modern API with metadata structures */

void nrf_802154_transmitted_raw(uint8_t *p_frame,
				const nrf_802154_transmit_done_metadata_t *p_metadata)
{
	ARG_UNUSED(p_frame);

	uint8_t *ack = p_metadata->data.transmitted.p_ack;
	zb_macll_transmitted_raw(ack);

	nrf5_data.state = ZB_RADIO_STATE_RECEIVE;
	zigbee_event_notify(ZIGBEE_EVENT_TX_DONE);
}

void nrf_802154_transmit_failed(uint8_t *p_frame,
				nrf_802154_tx_error_t error,
				const nrf_802154_transmit_done_metadata_t *p_metadata)
{
	ARG_UNUSED(p_frame);
	ARG_UNUSED(p_metadata);

	switch (error) {
	case NRF_802154_TX_ERROR_NO_MEM:
	case NRF_802154_TX_ERROR_ABORTED:
	case NRF_802154_TX_ERROR_TIMESLOT_DENIED:
	case NRF_802154_TX_ERROR_TIMESLOT_ENDED:
	case NRF_802154_TX_ERROR_BUSY_CHANNEL:
		zb_macll_transmit_failed(ZB_TRANS_CHANNEL_BUSY_ERROR);
		break;

	case NRF_802154_TX_ERROR_INVALID_ACK:
	case NRF_802154_TX_ERROR_NO_ACK:
		zb_macll_transmit_failed(ZB_TRANS_NO_ACK);
		break;
	default:
		break;
	}

	nrf5_data.state = ZB_RADIO_STATE_RECEIVE;
	zigbee_event_notify(ZIGBEE_EVENT_TX_FAILED);
}

void nrf_802154_tx_ack_started(const uint8_t *p_data)
{
	nrf5_data.rx.last_frame_ack_fpb = p_data[FRAME_PENDING_OFFSET] & FRAME_PENDING_BIT;
}

void nrf_802154_received_timestamp_raw(uint8_t *p_data, int8_t power,
				       uint8_t lqi, uint64_t time)
{
	struct zboss_rx_frame *rx_frame_free_slot = NULL;

	for (uint32_t i = 0; i < ARRAY_SIZE(nrf5_data.rx.frames); i++) {
		if (nrf5_data.rx.frames[i].psdu == NULL) {
			rx_frame_free_slot = &nrf5_data.rx.frames[i];
			break;
		}
	}

	if (rx_frame_free_slot == NULL) {
		__ASSERT(false, "Not enough rx frames allocated");
		return;
	}

	rx_frame_free_slot->psdu = p_data;
	rx_frame_free_slot->power = power;
	rx_frame_free_slot->lqi = lqi;
	rx_frame_free_slot->time = time;

	if (p_data[ACK_REQUEST_OFFSET] & ACK_REQUEST_BIT) {
		rx_frame_free_slot->ack_fpb = nrf5_data.rx.last_frame_ack_fpb;
	} else {
		rx_frame_free_slot->ack_fpb = false;
	}

	nrf5_data.rx.last_frame_ack_fpb = false;
	k_fifo_put(&nrf5_data.rx.fifo, rx_frame_free_slot);

	zb_macll_set_rx_flag();
	zb_macll_set_trans_int();
	zigbee_event_notify(ZIGBEE_EVENT_RX_DONE);
}

void nrf_802154_receive_failed(nrf_802154_rx_error_t error, uint32_t id)
{
	ARG_UNUSED(id);
	ARG_UNUSED(error);
	nrf5_data.rx.last_frame_ack_fpb = false;
}

void nrf_802154_energy_detected(const nrf_802154_energy_detected_t *p_result)
{
	nrf5_data.energy_detection.value = p_result->ed_dbm;
	k_sem_give(&nrf5_data.rssi_wait);
}

void nrf_802154_energy_detection_failed(nrf_802154_ed_error_t error)
{
	ARG_UNUSED(error);
	
	int err = zboss_energy_detection_start(nrf5_data.energy_detection.time_us);
	
	if (err != 0) {
		LOG_ERR("Failed to restart energy detection after failure");
		nrf5_data.energy_detection.value = INT8_MAX;
		k_sem_give(&nrf5_data.rssi_wait);
	}
}
