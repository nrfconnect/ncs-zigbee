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
/* PURPOSE: Low-Level Radio API
*/

#ifndef ZB_LL_RADIO_API_H
#define ZB_LL_RADIO_API_H 1


/* Moved zb_phy_status_t enum here from zb_mac.h header
    in order to be able to use zb_ll_radio_api.h without
    other internal headers inclusion.
   Now, zb_ll_radio_api.h can be used in SDK if necessary. */
/**
 *  \addtogroup PHY common constants and types.
 *  @{
 */

/**
 *  @brief PHY enumerations descriptions
 */
typedef enum zb_phy_status_e
{
  PHY_BUSY                  = 0x00,
  PHY_BUSY_RX               = 0x01,
  PHY_BUSY_TX               = 0x02,
  PHY_FORCE_TRX_OFF         = 0x03,
  PHY_IDLE                  = 0x04,
  PHY_INVALID_PARAMETER     = 0x05,
  PHY_RX_ON                 = 0x06,
  PHY_SUCCESS               = 0x07,
  PHY_TRX_OFF               = 0x08,
  PHY_TX_ON                 = 0x09,
  PHY_UNSUPPORTED_ATTRIBUTE = 0x0a,
  PHY_READ_ONLY             = 0x0b
} zb_phy_status_t;

/** @} */ /* PHY common constants and types */

/** @addtogroup ll_radio_api
  * @{
  */

/**
   The low-level radio API provides low-level interface between ZB MAC (or Multi-PAN) and
   platform-dependent radio functions.

   If the Multi-PAN feature is enabled, ZB MAC and OT MAC use Multi-PAN radio functions and
     those functions call radio API functions.
   Otherwise, the ZB MAC uses the radio API directly.

   To keep ZB MAC layer compatible with both Multi-PAN enabled and disabled modes,
     the special zb_map_ll_radio_api.h file is introduced.
   Platforms without Multi-PAN support should not use the zb_map_ll_radio_api.h file and should
   provide their own mapping between ZB_TRANSCEIVER_ macros and radio functions.
*/

/**
 * @name Multi-PAN stack id
 * @anchor mpan_stack_id
 */


#define ZB_MPAN_STACK_ID_UNDEFINED 0xFFU
/**
   The special value indicates that the Multi-PAN command or received frame is intended for both stacks.
   It is mostly needed to properly handle broadcast frames.
*/
#define ZB_MPAN_STACK_ID_BROADCAST 0xFEU
#define ZB_MPAN_STACK_ID_OT 0U
#define ZB_MPAN_STACK_ID_ZB 1U
/** @} */

/**
   Stack id when in Multi-PAN mode

   Always 0 in a single-PAN build (either ZB only or Thread-only).
 */
typedef zb_uint8_t zb_mpan_stack_id_t;

/** @} */


/** @addtogroup ll_radio_api
  * @{
  */


#define ZB_RADIO_API_TX_STATUS_OK 0U
#define ZB_RADIO_API_TX_STATUS_CHANNEL_BUSY 1U
#define ZB_RADIO_API_TX_STATUS_LBT_TO_ERROR 2U
#define ZB_RADIO_API_TX_STATUS_RETRY_COUNT_EXCEEDED 3U
/**
   The status should be used only if Auto Ack receiving is enabled
   and Ack Timeout event happened.
 */
#define ZB_RADIO_API_TX_STATUS_NO_ACK 4U
#define ZB_RADIO_API_TX_STATUS_UNDEFINED 255U

typedef zb_uint8_t zb_radio_api_tx_status_t;


#define ZB_RADIO_API_TX_POWER_UNDEFINED -128

#define ZB_RADIO_API_ED_THRESHOLD_UNDEFINED 127

#define ZB_RADIO_API_SRC_MATCH_UNKNOWN_ADDR_REF (-1)

#define ZB_RADIO_API_INVALID_RSSI 127

