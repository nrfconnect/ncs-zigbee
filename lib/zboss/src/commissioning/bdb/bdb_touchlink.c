/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2026 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*  PURPOSE: Touchlink commissioning for BDB. Reuse some ZLL stuff.
*/

#define ZB_TRACE_FILE_ID 2057
#include "zb_common.h"
#include "zb_zdo.h"
#include "zb_aps.h"
#include "zb_nwk.h"
#include "zb_secur.h"
#include "zb_bdb_internal.h"
#include "zll/zb_zll_nwk_features.h"
#include "zll/zb_zll_sas.h"
#include "zll/zll_commissioning_internals.h"
#include "zboss_api.h"

#if defined ZB_BDB_MODE && defined ZB_BDB_TOUCHLINK && !defined ZB_COORDINATOR_ONLY && defined ZB_DISTRIBUTED_SECURITY_ON

/** @internal @brief Schedules for sending next device information request.
  * @param param reference to the buffer to put packet to.
  */
void zll_send_next_devinfo_req(zb_uint8_t param);
void zll_dev_start_continue_fn(zb_uint8_t param);
#ifdef ZB_ROUTER_ROLE
static void bdb_tl_target_dev_start_continue_fn(zb_uint8_t param);
static void bdb_tl_target_dev_start_continue_nfn(zb_uint8_t param);
#endif

static void bdb_touchlink_scan(zb_uint8_t param);
static void bdb_touchlink_send_scan(zb_uint8_t param);
static void bdb_touchlink_scan_req_sent(zb_uint8_t param);
static zb_int_t bdb_touchlink_next_channel(void);

/**
   Initiate (enable) Touchlink target.

   To be run after zb_zdo_start_no_autostart() and receiving
   ZB_ZDO_SIGNAL_SKIP_STARTUP, or after formation/join.

   Note that Touchlink Target start can't be combined with normal BDB
   Initialization procedure.
 */
void bdb_touchlink_target_start(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZLL1, "> bdb_touchlink_target_start param %hd", (FMT__H, param));
  /* 8.8 Touchlink procedure for a target

     As this procedure is followed as a response to touchlink requests from an
     initiator, it is not instigated via the top-level commissioning procedure.
  */

  /* Note that in ZLL ZED is definitely an Initiator and ZR is always Target.
     But in BDB ZED can be a Target:
     "
     If the target is a sleeping Zigbee end device it SHALL first need to be woken up
     by some application means so that it can enable its receiver and
     respond to the scan from the initiator.
     "
  */
  if (ZB_BDB().bdb_commissioning_step == ZB_BDB_TOUCHLINK_TARGET
      && ZB_BDB().bdb_commissioning_status == ZB_BDB_STATUS_IN_PROGRESS)
  {
    TRACE_MSG(TRACE_ZDO1, "Touchlink Target started, zll task %d.", (FMT__D, ZLL_TRAN_CTX().transaction_task));
    if (ZLL_TRAN_CTX().transaction_task == ZB_ZLL_TRANSACTION_NWK_START_TASK_TGT)
    {
      ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_TOUCHLINK_NWK;
    }
    else
    {
      ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_TOUCHLINK_TARGET;
    }
    zb_app_signal_pack(param, ZB_BDB().bdb_application_signal,
                      ZB_BDB_STATUS_SUCCESS,
                      0);
    ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
  }
  else if (ZB_BDB().bdb_commissioning_step == ZB_BDB_TOUCHLINK_TARGET
           && ZB_BDB().bdb_commissioning_status == ZB_BDB_STATUS_NO_NETWORK)
  {
    /* We are here after nwk discovery when starting network, from
       zb_nlme_network_discovery_confirm */
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    ZB_IEEE_ADDR_COPY(ZB_AIB().trust_center_address, &g_unknown_ieee_addr);
    TRACE_MSG(TRACE_ZLL1, "Continue zll network start", (FMT__0));
    ZB_SCHEDULE_CALLBACK(zll_network_start_continue, param);
  }
  else
  {
    TRACE_MSG(TRACE_ZDO1, "Start Touchlink Target", (FMT__0));

    bdb_check_fn();

    ZB_BDB().bdb_commissioning_step = ZB_BDB_TOUCHLINK_TARGET;
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    if (ZB_JOINED())
    {
      TRACE_MSG(TRACE_ZLL1, "Starting Touchlink target when already joined - do nothing", (FMT__0));
      ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param);
    }
