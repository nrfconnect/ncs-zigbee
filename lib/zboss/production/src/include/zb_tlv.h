/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: TLV API header
*/
#ifndef ZB_TLV_H
#define ZB_TLV_H 1

#include "zb_ecc.h"

/**
 * TLV tags enumeration.
 *
 * Note: These values were members of zb_tlv_tag_t `enum zb_tlv_tag_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
typedef zb_uint8_t zb_tlv_tag_t;
/* 2.4.3.4.1.2 Key Update Req/Rsp Selected Key Negotiation Method TLV (ID=0) */
#define ZB_TLV_KEY_UPD_SELECTED_KEY_NEGOTIATION_METHOD    0x00U
/* 2.4.3.4.1.2 Key Negotiation Req/Rsp Curve25519 Public Point TLV (ID=0) */
#define ZB_TLV_KEY_NEG_CURVE25519_PUBLIC_POINT            0x01U
/* 2.4.3.4.2.2 Requested Authentication Token ID TLV (ID=0) */
#define ZB_TLV_REQUESTED_AUTHENTICATION_TOKEN_ID          0x02U
/* 2.4.3.2.12.2 Clear All Bindings Req EUI64 TLV (ID=0) */
#define ZB_TLV_CLEAR_ALL_BIND_REQ_EUI64                   0x03U
/* 2.4.3.4.7.2 Security Decommission Req EUI64 TLV (ID=0) */
#define ZB_TLV_SECUR_DECOMMISSION_REQ_EUI64               0x04U
#define ZB_TLV_DIRECT_KEY_NEG_METHOD                      0x05U
#define ZB_TLV_DIRECT_KEY_NEG_P256_PUBLIC_POINT           0x06U
#define ZB_TLV_DIRECT_KEY_NEG_C25519_PUBLIC_POINT         0x07U
#define ZB_TLV_DIRECT_NWK_KEY_SEQ_NUM                     0x08U
#define ZB_TLV_DIRECT_MAC_TAG_P256                        0x09U
#define ZB_TLV_DIRECT_MAC_TAG_C25519                      0x0AU
#define ZB_TLV_DIRECT_EXT_PAN_ID                          0x0BU
#define ZB_TLV_DIRECT_SHORT_PAN_ID                        0x0CU
#define ZB_TLV_DIRECT_NWK_CHANNEL                         0x0DU
#define ZB_TLV_DIRECT_NWK_KEY                             0x0EU
#define ZB_TLV_DIRECT_LINK_KEY                            0x0FU
#define ZB_TLV_DIRECT_DEVICE_TYPE                         0x10U
#define ZB_TLV_DIRECT_NWK_ADDR                            0x11U
#define ZB_TLV_DIRECT_JOINING_METHOD                      0x12U
#define ZB_TLV_DIRECT_IEEE_ADDR                           0x13U
#define ZB_TLV_DIRECT_TC_ADDR                             0x14U
#define ZB_TLV_DIRECT_NWK_STATUS                          0x15U
#define ZB_TLV_DIRECT_NWK_UPDATE_ID                       0x16U
#define ZB_TLV_DIRECT_NWK_ACTIVE_KEY_SEQ_NUM              0x17U
#define ZB_TLV_DIRECT_DISTRIBUTED_SEC_ADMIN_KEY           0x18U
#define ZB_TLV_DIRECT_STATUS_CODE                         0x19U
#define ZB_TLV_DIRECT_MANAGE_JOINERS_PROVISIONAL_LINK_KEY 0x1AU
#define ZB_TLV_DIRECT_MANAGE_JOINERS_IEEE_ADDRESS         0x1BU
#define ZB_TLV_DIRECT_MANAGE_JOINERS_CMD                  0x1CU
#define ZB_TLV_DIRECT_TUNNEL_NPDU                         0x1DU
/* Having variable length - is it legal for tlv? */
/* ZB_TLV_DIRECT_STATUS_CODE_EXT  */
/* 2.4.3.3.12.1 Beacon Survey Configuration TLV (ID=0) */
#define ZB_TLV_BEACON_SURVEY_CONFIGURATION                0x1EU
/* 2.4.4.3.13.1 Beacon Survey Results TLV (ID=1) */
#define ZB_TLV_BEACON_SURVEY_RESULTS                      0x1FU
/* 2.4.4.3.13.2 Beacon Survey Potential Parents TLV (ID=2) */
#define ZB_TLV_POTENTIAL_PARENTS                          0x20U
/* 2.4.3.4.8.2 APS Frame Counter Challenge Req TLV (ID=0) */
#define ZB_TLV_APS_FRAME_CNT_CHALLENGE_REQ                0x21U
/* 2.4.4.4.8.2 APS Frame Counter Challenge Rsp TLV (ID=0) */
#define ZB_TLV_APS_FRAME_CNT_CHALLENGE_RSP                0x22U
/* 2.4.3.4.3.2 Target IEEE Address local TLV (ID=0)*/
#define ZB_TLV_TARGET_IEEE_ADDR                           0x23U
/* 2.4.4.4.3.2 Device auth level local TLV (ID=0)*/
#define ZB_TLV_DEVICE_AUTH_LEVEL                          0x24U
/* 4.4.12.10.2 Relay Message local TLV (ID=0)*/
#define ZB_TLV_RELAY_MESSAGE                              0x25U
/* 4.4.11.1.4.1 Link-Key Features and Capabilities TLV (ID = 0)*/
#define ZB_TLV_LINK_KEY_CAPABILITIES                      0x26U
/* 2.4.4.4.4.1.1 Processing status TLV */
#define ZB_TLV_PROCESSING_STATUS                          0x27U
/* I.6 Global TLV IDs */
#define ZB_TLV_MANUFACTURER_SPECIFIC                      0x28U
#define ZB_TLV_SUPPORTED_KEY_NEGOTIATION_METHODS          0x29U
#define ZB_TLV_PANID_CONFLICT_REPORT                      0x2AU
#define ZB_TLV_NEXT_PAN_ID                                0x2BU
#define ZB_TLV_NEXT_CANNEL_CHANGE                         0x2CU
#define ZB_TLV_SYMMETRIC_PASSPHRASE                       0x2DU
#define ZB_TLV_ROUTER_INFORMATION                         0x2EU
#define ZB_TLV_FRAGMENTATION_PARAMETERS                   0x2FU
#define ZB_TLV_JOINER_ENCAPSULATION                       0x30U
#define ZB_TLV_BEACON_APPENDIX_ENCAPSULATION              0x31U
#define ZB_TLV_CONFIGURATION_MODE_PARAMETERS              0x32U
#define ZB_TLV_DEVICE_CAPABILITY_EXTENSION                0x33U
#define ZB_TLV_RESERVED_MAX                               0x34U