/* Get ptr to frame parameters structure in the buffer by buffer start ptr.
   Put frame params right after pkt ends.  */
#define ZB_RADIO_API_GET_FRAME_PARAMS_PTR(buf_ptr, frame_len) ZB_ALIGN_UP_TO_4_IF_NOT_ALIGNED((zb_uint8_t *)(buf_ptr) + (zb_size_t)(frame_len))

/**
   Parameters of the received frame.
 */
typedef struct zb_radio_api_frame_param_s
{
  zb_mpan_stack_id_t stack_id;  /*!< destination stack id as defined by the filtering logic  */
  zb_uint8_t lqi;               /*!< LQI (correlation at some HW)  */
  zb_int8_t rssi;               /*!< RSSI of the received frame  */
  zb_uint8_t need_src_match;    /*!< 1 if radio layer did check over source matching  */
  zb_uint8_t pending_bit;       /*!< Pending bit according to the source matching hw/sw and in Imm-ACK sent automatically  */
  zb_uint8_t secure_enh_ack;    /*!< Secured Enh ACK was sent  */
  zb_time_t rx_timestamp;       /*!< rx timestamp in us according to the radio clock  */
} zb_radio_api_frame_param_t;


/**
   Parameters for frame TX operation
 */
typedef struct zb_radio_api_tx_param_s
{
  zb_uint8_t wait_type;         /*!< TX wait type @ref mac_tx_wait  */
  zb_uint8_t retransmit;        /*!< A hint to the radio layer: this is retransmit of the same frame (maybe after missing MAC ACK)  */
  zb_uint8_t repeats;           /*!< Number of transmit repeats for GreenPower frame  */
  zb_uint32_t tx_at;            /*!< Start transmission at specified time (in us, related to rx_timestamp of some another frame)  */
} zb_radio_api_tx_param_t;


/**
   Parameters for frame TX complete
 */
typedef struct zb_radio_api_tx_conf_s
{
  zb_uint8_t status;            /*!< TX status @ref mac_tx_status */
  zb_uint32_t tx_timestamp;     /*!< Timestamp is TX complete */
  zb_uint8_t lqi;               /*!< LQI of MAC ACK received */
  zb_int8_t rssi;               /*!< RSSI of MAC ACK received */
} zb_radio_api_tx_conf_t;




/**
   High-level callback to allocate a buffer for a single frame.

   The buffer is either to be freed by calling @ref zb_ll_radio_buf_free_func_t or
   passed up into @ref zb_ll_radio_received_func_t.

   @return pointer to the buffer (bytes array) or NULL if there are no buffers available
 */
typedef zb_uint8_t * (*zb_ll_radio_buf_alloc_func_t)(void);

/**
   High-level callback to free buffer allocated by the call of @ref zb_ll_radio_buf_alloc_func_t.

   @param buf - IN pointer returned by @ref zb_ll_radio_buf_alloc_func_t
 */
typedef void (*zb_ll_radio_buf_free_func_t)(zb_uint8_t *buf);

/**
   High-level callback to frame receive indication for the upper layer.
   The callback takes the buffer ownership.
     - If the frame is passed to a stack, the stack should free the buffer.
     - If the frame is not passed to any stack, the callback frees the buffer itself.

   If the frame is passed to ZB, the callback may call the following functions:
     - zb_radio_api_set_rx_done_bit() - to notify that receiving can be continued.
     - zb_radio_api_set_transceiver_int_flag() - to notify that transceiver event happened.
   OT doesn't use interrupts flags and status bits.

   @param buf - frame body. The buffer is allocated by the call to @ref zb_ll_radio_buf_alloc_func_t. The callee should free the buffer
   @param len - frame body length
   @param params - parameters of the received frame. May be allocated in the caller stack.
 */
typedef void (*zb_ll_radio_received_func_t)(zb_uint8_t *buf, zb_uint8_t len, zb_radio_api_frame_param_t *params);