#if defined ZB_ROUTER_ROLE
    else
    {
      if (ZB_NIB_SECURITY_LEVEL() && ZB_IS_DEVICE_ZR())
      {
        /* forming distributed network */
        TRACE_MSG(TRACE_ZLL1, "Starting ZLL target at ZR in Distributed security network", (FMT__0));
        zb_bdb_enable_distributed_network_formation();
        zb_sync_distributed();
        secur_tc_init();
      }
      if (ZB_ZLL_IS_FACTORY_NEW())
      {
        TRACE_MSG(TRACE_ZLL1, "Starting Factory New Touchlink target", (FMT__0));
        ZDO_CTX().continue_start_after_nwk_cb = bdb_tl_target_dev_start_continue_fn;
      }
      else
      {
        TRACE_MSG(TRACE_ZLL1, "Starting Not Factory New Touchlink target", (FMT__0));
        ZDO_CTX().continue_start_after_nwk_cb = bdb_tl_target_dev_start_continue_nfn;
      }

      zb_buf_free(param);
      ZB_SCHEDULE_CALLBACK(zb_zdo_dev_start_cont, 0);
    }
#endif
  }

  TRACE_MSG(TRACE_ZLL1, "< bdb_touchlink_target_start", (FMT__0));
}


#ifdef ZB_ROUTER_ROLE
static void bdb_tl_target_dev_start_continue_fn(zb_uint8_t param)
{
  zb_uint16_t panid;
  TRACE_MSG(TRACE_ZLL1, "bdb_tl_target_dev_start_continue_fn param %hd", (FMT__H, param));
  ZB_PIBCACHE_NETWORK_ADDRESS() = ZB_ZLL_SAS_SHORT_ADDRESS;

  /* AT:
     7.1.2.3.1 Scan response command frame
     The scan response command frame is used to respond to the originator of a scan request
     command frame with device details.
     ....
     PAN ID field shall be set to any value in the range 0x0001 - 0xfffe,
     if the device is factory new, or the PAN identifier of the device, otherwise.
  */
  panid = ZLL_SRC_PANID_FIRST_VALUE + ZB_RANDOM_VALUE(ZLL_SRC_PANID_LAST_VALUE - ZLL_SRC_PANID_FIRST_VALUE);
  zb_nwk_pib_set(param, ZB_NWK_MULTIMAC_ALL_INTERFACES, ZB_PIB_ATTRIBUTE_PANID, &panid, sizeof(panid), zll_dev_start_continue_fn);
}


static void bdb_tl_target_dev_start_continue_nfn(zb_uint8_t param)
{
  zb_ret_t status = RET_OK;

  ZVUNUSED(status);

  TRACE_MSG(TRACE_ZLL1, "bdb_tl_target_dev_start_continue_nfn param %hd", (FMT__H, param));

  /* TODO: [Multi-MAC] review this logic. It is valid? */

  TRACE_MSG(TRACE_ZLL2, "Starting ZLL NFN device. channel %hd, ext_panid " TRACE_FORMAT_64,
            (FMT__H_A,
             ZB_PIBCACHE_CURRENT_CHANNEL(), TRACE_ARG_64(ZB_NIB_EXT_PAN_ID())));

  ZLL_DEVICE_INFO().freqagility_channel = ZB_PIBCACHE_CURRENT_CHANNEL();

  /* if device is address assignable, modify start/end addr, start/end group id */
#ifndef ZB_ED_ROLE
  if (ZB_IS_DEVICE_ZED())
#endif
  {
    zb_channel_list_t channel_list;
    zb_channel_list_init(channel_list);
    zb_channel_page_list_set_2_4GHz_mask(channel_list, (1u << ZB_PIBCACHE_CURRENT_CHANNEL()));
    zdo_initiate_rejoin(param, ZB_NIB_EXT_PAN_ID(),
                        channel_list,
                        ZG->aps.authenticated,
                        bdb_get_scan_duration());
  }
#ifndef ZB_ED_ROLE
  else
  {
    ZB_SET_JOINED_STATUS(ZB_TRUE);            /* hmm */
    ZB_ZLL_START_ROUTER(param, ZB_NIB_EXT_PAN_ID(), ZB_PIBCACHE_PAN_ID(),
                        ZB_PIBCACHE_CURRENT_CHANNEL(), ZB_PIBCACHE_NETWORK_ADDRESS(), status);
  }
#endif
}
#endif  /* ZB_ROUTER_ROLE */