/**
 * Router Information TLV bitmask
 */
typedef enum zb_tlv_router_information_e
{
  ZB_TLV_ROUTER_INFORMATION_HUB_CONNECTIVITY = 0u,
  ZB_TLV_ROUTER_INFORMATION_UPTIME,
  ZB_TLV_ROUTER_INFORMATION_PREFERRED_PARENT,
  ZB_TLV_ROUTER_INFORMATION_BATTERY_BACKUP,
  ZB_TLV_ROUTER_INFORMATION_ENHANCED_BEACON_REQ_SUPPORT,
  ZB_TLV_ROUTER_INFORMATION_MAC_DATA_POLL_KEEPALIVE_SUPPORT,
  ZB_TLV_ROUTER_INFORMATION_END_DEVICE_KEEPALIVE_SUPPORT,
  ZB_TLV_ROUTER_INFORMATION_POWER_NEGOTIATION_SUPPORT,
  ZB_TLV_ROUTER_INFORMATION_RESERVED
} zb_tlv_router_information_t;

/**
 * Fragmentation TLV options bitmask
 */
typedef enum zb_tlv_fragmentation_opt_e
{
  ZB_TLV_FRAGMENTATION_OPT_APS_FRAG_SUPPORT = 0u
  /* bits 1-7 is reserved */
} zb_tlv_fragmentation_opt_t;