/**
   Callback called when TX (optionally with waiting for ACK rx) is done

   In more detail, the radio API should call the callback in the following cases:
     - If Auto-Ack receiving is enabled:
       - If the outgoing frame is transmitted and the Ack is received.
         The callback calls the zb_mpan_radio_set_ack_received() itself.
       - If the outgoing frame is not transmitted because of some error or
         outgoing Ack is not received.
     - If Auto-Ack receiving is not enabled:
       - If the outgoing frame is transmitted and Ack waiting is started.
         To confirm Ack receiving, it is needed to manually call zb_mpan_radio_set_ack_received().
         Ack timeout will be handled by MAC layer.
       - If the outgoing frame is not transmitted because of some error.

   Additionally, the callback may call the following functions:
     - zb_radio_api_set_rx_done_bit() - to notify that receiving can be continued.
     - zb_radio_api_set_transceiver_int_flag() - to notify that transceiver event happened.

   @param status - TX status @ref mac_tx_status. 0 of TX ok
   @param ack_pkt - pointer to the buffer with Enh-Ack frame if its receive implemented in the radio layer on that HW else NULL
 */
typedef void (*zb_ll_radio_tx_complete_func_t)(zb_radio_api_tx_status_t status, zb_uint8_t *ack_pkt);

/**
   Callback called when ED scan is completed

   In more detail, the radio API should call the callback in the following cases:
     - ED scan has been started by zb_radio_api_start_get_energy_level() and finished with success or some error.
       Note that there can be a case when ED scan has been started by zb_radio_api_start_get_energy_level(),
       but has not yet finished, and the zb_radio_api_get_energy_level() function is called.

   @param status - Scan status. 0 of scan is ok
   @param raw_energy_level - Energy level in [0, 255] units
    (see Zigbee spec, Table 3-9. Field Descriptions of the EnergyDetectListStructure)
 */
typedef void (*zb_ll_radio_ed_scan_complete_func_t)(zb_ret_t status, zb_uint8_t raw_energy_level);

/**
   Callback called when radio API needs to send Enhanced ACK for a received frame.
   If the Enhanced Ack is not needed for the frame, the callbacks returns ZB_FALSE
 */
typedef zb_bool_t (*zb_ll_radio_gen_enh_ack_func_t)(zb_uint8_t *rx_buffer, zb_uint8_t len, zb_radio_api_frame_param_t *frame_param, zb_uint8_t **enh_ack, zb_uint8_t *enh_ack_len);


/**
   Callback called when 802.15.4 security must be applied to frame.
   Currently, used only to apply 802.15.4 security for enh-ack frames (OT stack) from interrupt on nRF52 platform.
 */
typedef zb_bool_t (*zb_ll_radio_apply_802_15_4_secur_func_t) (zb_uint8_t *frame_buffer, zb_uint8_t len, zb_radio_api_frame_param_t *frame_param);

/**
   The definition of upper layer callbacks for radio Ll API
 */
typedef struct zb_radio_api_callbacks_s
{
  zb_ll_radio_buf_alloc_func_t   buf_alloc; /*!< Upper level callback allocating a buffer for one frame  */
  zb_ll_radio_buf_free_func_t    buf_free;  /*!< Upper level callback freeing a buffer.  */
  zb_ll_radio_received_func_t    rx_ind;    /*!< Upper level callback called to pass received frame up from the radio layer  */
  zb_ll_radio_tx_complete_func_t tx_complete; /*!< Upper level callback called by the radio level when TX is complete  */
  zb_ll_radio_ed_scan_complete_func_t ed_scan_complete; /*!< Upper level callback called to pass ED scan result from the radio layer  */
  zb_ll_radio_gen_enh_ack_func_t gen_enh_ack; /*!< Upper level callback called to generate Enh ACK for the radio layer  */
  zb_ll_radio_apply_802_15_4_secur_func_t apply_802_15_4_secur;   /*!< Upper level callback called to apply security to provided frame */
} zb_radio_api_callbacks_t;