/* BDB Touchlink Initiator logic */

#if defined ZB_ZLL_ENABLE_COMMISSIONING_CLIENT

void bdb_touchlink_initiator_cont(zb_uint8_t param);
/**
   Start BDB touchlink (AKA ZLL) commissioning
 */
void bdb_touchlink_initiator(zb_uint8_t param)
{
  zb_uint16_t panid;
  TRACE_MSG(TRACE_ZLL1, "bdb_touchlink_initiator param %hd", (FMT__H, param));
  /* 7.1.2.2.1.3  ZLL information field
     The link initiator subfield is 1 bit in length and specifies whether the device is capable of
     initiating a link operation.  The value of this subfield shall be set to 1 to indicate the
     device is capable of initiating a link (i.e. it supports the ZLL commissioning cluster at the
     client side) or 0 otherwise (i.e. it does not support the ZLL commissioning cluster at the
     client side.
   */
  ZLL_DEVICE_INFO().zll_info |= ZB_ZLL_INFO_TOUCHLINK_INITIATOR;

  /* 7.1.2.2.1 Scan request command frame
     The source PAN ID field shall be set to any value in the range 0x0001-0xfffe, if the device is
     factory new, or the PAN identifier of the device, otherwise
  */
  if (ZB_ZLL_IS_FACTORY_NEW())
  {
    panid = ZLL_SRC_PANID_FIRST_VALUE + ZB_RANDOM_VALUE(ZLL_SRC_PANID_LAST_VALUE - ZLL_SRC_PANID_FIRST_VALUE);
    zb_nwk_pib_set(param, ZB_NWK_MULTIMAC_ALL_INTERFACES, ZB_PIB_ATTRIBUTE_PANID, &panid,
      sizeof(panid), bdb_touchlink_initiator_cont);
  }
  else
  {
    ZB_SCHEDULE_CALLBACK(bdb_touchlink_initiator_cont, param);
  }
}