/**
 * TLV context
 */
typedef ZB_PACKED_PRE struct zb_tlv_hdr_s
{
  zb_uint8_t tlv_id;
  zb_uint8_t length;
} ZB_PACKED_STRUCT zb_tlv_hdr_t;

#define ZB_TLV_DIRECT_KEY_NEG_PUBLIC_POINT(curve_id)   \
  (((curve_id) == ZB_ECC_CURVE_25519)?              \
    ZB_TLV_DIRECT_KEY_NEG_C25519_PUBLIC_POINT :     \
    ZB_TLV_DIRECT_KEY_NEG_P256_PUBLIC_POINT)

#define ZB_TLV_ID_LIST_MAX_CNT      64U
#define ZB_TLV_EUI64_ARRAY_MAX_CNT  8U
typedef struct zb_tlv_eui64_array_s
{
  zb_uint8_t eui64_cnt;
  zb_ieee_addr_t eui64_arr[ZB_TLV_EUI64_ARRAY_MAX_CNT];
} zb_tlv_eui64_array_t;

typedef zb_tlv_eui64_array_t zb_tlv_clear_all_bind_req_eui64_t;
typedef zb_tlv_eui64_array_t zb_tlv_decommission_req_eui64_t;

#define ZB_TLV_KEY_ECDHE_METHOD_DISABLE(var, method)    ((var) &= ~(1u << (method)))
#define ZB_TLV_KEY_ECDHE_METHOD_IS_ENABLED(var, method) ZB_U2B(ZB_U2B((var) & ((zb_uint16_t)1U << (method))))

#define ZB_TLV_PSK_SECRET_DISABLE ZB_TLV_KEY_ECDHE_METHOD_DISABLE
#define ZB_TLV_PSK_SECRET_IS_ENABLE ZB_TLV_KEY_ECDHE_METHOD_IS_ENABLED

#define ZB_TLV_ROUTER_INFO_BIT_IS_SET(var, bit) ZB_U2B((var) & (1u << (bit)))

#define ZB_TLV_GET_BODY_PTR(tlv_hdr) \
  (&(((zb_uint8_t *)(tlv_hdr))[sizeof(zb_tlv_hdr_t)]))

#define ZB_TLV_NOT_FOUND      ZB_TLV_RESERVED_MAX
#define ZB_TLV_TAG_LOCAL_MIN  0U
#define ZB_TLV_TAG_GLOBAL_MIN ZB_TLV_MANUFACTURER_SPECIFIC

#define ZB_TLV_KEY_ECDHE_NOT_FOUND ZB_TLV_KEY_ECDHE_RESERVED_MAX

#define ZB_TLV_PUBLIC_POINT_P256_LEN       64U
#define ZB_TLV_PUBLIC_POINT_CURVE25519_LEN 32U

/* The only supported authentication token in this specification is 0x45, 128-bit Symmetric Passphrase TLV. */
#define ZB_TLV_SUPPORTED_AUTHENTICATION_TOKEN 0x45U

#define ZB_BEACON_SURVEY_POTENTIAL_PARENTS_TLV_MAX_PARENTS 5U

#define ZB_TLV_RELAY_MSG_MIN_LEN 9U /* joiner_ieee + fc + aps_cnt - 1 */

#define ZB_TLV_DIRECT_NWK_KEY_SEQ_NUM_DEFAULT_VALUE (zb_uint8_t)0U

typedef ZB_PACKED_PRE struct zb_tlv_status_s
{
  zb_uint8_t id;
  zb_uint8_t status;
} ZB_PACKED_STRUCT zb_tlv_status_t;