/**
   Initialize radio API, setup callbacks for upper layers.
   Do not switch radio power yet

   The function is called on first radio initialization or after zb_radio_api_deinit() call.

   @param callbacks - pointers to the callbacks data structure
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_init(zb_radio_api_callbacks_t *callbacks);


/**
   De-initialize radio API.

   The function is called to de-initialize the radio if it is initialized.
 */
zb_ret_t zb_radio_api_deinit(void);

/**
   Request radio to perform IO iteration.
   IO iteration can be used to handle queued events, incoming frames, etc.

   The function is called from the main loop, out of ISR context.
 */
void zb_radio_api_io_iteration(void);

/**
   Set or reset working page and channel.

   @param page - radio page, e.g. 0 for 2.4GHz or ZB_MAC_INVALID_LOGICAL_PAGE to reset the page.
   @param channel - channel number, e.g. 11 or ZB_MAC_INVALID_LOGICAL_CHANNEL to reset the channel.
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_set_channel(zb_uint8_t page, zb_uint8_t channel);

/**
   Set or reset promiscuous mode (if supported by the low-level radio).

   That mode is to be used to implement a sniffer.

   @param mode - ZB_TRUE to enable the mode and ZB_FALSE to disable
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_set_promiscuous_mode(zb_bool_t mode);

/**
   Get the current promiscuous mode status

   @return ZB_TRUE if the mode is enabled, ZB_FALSE if the mode is disabled
 */
zb_bool_t zb_radio_api_get_promiscuous_mode(void);

/**
   Return radio IEEE address from hardware

   @param stack_id stack id that requests HW IEEE address if that build supports Multi-PAN, ignored otherwise
   @param out_ieee_addr IEEE address
   @return zb_ret_t result code
 */
zb_ret_t zb_radio_api_get_hw_long_addr(zb_mpan_stack_id_t stack_id, zb_ieee_addr_t out_ieee_addr);

/**
   Set long (IEEE) address for the specified stack.

   Note that HW may or may not support different long addresses per stack id even in a Multi-PAN build.

   @param stack_id - stack number if that build supports Multi-PAN, ignored otherwise
   @param mac_addr - long address
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_set_mac_addr(zb_mpan_stack_id_t stack_id, const zb_ieee_addr_t mac_addr);

/**
   Get current value of the long address.

   The address can be either set by the call to zb_radio_api_set_mac_addr or got from the HW.

   @param stack_id - stack number if that build supports Multi-PAN, ignored otherwise
   @param out_mac_addr - long address
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_get_mac_addr(zb_mpan_stack_id_t stack_id, zb_ieee_addr_t out_mac_addr);

/**
   Set local PAN ID to be used for incoming frames filtering

   Setting PAN ID to 0xFFFF switches off filtering by PAN ID.

   @param stack_id - stack number if that build supports Multi-PAN, ignored otherwise
   @param pan_id - PAN ID to be set
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_set_pan_id(zb_mpan_stack_id_t stack_id, zb_uint16_t pan_id);

/**
   Get local short address to be used for incoming frames filtering

   Setting short address to 0xFFFF switches off filtering by short address.

   @param stack_id - stack number if that build supports Multi-PAN, ignored otherwise
   @param short_addr - short address to be set
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_set_short_addr(zb_mpan_stack_id_t stack_id, zb_uint16_t short_addr);

/**
   Get timestamp of the last transmitted frame.

   The function is intended to be used in OT platform and at ZB MAC certification time.

   @return TX timestamp in microseconds
 */
zb_time_t zb_radio_api_get_tx_timestamp(void);


/**
   Start ED scan on a current channel.

   If the ED scan is started, the function should return RET_OK.
   Then, the radio API should call zb_ll_radio_ed_scan_complete_func_t callback with appropriate status and energy
   level value.

   If the ED scan is not started, the function should return an error code. zb_ll_radio_ed_scan_complete_func_t
   callback should not be called.

   @param scan_duration_bi - ED time in beacon interval units @ref ZB_BEACON_INTERVAL_USEC
   @return RET_OK or error code if the ED scan is not started
 */
