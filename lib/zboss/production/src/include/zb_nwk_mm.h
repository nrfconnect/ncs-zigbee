/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2023 DSR Corporation, Denver CO, USA.
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
/*  PURPOSE: Header file for NWK MultiMAC
*/
#ifndef ZB_NWK_MM_H
#define ZB_NWK_MM_H 1

#define ZB_NWK_MAC_IFACE_SCAN_TYPE_ACTIVE     0U
#define ZB_NWK_MAC_IFACE_SCAN_TYPE_ENH_ACTIVE 1U

void zb_nwk_mm_mac_iface_table_init(void);

zb_ret_t zb_mm_register_interface(const zb_mac_interface_t *mac_interface, zb_uint8_t *interface_id);

zb_uint8_t zb_nwk_mm_get_freq_band(void);

void zb_nlme_set_interface_request(zb_uint8_t param);

void zb_nlme_get_interface_request(zb_uint8_t param);

zb_uint8_t zb_multimac_get_iface_id_by_channel_page(zb_uint8_t page_idx, zb_uint8_t channel_idx);

/* Multi-MAC proxy functions */
void zb_nwk_handle_mlme_start_confirm(zb_uint8_t param);
void zb_nwk_handle_mlme_reset_confirm(zb_uint8_t param);
void zb_nwk_handle_mcps_data_confirm(zb_uint8_t param);

void zb_multimac_mlme_start_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_start_confirm_proxy(zb_uint8_t param);
void zb_multimac_mcps_data_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_set_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_reset_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_reset_confirm_proxy(zb_uint8_t param);
void zb_multimac_mlme_get_request_proxy(zb_uint8_t param);

void zb_multimac_mlme_scan_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_scan_confirm_proxy(zb_uint8_t param);

void zb_multimac_mcps_data_confirm_proxy(zb_uint8_t param);

/* MAC split interface API */
#ifdef ZB_MACSPLIT_HOST
void zb_mac_macsplit_host_init_instance(void);
void zb_mac_macsplit_host_deinit_instance(void);

void zb_mcps_data_request_macsplit(zb_uint8_t param);
void zb_mlme_get_request_macsplit(zb_uint8_t param);
void zb_mlme_set_request_macsplit(zb_uint8_t param);
void zb_mlme_reset_request_macsplit(zb_uint8_t param);
void zb_mlme_scan_request_macsplit(zb_uint8_t param);
void zb_mlme_associate_request_macsplit(zb_uint8_t param);
void zb_mlme_associate_response_macsplit(zb_uint8_t param);
void zb_mlme_poll_request_macsplit(zb_uint8_t param);
void zb_mlme_orphan_response_macsplit(zb_uint8_t param);
void zb_mlme_start_request_macsplit(zb_uint8_t param);
void zb_mac_resp_by_empty_frame_macsplit(zb_uint8_t param);

zb_ret_t zb_mac_logic_iteration_macsplit();
#endif /* ZB_MACSPLIT_HOST */

/**
 * @brief Synchronizes association parameters (MAC short address, coordinator extended address)
 * from primary interface with other interfaces
 *
 * @param param
 */
void zb_nwk_mm_sync_assoc(zb_uint8_t param);


void zb_multimac_mac_init(void);
void zb_multimac_mac_deinit(void);

#define ZB_MAC_INIT() zb_multimac_mac_init()
#define ZB_MAC_DEINIT() zb_multimac_mac_deinit()

/**
   Get the primary MAC interface.
*/
zb_uint8_t zb_get_primary_interface(void);


#define ZB_MAC_INTERFACE_UNKNOWN_ID ZB_NWK_MAC_IFACE_TBL_SIZE

/**
 * @brief Enables MAC interface by id
 *
 * @param iface_id - interface id
 * @return zb_ret_t RET_OK on success, error code otherwise.
 */
zb_ret_t zb_mm_enable_interface(zb_uint8_t iface_id);

/**
 * @brief Disables MAC interface by id
 *
 * @param iface_id - interface id
 * @return zb_ret_t RET_OK on success, error code otherwise.
 */
zb_ret_t zb_mm_disable_interface(zb_uint8_t iface_id);

/**
 * @brief Disables all MAC interfaces
 *
 * @return zb_ret_t RET_OK on success, error code otherwise.
 */
zb_ret_t zb_mm_disable_all_interfaces(void);

/**
 * @brief Checks whether MAC interface is active or not
 *
 * @param iface_id - interface id
 * @return zb_bool_t interface active flag
 */
zb_bool_t zb_mm_is_interface_active(zb_uint8_t iface_id);

/**
 * @brief Sets supported channels mask for specified interface
 *
 * @param iface_id - interface id
 * @param page_index - index of the page
 * @param channel_mask - mask of supported channels
 */
void zb_mm_set_iface_supported_channel_mask(zb_uint8_t iface_id, zb_uint8_t page_index, zb_uint32_t channel_mask);

/**
 * @brief Registers new MAC-split host instance
 *
 * @param [out] iface_id - interface id of new MAC-split host instance
 * @return zb_ret_t - RET_OK for success, error code otherwise
 */
zb_ret_t zb_mm_macsplit_host_register_and_enable_instance(zb_uint8_t *iface_id);

/**
 * @brief Registers new BLE MAC instance
 *
 * @param iface_id - interface id of new BLE MAC instance
 * @return zb_ret_t - RET_OK for success, error code otherwise
 */
zb_ret_t zb_mm_ble_register_and_enable_instance(zb_uint8_t *iface_id);


/**
 * @brief Locks MAC interface (increase locks counter)
 * Interface locking prevents its disabled
 *
 * @param iface_id - interface id
 */
void zb_mm_lock_interface(zb_uint8_t iface_id);


/**
 * @brief Unlocks MAC interface (decrease locks counter)
 *
 * @param iface_id - interface id
 */
void zb_mm_unlock_interface(zb_uint8_t iface_id);

/**
 * @brief Checks whether MAC interface is locked or not (locks counters should be 0)
 *
 * @param iface_id - interface id
 * @return zb_bool_t
 */
zb_bool_t zb_mm_is_interface_locked(zb_uint8_t iface_id);

#endif /* ZB_NWK_MM_H */