/**
 * Put TLV header for the certain tlv_tag (@ref zb_tlv_tag_t) and moove pointer (ptr)
 *
 * @param ptr     - pointer for tlv header insertion
 * @param tlv_tag - @ref zb_tlv_tag_t
 */
void zb_tlv_put_tlv_hdr(zb_uint8_t **ptr, zb_tlv_tag_t tlv_tag);

/**
 * Alloc space for tlv at the end of the buffer and put TLV header.
 *
 * @param param   - buffer id
 * @param tlv_tag - tlv tag that will be added (@ref zb_tlv_tag_t)
 *
 * @return        - pointer to the TLV payload position
 */
zb_uint8_t *zb_tlv_put_next(zb_uint8_t param, zb_tlv_tag_t tlv_tag);

/**
 * Alloc space and put zb_tlv_status_t at the end of the buffer.
 *
 * @param param   - buffer id
 * @param tag     - tlv tag that will be append (@ref zb_tlv_tag_t)
 * @param status  - status that will be append
 *
 * @return        - RET_OK if all OK
 */
zb_ret_t zb_tlv_status_append(zb_uint8_t param, zb_tlv_tag_t tag, zb_uint8_t status);

/**
 * Put Supported Key Negotiations Global TLV at the end of the packet.
 *
 * @param param                 - buffer id
 * @param ieee_addr             - Source Device EUI64
 * @param supported_key_methods - Key Negotiation Protocols Bitmask
 * @param supported_secrets     - Supported Pre-shared Secrets Bitmask
 */
void zb_tlv_put_value_supported_key_neg_methods(zb_uint8_t param,
                                                zb_ieee_addr_t ieee_addr,
                                                zb_uint8_t *supported_key_methods,
                                                zb_uint8_t *supported_secrets);

/**
 * Put Fragmentation Parameters Global TLV at the end of the packet.
 *
 * @param param         - buffer id
 * @param node_id       - Source Device EUI64
 * @param transfer_size - Maximum Reassembled Input Buffer Size
 */
void zb_tlv_put_value_fragmentation_parameters(zb_uint8_t param,
                                               zb_uint16_t node_id,
                                               zb_uint16_t transfer_size);

#if defined ZB_ROUTER_ROLE
/**
 * Put Router Information Global TLV at the end of the packet.
 *
 * @param param       - buffer id
 */
void zb_tlv_put_value_router_information(zb_uint8_t param);
#endif

/**
 * @brief Put Device Capability Extension Global TLV at the end of the packet.
 *
 * @param param                    - buffer id
 * @param dev_capability_ext_value - bitmask indicating data about the device capability
 */
void zb_tlv_put_value_device_capability_extension(zb_uint8_t param, zb_uint16_t dev_capability_ext_value);

/**
 * Put Key Negotiation Req Selected Key Negotiation Method TLV at the end of the packet.
 *
 * @param param       - buffer id
 * @param ieee_addr   - Source Device EUI64
 * @param selected_key_neg_method - Selected Key Enumeration
 * @param selected_psk_secret     - Selected Pre-shared Secret Enumeration
 */
void zb_tlv_put_value_selected_key_neg_method(zb_uint8_t param,
                                              zb_ieee_addr_t ieee_addr,
                                              zb_uint8_t *selected_key_neg_method,
                                              zb_uint8_t *selected_psk_secret);


#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE

/**
 * Put 128-bit Symmetric Passphrase Global TLV at the end of the packet.
 *
 * @param param      - buffer id
 * @param passphrase - passphrase
 */
void zb_tlv_put_value_symmetric_passphrase(zb_uint8_t param, zb_uint8_t *passphrase);

#endif /* ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE */

/**
 * Put Key Negotiation Req/Rsp Curve25519 Public Point TLV at the end of the packet.
 *
 * @param param        - buffer id
 * @param ieee_addr    - Source Device EUI64
 * @param public_point - Public Point
 */
