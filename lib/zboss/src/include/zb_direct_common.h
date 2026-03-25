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
/**
 *  PURPOSE: Common internal include file for ZigBee Direct
*/
#ifndef ZB_DIRECT_COMMON_H
#define ZB_DIRECT_COMMON_H 1

#ifdef ZB_DIRECT_ENABLED

#include "zb_direct_ble_platform.h"
#include "zboss_api_direct.h"

#define ZB_DIRECT_MAX_MAC_TAG_SIZE                          32
#define ZB_DIRECT_AUTH_STRING_LEN                           34

/* The NRF_SDH_BLE_GATT_MAX_MTU_SIZE must be defined in the platform/build/Nordic_nRF52840/gcc/Makefile.nsdk_zigbee_direct
and it SHOULD have the same size as the ZIGBEE_DIRECT_MAX_ATT_SIZE  */
#define ZIGBEE_DIRECT_MAX_ATT_SIZE                          248                                     /**< Maximal data size can be transmitted in one packet. */

typedef enum zbd_mac_tag_preamble_e
{
  ZBD_MAC_TAG_PREAMBLE_SE3,
  ZBD_MAC_TAG_PREAMBLE_SE4,
  ZBD_MAC_TAG_PREAMBLE_RESERVED_MAX
} zbd_mac_tag_preamble_t;

/* Opcodes from 6.5.1.1. Authenticate: Table 5. Payload of Session Establishment message */
typedef enum zbd_se_opcodes_e
{
  ZBD_SE1_OPCODE = 1,
  ZBD_SE2_OPCODE = 2,
  ZBD_SE3_OPCODE = 3,
  ZBD_SE4_OPCODE = 4
} zbd_se_opcodes_t;

/* Type for masking selected network parameters */
typedef enum nwk_param_type_e
{
  NWK_PARAM_EXT_PAN_ID = 0x00000001,
  NWK_PARAM_SHORT_PAN_ID = 0x00000002,
  NWK_PARAM_CHANNEL = 0x00000004,
  NWK_PARAM_NWK_KEY = 0x00000008,
  NWK_PARAM_LINK_KEY = 0x00000010,
  NWK_PARAM_DEVICE_TYPE = 0x00000020,
  NWK_PARAM_NWK_ADDR = 0x00000040,
  NWK_PARAM_JOINING_METHOD = 0x00000080,
  NWK_PARAM_IEEE_ADDR = 0x00000100,
  NWK_PARAM_TC_ADDR = 0x00000200,
  NWK_PARAM_STATUS_MAP = 0x00000400,
  NWK_PARAM_NWK_UPDATE_ID = 0x00000800,
  NWK_PARAM_ACTIVE_KEY_SEQ_NUM = 0x00001000,
  NWK_PARAM_ADMIN_KEY = 0x00002000,
  NWK_PARAM_STATUS_CODE = 0x00004000,
  NWK_PARAM_MANAGE_JOINERS_CMD = 0x00010000,
  NWK_PARAM_MANAGE_JOINERS_IEEE_ADDRESS = 0x00020000,
  NWK_PARAM_MANAGE_JOINERS_LINK_KEY = 0x00040000,
} zbd_nwk_param_type_t;

typedef zb_uint16_t zbd_data_len_t;

/**
 * @name ZBD ZDD Advert Flags
 * @anchor zbd_zdd_advert_flags_t
 * @brief ZBD ZDD Advert Flags
 */
/** @{ */
#define ZBD_ZDD_ADVERT_FLAGS_TUNNELING_SUPPORT 0b0001U
#define ZBD_ZDD_ADVERT_FLAGS_PERMIT_JOINING 0b0010U
/** @} */

/**
 * @brief Type for ZBD ZDD Advert Flags.
 *
 * @ref zbd_zdd_advert_flags_t were declared previously as enum
 */
typedef zb_uint8_t zbd_zdd_advert_flags_t;


typedef struct zbd_tunnel_ctx_s
{
  /* A buffer for TLVs received from a tunnel */
  zb_uint8_t tunnel_buffer[ZIGBEE_DIRECT_MAX_ATT_SIZE];
  zbd_data_len_t tunnel_data_size;
  zbd_data_len_t tunnel_data_offset;
  zb_bool_t tunnel_buffer_busy;
  zb_bool_t connected;
  zb_bool_t is_tunnel_out_available;
  zb_bool_t is_tunneling_enabled;
} zbd_tunnel_ctx_t;

#define ZIGBEE_DIRECT_SERVICE_UUID_SIZE 16U

/* Secure Service byte-array UUIDs */
extern const zb_uint8_t zbd_service_sec_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_sec_p256_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_sec_c25519_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];

/* Commissioning Service byte-array UUIDs */
extern const zb_uint8_t zbd_service_comm_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_comm_form_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_comm_join_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_comm_permit_join_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_comm_leave_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_comm_status_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_comm_manage_joiners_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_comm_identification_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_comm_finding_binding_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];

extern const zb_uint8_t zbd_service_tunnel_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];
extern const zb_uint8_t zbd_char_tunnel_uuid[ZIGBEE_DIRECT_SERVICE_UUID_SIZE];