zb_ret_t zb_radio_api_start_get_energy_level(zb_uint32_t scan_duration_bi);

/**
   Get the most recent energy level value

   If the ED scan is in progress, the function should stop it, then call zb_ll_radio_ed_scan_complete_func_t callback
   with appropriate status and energy level value.
   Finally, the function should return the most recent energy level value or error code if the value is not
   available.

   @param out_energy_level - energy level.
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_get_energy_level(zb_uint8_t *out_energy_level);


/**
   Switch background RX on or off

   @param rx_on - if ZB_TRUE switch RX ON else OFF
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_set_rx_on(zb_bool_t rx_on);

/**
   Get current state of background RX

   @return ZB_TRUE if background RX is ON, ZB_FALSE if off
 */
zb_bool_t zb_radio_api_is_rx_on(void);

/**
   Set transmit power on the current channel

   @param tx_power_dbm - TX power in dBm
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_set_tx_power(zb_int8_t tx_power_dbm);

/**
   Get TX power level at the current channel in dBm units

   @param out_tx_power_dbm - out TX power value in dBm
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_get_tx_power(zb_int8_t *out_tx_power_dbm);

/**
   Start frame transmission operation.

   Depending on a parameters it can be data (beacon, command) frame TX, GPFS TX, CSL TX, Enh-ACK TX.
   zb_ll_radio_tx_complete_func_t is called when TX operation, optionally with ACK receiving, is done.

   The function should start frame transmission or return error code if the transmission can not be started.
   If the transmission is started, the radio API should call the zb_ll_radio_tx_complete_func_t callback when
   it is finished.

   If the Auto Ack receiving is not enabled, the radio API should call zb_ll_radio_tx_complete_func_t right
   after frame transmission and then call zb_mpan_radio_set_ack_received() if incoming Ack is receiving
.
   In that case Multi-PAN will handle Ack timeout itself.

   If the Auto Ack receiving is enabled, the radio API should call zb_ll_radio_tx_complete_func_t when
   transmission is finished, incoming Ack received or there is Ack timeout.
   Multi-PAN will not handle Ack timeout itself.

   @param frame_buffer - frame data
   @param frame_len - frame length
   @param params - TX parameters.
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_send_frame(zb_uint8_t *frame_buffer, zb_uint8_t frame_len, zb_radio_api_tx_param_t *params);

/**
   The function is called when the radio will not be used by some small amount of time and
   it is possible to switch it to idle mode (i.e. switch radio power off).
   (see ZG->sleep.threshold).

   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_go_idle(void);


/**
   Enable/disable radio interrupts (or lock/unlock the radio access mutex)

   That call is used to protect data structures accessed by the main loop and by the radio callbacks
   (which might be called in ISR or another thread/task).
   Disabling interrupts (or its equivalent by locking mutex etc) guarantees that
   no callback will be called until interrupt enable.

   @param enable - if ZB_TRUE enable radio interrupts, if ZB_FALSE disable radio interrupts
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_enable_interrupts(zb_bool_t enable);

#if 0
/* Legacy for UZ2400. Kill it later */

/**
   Return ZB_TRUE if some transceiver event (interrupt) happened (e.g. frame or Ack is transmitted or received).
   The function is used by ZB MAC only.
   Initially in ZBOSS MAC there was check that interrupt status register is not zero, now this is just an API.

   The Multi-PAN layer sets the transceiver event flag itself by zb_radio_api_set_transceiver_int_flag() call.
   If the Multi-PAN is disabled, the platform MAC implementation should call zb_radio_api_set_transceiver_int_flag()
   or ZB_MAC_SET_TRANS_INT() manually.

   @return ZB_TRUE if "event" flag is set.
 */