void put_val_start_k_neg_rq_rsp(zb_uint8_t param,
                                zb_ieee_addr_t ieee_addr,
                                zb_uint8_t *public_point);

#define zb_tlv_put_value_start_key_neg_req_rsp_public_point put_val_start_k_neg_rq_rsp

/**
 * Put Key Negotiation Req/Rsp Public Point TLV at the end of the packet.
 *
 * @param param        - buffer id
 * @param ieee_addr    - Source Device EUI64
 * @param public_point - Public Point
 * @param curve_id     - id of the curve, corresponding to the current key negotiation method
 * @param is_dlk       - if true, uses dlk tlvs, otherwise zigbee direct
 */
void put_val_start_k_neg_rq_rsp_cmn(zb_uint8_t param,
                                    zb_ieee_addr_t ieee_addr,
                                    zb_uint8_t *public_point,
                                    zb_uint8_t curve_id,
                                    zb_bool_t is_dlk);

#define zb_tlv_put_value_start_key_neg_req_rsp_public_point_common put_val_start_k_neg_rq_rsp_cmn

#if defined ZB_JOIN_CLIENT

/**
 * Put Requested Authentication Token ID TLV at the end of the packet.
 *
 * @param param - buffer id
 */
void zb_tlv_put_value_authentication_token(zb_uint8_t param);

#endif /* ZB_JOIN_CLIENT */

/**
 * Get ID of the TLV tag (Table I.4.8-4-53)
 *
 * @param tlv_tag - @ref zb_tlv_tag_t
 *
 * @return        - ID of the requested tag
 */
zb_uint8_t zb_tlv_get_id(zb_tlv_tag_t tlv_tag);

/**
 * Get tag of the TLV id from zb_tlv_tag_t. Search begins from the @param tlv_tag.
 *
 * @param tlv_tag - tag from which the search will begin
 * @param tlv_id  - @ref g_tlv_id_by_tag
 *
 * @return        - tag of the requested ID @ref zb_tlv_tag_t ZB_TLV_RESERVED_MAX if not found
 */
zb_tlv_tag_t zb_tlv_get_tag(zb_tlv_tag_t tlv_tag, zb_uint8_t tlv_id);

/**
 * Get Length of the TLV tag (Table I.4.8-4-53)
 *
 * @param tlv_tag - @ref zb_tlv_tag_t
 *
 * @return        - size of the requested tag
 */
zb_uint8_t zb_tlv_get_size(zb_tlv_tag_t tlv_tag);

/**
 * Check TLV hdr, move buf_ptr to the next tlv and decrease buf_len.
 *
 * @param buf_ptr    - pointer on the tlv
 * @param buf_len    - length of the data with TLVs
 *
 * @return RET_OK    - TLV is correct, pointer is mooved
 *         RET_ERROR - TLV is malformed
 */
zb_ret_t zb_tlv_get_next(zb_uint8_t const **buf_ptr, zb_uint8_t *buf_len);

/**
 * I.4.8 General TLV processing implementation. Only common validation of the all TLVs.
 *
 * @param tlv_ptr             - pointer on the TLVs first byte
 * @param tlv_data_len        - length of the data with TLVs
 *
 * @return RET_OK             - if processing of all TLVs was successful
 *         RET_NOT_FOUND      - no TLVs found
 *         RET_INVALID_FORMAT - processing was terminated (malformed TLV found)
 */
zb_ret_t zb_tlv_general_tlv_processing(zb_uint8_t const *tlv_ptr, zb_uint8_t tlv_data_len);

/**
 * Function returns pointer on the certain TLV. There is no any validation of TLVs so do it
 * yourself or call @ref zb_tlv_general_tlv_processing() before using this function.
 *
 * @param tlv_tag      - TLV tag @ref zb_tlv_tag_t
 * @param tlv_ptr      - pointer on the tlv
 * @param tlv_data_len - length of the data with TLVs
 *
 * @return             - pointer on the TLV that was passed into tlv_tag or NULL if TLV is
 *                       not found/some TLV is malformed
 */
