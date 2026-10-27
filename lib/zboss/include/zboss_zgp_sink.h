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
/* PURPOSE: ZGP Sink public header
*/

#ifndef ZBOSS_ZGP_SINK_H
#define ZBOSS_ZGP_SINK_H 1

#include "zboss_api.h"

#ifdef ZB_ENABLE_ZGP_SINK

/* ZGP spec, A.3.3.2.7 */
typedef enum zgp_gps_functionality_e {
  ZGP_GPS_GP_FEATURE                               = (1 <<  0),
  ZGP_GPS_DIRECT_COMMUNICATION                     = (1 <<  1),
  ZGP_GPS_DERIVED_GROUPCAST_COMMUNICATION          = (1 <<  2),
  ZGP_GPS_PRECOMMISSIONED_GROUPCAST_COMMUNICATION  = (1 <<  3),
  ZGP_GPS_FULL_UNICAST_COMMUNICATION               = (1 <<  4),
  ZGP_GPS_LIGHTWEIGHT_UNICAST_COMMUNICATION        = (1 <<  5),
  ZGP_GPS_PROXIMITY_BIDIRECTIONAL_OPERATION        = (1 <<  6),
  ZGP_GPS_MULTIHOP_BIDIRECTIONAL_OPERATION         = (1 <<  7),
  ZGP_GPS_PROXY_TABLE_MAINTENANCE                  = (1 <<  8),
  ZGP_GPS_PROXIMITY_COMMISSIONING                  = (1 <<  9),
  ZGP_GPS_MULTIHOP_COMMISSIONING                   = (1 << 10),
  ZGP_GPS_CT_BASED_COMMISSIONING                   = (1 << 11),
  ZGP_GPS_MAINTENANCE_OF_GPD                       = (1 << 12),
  ZGP_GPS_SEC_LEVEL_NO_SECURITY                    = (1 << 13),
  ZGP_GPS_SEC_LEVEL_REDUCED                        = (1 << 14),
  ZGP_GPS_SEC_LEVEL_FULL_NO_ENC                    = (1 << 15),
  ZGP_GPS_SEC_LEVEL_FULL_WITH_ENC                  = (1 << 16),
  ZGP_GPS_SINK_TABLE_BASED_GROUPCAST_FORWARDING    = (1 << 17),
  ZGP_GPS_TRANSLATION_TABLE                        = (1 << 18),
  ZGP_GPS_GPD_IEEE_ADDRESS                         = (1 << 19)
} zgp_gps_functionality_t;

/**
 * @brief Check that sink support requested functionality
 *
 * @param rfb [in]  Requested functionality
 *
 * @return ZB_TRUE if requested functionality is supported, ZB_FALSE otherwise
 *
 * @see ZGP spec, A.3.3.2.7
 */
zb_bool_t zgp_sink_is_support_functionality(zgp_gps_functionality_t gps_f);
#define ZB_ZGP_SINK_IS_SUPPORT_FUNCTIONALITY(f) zgp_sink_is_support_functionality(f)

/**
 * @brief Check that sink functionality support requested communication mode
 *
 * @param cm [in]  Requested communication mode
 *
 * @return ZB_TRUE if requested communication mode supported, ZB_FALSE otherwise
 *
 * @see ZGP spec, A.3.3.2.7
 */
zb_bool_t zgp_sink_is_support_communication_mode(zb_uint8_t cm);
#define ZB_ZGP_SINK_IS_SUPPORT_COMMUNICATION_MODE(cm) zgp_sink_is_support_communication_mode(cm)

/* ZGP InvolveTC (A.3.3.2.6 gpsSecurityLevel attribute) */
void zb_zgp_notification_network_join_begins(zb_bool_t ic_based_join_type);
void zb_zgp_notification_network_join_done(void);

#ifdef ZB_ENABLE_ZGP_DIRECT
/**
 * @brief ZGP message status
 *
 * Possible values of Status field in zgp data confirmations,
 * responses and indications.
 *
 * Status codes are not specified in ZGP specification, only their names.
 * For ZGP status codes range 0x80-0xda is used since it was reserved and not
 * used in MAC status enumeration.
 */
typedef enum zb_zgp_status_e
{
  ZB_ZGP_STATUS_ENTRY_REPLACED = 0x80,
  ZB_ZGP_STATUS_ENTRY_ADDED    = 0x81,
  ZB_ZGP_STATUS_ENTRY_EXPIRED  = 0x82,
  ZB_ZGP_STATUS_ENTRY_REMOVED  = 0x83,
  ZB_ZGP_STATUS_TX_QUEUE_FULL  = 0x84,
}
zb_zgp_status_t;

typedef struct zb_zgps_send_cmd_params_s
{
  zb_uint8_t     cmd_id;
  zb_zgpd_id_t   zgpd_id;
  zb_ieee_addr_t ieee_addr;
  zb_time_t      lifetime;
  zb_uint8_t     tx_options;
  zb_uint8_t     handle;
}
zb_zgps_send_cmd_params_t;

/**
 * @brief Send provided packet to ZGPD
 *
 * Buffer data is command payload to send.
 * Other parameters are in the buffer tail (see @ref zb_zgps_send_cmd_params_t).
 *
 * @param param[in, out]    Reference to buffer.
 *
 * @note maximum length of data payload is @ref ZB_ZGP_TX_CMD_PLD_MAX_SIZE
 *
 * @note zb_gp_data_cfm is called from:
 *  - gp_data_req_send_cnf       to notify about status of adding data to tx_packet_info_queue;
 *  - notify_about_expired_entry to notify about expired entry;
 *  - zb_cgp_data_cfm            to notify about status from MAC layer.
 *
 * @note Status of confirm (ZGP TX queue is used) can be:
 *      ZB_ZGP_STATUS_ENTRY_REPLACED
 *      ZB_ZGP_STATUS_ENTRY_ADDED
 *      ZB_ZGP_STATUS_ENTRY_EXPIRED
 *      ZB_ZGP_STATUS_ENTRY_REMOVED
 *      ZB_ZGP_STATUS_TX_QUEUE_FULL
 *
 *      MAC_SUCCESS
 *
 * @note Status of confirm (ZGP TX queue is not used) can be:
 *      ZB_ZGP_STATUS_TX_QUEUE_FULL
 *
 *      MAC_SUCCESS
 *      MAC_NO_ACK
 *
 */
void zb_zgps_send_data(zb_bufid_t param);

void zb_zgps_read_attrs_command(
  zb_bufid_t param, zb_zgpd_id_t *dev, zb_uint16_t cluster_id,
  zb_uint16_t *attrs, zb_uint8_t attrs_len);

/**
 * @typedef zgp_runtime_app_tbl_ent_t
 * @brief Attribute field of attribute write command
 * @see ZGP spec, A.4.2.6.3
 */
typedef struct zb_gpdf_attr_write_fld_s
{
  zb_uint16_t attr_id;   /**< Attribute ID specific to cluster */
  zb_uint8_t attr_type;  /**< Attribute type (see @ref zcl_attr_type) */
  void* data_p;     /**< Attribute data */
}
zb_gpdf_attr_write_fld_t;

void zb_zgps_write_attrs_command(
  zb_bufid_t param, zb_zgpd_id_t *zgpd_id, zb_uint16_t cluster_id,
  zb_uint8_t attrs_size, zb_gpdf_attr_write_fld_t *attrs);
#endif  /* ZB_ENABLE_ZGP_DIRECT */

#endif /* ZB_ENABLE_ZGP_SINK */
#endif /* ZBOSS_ZGP_SINK_H */
