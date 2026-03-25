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
/* PURPOSE: ZLL commissioning function declarations.
*/

#ifndef ZLL_COMMISSIONING_INTERNALS_H
#define ZLL_COMMISSIONING_INTERNALS_H 1

#include "zb_zdo.h"

/**
 * @addtogroup ZB_ZLL
 * @{
 */

/** */
#define ZLL_SRC_PANID_FIRST_VALUE   0x0001
#define ZLL_SRC_PANID_LAST_VALUE    0xfffe

/** @brief Maximum number of iterations for zll_random_primary_channell() function. */
#define ZLL_MAX_RANDOM_CHANNEL_ITERATIONS 4

/** @brief Address error value. It is returned if device is unable to generate new address */
#define ZB_ZLL_ADDR_ERROR 0xFFFF

/** @brief Internal address error value. It is returned if there is
 * no more free address in address range */
#define ZB_ZLL_NO_MORE_ADDR 0xFFFE

#if defined ZB_ENABLE_ZLL
/**
 *  @brief Provides random primary ZLL channel number.
 *  @returns Primary ZLL channel that is correct with respect to AIB channel mask, or @ref
 *  ZB_ZLL_FIRST_PRIMARY_CHANNEL if could not generate any.
 *  @note Channel mask must contain at least one primary ZLL channel.
 */
zb_uint8_t zll_random_primary_channel(void);

/** @internal @brief Cancels current ZLL inter-PAN transaction. */
void zll_cancel_transaction(void);

/** @brief Initializes data for action sequence rollback. */
#define ZLL_ACTION_SEQUENCE_INIT()            \
{                                             \
  ZB_MEMCPY(                                  \
      &ZLL_TRAN_CTX().addr_range,             \
      &ZLL_DEVICE_INFO().addr_range,          \
      sizeof(ZLL_TRAN_CTX().addr_range));     \
  ZB_MEMCPY(                                  \
      &ZLL_TRAN_CTX().group_id_range,         \
      &ZLL_DEVICE_INFO().group_id_range,      \
      sizeof(ZLL_TRAN_CTX().group_id_range)); \
}

/** @brief Rolls back action sequence. */
#define ZLL_ACTION_SEQUENCE_ROLLBACK()        \
{                                             \
  ZB_MEMCPY(                                  \
      &ZLL_DEVICE_INFO().addr_range,          \
      &ZLL_TRAN_CTX().addr_range,             \
      sizeof(ZLL_TRAN_CTX().addr_range));     \
  ZB_MEMCPY(                                  \
      &ZLL_DEVICE_INFO().group_id_range,      \
      &ZLL_TRAN_CTX().group_id_range,         \
      sizeof(ZLL_TRAN_CTX().group_id_range)); \
  ZLL_SET_TRANSACTION_TASK_ID(ZB_ZLL_NO_TASK);  \
}

/** @internal @brief Sets transaction task ID. */
#define ZLL_SET_TRANSACTION_TASK_ID(task_id) (ZLL_TRAN_CTX().transaction_task = (task_id))

/** @internal @brief Returns transaction current task ID. */
#define ZLL_GET_TRANSACTION_TASK_ID(task_id) (ZLL_TRAN_CTX().transaction_task)

/**
 *  @internal @brief Inter-PAN transaction lifetime guard function.
 *  Takes appropriate actions on transaction identifier lifetime timeout.
 *  @param param [IN] - just a formal parameter for ZB_CALLBACK functions, should be ignored.
 */
void zll_intrp_transaction_guard(zb_uint8_t param);

/**
 *  @internal @brief ZLL.ScanRequest lifetime guard function.
 *  Takes appropriate actions on ScanRequest timeout.
 *  @param param [IN] - reference (number in the pool) to the @ref zb_buf_t "buffer" could be used
 *  on the next step.
 */
void zll_scan_req_guard(zb_uint8_t param);

/**
 *  @internal @brief Puts task status into the buffer and notifies user on transaction state change.
 *  @param buffer [IN] - pointer to the @ref zb_buf_t "buffer" to put task status to.
 *  @param _status [OUT] - transaction task status (see @ref zb_zll_transaction_task_e).
 */
void zll_notify_task_result(zb_bufid_t buffer, zb_uint8_t _status);