zb_tlv_hdr_t* zb_tlv_get_tlv_ptr(zb_uint8_t tlv_tag,
                                 zb_uint8_t const *tlv_ptr,
                                 zb_uint8_t tlv_data_len);

/**
 * Parse Supported Key Negotiations Global TLV.
 *
 * @param tlv_ptr               - pointer on the TLV
 * @param tlv_data_len          - length of the data with TLVs
 * @param ieee_addr             - (out) Source Device EUI64
 * @param supported_key_methods - (out) Key Negotiation Protocols Bitmask
 * @param supported_secrets     - (out) Supported Pre-shared Secrets Bitmask
 *
 * @return RET_OK               - TLV is found and parsed successfully
 *         RET_ERROR            - some error occurred
 */
zb_ret_t zb_tlv_parse_value_supported_key_neg_methods(zb_uint8_t *tlv_ptr,
                                                      zb_uint8_t tlv_data_len,
                                                      zb_ieee_addr_t ieee_addr,
                                                      zb_uint8_t *supported_key_methods,
                                                      zb_uint8_t *supported_secrets);

/**
 * Parse Fragmentation Parameters Global TLV.
 *
 * @param tlv_ptr       - pointer on the TLV
 * @param tlv_data_len  - length of the data with TLVs
 * @param node_id       - (out) Source Device EUI64
 * @param frag_opt      - (out) Fragmentation options
 * @param transfer_size - (out) Maximum Reassembled Input Buffer Size
 *
 * @return RET_OK       - TLV is found and parsed successfully
 *         RET_ERROR    - some error occurred
 */
zb_ret_t zb_tlv_parse_value_fragmentation_parameters(zb_uint8_t *tlv_ptr,
                                                     zb_uint8_t tlv_data_len,
                                                     zb_uint16_t *short_addr,
                                                     zb_uint8_t *frag_opt,
                                                     zb_uint16_t *max_incoming_transfer_size);

#ifdef ZB_DIRECT_ENABLED
/**
 * Parse Device Capability Extension Global TLV.
 *
 * @param tlv_ptr       - pointer on the TLV
 * @param tlv_data_len  - length of the data with TLVs
 * @param node_id       - (out) device capability extension value
 *
 * @return RET_OK       - TLV is found and parsed successfully
 *         RET_ERROR    - some error occurred
 */
zb_ret_t zb_tlv_parse_value_device_capability_extension_parameters(zb_uint8_t *tlv_ptr,
                                                                   zb_uint8_t tlv_data_len,
                                                                   zb_uint16_t *capability_extension);
#endif /* ZB_DIRECT_ENABLED */

/**
 * Parse Router Information Global TLV.
 *
 * @param tlv_ptr      - pointer on the TLV
 * @param tlv_data_len - length of the data with TLVs
 * @param router_info  - (out) bitmask indicating data about the local router
 *
 * @return RET_OK      - TLV is found and parsed successfully
 *         RET_ERROR   - some error occurred
 */
zb_ret_t zb_tlv_parse_value_router_information(zb_uint8_t *tlv_ptr,
                                              zb_uint8_t tlv_data_len,
                                              zb_uint16_t *router_info);

/**
 * Parse Configuration Mode Parameters TLV.
 *
 * @param tlv_ptr      - pointer on the TLV
 * @param tlv_data_len - length of the data with TLVs
 * @param bitmask      - (out) bitmask indicating data
 *
 * @return RET_OK      - TLV is found and parsed successfully
 *         RET_ERROR   - some error occurred
 */
zb_ret_t zb_tlv_parse_value_conf_mode_param(zb_uint8_t *tlv_ptr,
                                            zb_uint8_t tlv_data_len,
                                            zb_uint16_t *bitmask);