/**
 * @brief Create session identifier (6.8.1.2.1. Session establishment Overview / SE flow figure)
 *
 * @param curve_id   - curve type used in negotiations
 * @param Ai         - initiator identity, IEEE EUI-64 of initiator in LE
 * @param Qi         - initiator public key point
 * @param Ar         - responder identity, IEEE EUI-64 of responder in LE
 * @param Qr         - responder public key point
 * @param output_identifier - generated session identifier
 */
zb_ret_t calculate_session_identifier(
  zb_uint8_t curve_id,
  const zb_uint8_t *Ai, const zb_uint8_t *Qi,
  const zb_uint8_t *Ar, const zb_uint8_t *Qr,
  zb_uint8_t *output_identifier);

/**
 * @brief Calculate the mac tag value (See Zigbee Direct Spec 6.8.1.2.3-4 SE Messages 3 & 4)
 *
 * @param selected_key_neg_method curve and hash, used in current negotiations
 * @param zdd_addr                zdd ieee address
 * @param zdd_public_key          zdd's public ecdh key
 * @param zvd_addr                zvd ieee address
 * @param zvd_public_key          zvd's public ecdh key
 * @param session_key             ccm key, derived during secure session establishment
 * @param preamble                preamble to be put to MAC data
 * @param output                  buffer to store calculated MAC Tag
 */
zb_ret_t zbd_secur_calc_mac_tag(
  zb_tlv_key_negotiation_methods_t selected_key_neg_method,
  const zb_ieee_addr_t zdd_addr, const zb_uint8_t* zdd_public_key,
  const zb_ieee_addr_t zvd_addr, const zb_uint8_t* zvd_public_key,
  const zb_uint8_t* session_key,
  zbd_mac_tag_preamble_t preamble, zb_uint8_t output[ZB_DIRECT_MAX_MAC_TAG_SIZE]);

/**
 * @brief Create authentication string for CCM* and write it to auth_string (ZigBee Direct Specification 6.4.3. Unique address)
 *
 * @param charact_tag characteristic, for which the auth string should be generated
 * @param auth_string [IN]:  must be a buffer of 2 * ( UUID_LEN + 1 ) bytes
 *                    [OUT]: computed authentication string
 */
zb_ret_t create_auth_string(zb_uint8_t charact_tag, zb_uint8_t auth_string[ZB_DIRECT_AUTH_STRING_LEN]);


/**
 * @brief Set Zigbee Direct Virtual Device capability (ZVD)
 *
 * @param zvd_capability ZVD capability flag
 */
void zbd_set_zigbee_virtual_device_capability(zb_bool_t zvd_capability);

zbd_tunnel_ctx_t* zbd_get_tunnel_context(zb_uint8_t ctx_id);

/* BLE MAC interface API */
void zb_mcps_data_request_ble(zb_uint8_t param);
void zb_mlme_get_request_ble(zb_uint8_t param);
void zb_mlme_set_request_ble(zb_uint8_t param);
void zb_mlme_reset_request_ble(zb_uint8_t param);
void zb_mlme_scan_request_ble(zb_uint8_t param);
void zb_mac_cancel_scan_ble(zb_uint8_t param);
void zb_mlme_associate_request_ble(zb_uint8_t param);
void zb_mlme_associate_response_ble(zb_uint8_t param);
void zb_mlme_poll_request_ble(zb_uint8_t param);
void zb_mlme_orphan_response_ble(zb_uint8_t param);
void zb_mlme_start_request_ble(zb_uint8_t param);
void zb_mcps_purge_indirect_queue_request_ble(zb_uint8_t param);
void zb_mac_resp_by_empty_frame_ble(zb_uint8_t param);

#define ZB_DIRECT_CTX() (ZG->zb_direct)
#define ZB_DIRECT_GET_ROLE() (ZB_DIRECT_CTX().zbd_capability)

#define ZB_DIRECT_IS_EPHEMERAL_SESSION_STARTED() (ZB_DIRECT_CTX().is_ephemeral_session_started)

#define ZB_DIRECT_IS_NEIGHBOR_ZVD_ROUTING_DISABLED(neighbor_entry) (  \
    ZB_DIRECT_GET_ROLE() == ZB_ZBD_CAPABILITY_ZDD &&                  \
    !ZB_DIRECT_CTX().is_zvd_routing_allowed &&                        \
    (zb_bool_t)ZB_DIRECT_CTX().is_neighbor_zvd(neighbor_entry)        \
    )

#define ZB_DIRECT_IS_ZVD_ROUTING_DISABLED() (                 \
    ZB_DIRECT_GET_ROLE() == ZB_ZBD_CAPABILITY_ZVD &&          \
    !ZB_DIRECT_CTX().is_zvd_routing_allowed)

#else /* ZB_DIRECT_ENABLED */


#define ZB_DIRECT_GET_ROLE() (0xffu)
#define ZB_DIRECT_IS_EPHEMERAL_SESSION_STARTED() (ZB_FALSE)

#define ZB_DIRECT_IS_NEIGHBOR_ZVD_ROUTING_DISABLED(neighbor_entry) (ZB_FALSE)

#define ZB_DIRECT_IS_ZVD_ROUTING_DISABLED() (ZB_FALSE)

#endif /* ZB_DIRECT_ENABLED */

#endif /* ZB_DIRECT_COMMON_H */