/**
 *  @brief Provides new short address.
 *  Short address will be taken out from the beginning of the free network addresses range, or
 *  generated stochastically, if the free address range is empty.
 *  @returns 0 on failure, or a valid network address otherwise.
 */
zb_uint16_t zll_get_new_addr(void);


struct zb_zll_group_id_range_s;

#ifdef ZB_ZLL_ADDR_ASSIGN_CAPABLE
/**
 *  @brief Provides group identifier range of the specified length from free group identifiers
 *  range of the current device adjusting free identifiers range accordingly.
 *  @param range [OUT] - pointer to the structure to put resulting range to. If the range cannot be
 *  allocated, it'll have both fields zeroed.
 *  @param range_len [IN] - length of the range requested.
 *  @return RET_OK on success, other value in case of error
 */
zb_ret_t zll_get_group_id_range(struct zb_zll_group_id_range_s* range, zb_uint16_t range_len);
#endif /* ZB_ZLL_ADDR_ASSIGN_CAPABLE */

struct zb_zll_addr_range_s;

#ifdef ZB_ZLL_ADDR_ASSIGN_CAPABLE
/**
 *  @brief Provides network address range of the specified length from freenetwork addresses
 *  range of the current device adjusting free addresses range accordingly.
 *  @param range [OUT] - pointer to the structure to put resulting range to. If the range cannot be
 *  allocated, it'll have both fields zeroed.
 *  @return RET_OK on success, other value in case of error
 */
zb_ret_t zll_get_addr_range(struct zb_zll_addr_range_s* range);
#endif /* ZB_ZLL_ADDR_ASSIGN_CAPABLE */

/**
 *
 */
zb_ret_t zll_handle_scan_req(zb_uint8_t param);

/** @internal @brief Handle incoming DeviceInformationRequest frame.
  * @param param reference to the buffer containing the packet.
  */
zb_ret_t zll_handle_devinfo_req(zb_uint8_t param);

/**
 *  @brief Handles ZLL Commissioning Identify command.
 *  Implements identify process utilizing ZCL Identify cluster functionality.
 *  @param param - reference to the buffer containing @ref zb_zll_commissioning_identify_req_s
 *  structure allocated, and @ref zb_zcl_parsed_hdr_s structure as buffer parameter.
 *  @return Command processing status.
 */
zb_ret_t zb_zll_identify_handler(zb_uint8_t param);

zb_ret_t zll_network_start_req_handler(zb_uint8_t param);

#ifdef ZB_ZLL_ENABLE_COMMISSIONING_SERVER

zb_ret_t zll_reset_to_fn_req_handler(zb_uint8_t param);

#if defined ZB_ROUTER_ROLE
zb_ret_t zll_join_router_req_handler(zb_uint8_t param);
#endif

#if defined ZB_ED_ROLE
zb_ret_t zll_join_ed_req_handler(zb_uint8_t param);
#endif

#endif  /* ZB_ZLL_ENABLE_COMMISSIONING_SERVER */

void zll_handle_scan_res(zb_uint8_t param);

#ifdef ZB_ZLL_ENABLE_COMMISSIONING_CLIENT

zb_ret_t zll_join_router_res_handler(zb_uint8_t param);

zb_ret_t zll_join_ed_res_handler(zb_uint8_t param);

#endif /* ZB_ZLL_ENABLE_COMMISSIONING_CLIENT */

void zll_handle_devinfo_res(zb_uint8_t param);

zb_ret_t zll_network_start_res_handler(zb_uint8_t param);

zb_ret_t zll_network_update_req_handler(zb_uint8_t param);

zb_uint8_t zll_calc_enc_dec_nwk_key(zb_uint8_t *key_encrypted,  /* pointer on encrypted key data */
                                    zb_uint8_t *key_decrypted,  /* pointer on decrypted key data */
                                    zb_uint16_t key_info,       /* key info bitfiled (2B) or key index(1B) (see is_out param) */
                                    zb_uint32_t transaction_id, /* transaction_id */
                                    zb_uint32_t response_id,    /* response_id */
                                    zb_bool_t is_out);          /* direction flag (encrypt or decrypt flag) */

#endif /* ZB_ENABLE_ZLL */

/**
 * @}
 */

#endif /* ZLL_COMMISSIONING_INTERNALS_H */