void bdb_touchlink_initiator_cont(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZLL1, "> bdb_touchlink_initiator_cont param %hd", (FMT__H, param));
  /* Touchlink commissioning finishes with either success or ZB_BDB_STATUS_NO_SCAN_RESPONSE */

  /* See BDB 8.7 Touchlink procedure for an initiator */
  /* 1. The initiator first sets bdbCommissioningStatus to IN_PROGRESS */

  switch (ZB_BDB().v_do_primary_scan)
  {
    case ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN:
      if (ZB_NIB_SECURITY_LEVEL())
      {
#ifdef ZB_ROUTER_ROLE
        if (secur_nwk_key_is_empty(ZB_NIB().secur_material_set[0].key))
        {
          TRACE_MSG(TRACE_SECUR1, "Genetating NWK keys", (FMT__0));
          secur_generate_key(ZB_NIB().secur_material_set[0].key, ZB_CCM_KEY_SIZE);
        }
#endif
        /* We are not joined yet, do not set authenticated flag. Set this flag in
         * zll_network_start_res_handler_continue() instead. */
        /* ZG->aps.authenticated = ZB_TRUE; */
      }
      ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
      /* if v_is_first_channel, switch to the first channel defined by
       * vScanChannels and broadcast five consecutive touchlink commissioning
       * cluster scan request inter-PAN command frames */
      ZB_BDB().tl_first_channel_rpt = 5;
      ZB_BDB().tl_channel_i = 0;
      ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_START;

      /* Do not use top level ZLL code but copy-paste and use some its internals */
      ZB_MEMSET(&ZLL_TRAN_CTX(), 0, sizeof(ZLL_TRAN_CTX()));
      /* 2. The initiator SHALL generate a 32-bit transaction identifier */
      ZLL_TRAN_CTX().transaction_id = ZB_ZLL_GET_NEW_TRANS_ID();
      ZLL_TRAN_CTX().transaction_task = ZB_ZLL_DEVICE_DISCOVERY_TASK;
      /*
        For a normal channel scan, bdbPrimaryChannelSet and bdbSecondaryChannelSet
        SHALL be set to bdbcTLPrimaryChannelSet and 0x00000000, respectively. For an
        extended channel scan, bdbPrimaryChannelSet and bdbSecondaryChannelSet SHALL be
        set to bdbcTLPrimaryChannelSet and bdbcTLSecondaryChannelSet
      */
      if (ZB_BDB().ignore_aps_channel_mask)
      {
        ZB_BDB().v_scan_channels = ZB_BDBC_TL_PRIMARY_CHANNEL_SET;
      }
      else
      {
        ZB_BDB().v_scan_channels = zb_aib_channel_page_list_get_2_4GHz_mask() & ZB_BDBC_TL_PRIMARY_CHANNEL_SET;
      }
#ifdef ZB_STACK_REGRESSION_TESTING_API
      /* rtp_touchlink_01 */
      TRACE_MSG(TRACE_TEST2, "Initiating primary Touchlink scan channels 0x%x", (FMT__L, ZB_BDB().v_scan_channels));
#else
      TRACE_MSG(TRACE_ZLL2, "Initiating primary Touchlink scan channels 0x%x", (FMT__L, ZB_BDB().v_scan_channels));
#endif
      /* 3. The initiator SHALL perform touchlink device discovery. */

      /* scan request inter-PAN command frames SHALL be broadcast with
       * appropriate values for the Zigbee information and touchlink
       * information fields and with a nominal output power of 0dBm. */


      /* TODO: [Multi-MAC] How a device should organize joining when multiple interfaces are active?
       *   Should it perform scanning on all interfaces and then select the best page/channel pair?
       *   How to choose an active page/channel on the other interfaces in that case? */
      {
        zb_uint8_t page = 0; /* 2.4Ghz */
        zb_nwk_pib_set(param, ZB_PIBCACHE_PRIMARY_IFACE(), ZB_PHY_PIB_CURRENT_PAGE, &page,
                       sizeof(page), bdb_touchlink_scan);
      }
      break;

    case ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_START:
      if (ZB_BDB().bdb_ext_channel_scan)
      {
        if (ZB_BDB().ignore_aps_channel_mask)
        {
          ZB_BDB().v_scan_channels = ZB_BDBC_TL_SECONDARY_CHANNEL_SET;
        }
        else
        {
          ZB_BDB().v_scan_channels = zb_aib_channel_page_list_get_2_4GHz_mask() & ZB_BDBC_TL_SECONDARY_CHANNEL_SET;
        }
#ifdef ZB_STACK_REGRESSION_TESTING_API
        /* rtp_touchlink_01 */
        TRACE_MSG(TRACE_TEST2, "Initiating secondary Touchlink scan channels 0x%x", (FMT__L, ZB_BDB().v_scan_channels));
#else
        TRACE_MSG(TRACE_ZLL2, "Initiating secondary Touchlink scan channels 0x%x", (FMT__L, ZB_BDB().v_scan_channels));
#endif
        ZB_BDB().tl_first_channel_rpt = 1;
        ZB_BDB().tl_channel_i = 0;
        ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_DONE;
        ZB_SCHEDULE_CALLBACK(bdb_touchlink_scan, param);
        break;
      }
      else
      {
        /* FALLTHROUGH */
      }

      /* FALLTHROUGH */
    case ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_DONE:
      TRACE_MSG(TRACE_ZLL2, "Touchlink scan done, found %hd ZLL devices", (FMT__H, ZLL_TRAN_CTX().n_device_infos));
      /* done touchlink scan */

      /* Scan resp is handled by zll_handle_scan_res() */
      if (ZLL_TRAN_CTX().n_device_infos == 0)
      {
        /*
          5. If no touchlink commissioning cluster scan response inter-PAN command
          frames are received or no touchlink commissioning cluster scan response
          inter-PAN command frames are received with the inter-PAN transaction
          identifier field equal to that used by the initiator in its scan request
          command frame, the node sets bdbCommissioningStatus to NO_SCAN_RESPONSE and it
          SHALL terminate the touchlink procedure for an initiator.
        */
        ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_TOUCHLINK;
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_SCAN_RESPONSE;
        ZB_BDB().signal = BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_FAILED;
#ifdef ZB_STACK_REGRESSION_TESTING_API
        /* rtp_touchlink_01 */
        TRACE_MSG(TRACE_TEST1, "Found no ZLL devices, continue BDB commissioning, status NO_SCAN_RESPONSE", (FMT__0));
#else
        TRACE_MSG(TRACE_ZLL1, "Found no ZLL devices, continue BDB commissioning, status NO_SCAN_RESPONSE", (FMT__0));
#endif

        /* Restore original nwk channel. We may be already joined...  */
        if (ZLL_DEVICE_INFO().nwk_channel)
        {
          zb_nwk_pib_set(param, ZB_PIBCACHE_PRIMARY_IFACE(), ZB_PHY_PIB_CURRENT_CHANNEL,
            &ZLL_DEVICE_INFO().nwk_channel, sizeof(ZLL_DEVICE_INFO().nwk_channel), bdb_commissioning_machine);
        }
        else
        {
        ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param);
      }
      }
      else
      {
        /*
          7. In any order, the initiator MAY request more device information from
          the target,

          To request more device information from the target, the initiator
          SHALL generate and transmit a touchlink commissioning cluster device
          information request
        */
        TRACE_MSG(TRACE_ZLL3, "Touchlink: gather sub-device information by devinfo req", (FMT__0));
        ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_DEVINFO_GATHER;
        ZLL_TRAN_CTX().current_dev_info_idx = 0;
        /* discover sub-devices */
        ZB_SCHEDULE_CALLBACK(zll_send_next_devinfo_req, param);
      }
      break;

    case ZB_BDB_JOIN_MACHINE_DEVINFO_GATHER:
      /* We are called from zll_send_next_devinfo_req */
      TRACE_MSG(TRACE_ZLL3, "Touchlink: device discover done, found %hd ZLL devices", (FMT__H, ZLL_TRAN_CTX().n_device_infos));
      /*
        If the extended PAN identifier field of the scan response command frame 1290 is
        not equal to nwkExtendedPANID (i.e., the target is not on the same 1291 network
        as the initiator), the initiator SHALL continue from step 10.
      */
      ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_ADDING_TO_NETWORK;
      ZB_SCHEDULE_CALLBACK(zll_add_device_to_network, param);
      break;

    case ZB_BDB_JOIN_MACHINE_ADDING_TO_NETWORK:
      ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_TOUCHLINK;
      /* clear state for next attempt */
      ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN;
      if (zb_buf_get_status(param) != 0)
      {
        TRACE_MSG(TRACE_ZLL3, "Touchlink initiator: done - can't join", (FMT__0));
        /* Why to reset ZG->aps.authenticated?? */
        /* ZG->aps.authenticated = ZB_FALSE; */
#ifdef BDB_OLD
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_TARGET_FAILURE;
        ZB_BDB().bdb_commissioning_step <<= 1;
#else
        bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_FAILED, param);