zb_bool_t zb_radio_api_get_transceiver_int_flag(void);

/**
   Set or reset the transceiver "event" flag. To be used from ISR/callback etc.
   The function is used by ZB MAC only.
   See comments for zb_radio_api_get_transceiver_int_flag().

   @param int_flag - flag value
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_set_transceiver_int_flag(zb_bool_t int_flag);
#endif  /* 0 */

/**
   Set or reset "RX done" flag.
   To be used from ISR/callback etc.
   The function is used by ZB MAC only.

   If the Multi-PAN layer is enabled, it calls the function itself.
   Otherwise, the platform MAC implementation should call it manually if some incoming frame is received.
     If Auto Ack receiving is disabled, the platform MAC should call the function if incoming Ack is received.

   @param enable - flag value
   @return RET_OK or error code
 */

zb_ret_t zb_radio_api_set_rx_done_bit(zb_bool_t enable);


/*
Group of functions to decode TX status to pass it into ZBOSS using its legacy flags.
 */

/**
   Check last TX result for "channel busy" status

   @return ZB_TRUE if TX failed due to channel busy (CCA or CSMA/CA failure)
 */
zb_bool_t zb_radio_api_check_channel_busy_error(void);


/**
   Check last TX result for "LBT timeout" status

   That function is valid in SubGHZ mode only

   @return ZB_TRUE if TX failed due to LBT timeout
 */
zb_bool_t zb_radio_api_check_tx_lbt_to_error(void);


/**
   Check last TX result for "out of retransmits" status

   That function is valid only on some radio which provides a retransmit feature, like UZ2400. Most radio does not have that feature.

   @return ZB_TRUE if TX failed due to out of retransmits at MAC level
 */
zb_bool_t zb_radio_api_check_tx_retry_count_exceeded_error(void);

/**
   Check last TX result for "no MAC ACK received" status

   The function is used if Auto Ack receiving feature is enabled,
   i.e. the platform implements MAC Ack waiting after TX in HW or in the radio support layer
   (most radio).

   @return ZB_TRUE if TX failed due to no MAC ACK received after TX
 */
zb_bool_t zb_radio_api_check_no_ack(void);


/**
   Enable Auto-Imm-ACK on a radio supporting it (sending Imm-ACK by the HW or radio level).

   @param enable - if ZB_TRUE, enable Auto-Imm-ACK, else disable it
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_enable_auto_imm_ack(zb_bool_t enable);


/**
   Enable Auto-Enh-ACK on a radio supporting it (sending Enh-ACK by the HW or radio level)

   @param enable - if ZB_TRUE, enable Auto-Enh-ACK, else disable it
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_enable_auto_enh_ack(zb_bool_t enable);

/**
 * @brief Get platform-specific CSL uncertainty (in 10 us units).
 *        Some vendors propose to not use value less than 12.
 *
 * @note  Some SSEDs may not enable RX for the whole window
 *          if both coefficients (uncertainty and accuracy) are equal to their max values.
 *
 * @return zb_uint8_t CSL uncertainty value.
 */
zb_uint8_t zb_radio_api_get_csl_uncertainty(void);

/**
 * @brief Get platform-specific CSL accuracy.
 *        Usually, it is equal to clock accuracy in ppm.
 *
 * @note  Some SSEDs may not enable RX for the whole window
 *          if both coefficients (uncertainty and accuracy) are equal to their max values.
 *
 * @return zb_uint8_t CSL accuracy value.
 */
zb_uint8_t zb_radio_api_get_csl_accuracy(void);

#if defined(ZB_MAC_HARDWARE_PB_MATCHING)
/*
  Source matching if supported by HW.
  The functions are available when ZB_MAC_HARDWARE_PB_MATCHING or ZB_MAC_SWITCHABLE_PB_MATCHING options
  are enabled.
 */