/**
 * Parse Next Channel Change Global TLV
 *
 * @param tlv_ptr      - pointer on the TLV
 * @param tlv_data_len - length of the data with TLVs
 * @param channel_mask - (out) channel mask
 *
 * @return RET_OK      - TLV is found and parsed successfully
 *         RET_ERROR   - some error occurred
 */
zb_ret_t zb_tlv_parse_value_next_channel_change(zb_uint8_t *tlv_ptr,
                                                zb_uint8_t tlv_data_len,
                                                zb_uint32_t *channel_mask);

/**
 * Parse Next PAN ID Change Global TLV
 *
 * @param tlv_ptr      - pointer on the TLV
 * @param tlv_data_len - length of the data with TLVs
 * @param pan_id       - (out) pan id
 *
 * @return RET_OK      - TLV is found and parsed successfully
 *         RET_ERROR   - some error occurred
 */
zb_ret_t zb_tlv_parse_value_next_pan_id(zb_uint8_t *tlv_ptr,
                                                zb_uint8_t tlv_data_len,
                                                zb_uint16_t *pan_id);

/**
 * Put Configuration Mode Parameters TLV at the end of the packet.
 *
 * @param param                 - buffer id
 * @param bitmask               - configuration parameters bitmask
 */
void zb_tlv_put_value_conf_mode_param(zb_uint8_t param,
                                      zb_uint16_t *bitmask);

/**
 * Put Next Channel Change Global TLV at the end of the packet.
 *
 * @param param                 - buffer id
 * @param channel_mask          - channel mask
 */
void zb_tlv_put_value_next_channel_change(zb_uint8_t param,
                                          zb_uint32_t *channel_mask);

/**
 * Put Next PAN ID Change Global TLV at the end of the packet.
 *
 * @param param                 - buffer id
 * @param pan_id                - pan id
 */
void zb_tlv_put_value_next_pan_id(zb_uint8_t param,
                                  zb_uint16_t *pan_id);

void zb_tlv_put_panid_conflict_report(zb_uint8_t param);


#if defined ZB_JOIN_CLIENT

/**
 * Parse 128-bit Symmetric Passphrase Global TLV.
 *
 * @param tlv_ptr      - pointer on the TLV
 * @param tlv_data_len - length of the data with TLVs
 * @param passphrase   - (out) passphrase
 *
 * @return RET_OK      - TLV is found and parsed successfully
 *         RET_ERROR   - some error occurred
 */
zb_ret_t zb_tlv_parse_value_symmetric_passphrase(zb_uint8_t *tlv_ptr,
                                                 zb_uint8_t tlv_data_len,
                                                 zb_uint8_t *passphrase);

#endif /* ZB_JOIN_CLIENT */

/**
 * Parse Key Negotiation Req/Rsp Curve25519 Public Point TLV.
 *
 * @param tlv_ptr      - pointer on the TLV
 * @param tlv_data_len - length of the data with TLVs
 * @param ieee_addr    - (out) Source Device EUI64
 * @param public_point - (out) Public Point
 *
 * @return RET_OK      - TLV is found and parsed successfully
 *         RET_ERROR   - some error occurred
 */
zb_ret_t prs_vl_st_key_neg_rq_rsp_pb_pt(zb_uint8_t *tlv_ptr,
                                        zb_uint8_t tlv_data_len,
                                        zb_ieee_addr_t ieee_addr,
                                        zb_uint8_t *public_point);

#define zb_tlv_parse_value_start_key_neg_req_rsp_public_point prs_vl_st_key_neg_rq_rsp_pb_pt