#endif
      }
      else
      {
        TRACE_MSG(TRACE_ZLL3, "Touchlink initiator: done - success", (FMT__0));
#ifdef BDB_OLD
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
        ZB_BDB().bdb_commissioning_step = ZB_BDB_LAST_COMMISSIONING_STEP;
#else
        bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_DONE, param);
#endif
      }
#ifdef BDB_OLD
      ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param);
#endif
      break;

    default:
      TRACE_MSG(TRACE_ERROR, "Why are we here???", (FMT__0));
      ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param);
      break;
  }
  TRACE_MSG(TRACE_ZLL1, "< bdb_touchlink_initiator", (FMT__0));
}

#endif /* ZB_ZLL_ENABLE_COMMISSIONING_CLIENT */


static void bdb_touchlink_scan(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZLL3, "bdb_touchlink_scan param %hd", (FMT__H, param));
  if (!param)
  {
    zb_buf_get_out_delayed(bdb_touchlink_scan);
  }
  else
  {
    zb_int_t channel = bdb_touchlink_next_channel();

    if (channel != -1)
    {
      if (ZB_PIBCACHE_CURRENT_CHANNEL() != channel)
      {
#ifdef ZB_STACK_REGRESSION_TESTING_API
        /* rtp_touchlink_01 */
        TRACE_MSG(TRACE_TEST3, "set channel %hd then send scan req", (FMT__H, channel));
#else
        TRACE_MSG(TRACE_ZLL3, "set channel %hd then send scan req", (FMT__H, channel));
#endif
        ZB_PIBCACHE_CURRENT_CHANNEL() = channel;
        zb_nwk_pib_set(param, ZB_PIBCACHE_CURRENT_PAGE(), ZB_PHY_PIB_CURRENT_CHANNEL,
          &ZB_PIBCACHE_CURRENT_CHANNEL(), 1, bdb_touchlink_send_scan);
      }
      else
      {
#ifdef ZB_STACK_REGRESSION_TESTING_API
        /* rtp_touchlink_01 */
        TRACE_MSG(TRACE_TEST3, "keep channel %hd and send scan req", (FMT__H, channel));
#else
        TRACE_MSG(TRACE_ZLL3, "keep channel %hd and send scan req", (FMT__H, channel));
#endif
        ZB_SCHEDULE_CALLBACK(bdb_touchlink_send_scan, param);
      }
    }
    else
    {
      ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param);
    }
  }
}