/**
   Add short address into the source matching storage

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @param nb_index - index in the Neighbor Table in Zigbee stack - a bit of optimization for one HW platform (TI CC1352). Ignored at most platforms
   @param short_addr - short address to add into Source matching data
   @return RET_OK, RET_ALREADY_EXISTS or other error code
 */
zb_ret_t zb_radio_api_src_match_add_short_addr(zb_mpan_stack_id_t stack_id, zb_uint8_t nb_index, zb_uint16_t short_addr);

/**
   Remove short address from the source matching storage

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @param nb_index - index in the Neighbor Table in Zigbee stack - a bit of optimization for one HW platform (TI CC1352). Ignored at most platforms
   @param short_addr - short address to add into Source matching data
   @return RET_OK or other error code
 */
zb_ret_t zb_radio_api_src_match_delete_short_addr(zb_mpan_stack_id_t stack_id, zb_uint8_t nb_index, zb_uint16_t short_addr);

/**
   Set pending bit value in the source matching storage

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @param short_addr - short address to add into Source matching data
   @param pending_bit - if ZB_TRUE, set bit to 1, else to 0
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_src_match_set_pending_bit_by_short(zb_mpan_stack_id_t stack_id, zb_uint16_t short_addr, zb_bool_t pending_bit);

/**
   Add long address into the source matching storage

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @param nb_index - index in the Neighbor Table in Zigbee stack - a bit of optimization for one HW platform (TI CC1352). Ignored at most platforms
   @param ieee_addr - short address to add into Source matching data
   @return RET_OK, RET_ALREADY_EXISTS or other error code
 */
zb_ret_t zb_radio_api_src_match_add_ieee_addr(zb_mpan_stack_id_t stack_id, zb_uint8_t nb_index, const zb_ieee_addr_t ieee_addr);

/**
   Remove long address from the source matching storage

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @param nb_index - index in the Neighbor Table in Zigbee stack - a bit of optimization for one HW platform (TI CC1352). Ignored at most platforms
   @param ieee_addr - short address to add into Source matching data
   @return RET_OK or other error code
 */
zb_ret_t zb_radio_api_src_match_delete_ieee_addr(zb_mpan_stack_id_t stack_id, zb_uint8_t nb_index, const zb_ieee_addr_t ieee_addr);

/**
   Set pending bit value in the source matching storage

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @param short_addr - short address to add into Source matching data
   @param pending_bit - if ZB_TRUE, set bit to 1, else to 0
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_src_match_set_pending_bit_by_ieee(zb_mpan_stack_id_t stack_id, const zb_ieee_addr_t ieee_addr, zb_bool_t pending_bit);


/**
   Drop all (both short and long) source matching table entries for the particular stack

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @return RET_OK or error code
 */
zb_ret_t zb_radio_api_source_match_table_drop(zb_mpan_stack_id_t stack_id);

/**
   Drop all long source matching table entries for the particular stack

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @return RET_OK or error code
*/
zb_ret_t zb_radio_api_source_match_table_drop_ieee(zb_mpan_stack_id_t stack_id);

/**
   Drop all short source matching table entries for the particular stack

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @return RET_OK or error code
*/
zb_ret_t zb_radio_api_source_match_table_drop_short(zb_mpan_stack_id_t stack_id);

/**
   Tries to find Source Match table entry by stack id and short MAC address.
   The function is optional as some HW platforms may not support it.
   In that case ZB_RADIO_API_SRC_MATCH_UNKNOWN_ADDR_REF will be returned.

   @param stack_id - stack id in Multi-PAN @ref mpan_stack_id
   @param short_addr - short MAC address
   @return zb_int_t address reference
*/
zb_int_t zb_radio_api_src_match_seek_short_addr(zb_mpan_stack_id_t stack_id, zb_uint16_t short_addr);