/**
 * Parse Key Negotiation Req/Rsp Public Point TLV.
 *
 * @param tlv_ptr      - pointer on the TLV
 * @param tlv_data_len - length of the data with TLVs
 * @param ieee_addr    - (out) Source Device EUI64
 * @param public_point - (out) Public Point
 * @param curve_id     - ID of a curvem used in current key negotiation method
 * @param is_dlk       - if true, uses dlk tlvs, otherwise zigbee direct
 *
 * @return RET_OK      - TLV is found and parsed successfully
 *         RET_ERROR   - some error occurred
 */
zb_ret_t prs_vl_st_keyn_rq_rsp_pb_pt_cmn(const zb_uint8_t *tlv_ptr,
                                         zb_uint8_t tlv_data_len,
                                         zb_ieee_addr_t ieee_addr,
                                         zb_uint8_t *public_point,
                                         zb_uint8_t curve_id,
                                         zb_bool_t is_dlk);

#define zb_tlv_parse_value_start_key_neg_req_rsp_public_point_common prs_vl_st_keyn_rq_rsp_pb_pt_cmn

/**
 * Parse Key Negotiation Request Network Key Sequence Number.
 *
 * @param tlv_ptr               pointer on the TLV
 * @param tlv_data_len          length of the data with TLVs
 * @param [out] nwk_key_seq_num address to store found network key sequence number in
 *
 * @return RET_OK     TLV is found and parsed successfully
 *         RET_ERROR  some error occurred
 */
zb_ret_t zb_tlv_direct_parse_value_start_key_neg_req_rsp_nwk_key_seq_num(
    const zb_uint8_t *tlv_ptr, zb_uint8_t tlv_data_len, zb_uint8_t *nwk_key_seq_num);

#if defined ZB_JOIN_CLIENT

/**
 * Parse Key Negotiation Req Selected Key Negotiation Method TLV.
 *
 * @param tlv_ptr                 - pointer on the TLV
 * @param tlv_data_len            - length of the data with TLVs
 * @param ieee_addr               - (out) Source Device EUI64
 * @param selected_key_neg_method - (out) Selected Key Enumeration
 * @param selected_psk_secret     - (out) Selected Pre-shared Secret Enumeration
 *
 * @return RET_OK                 - TLV is found and parsed successfully
 *         RET_ERROR              - some error occurred
 */
zb_ret_t zb_tlv_parse_value_selected_key_neg_method(zb_uint8_t *tlv_ptr,
                                                    zb_uint8_t tlv_data_len,
                                                    zb_ieee_addr_t ieee_addr,
                                                    zb_uint8_t *selected_key_neg_method,
                                                    zb_uint8_t *selected_psk_secret);
/**
 * Parse Key Negotiation Req Selected Key Negotiation Method TLV. For Zigbee Direct
 *
 * @param tlv_ptr                 - pointer on the TLV
 * @param tlv_data_len            - length of the data with TLVs
 * @param ieee_addr               - (out) Source Device EUI64
 * @param selected_key_neg_method - (out) Selected Key Enumeration
 *
 * @return RET_OK                 - TLV is found and parsed successfully
 *         RET_ERROR              - some error occurred
 */
zb_ret_t zb_tlv_direct_parse_value_selected_key_neg_method(const zb_uint8_t *tlv_ptr,
                                                           zb_uint8_t tlv_data_len,
                                                           zb_uint8_t *selected_key_neg_method,
                                                           zb_uint8_t *selected_secret);

#endif /* ZB_JOIN_CLIENT */

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE

/**
 * Parse Requested Authentication Token ID TLV.
 *
 * @param tlv_ptr       - pointer on the TLV
 * @param tlv_data_len  - length of the data with TLVs
 * @param auth_token_id - (out) Requested Authentication Token ID
 *
 * @return RET_OK       - TLV is found and parsed successfully
 *         RET_ERROR    - some error occurred
 */
zb_ret_t zb_tlv_parse_value_authentication_token(zb_uint8_t *tlv_ptr,
                                                 zb_uint8_t tlv_data_len,
                                                 zb_uint8_t *auth_token_id);

#endif /* ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE */

#endif /* ZB_TLV_H */