static void bdb_touchlink_send_scan(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZLL3, "bdb_touchlink_send_scan param %hd", (FMT__H, param));
  /*
Each scan request inter-PAN command frames SHALL be 1232 broadcast with
appropriate values for the Zigbee information and touchlink 1233 information
fields and with a nominal output power of 0dBm.
   */
  ZB_ZLL_COMMISSIONING_SEND_SCAN_REQ(param, bdb_touchlink_scan_req_sent);
  ZB_SCHEDULE_ALARM(bdb_touchlink_scan, 0, ZB_BDBC_TL_SCAN_TIME_BASE_DURATION);
}


static void bdb_touchlink_scan_req_sent(zb_uint8_t param)
{
  zb_ret_t status = zb_buf_get_status(param);

#ifdef ZB_STACK_REGRESSION_TESTING_API
  /* rtp_touchlink_01 */
  TRACE_MSG(TRACE_TEST3, "bdb_touchlink_scan_req_sent param %hd status %d", (FMT__H_D, param, status));
#else
  TRACE_MSG(TRACE_ZLL3, "bdb_touchlink_scan_req_sent param %hd status %d", (FMT__H_D, param, status));
#endif

  if (status)
  {
    /* Failed to send a packet. Specification does not cover if we should retransmit,
     * but we shall cover every channel from the channel mask so do not end scan procedure. */
    TRACE_MSG(TRACE_ZLL1, "Scan request sending failed the status %hd", (FMT__H, status));

     /* No need for the timeout, schedule bdb_touchlink_send_scan now */
    ZB_SCHEDULE_ALARM_CANCEL(bdb_touchlink_scan, ZB_ALARM_ANY_PARAM);
    ZB_SCHEDULE_CALLBACK(bdb_touchlink_scan, param);
  }
  else
  {
    /* MP: Waiting for response or bdb_touchlink_scan, no need for the buffer. */
    zb_buf_free(param);
  }
}


static zb_int_t bdb_touchlink_next_channel()
{
  zb_int_t ret;

  if (ZB_BDB().tl_first_channel_rpt)
  {
    ZB_BDB().tl_first_channel_rpt--;
  }
  else
  {
    ZB_BDB().tl_channel_i++;
  }
  while (ZB_BDB().tl_channel_i < 32
         && (ZB_BDB().v_scan_channels & (1<<ZB_BDB().tl_channel_i)) == 0)
  {
    ZB_BDB().tl_channel_i++;
  }
  ret = ZB_BDB().tl_channel_i < 32 ? ZB_BDB().tl_channel_i : -1;
  TRACE_MSG(TRACE_ZLL3, "bdb_touchlink_next_channel ret %d", (FMT__D, ret));
  return ret;
}


/*
Notes


- after successful association
If the node supports touchlink, it sets the values of the aplFreeNwkAddrRangeBegin, aplFreeNwkAddrRangeEnd, aplFreeGroupID-RangeBegin and
aplFreeGroupIDRangeEnd attributes all to 0x0000 (indicating the node having joined the network using MAC association).


 */



#endif  /* ZB_BDB_MODE */