/**
   Tries to find Source Match table entry by IEEE address.
   The function is optional as some HW platforms may not support it.
   In that case ZB_RADIO_API_SRC_MATCH_UNKNOWN_ADDR_REF will be returned.

   @param long_addr - IEEE address
   @return zb_int_t address reference
*/
zb_int_t zb_radio_api_src_match_seek_ieee_addr(zb_mpan_stack_id_t stack_id, const zb_ieee_addr_t long_addr);

#endif

/*
  Next functions are not used now.
 */


#ifdef ZB_MAC_TESTING_MODE

/**
   The function is intended for MAC testing mode to
   start continuous transmission on the specified channel and for the
   specified time interval.

   @param channel - channel to use
   @param timeout_bi - timeout in beacon interval units @ref ZB_BEACON_INTERVAL_USEC
*/
zb_ret_t zb_radio_api_tx_carrier_data(zb_uint8_t channel, zb_uint32_t timeout_bi);

#endif

/**
   Performs CCA procedure and returns the measured RSSI value in dBm

   @param out_rssi out RSSI value in dBm
   @return result code
*/
zb_phy_status_t zb_radio_api_perform_cca(zb_int8_t *out_rssi);

/**
   Measures RSSI synchronously and returns the value

   @param out_rssi out RSSI value in dBm
   @return RSSI value in dBm
*/
zb_int8_t zb_radio_api_get_sync_rssi_dbm(void);

/**
 * @brief Converts raw energy level value from radio to dBm units.
 *
 * @param raw_ed_level raw energy level
 * @return zb_int8_t energy level in dBm
 */
zb_int8_t zb_radio_api_convert_ed_level_to_dbm(zb_uint8_t raw_ed_level);

/**
 * @brief Gets radio receive sensitivity
 *
 * @return zb_int8_t receive sensitivity value in dBm
 */
zb_int8_t zb_radio_api_get_receive_sensitivity_dbm(void);

/**
 * @brief Sets Energy Detect Threshold
 *
 * @param threshold_dbm threshold value in dBm
 * @return zb_ret_t status code
 */
zb_ret_t zb_radio_api_set_energy_detect_threshold(zb_int8_t threshold_dbm);

/**
 * @brief Gets Energy Detect Threshold
 *
 * @param out_threshold_dbm out threshold value in dBm
 * @return zb_ret_t status code
 */
zb_ret_t zb_radio_api_get_energy_detect_threshold(zb_int8_t *out_threshold_dbm);

/* ZB-only build support */

zb_bool_t zb_radio_api_is_transport_busy(void);

zb_bool_t zb_radio_api_get_rx_done_bit(void);

zb_bool_t zb_radio_api_get_tx_done_bit(void);

zb_ret_t zb_radio_api_set_tx_done_bit(zb_bool_t tx_done);

zb_ret_t zb_radio_api_send_zb(zb_bufid_t buf, zb_uint8_t tx_wait_type);

zb_ret_t zb_radio_api_init_zb(void);

#ifndef ZB_MULTIPAN

zb_bool_t zb_mpan_can_imm_ack(zb_uint8_t *frame_buffer);
zb_bool_t zb_mpan_radio_get_stack_id_by_frame_ptr(const zb_uint8_t *frame_payload_ptr, zb_radio_api_frame_param_t *frame_param);
zb_bool_t zb_mpan_need_src_match(zb_uint8_t *cmd_ptr, zb_radio_api_frame_param_t *frame_param);
zb_bool_t mpan_sw_src_match_calc_pend_bit(zb_uint8_t *buf, zb_radio_api_frame_param_t *frame_param);
zb_mpan_stack_id_t zb_mpan_radio_get_stack_waiting_for_ack(zb_uint8_t ack_dsn);
void zb_mpan_radio_set_ack_received(zb_mpan_stack_id_t stack_id, zb_uint8_t ack_dsn, zb_bool_t ack_pending_bit, zb_uint8_t ack_version);
zb_uint8_t zb_mpan_radio_get_ack_frame_version(zb_mpan_stack_id_t stack_id);
#endif

/** @} */

#endif /* ZB_LL_RADIO_API_H */
