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
/* PURPOSE: Private header for ZGP common internal declarations
*/

#ifndef ZB_ZGP_COMMON_H
#define ZB_ZGP_COMMON_H 1

#ifdef ZB_ENABLE_ZGP

#include "zboss_api_zgp.h"

/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/

#define ZGP_PROXY_LWADDR_INVALID_IDX ZB_ADDRESS_IEEE_REF_NONE
#define ZGP_PROXY_GROUP_INVALID_IDX 0xffff
#define ZGP_PROXY_GROUP_DERIVED_ALIAS 0xffff
#define ZGP_PROXY_GP_NOTIF_ITERATOR_INVALID_IDX 0xff

typedef ZB_PACKED_PRE struct zb_zgp_gp_proxy_info_s
{
  zb_uint16_t short_addr;
  zb_uint8_t  link;
} ZB_PACKED_STRUCT zb_zgp_gp_proxy_info_t;

/**
 * @brief Security key sub-field value of Extended NWK frame control field
 *        derived from ZGPD key type (see @ref zb_zgp_security_key_type_e)
 *
 * ZGP spec, A.1.4.1.3
 * The Security Key sub-field, if set to 0b1, indicates an individual key
 * (KeyType 0b100 or 0b111). If set to 0b0, it indicates a shared key
 * (KeyType 0b011, 0b01017 or 0b001) or no key.
 */
#define ZGP_KEY_TYPE_IS_INDIVIDUAL(kt)                                  \
  ((kt) == ZB_ZGP_SEC_KEY_TYPE_ZGPD_INDIVIDUAL                          \
   || (kt) == ZB_ZGP_SEC_KEY_TYPE_DERIVED_INDIVIDUAL)

/**
 * @brief MAC addressing fields of GPDF as they are transmitted over the air.
 *
 * This structure is used during security processing for
 * security level @ref ZB_ZGP_SEC_LEVEL_REDUCED.
 *
 * From ZGP spec, A.1.5.4.2 "Initialization":
 * Header = MAC sequence number || MAC addressing fields || NWK Frame Control || Extended NWK
 * Frame Control || SrcID.
 *
 * When ZGP or ZGPD is encrypting message, MAC addressing fields of the packet are not ready.
 * They will be calculated only on the MAC layer. This is a problem and dirty hack is implemented:
 * MAC addressing fields are filled at ZGP level.
 *
 * For incoming frames MAC addressing fields are just copied from incoming frames.
 *
 * This union contains MAC addressing fields of GPDF as they are transmitted over the air.
 */
typedef ZB_PACKED_PRE union zb_gpdf_mac_addr_flds_u
{
  ZB_PACKED_PRE struct zgp_mac_addr_flds_short_s
  {
    zb_uint16_t  dst_addr;
    zb_uint16_t  dst_pan_id;
  }
  ZB_PACKED_STRUCT s;

  ZB_PACKED_PRE struct zgp_mac_addr_flds_long_s
  {
    zb_ieee_addr_t  addr;
  }
  ZB_PACKED_STRUCT l;

  ZB_PACKED_PRE struct zgp_mac_addr_flds_combined_s
  {
    zb_uint16_t     dst_addr;
    zb_uint16_t     dst_pan_id;
    zb_ieee_addr_t  src_addr;
  }
  ZB_PACKED_STRUCT comb;

  zb_uint8_t in[12];         /**< MAC addressing fields of incoming frame */
}
zb_gpdf_mac_addr_flds_t;

/**
 * @brief Parsed values of GPDF frame
 *
 * Structure contains GPDF information that is needed by ZGP stub and
 * upper layers.
 */
typedef ZB_PACKED_PRE struct zb_gpdf_info_s
{
  zb_time_t                 recv_timestamp;    /**< Packet reception time */
  zb_gpdf_mac_addr_flds_t   mac_addr_flds;     /**< MAC addressing fields */
  zb_zgp_gp_proxy_info_t    proxy_info;        /**< Proxy info - short addr and link quality */
  zb_uint8_t                mac_addr_flds_len; /**< Length of data in MAC addressing fields */
  zb_uint8_t                mac_seq_num;       /**< MAC sequence number */
  zb_uint8_t                nwk_frame_ctl;     /**< NWK frame control */
  zb_uint8_t                nwk_ext_frame_ctl; /**< Extended NWK frame control */
  zb_zgpd_id_t              zgpd_id;           /**< ZGPD ID */
  zb_uint32_t               sec_frame_counter; /**< Security frame counter */
  zb_uint8_t                zgpd_cmd_id;       /**< ZGPD command ID */
  zb_uint8_t                nwk_hdr_len;       /**< Length of the GPDF NWK header */
  zb_uint8_t                lqi;               /**< LQI value from alien MAC */
  zb_int8_t                 rssi;              /**< RSSI value from alien MAC */
  zb_uint8_t                status;            /**< 'status' for GP-DATA.indication @see  */
  zb_uint8_t                key[ZB_CCM_KEY_SIZE];/**< Key to decrypt the frame */
  zb_uint8_t                key_type;       /**< type of key to be used for
                                                 that frame @see zb_zgp_security_key_type_e */
  zb_uint8_t                mic[ZB_CCM_M];
  zb_bitfield_t             rx_directly:1;       /**< If 1, received directly by
                                                   * GP stub, else got from GPP */
  zb_bitfield_t             recv_as_unicast:1; /**< Unicast mode of
                                                   * packet received via GP Notification. */
  /* WARNING: Aligned to 62 bytes here - struct is not stored in NVRAM so it is ok.
     Aligning to 64 bytes (closest multiple of 4) brings strange errors and overlapping of data!
     TODO: Debug overlapping. Possibly it is better to divide this struct to some parts and do not
     play with such big structure in the packet tail. Here we have timestamps, lqi/rssi, mac, nwk
     and security payloads at the same structure!
     EE: maybe, can win 1 byte by using bitfield for key type so we can remove 1 byte of align?
     Also, recheck twice: do we need all that info in GPPB? Some can came from Target- and can be not used now.
     TC: If unaligned, the following allocation may lead to memory corruption:
        zgp_gp_notif_iterator_t *it = zb_buf_get_tail(param, sizeof(zgp_gp_notif_iterator_t) + sizeof(zb_gpdf_info_t));
        zb_gpdf_info_t *gpdf_info = ZB_BUF_GET_PARAM(param, zb_gpdf_info_t);
      The zb_buf_get_tail always returns aligned pointer, so if the zb_gpdf_info_t is unaligned, the gpdf_info pointer
      will point to some bytes inside the zgp_gp_notif_iterator_t.
      There are around 8 allocations of this type inside zgp_sink.c and zgp_cluster_gp.c files.
  */
}
ZB_PACKED_STRUCT zb_gpdf_info_t;

typedef ZB_PACKED_PRE struct zb_zgp_aes_nonce_s
{
  ZB_PACKED_PRE union zb_zgp_nonce_source_addr_u
  {
    zb_64bit_addr_t  ieee_addr;
    zb_uint32_t      splitted_addr[2];
  } src_addr;

  zb_uint32_t       frame_counter;
  zb_uint8_t        security_control;
}
ZB_PACKED_STRUCT zb_zgp_aes_nonce_t;

#ifdef ZB_TRACE_LEVEL
/**
 * @brief dump @ref zb_gpdf_info_t structure into trace log
 */
void zb_zgp_dump_gpdf_info(zb_gpdf_info_t *gpdf);
#define ZB_DUMP_GPDF_INFO(gpdf) zb_zgp_dump_gpdf_info(gpdf)
/**
 * @brief dump @ref zb_zgpd_id_t structure into trace log
 */
void zb_zgp_dump_zgpd_id(zb_zgpd_id_t *id);
#define ZB_DUMP_ZGPD_ID(id) zb_zgp_dump_zgpd_id(&(id))
#else
#define ZB_DUMP_GPDF_INFO(gpdf)
#define ZB_DUMP_ZGPD_ID(id)
#endif  /* ZB_TRACE_LEVEL */

/**
 * @brief Get duplicate filtering counter from GPDF
 *
 * ZGP spec, A.3.6.1.2:
 * If the ZGPD command used SecurityLevel 0b00, the filtering of duplicate
 * ZGPD messages is based on the MAC sequence number of a particular ZGPD,
 * identified by ZGPD SrcID. If the ZGPD command used SecurityLevel 0b01,
 * 0b10 or 0b11, then the filtering of duplicate messages is performed
 * based on the ZGPD security frame counter.
 */
#define ZB_GPDF_INFO_GET_DUP_COUNTER(gpdf_info) \
  ((ZB_GPDF_EXT_NFC_GET_SEC_LEVEL((gpdf_info)->nwk_ext_frame_ctl) == 0) ? \
                 (gpdf_info)->mac_seq_num : (gpdf_info)->sec_frame_counter)

#define ZB_ZGP_CLUSTER_SET_DUP_COUNTER(counter, gpdf_info) \
  (gpdf_info)->mac_seq_num = ((counter) & 0xFF);\
  (gpdf_info)->sec_frame_counter = (counter);

/********************************************************************/
/************ Sizes of different GPDF variable fields ***************/
/********************************************************************/
/**
 * @brief Extended NWK frame control size based on ZGPD context
 *
 * ZGP spec, A.1.4.1.3 (about Extended NWK frame control presence):
 * It shall be present if the ApplicationID different than 0b000...
 * For ApplicationID 0b000 (ZGP), the Extended NWK Frame Control field
 * shall be present if the GPDF is protected, if RxAfterTx is set, or
 * if the GPDF is sent to the ZGPD.
 */
#define ZGPD_EXTENDED_NWK_FRAME_CTL_SIZE(frame_type, _sec_level) \
  (((frame_type == ZGP_FRAME_TYPE_DATA) && \
   ((ZGPD->id.app_id != ZB_ZGP_APP_ID_0000) || \
   (ZGPD->commissioning_method == ZB_ZGPD_COMMISSIONING_BIDIR) || \
   ((_sec_level) > 0) || ZGPD->ext_nwk_present)) \
   ? 1 : 0)

/**
 * @brief GPDF SrcID field size
 *
 * ZGP spec, A.1.4.1.4:
 * The ZGPDSrcID field is present if the FrameType sub-field is set to 0b00
 * and the ApplicationID sub-field of the Extended NWK Frame Control field
 * is set to 0b000 (or not present)
 */
#define ZGPD_SRC_ID_SIZE(app_id, frame_type) \
  (((app_id == ZB_ZGP_APP_ID_0000) && (frame_type == ZGP_FRAME_TYPE_DATA)) ? 4 : 0)

/* ZGP spec, 1.4.1.3:
 *
 * If the SecurityLevel is set to 0b00, the fields Security frame counter
 * and MIC are not present. ...
 * If the SecurityLevel is set to 0b01, the Security Frame counter field is
 * not present and the MIC field is present, has the length of 2B ...
 * If the SecurityLevel is set to 0b10 or 0b11, the Security Frame counter field
 * is present, has the length of 4B, and carries the full 4B security frame counter,
 * the MIC field is present, has the length of 4B.
 */

/**
 * @brief Size of Security frame counter field
 */
#define GPDF_SECURITY_FRAME_COUNTER_SIZE(sec_level) \
  ((sec_level > ZB_ZGP_SEC_LEVEL_REDUCED) ? 4 : 0)

/**
 * @brief Size of MIC field
 */
#define ZB_GPDF_MIC_SIZE(sec_level) \
  ((sec_level > 0) ? ((sec_level == ZB_ZGP_SEC_LEVEL_REDUCED) ? 2 : 4) : 0)

/********************************************************************/
/************ Macros for filling GPDF fields with values ************/
/********************************************************************/

/**
 * @brief Construct GPDF NWK Frame control from given values
 */
#define ZB_GPDF_NWK_FRAME_CONTROL(frame_ctl, frame_type, auto_comm, frame_ext) \
  (frame_ctl) = (  (frame_type) \
               | (ZB_ZGP_PROTOCOL_VERSION << 2) \
               | ((auto_comm) << 6) \
               | ((frame_ext) << 7))

/**
 * @brief Construct GPDF Extended NWK Frame control for ZGPD outgoing frame
 */
#define ZB_GPDF_NWK_FRAME_CTL_EXT(ext_frame_ctl, app_id, sec_level, sec_key, rx_after_tx, dir) \
  ext_frame_ctl = (  (app_id) \
                   | ((sec_level) << 3) \
                   | (!!(sec_key) << 5) \
                   | (!!(rx_after_tx) << 6) \
                   | (!!(dir) << 7))

/********************************************************************/
/********** Get/set macros for individual bit sub-fields ************/
/********************************************************************/

#define ZB_GPDF_NFC_GET_NFC_EXT(frame_ctl) (((frame_ctl) & 0x80) >> 7)

#define ZB_GPDF_NFC_SET_NFC_EXT(frame_ctl, extension) ((frame_ctl) |= ((!!(extension)) << 7))

#define ZB_GPDF_NFC_GET_FRAME_TYPE(frame_ctl) ((frame_ctl) & 0x03)

#define ZB_GPDF_NFC_SET_FRAME_TYPE(frame_ctl, frame_type) ((frame_ctl) |= ((frame_type) & 0x03))

#define ZB_GPDF_NFC_GET_AUTO_COMMISSIONING(frame_ctl) (((frame_ctl) >> 6) & 0x01)

#define ZB_GPDF_NFC_SET_AUTO_COMMISSIONING(frame_ctl, auto_comm) ((frame_ctl) |= ((!!(auto_comm)) << 6))

#define ZB_GPDF_EXT_NFC_GET_APP_ID(ext_fc) ((ext_fc) & 0x03)

#define ZB_GPDF_EXT_NFC_SET_APP_ID(ext_fc, app_id) ((ext_fc) |= ((app_id) & 0x07))

#define ZB_GPDF_EXT_NFC_GET_SEC_LEVEL(ext_fc) (((ext_fc) >> 3) & 0x03)

#define ZB_GPDF_EXT_NFC_SET_SEC_LEVEL(ext_fc, lvl) ((ext_fc) |= (((lvl) & 0x03) << 3))

#define ZB_GPDF_EXT_NFC_GET_SEC_KEY(ext_fc) (((ext_fc) >> 5) & 0x01)

#define ZB_GPDF_EXT_NFC_SET_SEC_KEY(ext_fc, sec_key) ((ext_fc) |= ((!!(sec_key)) << 5))

#define ZB_GPDF_EXT_NFC_GET_RX_AFTER_TX(ext_fc) (((ext_fc) >> 6) & 0x01)

#define ZB_GPDF_EXT_NFC_SET_RX_AFTER_TX(ext_fc, rx_after_tx) ((ext_fc) |= ((!!(rx_after_tx))<<6))

#define ZB_GPDF_EXT_NFC_CLR_RX_AFTER_TX(ext_fc) (ext_fc) &= ~(1<<6)

#define ZB_GPDF_EXT_NFC_GET_DIRECTION(ext_fc) (((ext_fc) & 0x80) >> 7)

#define ZB_GPDF_EXT_NFC_SET_DIRECTION(ext_fc, dir) ((ext_fc) |= ((!!(dir))<<7))

/* GP security key generator with given key_type, see A.3.7.1.2. Table 48 values of gpSecurityKeyType */
zb_ret_t zb_zgp_key_gen(enum zb_zgp_security_key_type_e security_key_type, zb_zgpd_id_t *zgpd_id, zb_uint8_t *oob, zb_uint8_t *key);

zb_ret_t zb_zgp_protect_frame(
    zb_gpdf_info_t *gpdf_info,
    zb_uint8_t *key,
    zb_bufid_t packet);

zb_ret_t zb_zgp_decrypt_and_auth(zb_bufid_t param);

void zb_zgp_protect_gpd_key(
    zb_bool_t from_gpd,
    zb_zgpd_id_t *zgpd_id,
    zb_uint8_t *key_to_encrypt,
    zb_uint8_t *key_encrypt_with,
    zb_uint8_t *crypted_key,
    zb_uint32_t security_frame_counter,
    zb_uint8_t *mic);

zb_ret_t zb_zgp_decrypt_n_auth_gpd_key(
    zb_bool_t from_gpd,
    zb_zgpd_id_t *zgpd_id,
    zb_uint8_t *key_decrypt_with,
    zb_uint8_t *crypted_key,
    zb_uint32_t security_frame_counter,
    zb_uint8_t *mic,
    zb_uint8_t *plain_key);

zb_uint8_t zgp_parse_gpdf_nwk_hdr(zb_uint8_t *gpdf, zb_uint8_t gpdf_len, zb_gpdf_info_t *gpdf_info);

#ifndef ZB_ZGPD_ROLE

typedef struct zb_zgp_tbl_array_s
{
  zb_uint32_t back_sec_counter;
  zb_uint32_t security_counter;
  zb_uint32_t nvram_offset;
  zb_uint8_t  lqi;
  zb_int8_t   rssi;

  /* bit 0 - EntryValid flag
   * bit 1 - FirstToForward flag
   * bit 2 - HasAllUnicastRoutes flag
   * bits 3-7 reserved
 */
  zb_uint8_t runtime_options;
  zb_uint8_t search_counter;
#ifdef ZB_ZGP_ENABLE_TBL_RAM_STORAGE
  zgp_tbl_ent_t tbl_entry;
#endif
} zb_zgp_tbl_array_t;

#if defined ZB_ENABLE_ZGP_SINK && defined ZB_ENABLE_ZGP_PROXY
#define ZB_ZGP_SEC_CNT_TIMEOUT_TBL_SIZE (((ZB_ZGP_PROXY_TBL_SIZE > ZB_ZGP_SINK_TBL_SIZE) ? ZB_ZGP_PROXY_TBL_SIZE : ZB_ZGP_SINK_TBL_SIZE + 7) / 8)
#else
#ifdef ZB_ENABLE_ZGP_SINK
#define ZB_ZGP_SEC_CNT_TIMEOUT_TBL_SIZE ((ZB_ZGP_SINK_TBL_SIZE + 7) / 8)
#endif  /* ZB_ENABLE_ZGP_SINK */
#ifdef ZB_ENABLE_ZGP_PROXY
#define ZB_ZGP_SEC_CNT_TIMEOUT_TBL_SIZE ((ZB_ZGP_PROXY_TBL_SIZE + 7) / 8)
#endif  /* ZB_ENABLE_ZGP_PROXY */
#endif  /* defined ZB_ENABLE_ZGP_SINK && defined ZB_ENABLE_ZGP_PROXY */

typedef struct zb_zgp_tbl_s
{
  zgp_tbl_ent_t cached;
  /* pack 8 4-bit entries to every array element */
  zb_uint32_t security_counter_timeouts[ZB_ZGP_SEC_CNT_TIMEOUT_TBL_SIZE];
  zb_uint_t cached_i;
  zb_uint_t entry_size;
  zb_uint_t tbl_size;
  zb_uint8_t nvram_dataset;     /*<! @see zb_nvram_dataset_types_t  */
  zb_uint8_t nvram_page;

  zb_zgp_tbl_array_t array[1];
} zb_zgp_tbl_t;

zb_bool_t age_table(zb_zgp_tbl_t *tbl);

#ifdef ZB_ENABLE_ZGP_DIRECT
typedef ZB_PACKED_PRE struct zb_cgp_data_req_s
{
  zb_uint8_t      tx_options;
  zb_uint8_t      src_addr_mode;
  zb_uint16_t     src_pan_id;
  zb_addr_u src_addr;
  zb_uint8_t      dst_addr_mode;
  zb_uint16_t     dst_pan_id;
  zb_addr_u dst_addr;
  zb_bufid_t      buf_ref;
  zb_uint8_t      handle;
  zb_time_t       recv_timestamp;
}
ZB_PACKED_STRUCT
zb_cgp_data_req_t;
#endif  /* ZB_ENABLE_ZGP_DIRECT */

typedef enum gp_sec_resp_e
{
  GP_SEC_RESPONSE_DROP_FRAME,
  GP_SEC_RESPONSE_PASS_UNPROCESSED,
  GP_SEC_RESPONSE_TX_THEN_DROP,
  GP_SEC_RESPONSE_MATCH,
  GP_SEC_RESPONSE_TX_THEN_PASS_COMMISSIONING_UNPROCESSED,
  GP_SEC_RESPONSE_ILLEGAL = 0xff
} gp_sec_resp_t;

typedef enum gp_data_ind_status_e
{
  GP_DATA_IND_STATUS_SECURITY_SUCCESS,
  GP_DATA_IND_STATUS_NO_SECURITY,
  GP_DATA_IND_STATUS_AUTH_FAILURE,
  GP_DATA_IND_STATUS_UNPROCESSED,
  GP_DATA_IND_STATUS_COMMISSIONING_UNPROCESSED
} gp_data_ind_status_t;

/*
  entry with the exact GPD ID as in the GPDF and Endpoint = 0xff, or - if the
  Endpoint in the GP-DATA.indication is 0xff  or 0x00 - if it has an entry with
  the exact GPD ID
*/
#define GP_ENPOINT_MATCH(gpdf_ep, table_ep)     \
  ((gpdf_ep) == (table_ep)                      \
   || (table_ep) == 0xff                        \
    || (gpdf_ep) == 0                           \
    || (gpdf_ep) == 0xff)

/* ZGP spec, A.3.6.1.3.1:
 *
 * For the incremental sequence number (when SecurityLevel = 0b00), the
 * counter must be allowed to roll over, because of the limited
 * sequence number length of 1 octet, so care must be taken when
 * comparing for freshness.  It is recommended that this comparison be
 * accomplished as follows:
 * define
 * a = sequence number stored by the GPP/GPS;
 * b = sequence number from the GPDF;
 * if 1 ≤ ( (b – a)mod 256 ) < 128 accept GPDF;
 * else drop GPDF;
 */

/* NK: Skip freshness check for some special cases, for example, to do not drop packets after ZGP
 * reset. This behaviour is not clear in general, seems like it is temporary feature. */
#ifdef ZB_ZGP_SKIP_FRESHNESS_CHECK
#define ZB_ZGP_CHECK_MAC_SEQ_NUM_FRESHNESS(prev_sn, new_sn, ret)        \
{                                                                       \
  ret = ZB_TRUE;                                                        \
}
#else
#define ZB_ZGP_CHECK_MAC_SEQ_NUM_FRESHNESS(prev_sn, new_sn, ret)                                         \
{                                                                                                        \
  /* Modular arithmetic takes place here, result value is "wrap around" */                               \
  /* NK: Another overflow processing. */                                                                 \
  /*     For example, prev_sn is 125, new_sn is 9; in this case: */                                      \
  /*     (new_sn - prev_sn) = 9 - 125 (overflowed) = 255 - 116 = 139 > 128 - frame dropped  */           \
  /*     abs(new_sn - prev_sn) = 125 - 9 = 116 < 128 - frame passed  */                                  \
  /*     It is needed to avoid too many drop situations after ZGP reset.  */                             \
  /* NK:TODO: We do not store this counter in nvram, so when GW reset occurs, it will skip some pkts. */ \
  zb_uint8_t diff = (new_sn > prev_sn) ? (new_sn - prev_sn) : (prev_sn - new_sn);                        \
  /* Check that diff in seq nums falls into half of seq num value range */                               \
  ret = ((diff > 0) && (diff < 128)) ? ZB_TRUE : ZB_FALSE;                                               \
}
#endif

/********************************************************************/
/**************** ZGP Proxy/Sink table definitions ******************/
/********************************************************************/
#define ZGP_TBL_IS_SINK(ent) ((ent)->is_sink==ZB_TRUE)

#define ZGP_TBL_SINK_GET_SEC_PRESENT(ent) (((ent)->options & (1<<9)) ? 1 : 0)
#define ZGP_TBL_PROXY_GET_SEC_PRESENT(ent) (((ent)->options & (1<<14)) ? 1 : 0)

#define ZGP_TBL_GET_SEC_PRESENT(ent) ( ZGP_TBL_IS_SINK(ent) ? ZGP_TBL_SINK_GET_SEC_PRESENT(ent) : ZGP_TBL_PROXY_GET_SEC_PRESENT(ent))

#define ZGP_TBL_SINK_GET_SEC_KEY_TYPE(ent) (((ent)->options & (1<<9)) ? (((ent)->sec_options >> 2) & 7) : 0)
#define ZGP_TBL_PROXY_GET_SEC_KEY_TYPE(ent) (((ent)->options & (1<<14)) ? (((ent)->sec_options >> 2) & 7) : 0)

#define ZGP_TBL_GET_SEC_KEY_TYPE(ent) ( ZGP_TBL_IS_SINK(ent) ? ZGP_TBL_SINK_GET_SEC_KEY_TYPE(ent) : ZGP_TBL_PROXY_GET_SEC_KEY_TYPE(ent))

#define ZGP_SINK_GET_SEC_LEVEL(opt) ((opt) & 3)

#define ZGP_TBL_SINK_GET_SEC_LEVEL(ent)    (((ent)->options & (1<<9)) ? ZGP_SINK_GET_SEC_LEVEL((ent)->sec_options): 0)
#define ZGP_TBL_PROXY_GET_SEC_LEVEL(ent)    (((ent)->options & (1<<14)) ? ZGP_SINK_GET_SEC_LEVEL((ent)->sec_options): 0)

#define ZGP_TBL_GET_SEC_LEVEL(ent)    ( ZGP_TBL_IS_SINK(ent) ? ZGP_TBL_SINK_GET_SEC_LEVEL(ent) : ZGP_TBL_PROXY_GET_SEC_LEVEL(ent))

#define ZGP_TBL_GET_SEQ_NUM_CAP(ent) ((ent)->options & (1<<5))
#define ZGP_TBL_GET_APP_ID(ent) ((ent)->options & 7)
/**
   Get communication mode by Proxy/Sink table entry
   @see zgp_communication_mode_t
 */
#define ZGP_TBL_SINK_GET_COMMUNICATION_MODE(ent)    (((ent)->options >> 3) & 3)

#define ZGP_TBL_SINK_GET_FIXED_LOCATION(ent) (((ent)->options >> 7) & 1)
#define ZGP_TBL_PROXY_GET_FIXED_LOCATION(ent) (((ent)->options >> 11) & 1)

#define ZGP_TBL_GET_FIXED_LOCATION(ent)    ( ZGP_TBL_IS_SINK(ent) ? ZGP_TBL_SINK_GET_FIXED_LOCATION(ent) : ZGP_TBL_PROXY_GET_FIXED_LOCATION(ent))

#define ZGP_TBL_SINK_GET_ASSIGNED_ALIAS(ent) (((ent)->options >> 8) & 1)
#define ZGP_TBL_PROXY_GET_ASSIGNED_ALIAS(ent) (((ent)->options >> 13) & 1)

#define ZGP_TBL_SINK_SET_ASSIGNED_ALIAS(ent) (ent)->options |= (1<<8)
#define ZGP_TBL_PROXY_SET_ASSIGNED_ALIAS(ent) (ent)->options |= (1<<13)
#define ZGP_TBL_PROXY_CLR_ASSIGNED_ALIAS(ent) (ent)->options &= ~(1<<13)

#define ZGP_TBL_GET_ASSIGNED_ALIAS(ent)    ( ZGP_TBL_IS_SINK(ent) ? ZGP_TBL_SINK_GET_ASSIGNED_ALIAS(ent) : ZGP_TBL_PROXY_GET_ASSIGNED_ALIAS(ent))

#define ZGP_TBL_GET_INRANGE(ent)    (((ent)->options >> 10) & 1)
#define ZGP_TBL_SET_INRANGE(ent)    (ent)->options |= (1<<10)
#define ZGP_TBL_CLR_INRANGE(ent)    (ent)->options &= ~(1<<10)

#define ZGP_TBL_GET_ACTIVE(ent)     (((ent)->options >> 3) & 1)
#define ZGP_TBL_SET_ACTIVE(ent)     (ent)->options |= (1<<3)
#define ZGP_TBL_CLR_ACTIVE(ent)     (ent)->options &= ~(1<<3)

#define ZGP_TBL_GET_VALID(ent)      (((ent)->options >> 4) & 1)
#define ZGP_TBL_SET_VALID(ent)      (ent)->options |= (1<<4)
#define ZGP_TBL_CLR_VALID(ent)      (ent)->options &= ~(1<<4)

#define ZGP_TBL_GET_FIRST_TO_FORWARD(ent) (((ent)->options >> 9) & 1)
#define ZGP_TBL_SET_FIRST_TO_FORWARD(ent) (ent)->options |= (1<<9)
#define ZGP_TBL_CLR_FIRST_TO_FORWARD(ent) (ent)->options &= ~(1<<9)

#define ZGP_TBL_GET_HAS_ALL_UNICAST_ROUTES(ent) (((ent)->options >> 12) & 1)
#define ZGP_TBL_SET_HAS_ALL_UNICAST_ROUTES(ent) (ent)->options |= (1<<12)
#define ZGP_TBL_CLR_HAS_ALL_UNICAST_ROUTES(ent) (ent)->options &= ~(1<<12)

#define ZGP_TBL_SET_LWUC(ent, val)  (ent)->options = ((ent)->options & (~(1<<6))) | (val<<6)
#define ZGP_TBL_GET_LWUC(ent)       (((ent)->options >> 6) & 1)
#define ZGP_TBL_SET_DGGC(ent, val)  (ent)->options = ((ent)->options & (~(1<<7))) | (val<<7)
#define ZGP_TBL_GET_DGGC(ent)       (((ent)->options >> 7) & 1)
#define ZGP_TBL_SET_CGGC(ent, val)  (ent)->options = ((ent)->options & (~(1<<8))) | (val<<8)
#define ZGP_TBL_GET_CGGC(ent)       (((ent)->options >> 8) & 1)

#define ZGP_PROXY_TABLE_ENTRY_IS_EMPTY(ent)\
  (!(ZGP_TBL_GET_LWUC((ent)) || ZGP_TBL_GET_DGGC((ent)) || ZGP_TBL_GET_CGGC((ent))))

#define ZGP_TBL_GET_RXON_CAP0(ent)   (((ent)->options >> 6) & 1)

/*
Bits: 0-1       2-4             5-7
SecurityLevel   SecurityKeyType Reserved
*/
#define ZGP_TBL_FILL_SEC_OPTIONS(sec_lev, key_type) \
(((sec_lev) & 3) | (((key_type) & 7) << 2))

/********************************************************************/
/******************** ZGP TX Queue definitions **********************/
/********************************************************************/
#ifdef ZB_ENABLE_ZGP_DIRECT
#define USED_MASK(count) ((count)/8 + 1)

#ifndef ZB_ZGP_IMMED_TX
ZB_ASSERT_COMPILE_DECL(ZB_ZGP_TX_QUEUE_SIZE <= ZB_ZGP_TX_PACKET_INFO_COUNT);
#endif  /* ZB_ZGP_IMMED_TX */

#define TX_PACKET_INFO_QUEUE_USED_MASK USED_MASK(ZB_ZGP_TX_PACKET_INFO_COUNT)

#define ZB_ZGPD_CMD_ID_IS_PART_OF_COMMISSIONING_REPLIES(zgpd_cmd_id)      \
  (((zgpd_cmd_id) == ZB_GPDF_CMD_CHANNEL_CONFIGURATION) ||                \
   ((zgpd_cmd_id) == ZB_GPDF_CMD_COMMISSIONING_REPLY))

typedef struct zb_zgp_tx_pinfo_s
{
  zb_uint8_t                    handle;
  zb_zgpd_id_t                  zgpd_id;
  zb_bufid_t                    buf_ref;   /* Reference to buffer, which is used for GPDF */
  zb_bufid_t                    buf_ref_delayed; /* Reference to buffer, which is used for delayed gp_data_ind processing */
} zb_zgp_tx_pinfo_t;

typedef struct zb_zgp_tx_q_ent_s
{
  zb_uint8_t    tx_options;
  zb_uint8_t    cmd_id;
  zb_uint8_t    payload_len;
  zb_uint8_t    pld[ZB_ZGP_TX_CMD_PLD_MAX_SIZE]; /**< Payload */
  zb_bitfield_t is_expired:1;
  zb_bitfield_t sent:1;
  zb_bitfield_t reserved:6;
} zb_zgp_tx_q_ent_t;

typedef struct zb_zgp_tx_q_s
{
  zb_zgp_tx_q_ent_t queue[ZB_ZGP_TX_QUEUE_SIZE];
} zb_zgp_tx_q_t;

typedef struct zb_zgp_tx_packet_info_q_s
{
  zb_zgp_tx_pinfo_t queue[ZB_ZGP_TX_PACKET_INFO_COUNT];
  zb_uint8_t        used_mask[TX_PACKET_INFO_QUEUE_USED_MASK];
} zb_zgp_tx_packet_info_q_t;

enum zb_zgp_tx_packet_info_search_mode_e
{
  ZB_ZGP_TX_PACKET_INFO_ALL_PACKETS,
  ZB_ZGP_TX_PACKET_INFO_PENDING_PACKETS,
#ifdef ZB_ZGP_IMMED_TX
  ZB_ZGP_TX_PACKET_INFO_IMMED_PACKETS,
#endif  /* ZB_ZGP_IMMED_TX */
};

zb_uint8_t zb_zgp_tx_packet_info_q_find_pos(
    zb_zgp_tx_packet_info_q_t *tx_p_i_q,
    zb_zgpd_id_t *id,
    zb_uint8_t search_mode      /* zb_zgp_tx_packet_search_mode_e */
);

zb_uint8_t zb_zgp_tx_q_find_ent_pos_for_send(zb_zgp_tx_packet_info_q_t *tx_p_i_q,
    zb_zgp_tx_q_t *tx_q, zb_zgpd_id_t *id, zb_uint8_t type);

zb_uint8_t zb_zgp_tx_q_find_ent_pos_for_cfm(zb_zgp_tx_packet_info_q_t *tx_p_i_q,
                                            zb_bufid_t                 buf_ref);

zb_uint8_t zb_zgp_tx_q_find_expired_ent_pos(zb_zgp_tx_packet_info_q_t *tx_p_i_q,
    zb_zgp_tx_q_t *tx_q);

zb_uint8_t zb_zgp_tx_packet_info_q_grab_free_ent_pos(zb_zgp_tx_packet_info_q_t *tx_p_i_q,
    zb_uint8_t search_mode);

void zb_zgp_tx_packet_info_q_delete_ent(zb_zgp_tx_packet_info_q_t *tx_p_i_q, zb_uint8_t pos);

/**
 * @brief Delete any unsent commissioning-related entries from the gpTxQueue
 */
void zb_zgp_tx_q_delete_all_comm_ent(zb_zgp_tx_packet_info_q_t *tx_p_i_q, zb_zgp_tx_q_t *tx_q);

zb_bool_t zb_has_zgp_tx_packet_info_q_capacity_to_store(zb_zgp_tx_packet_info_q_t *tx_p_i_q,
    zb_uint8_t search_mode);

#define ZB_ZGP_TX_Q_FILLED_CNT(q, ret_cnt) \
  ret_cnt = zb_calc_non_zero_bits_in_bit_vector((q)->used_mask, USED_MASK_SIZE)

void zgp_clean_zgpd_info_from_queue(zb_bufid_t    buf_ref,
                                    zb_zgpd_id_t *zgpd_id,
                                    zb_uint8_t    handle);

#ifdef ZB_ENABLE_ZGP_TEST_HARNESS
#define ZGP_OUT_GPDF_INFO_GET_OPER_CHANNEL(ci) \
  (((ci) >> 4) + ZB_ZGPD_FIRST_CH)

#define ZGP_OUT_GPDF_INFO_SET_OPER_CHANNEL(ci, op) \
  (ci) = ((ci) & 0x0F) | (((op) - ZB_ZGPD_FIRST_CH) << 4)

#define ZGP_OUT_GPDF_INFO_GET_TEMP_CHANNEL(ci) \
  (((ci) & 0x0F) + ZB_ZGPD_FIRST_CH)

#define ZGP_OUT_GPDF_INFO_SET_TEMP_CHANNEL(ci, tp) \
  (ci) = ((ci) & 0xF0) | ((tp) - ZB_ZGPD_FIRST_CH)

enum zb_outgoing_gpdf_state_e
{
  ZB_OUTGOING_GPDF_STATE_GET_OPER_CHANNEL,
  ZB_OUTGOING_GPDF_STATE_SET_TEMP_CHANNEL,
  ZB_OUTGOING_GPDF_STATE_SET_OPER_CHANNEL
};

enum zb_outgoing_gpdf_status_e
{
  ZB_OUTGOING_GPDF_STATUS_SUCCESS,
  ZB_OUTGOING_GPDF_STATUS_PIB_ERROR,
  ZB_OUTGOING_GPDF_STATUS_ENC_ERROR,
};

typedef void (*zb_outgoing_gpdf_cb_t)(zb_cb_param_t param, zb_uint8_t status);

typedef struct zb_outgoing_gpdf_info_s
{
//internal
  zb_uint8_t     state;
  zb_uint8_t     tx_options;
  zb_uint8_t     channel_info;
  zb_uint8_t     channel;
  zb_bufid_t     buf_ref;
//gpdf
  zb_uint8_t     nwk_frame_ctl;
  zb_uint8_t     nwk_ext_frame_ctl;
  zb_zgpd_addr_t addr;
  zb_uint8_t     endpoint;
  zb_uint32_t    sec_frame_counter;
  zb_uint8_t     payload_len;
  zb_uint8_t     payload[ZB_ZGP_MAX_GPDF_CMD_PAYLOAD_APP_ID_0000];
  zb_outgoing_gpdf_cb_t cb;
} zb_outgoing_gpdf_info_t;

#define ZGP_GPDF_NWK_PUT_FCTL(ptr_, fctl_) \
  *(ptr_) = (fctl_); \
  (ptr_)++;

#define ZGP_GPDF_NWK_PUT_EFCTL(ptr_, efctl_) \
  *(ptr_) = (efctl_); \
  (ptr_)++;

#define ZGP_GPDF_NWK_PUT_SRC_ADDR(ptr_, addr_) \
  ZB_HTOLE32((ptr_), (addr_)); \
  (ptr_) += 4;

#define ZGP_GPDF_NWK_PUT_ENDP(ptr_, endp_) \
  *(ptr_) = (endp_); \
  (ptr_)++;

#define ZGP_GPDF_NWK_PUT_SFCNT(ptr_, sfcnt_) \
  ZB_HTOLE32((ptr_), (sfcnt_)); \
  (ptr_) += 4;

zb_ret_t zb_zgp_protect_out_gpdf(zb_bufid_t               buf_ref,
                                 zb_outgoing_gpdf_info_t *gpdf_info,
                                 zb_uint8_t              *key,
                                 zb_uint8_t               nwk_hdr_len);
zb_ret_t zgp_send_gpdf(zb_bufid_t buf_ref, zb_outgoing_gpdf_info_t *gpdf_info);
#endif  /* ZB_ENABLE_ZGP_TEST_HARNESS */
#endif  /* ZB_ENABLE_ZGP_DIRECT */

#define ZB_ZGP_SET_TEMP_CHANNEL_TRIES_MAX       5u
#define ZB_ZGP_SET_TEMP_CHANNEL_TRIES_DELAY_MS  25u

#ifndef ZB_ZGP_SINK_POSTPONED_DATA_FRAME_ARRAY_SIZE
#define ZB_ZGP_SINK_POSTPONED_DATA_FRAME_ARRAY_SIZE 3
#endif

/**
 * @brief Information about commissioning process with a ZGPD
 */
typedef struct zb_zgps_dev_comm_data_s
{
  zb_uint8_t     state;         /**< Current commissioning state \ref zb_zgp_comm_state_t */
  zb_zgpd_id_t   zgpd_id;       /**< ZGPD ID */
  zb_bool_t      approved;      /**< True, if application has already approved ZGPD */
  zb_uint8_t     oper_channel;  /**< ZGP physical operational channel */
  zb_uint8_t     temp_master_tx_chnl;  /**< Temp channel during commissioning */
#ifdef ZB_ENABLE_ZGP_DIRECT
  /* check if we are working on temp channel
   * Indicates that we have physically switched from the operational channel */
  zb_bool_t      is_work_on_temp_channel;
#endif  /* ZB_ENABLE_ZGP_DIRECT */
  zb_uint8_t     channel_conf_payload;
  zb_bool_t      channel_conf_sent;
  zb_bool_t      comm_reply_sent;
  zb_uint8_t     result;        /**< Commissioning result @ref zb_zgp_comm_status_t */
  /** Used for duplicate filtering during early stages of commissioning, when device is not
   * added to the sink table yet. Two frame types are filtered this way:
   * - Channel request frames (filtered by MAC sequence number). Also it is impossible to filter
   *   duplicates using sink table if Channel request is sent as Maintenance frame without concrete
   *   ZGPD ID.
   * - Commissioning frames (both secured and unsecured).
   */
  zb_uint32_t    comm_dup_counter;
  /* indicate for dup_counter check function that comm_dup_counter field have legal and valid value */
  zb_uint8_t     any_packet_received;

  /* A.3.3.5.3 The Options as shown in Figure 53 without action field */
  /*
    0-2:  exit mode
      3:  channel present
      4:  unicast communication
  */
  zb_uint8_t     proxy_comm_mode_options;
  zb_uint16_t    sink_addr;

  zb_uint8_t     gpdf_options;
  zb_uint8_t     gpdf_ext_options;
  zb_uint32_t    gpdf_security_frame_counter;

  zb_uint8_t     set_temp_channel_tries;      /**< Number of attempts to set temp channel, currently makes sense for DualPAN, as other stack can work with a transceiver */
  zb_time_t      set_temp_channel_timestamp;  /**< Timestamp when temp channel was set, to calculate proper GPDF queue timeout further */

#ifdef ZB_ENABLE_ZGP_SINK
  zb_uint8_t     selected_temp_master_idx;
  zb_zgp_gp_proxy_info_t temp_master_list[ZB_ZGP_MAX_TEMP_MASTER_COUNT];
  zb_uint8_t     need_send_dev_annce;
  /** Postponed data frames array. It is used to put here a buffer id
   *  that contains a data frame for the commissioned GPD device
   *  but the frame was received before we finished the commissioning procedure */
  zb_bufid_t     pdf_array[ZB_ZGP_SINK_POSTPONED_DATA_FRAME_ARRAY_SIZE];
  zb_bufid_t     comm_req_buf;  /**< Reference to buffer with Commissioning GPDF from ZGPD */
  /* Application information to be sent with GP Pairing Configuration */
  zb_gpdf_comm_app_info_t app_info;
#endif  /* ZB_ENABLE_ZGP_SINK */
} zb_zgps_dev_comm_data_t;

/* Structure stores GreenPower security and commissioning parameters */
typedef ZB_PACKED_PRE struct zb_zgp_cluster_s
{
  /* FIXME: Decrease memory consumed by: a) move this field after arrays; b) use bitfields where possible. */
  zb_uint8_t  gp_shared_security_key_type; /**< @see zb_zgp_shared_security_key_type_t */
  zb_uint8_t  gp_shared_security_key[ZB_CCM_KEY_SIZE];
  zb_uint8_t  gp_link_key[ZB_CCM_KEY_SIZE];
  zb_uint8_t  gps_communication_mode; /**< @see zgp_communication_mode_t  */
  zb_uint8_t  gps_commissioning_exit_mode; /**< @see zgp_commissioning_exit_mode_t  */
  zb_uint8_t  gps_security_level;
  zb_uint16_t gps_commissioning_window;
  /* let's align it to 4 bytes (38 + 2) */
  zb_uint16_t align_dummy;
} ZB_PACKED_STRUCT zb_zgp_cluster_t;

typedef ZB_PACKED_PRE struct zb_zgp_device_role_s
{
  zb_zgp_gp_device_t        requested_device_role;
  zb_zgp_gp_device_t        actual_device_role;
  zb_ret_t                  status;
} ZB_PACKED_STRUCT zb_zgp_device_role_t;

/** @brief global ZGP context */
typedef struct zb_zgp_ctx_s
{
  zb_zgp_device_role_t      device_role;
  zb_zgp_cluster_t          cluster;
#ifndef NCP_MODE_HOST
  zb_zgps_dev_comm_data_t   comm_data;  /**< Info about currently ongoing commissioning process */

#ifdef ZB_ENABLE_ZGP_DIRECT
  zb_zgp_tx_q_t             tx_queue;     /**< ZGP Tx queue */
  zb_zgp_tx_packet_info_q_t tx_packet_info_queue;
#ifdef ZB_ZGP_IMMED_TX
  zb_uint32_t               immed_tx_frame_counter;
#endif  /* ZB_ZGP_IMMED_TX */
#ifdef ZB_ENABLE_ZGP_TEST_HARNESS
  zb_outgoing_gpdf_info_t   out_gpdf_info;
#endif
#endif  /* ZB_ENABLE_ZGP_DIRECT */
#endif /* NCP_MODE_HOST */

  zb_bitfield_t skip_gpdf:1;
  zb_bitfield_t init_by_scheduler:1;
  zb_bitfield_t aligned:6;
} zb_zgp_ctx_t;

/**
   Direct access to GP context.

   To be used internally only.
 */
extern zb_zgp_ctx_t zb_zgp_ctx;
#define ZGP_CTX() zb_zgp_ctx

#ifdef ZB_ENABLE_ZGP_DIRECT
/**
 * Start of buffer contains zb_gp_data_req_t.
 *
 * @pre (req->tx_options & ZB_GP_DATA_REQ_USE_GP_TX_QUEUE) ==
 *      (req->tx_q_ent_lifetime != ZB_GP_TX_QUEUE_ENTRY_LIFETIME_NONE)
 */
void zb_gp_data_req(zb_cb_param_t param);

void zb_zgp_tx_q_entry_expired(zb_cb_param_t param);

void zb_gp_data_cfm(zb_cb_param_t param);
#endif  /* ZB_ENABLE_ZGP_DIRECT */

void zb_zgp_sync_pib(zb_cb_param_t param);
zb_ret_t zb_zgp_cluster_init();

/**
 * Return back operational channel
 *
 * Operational channel can be changed to send channel configuration
 * during commissioning. This function sets operational channel back.
 * If channel change is necessary, then request to MAC is initiated
 * and zgp_mlme_set_cfm_cb will be called after that. Otherwise,
 * zgp_mlme_set_cfm_cb is called immediately
 *
 * @param  param   reference to buffer
 */
void zgp_back_to_oper_channel(zb_bufid_t param);

#define ZB_ZGP_IS_COMM_STATE(checked_state) \
  (ZGP_CTX().comm_data.state == (checked_state))

#define ZB_ZGP_SET_COMM_STATE(new_state) \
{ \
  ZGP_CTX().comm_data.state = (new_state); \
}

/* Read commissioning options (in sink) */
#define ZB_ZGPS_COMM_GET_RX_AFTER_TX()         ZB_GPDF_EXT_NFC_GET_RX_AFTER_TX(ZGP_CTX().comm_data.gpdf_nwk_ext_frame_ctl)
#define ZB_ZGPS_COMM_GET_SEC_KEY_REQ()         ZB_GPDF_COMM_OPT_SEC_KEY_REQ(ZGP_CTX().comm_data.gpdf_options)
#define ZB_ZGPS_COMM_GET_PAN_ID_REQ()          ZB_GPDF_COMM_OPT_PAN_ID_REQ(ZGP_CTX().comm_data.gpdf_options)
#define ZB_ZGPS_COMM_GET_ZGPD_KEY_PRESENT()    ZB_GPDF_COMM_OPT_ZGPD_KEY_PRESENT(ZGP_CTX().comm_data.gpdf_ext_options)
#define ZB_ZGPS_COMM_GET_ZGPD_KEY_ENCRYPTED()  ZB_GPDF_COMM_OPT_ZGPD_KEY_ENCRYPTED(ZGP_CTX().comm_data.gpdf_ext_options)
#define ZB_ZGPS_COMM_GET_SEC_KEY_TYPE()        ZB_GPDF_COMM_OPT_SEC_KEY_TYPE(ZGP_CTX().comm_data.gpdf_ext_options)


void zgp_channel_config_transceiver_start(zb_bufid_t param);
void zb_zgp_channel_config_get_current_channel(zb_bufid_t param);
/**
 * @brief Prepare channel configuration packet and add it into TX queue
 *
 * @param param      [in]  Buffer reference
 *
 */
void zgp_channel_config_add_to_queue(zb_bufid_t param, zb_uint8_t payload);

zb_ret_t zgp_table_init(void);
void zgp_table_clear(void);

/********************************************************************/
/******************* ZGP CLUSTER definitions ************************/
/********************************************************************/

#define ZB_GP_VER 0

enum zgp_server_commands_e {
  ZGP_SERVER_CMD_GP_NOTIFICATION                        = 0x00,
  ZGP_SERVER_CMD_GP_PAIRING_SEARCH                      = 0x01,
  ZGP_SERVER_CMD_GP_TUNNELING_STOP                      = 0x03,
  ZGP_SERVER_CMD_GP_COMMISSIONING_NOTIFICATION          = 0x04,
  ZGP_SERVER_CMD_GP_SINK_COMMISSIONING_MODE             = 0x05,
  ZGP_SERVER_CMD_GP_TRANSLATION_TABLE_UPDATE_COMMAND    = 0x07,
  ZGP_SERVER_CMD_GP_TRANSLATION_TABLE_REQUEST           = 0x08,
  ZGP_SERVER_CMD_GP_PAIRING_CONFIGURATION               = 0x09,
  ZGP_SERVER_CMD_GP_SINK_TABLE_REQUEST                  = 0x0a,
  ZGP_SERVER_CMD_GP_PROXY_TABLE_RESPONSE                = 0x0b
};


enum zgp_client_commands_e {
  ZGP_CLIENT_CMD_GP_NOTIFICATION_RESPONSE     = 0x00, /* not for basic */
  ZGP_CLIENT_CMD_GP_PAIRING                   = 0x01,
  ZGP_CLIENT_CMD_GP_PROXY_COMMISSIONING_MODE  = 0x02,
  ZGP_CLIENT_CMD_GP_RESPONSE                  = 0x06,
  ZGP_CLIENT_CMD_GP_SINK_TABLE_RESPONSE       = 0x0a,
  ZGP_CLIENT_CMD_GP_PROXY_TABLE_REQUEST       = 0x0b
};

#define ZB_ZCL_GREEN_POWER_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

#define ZB_ZCL_GREEN_POWER_CLUSTER_REVISION_MAX ZB_ZCL_GREEN_POWER_CLUSTER_REVISION_DEFAULT

void zb_zcl_green_power_init_server(void);
void zb_zcl_green_power_init_client(void);
#define ZB_ZCL_CLUSTER_ID_GREEN_POWER_SERVER_ROLE_INIT zb_zcl_green_power_init_server
#define ZB_ZCL_CLUSTER_ID_GREEN_POWER_CLIENT_ROLE_INIT zb_zcl_green_power_init_client

#define ZGP_GPS_COMMUNICATION_MODE      ZGP_CTX().cluster.gps_communication_mode
#define ZGP_GPS_COMMISSIONING_EXIT_MODE ZGP_CTX().cluster.gps_commissioning_exit_mode
#define ZGP_GPS_COMMISSIONING_WINDOW    ZGP_CTX().cluster.gps_commissioning_window
#define ZGP_GPS_SECURITY_LEVEL          ZGP_CTX().cluster.gps_security_level

#define ZGP_GP_LINK_KEY                 ZGP_CTX().cluster.gp_link_key
#define ZGP_GP_SHARED_SECURITY_KEY_TYPE ZGP_CTX().cluster.gp_shared_security_key_type
#define ZGP_GP_SHARED_SECURITY_KEY      ZGP_CTX().cluster.gp_shared_security_key

#define ZGP_GPS_GET_SECURITY_LEVEL()\
  (ZGP_GPS_SECURITY_LEVEL & 3)

#define ZGP_GPS_GET_PROTECT_WITH_GP_LINK_KEY()\
  ((ZGP_GPS_SECURITY_LEVEL >> 2) & 1)

#define ZGP_GPS_GET_INVOLVE_TC()\
  ((ZGP_GPS_SECURITY_LEVEL >> 3) & 1)

#define ZB_ZGP_COMM_MODE_OPT_GET_ACTION(opt)\
  ((opt) & 1)

#define ZB_ZGP_COMM_MODE_OPT_GET_UNICAST(opt)\
  (((opt) >> 5) & 1)

#define ZB_ZGP_PROXY_COMM_MODE_INT_OPT_GET_EXIT_MODE(opt)\
  ((opt) & 7)

#define ZB_ZGP_PROXY_COMM_MODE_INT_OPT_GET_CHNL_PRESENT(opt)\
  (((opt) >> 3) & 1)

#define ZB_ZGP_PROXY_COMM_MODE_INT_OPT_GET_UNICAST_COMMUNICATION(opt)\
  (((opt) >> 4) & 1)

#define ZB_ZGP_PROXY_COMM_MODE_IS_UNICAST()\
  ZB_ZGP_PROXY_COMM_MODE_INT_OPT_GET_UNICAST_COMMUNICATION(\
    ZGP_CTX().comm_data.proxy_comm_mode_options)

#define ZB_ZGP_PROXY_COMM_MODE_IS_EXIT_AFTER_FIRST_PAIRING_SUCCESS()\
  (ZB_ZGP_PROXY_COMM_MODE_INT_OPT_GET_EXIT_MODE(\
    ZGP_CTX().comm_data.proxy_comm_mode_options) & \
    ZGP_COMMISSIONING_EXIT_MODE_ON_PAIRING_SUCCESS)

#define ZB_ZGP_COMM_MODE_OPT_GET_ON_COMM_WIND_EXP(opt)  \
  (((opt) >> 1) & 0x01)

#define ZB_ZGP_COMM_MODE_OPT_GET_CHNL_PRESENT(opt)\
  (((opt) >> 4) & 0x01)

#define ZGP_GPS_GET_COMMISSIONING_WINDOW()\
  ((ZGP_GPS_COMMISSIONING_EXIT_MODE & ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION) ?\
      ZGP_GPS_COMMISSIONING_WINDOW : 0)

#define ZB_ZGP_COMM_NOTIF_OPT_GET_APP_ID(opt)\
  ((opt) & 0x07)

#define ZB_ZGP_GP_COMM_NOTIF_OPT_GET_RX_AFTER_TX(opt)\
  (((opt) >> 3) & 0x01)

#define ZB_ZGP_GP_COMM_NOTIF_OPT_GET_SEC_LVL(opt)\
  (((opt) >> 4) & 0x03)

#define ZB_ZGP_GP_COMM_NOTIF_OPT_GET_KEY_TYPE(opt)\
  (((opt) >> 6) & 0x07)

#define ZB_ZGP_GP_COMM_NOTIF_OPT_GET_SECUR_FAILED(opt)\
  (((opt) >> 9) & 0x01)

/* The MIC field SHALL only be present if the Security processing failed
 * sub-field is set to 0b1 */
#define ZB_ZGP_COMM_NOTIF_OPT_GET_MIC_PRESENT(opt)\
  (((opt) >> 9) & 0x01)

#define ZB_ZGP_GP_COMM_NOTIF_OPT_GET_BIDIR_CAP(opt)\
  (((opt) >> 10) & 0x01)

#define ZB_ZGP_COMM_NOTIF_OPT_GET_PROXY_INFO_PRESENT(opt)\
  (((opt) >> 11) & 0x01)

/*
Bits: 0..2      3               4..5            6..8            9                               10                             11
ApplicationID   RxAfterTx       SecurityLevel   SecurityKeyType Security processing failed      Bidirectional Capability       Proxy info present

 */
#define ZB_ZGP_FILL_COMM_NOTIFICATION_OPTIONS(app_id, rx_after_tx, secur_level, key_type, secur_failed, bidir_cap, proxy_info) \
  (((app_id) & 7) | ((!!(rx_after_tx)) << 3) | (((secur_level) & 3) << 4) | (((key_type) & 7) << 6) | ((!!(secur_failed)) << 9) | ((!!(bidir_cap)) << 10) | ((!!(proxy_info)) << 11))

#define ZB_ZGP_PAIRING_OPT_GET_APP_ID(opt)\
  ((opt) & 0x07)

#define ZB_ZGP_PAIRING_OPT_GET_REMOVE_GPD(opt)\
  (((opt) >> 4) & 0x01)

#define ZB_ZGP_PAIRING_OPT_GET_COMMUNICATION_MODE(opt)\
  (((opt) >> 5) & 0x03)

#define ZB_ZGP_PAIRING_OPT_GET_ADD_SINK(opt)\
  (((opt) >> 3) & 0x01)

#define ZB_ZGP_PAIRING_OPT_GET_SEC_LEVEL(opt)\
  (((opt) >> 9) & 0x03)

#define ZB_ZGP_PAIRING_OPT_GET_KEY_TYPE(opt)\
  (((opt) >> 11) & 0x07)

#define ZB_ZGP_PAIRING_OPT_GET_FRAME_CNT_PRESENT(opt)\
  (((opt) >> 14) & 0x01)

#define ZB_ZGP_PAIRING_OPT_GET_SEQ_NUM_CAP(opt)\
  (((opt) >> 8) & 0x01)

#define ZB_ZGP_PAIRING_OPT_GET_FIX_LOC(opt)\
  (((opt) >> 7) & 0x01)

#define ZB_ZGP_PAIRING_OPT_GET_SEC_KEY_PRESENT(opt)\
  (((opt) >> 15) & 0x01)

#define ZB_ZGP_PAIRING_OPT_GET_ASSIGNED_ALIAS_PRESENT(opt)\
  (((opt) >> 16) & 0x01)

#define ZB_ZGP_PAIRING_OPT_GET_FRWD_RADIUS(opt)\
  (((opt) >> 17) & 0x01)

#define ZB_ZGP_GP_NOTIF_OPT_GET_APP_ID(opt)\
  ((opt) & 0x07)

#define ZB_ZGP_GP_NOTIF_OPT_GET_SEC_LVL(opt)\
  (((opt) >> 6) & 0x03)

#define ZB_ZGP_GP_NOTIF_OPT_GET_KEY_TYPE(opt)\
  (((opt) >> 8) & 0x07)

#define ZB_ZGP_GP_NOTIF_OPT_GET_RX_AFTER_TX(opt)\
  (((opt) >> 11) & 0x01)

#define ZB_ZGP_GP_NOTIF_OPT_GET_BIDIR_CAP(opt)\
  (((opt) >> 13) & 0x01)

#define ZB_ZGP_GP_NOTIF_OPT_GET_PROXY_INFO_PRESENT(opt)\
  (((opt) >> 14) & 0x01)

#define ZB_ZGP_GP_NOTIF_OPT_GET_RECV_AS_UNICAST(opt)\
  (((opt) >> 15) & 0x01)

#define ZB_ZGP_GP_NOTIF_OPT_SET_RECV_AS_UNICAST(opt)\
  ((opt) = (opt | (1 << 15)))

#define ZB_ZGP_GP_RESPONSE_OPT_GET_APP_ID(opt) \
  ((opt) & 0x07)

#define ZB_ZGP_FILL_GP_RESPONSE_OPTIONS(app_id, ep_match) \
  (((app_id) & 7) | ((!!(ep_match)) << 3))

#define ZB_ZGP_PROXY_ENTRY_OPT_GET_LW_GPS(opt)\
  (((opt) >> 6) & 0x01)

#define ZB_ZGP_PROXY_ENTRY_OPT_GET_DGROUP_GPS(opt)\
  (((opt) >> 7) & 0x01)

#define ZB_ZGP_PROXY_ENTRY_OPT_GET_PRECOMMISSIONED_GROUP_GPS(opt)\
  (((opt) >> 8) & 0x01)

#define ZB_ZGP_PROXY_ENTRY_OPT_GET_OPT_EXT(opt)\
  (((opt) >> 15) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_OPT_GET_APP_ID(opt) \
  ((opt) & 0x07)

#define ZB_ZGP_GP_PAIRING_CONF_GET_COMMUNICATION_MODE(opt)\
  (((opt) >> 3) & 0x03)

#define ZB_ZGP_GP_PAIRING_CONF_GET_SEQ_NUM_CAPS(opt)\
  (((opt) >> 5) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_GET_RX_ON_CAPS(opt)\
  (((opt) >> 6) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_GET_FIXED_LOC(opt)\
  (((opt) >> 7) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_GET_ASSIGNED_ALIAS_PRESENT(opt)\
  (((opt) >> 8) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_GET_SEC_USE(opt)\
  (((opt) >> 9) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_GET_APP_INFO_PRESENT(opt)\
  (((opt) >> 10) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_GET_MANUF_ID_PRESENT(app_info)\
  ((app_info) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_GET_MODEL_ID_PRESENT(app_info)\
  (((app_info) >> 1) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_GET_CMDS_PRESENT(app_info)\
  (((app_info) >> 2) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_GET_CLUSTERS_PRESENT(app_info)\
  (((app_info) >> 3) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_GET_SWITCH_INFO_PRESENT(app_info)\
  (((app_info) >> 4) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_GET_APP_DESCRIPTION_FOLLOWS(app_info)\
  (((app_info) >> 5) & 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_SET_MANUF_ID_PRESENT(app_info)\
  ((app_info) |= 0x01)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_SET_MODEL_ID_PRESENT(app_info)\
  ((app_info) |= 0x02)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_SET_CMDS_PRESENT(app_info)\
  ((app_info) |= 0x04)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_SET_CLUSTERS_PRESENT(app_info)\
  ((app_info) |= 0x08)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_SET_SWITCH_INFO_PRESENT(app_info)\
  ((app_info) |= 0x10)

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_SET_APP_DESCRIPTION_FOLLOWS(app_info)\
  ((app_info) |= 0x20)

#define ZB_ZGP_GP_PAIRING_CONF_GET_ACTIONS(actions)\
  ((actions) & 0x07)

#define ZB_ZGP_GP_PAIRING_CONF_GET_SEND_PAIRING(actions)\
  (((actions) >> 3) & 0x01)

#define ZB_ZGP_FILL_GP_PAIRING_CONF_OPTIONS(app_id, comm_mode, seq_num_cap, rx_on_cap, fix_loc, asgnd_alias, sec_use, app_info)\
  ((app_id) | (((comm_mode) & 3) << 3) | ((!!(seq_num_cap)) << 5) | ((!!(rx_on_cap)) << 6) | ((!!(fix_loc)) << 7) | ((!!(asgnd_alias)) << 8) | ((!!(sec_use)) << 9) | ((!!(app_info)) << 10))

#define ZB_ZGP_FILL_GP_PAIRING_CONF_ACTIONS(action, send_pairing)\
  ((action) | ((!!(send_pairing)) << 3))

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_MANUF_ID_NO_PRESENT 0
#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_MANUF_ID_PRESENT 1

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_MODEL_ID_NO_PRESENT 0
#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_MODEL_ID_PRESENT 1

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_GPD_CMDS_NO_PRESENT 0
#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_GPD_CMDS_PRESENT 1

#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_CLSTS_NO_PRESENT 0
#define ZB_ZGP_GP_PAIRING_CONF_APP_INFO_CLSTS_PRESENT 1

#define ZB_ZGP_FILL_GP_PAIRING_CONF_APP_INFO(manuf_id, model_id, gpd_cmds, clsts)\
  ((!!(manuf_id)) | ((!!(model_id)) << 1) | ((!!(gpd_cmds)) << 2) | ((!!(clsts)) << 3))

/*
Bits: 0-2 - AppID
      3-4 - Communication mode
        5 - Sequence number capabilities
        6 - RxOnCapability
        7 - FixedLocation
        8 - AssignedAlias
        9 - SecurityUse
    10-15 - Reserved
*/
/**
 * @brief Fill sink entry options
 *
 * @param ent [in]  Pointer to sink table entry
 *
 * @return Sink table entry options value
 *
 * @see ZGP spec, A.3.3.2.2.2.1
 */
zb_uint16_t zgp_sink_fill_spec_entry_options(zgp_tbl_ent_t *ent);

/**
 * @brief Transmit sink entry over the air
 *
 * @param buf [in]  Pointer to memory buffer
 * @param ptr [in]  Pointer to allocated memory space
 * @param ent [in]  Pointer to sink table entry
 *
 * @see ZGP spec, A.3.3.2.2.1
 */
void zgp_sink_table_entry_over_the_air_transmission(zb_bufid_t     buf,
                                                    zb_uint8_t   **ptr,
                                                    zgp_tbl_ent_t *ent,
                                                    zb_uint16_t    options);

/**
 * @brief Calculate sink entry size for over the air transmission
 *
 * @param ent [in]  Pointer to sink table entry
 *
 * @return sink entry size in bytes
 *
 * @see ZGP spec, A.3.3.2.2.1
 */
zb_uint8_t zgp_sink_table_entry_size_over_the_air(zgp_tbl_ent_t *ent);

typedef enum zgp_table_request_entries_type_e
{
  ZGP_REQUEST_TABLE_ENTRIES_BY_GPD_ID,
  ZGP_REQUEST_TABLE_ENTRIES_BY_INDEX
} zgp_table_request_entries_type_t;

#define ZB_ZGP_GP_PROXY_TBL_REQ_GET_APP_ID(opt)\
  ((opt) & 0x07)

#define ZB_ZGP_GP_PROXY_TBL_REQ_GET_REQ_TYPE(opt)\
  (((opt) >> 3) & 3)

#define ZB_ZGP_GP_PROXY_TBL_REQ_FILL_OPT(app_id, req_type)\
  ((app_id) | (((req_type) & 3) << 3))

#define ZB_ZGP_GP_SINK_COMM_MODE_FILL_OPT(action, inv_gpm_seq, inv_gpm_pair, inv_prx) \
  ((!!(action)) | ((!!(inv_gpm_seq)) << 1) | ((!!(inv_gpm_pair)) << 2) | ((!!(inv_prx)) << 3))

#define ZB_ZGP_GP_SINK_COMM_MODE_GET_ACTION(opt)\
  ((opt) & 0x01)

#define ZB_ZGP_GP_SINK_COMM_MODE_GET_INVOLVE_GPM_IN_SECURITY(opt)\
  (((opt) >> 1) & 0x01)

#define ZB_ZGP_GP_SINK_COMM_MODE_GET_INVOLVE_GPM_IN_PAIRING(opt)\
  (((opt) >> 2) & 0x01)

#define ZB_ZGP_GP_SINK_COMM_MODE_GET_INV_PROXIES(opt)\
  (((opt) >> 3) & 0x01)

/**
 * @brief Value of multi-record bit of options field
 *        in ZGPD Request attributes or Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_REQUEST_ATTR_IS_MULTI_RECORD(opts) \
  (opts & 0x01U)

/**
 * @brief Value of "manufacturer field present" bit of options field
 *        in ZGPD Request attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_REQUEST_ATTR_MANUF_FIELD_PRESENT(opts) \
  ((opts >> 1U) & 0x01U)

/**
 * @brief RX channel in the next attempt parameter of ZGPD Channel request command
 * @see ZGP spec, A.4.2.1.4
 */
#define ZB_GPDF_CHANNEL_REQ_NEXT_RX_CHANNEL(par) \
  ((par) & 0x0FU)

#ifdef ZB_ENABLE_ZGP_DIRECT
/**
 * @brief Converting LQI incoming from MAC into ZGP cluster specific format
 *
 * @param lqi  [in]  Incoming LQI value
 *
 * @return Encoded lqi specific value
 *
 * @see ZGP spec, A.3.3.4.1
 */
zb_uint8_t zb_zgp_encode_lqi(zb_uint8_t lqi);
/**
 * @brief Converting RSSI and LQI incoming from MAC into ZGP cluster specific format
 *
 * @param rssi [in]  Incoming RSSI value
 * @param lqi  [in]  Incoming LQI value
 *
 * @return Encoded rssi+lqi specific value
 *
 * @see ZGP spec, A.3.3.4.1
 */
zb_uint8_t zb_zgp_encode_link_quality(zb_int8_t rssi, zb_uint8_t lqi);
#endif  /* ZB_ENABLE_ZGP_DIRECT */

/**
 * @brief ZGP commissioning states enumeration
 */
typedef enum zb_zgp_comm_state_e
{
  ZGP_COMM_STATE_IDLE,  /* 0 */
  ZGP_COMM_STATE_CHANNEL_REQ_RECEIVED,
  ZGP_COMM_STATE_CHANNEL_CONFIG_GET_CUR_CHANNEL,
#ifdef ZB_MAC_COEX_CONTROL
  ZGP_COMM_STATE_SET_COEX_SHUTDOWN,
#endif /* ZB_MAC_COEX_CONTROL */
#ifdef ZB_ENABLE_ZGP_DIRECT
  ZGP_COMM_STATE_CHANNEL_CONFIG_SET_TEMP_CHANNEL,
  ZGP_COMM_STATE_CHANNEL_CONFIG_ADDED_TO_Q,
  ZGP_COMM_STATE_CHAN_CFG_SENT_RET_CHANNEL,
  ZGP_COMM_STATE_CHAN_CFG_FAILED_RET_CHANNEL,
#endif  /* ZB_ENABLE_ZGP_DIRECT */
  ZGP_COMM_STATE_CHANNEL_CONFIG_SENT,
  ZGP_COMM_STATE_COMM_REQ_RECEIVED_WAIT_FOR_APP,
  ZGP_COMM_STATE_COMM_REQ_RECEIVED_AND_APPROVED,
#ifdef ZB_ENABLE_ZGP_DIRECT
  ZGP_COMM_STATE_COMMISSIONING_REPLY_ADDED_TO_Q,
#endif  /* ZB_ENABLE_ZGP_DIRECT */
  ZGP_COMM_STATE_COMMISSIONING_REPLY_SENT,
  ZGP_COMM_STATE_CHANNEL_REQ_COLLECT,
  ZGP_COMM_STATE_COMMISSIONING_REQ_COLLECT,
  ZGP_COMM_STATE_COMMISSIONING_FINALIZING,
  ZGP_COMM_STATE_COMMISSIONING_TIMED_OUT,
  ZGP_COMM_STATE_COMMISSIONING_CANCELLED,
  ZGP_COMM_STATE_COMMISSIONING_WAIT_APP_DESCR
}
zb_zgp_comm_state_t;

typedef ZB_PACKED_PRE struct zb_gp_data_req_s
{
  zb_uint8_t      handle;
  zb_uint8_t      action;
  zb_uint8_t      tx_options;
  zb_zgpd_id_t    zgpd_id;
  zb_ieee_addr_t  ieee_addr;
  zb_uint8_t      cmd_id;
  zb_uint8_t      payload_len;
  zb_time_t       tx_q_ent_lifetime;
  zb_uint8_t      pld[ZB_ZGP_TX_CMD_PLD_MAX_SIZE]; /**< Payload */
}
ZB_PACKED_STRUCT
zb_gp_data_req_t;

#ifdef ZB_ENABLE_ZGP_DIRECT
typedef struct zb_gp_data_cfm_s
{
  zb_uint8_t        handle;
  /* Extension of the spec */
  zb_zgpd_id_t      zgpd_id;
  zb_uint8_t        cmd_id;
}
zb_gp_data_cfm_t;
#endif  /* ZB_ENABLE_ZGP_DIRECT */

void zb_dgp_data_ind(zb_cb_param_t param);

/**
    A.3.6.3.3.1   Derivation of alias source address
*/
zb_uint16_t zgp_calc_alias_source_address(zb_zgpd_id_t *zgpd_id);

/**
 * @brief Calculate count of nonempty groupcast addresses in group address list
 *
 * @param ent [in]  Pointer to proxy table entry
 *
 * @return Count of nonempty groupcast addresses
 */
zb_uint8_t zgp_get_group_list_size(zgp_pair_group_list_t *group_list);

#ifdef ZB_ALIEN_ZGP_STUB

void zgp_alien_stub_table_entry_add(zgp_tbl_ent_t *ent);
#define ALIEN_STUB_TBL_ENTRY_ADD(ent) zgp_alien_stub_table_entry_add(ent)

void zgp_alien_stub_table_entry_remove(zgp_tbl_ent_t *ent);
#define ALIEN_STUB_TBL_ENTRY_REMOVE(ent) zgp_alien_stub_table_entry_remove(ent)

void zgp_alien_stub_table_remove_all_entries(zb_uint8_t unused);
#define ALIEN_STUB_TBL_REMOVE_ALL_ENTRIES() zgp_alien_stub_table_remove_all_entries(0)

#else  /* ZB_ALIEN_ZGP_STUB */

#define ALIEN_STUB_TBL_ENTRY_ADD(ent)
#define ALIEN_STUB_TBL_ENTRY_REMOVE(ent)
#define ALIEN_STUB_TBL_REMOVE_ALL_ENTRIES(tbl)

#endif  /* ZB_ALIEN_ZGP_STUB */

zb_uint8_t zb_zgp_try_bidir_tx(zb_bufid_t param);
zb_ret_t zgp_key_recovery(zgp_tbl_ent_t *ent, zb_bool_t individual, zb_uint8_t *key, zb_uint8_t *key_type);

#define ZB_ZGP_ENT_ENUMERATE_CTX_START_IDX   0xff

typedef struct zb_zgp_ent_enumerate_ctx_s
{
  zb_uint8_t idx;
  zb_uint8_t entries_count;
} zb_zgp_ent_enumerate_ctx_t;

void zgp_init_by_scheduler(zb_cb_param_t param);

typedef ZB_PACKED_PRE struct zb_zgp_gp_response_s
{
  zb_uint8_t     options;
  zb_uint16_t    temp_master_addr;
  zb_uint8_t     temp_master_tx_chnl;
  zb_zgpd_addr_t zgpd_addr;
  zb_uint8_t     endpoint;
  zb_uint8_t     gpd_cmd_id;
  /* +1 bytes for payload size placed at beginning buffer */
  zb_uint8_t     payload[MAX_ZGP_CLUSTER_GPDF_PAYLOAD_SIZE+1];
}
ZB_PACKED_STRUCT zb_zgp_gp_response_t;

void zb_gp_data_indication(zb_cb_param_t param);

zb_ret_t zgp_add_group_alias_for_entry(zgp_pair_group_list_t *group_list,
                                       zb_uint16_t            sink_group,
                                       zb_uint16_t            alias);

zb_ret_t zgp_del_group_from_entry(zgp_pair_group_list_t *group_list, zb_uint16_t sink_group);

void zb_zgp_write_dataset(zb_cb_param_t param);

typedef ZB_PACKED_PRE struct zb_zgp_gp_pairing_send_req_s
{
  /* 0 bit - add_sink
   * 1 bit - remove gpd
   * 2 bit - send dev annce
   * 3 bit - indicate that dup for groupcast precommissioned is complete
   * 4 bit - need send incorrect LW unicast remove pairing, see A.3.5.2.4
   */
  zb_uint8_t       send_options;
  zb_zgpd_addr_t   zgpd_id;
  zb_uint8_t       endpoint;
  zb_uint16_t      options;
  zb_uint8_t       sec_options;
  zb_uint32_t      security_counter;
  zb_uint8_t       zgpd_key[ZB_CCM_KEY_SIZE];
  zb_uint16_t      zgpd_assigned_alias;
  zb_uint8_t       groupcast_radius;
  zb_uint8_t       device_id;
  zgp_pair_group_list_t sgrp[ZB_ZGP_MAX_SINK_GROUP_PER_GPD];
  zb_callback_t    callback;
}
ZB_PACKED_STRUCT zb_zgp_gp_pairing_send_req_t;

#define ZB_ZGP_GP_PAIRING_MAKE_SEND_OPTIONS(req, add_sink, remove_gpd, send_da)\
  (req)->send_options = ((!!(add_sink)) | ((!!(remove_gpd)) << 1) | ((!!(send_da)) << 2))

#define ZB_ZGP_GP_PAIRING_OPTIONS_SET_DUP_COMPLETE(req)\
  (req)->send_options |= (1 << 3)

#define ZB_ZGP_GP_PAIRING_OPTIONS_SET_SEND_INCORRECT_LW_PAIR_REMOVE(req)\
  (req)->send_options |= (1 << 4)

#define ZB_ZGP_GP_PAIRING_OPTIONS_UPDATE_SEND_INCORRECT_LW_PAIR_REMOVE(req)\
  ZB_ZGP_GP_PAIRING_MAKE_SEND_OPTIONS(req, 0, 0, 0);\
  ZB_ZGP_GP_PAIRING_OPTIONS_SET_SEND_INCORRECT_LW_PAIR_REMOVE(req)

#define ZB_ZGP_GP_PAIRING_SEND_REQ_CREATE(buf, req, ent, cb) \
  zb_buf_reuse((buf));\
  (req) = ZB_BUF_GET_PARAM((buf), zb_zgp_gp_pairing_send_req_t);\
  (req)->zgpd_id = (ent)->zgpd_id;\
  (req)->endpoint = (ent)->endpoint;\
  (req)->options = (ent)->options;\
  (req)->sec_options = (ent)->sec_options;\
  (req)->security_counter = (ent)->security_counter;\
  ZB_MEMCPY((req)->zgpd_key, (ent)->zgpd_key, sizeof((ent)->zgpd_key));\
  (req)->zgpd_assigned_alias = (ent)->zgpd_assigned_alias;\
  (req)->groupcast_radius = (ent)->groupcast_radius;\
  (req)->device_id = (ent)->u.sink.device_id;\
  ZB_MEMCPY((req)->sgrp, (ent)->u.sink.sgrp, sizeof((ent)->u.sink.sgrp)); \
  (req)->callback = (cb)

#define ZB_ZGP_GP_PCONF_PAIRING_SEND_REQ_CREATE(buf, req, conf, cb)\
  zb_buf_reuse((buf));                                             \
  (req) = ZB_BUF_GET_PARAM((buf), zb_zgp_gp_pairing_send_req_t);   \
  (req)->zgpd_id = (conf)->zgpd_addr;                              \
  (req)->endpoint = (conf)->endpoint;                              \
  (req)->options = (conf)->options;                                \
  (req)->sec_options = (conf)->u.action_flds.sec_options;                        \
  (req)->security_counter = (conf)->u.action_flds.sec_frame_counter;             \
  ZB_MEMCPY((req)->zgpd_key, (conf)->u.action_flds.key, sizeof((conf)->u.action_flds.key));    \
  (req)->zgpd_assigned_alias = (conf)->u.action_flds.assigned_alias;             \
  (req)->groupcast_radius = (conf)->frwd_radius;                   \
  (req)->device_id = (conf)->device_id;                            \
  ZB_MEMCPY(&(req)->sgrp[0], &(conf)->u.action_flds.sgrp[0], sizeof((conf)->u.action_flds.sgrp)); \
  (req)->callback = (cb)

#define ZGP_PAIRING_SEND_GET_ADD_SINK(req) ((req)->send_options & 1)
#define ZGP_PAIRING_SEND_GET_REMOVE_GPD(req) (((req)->send_options >> 1) & 1)
#define ZGP_PAIRING_SEND_GET_SEND_DEV_ANNCE(req) (((req)->send_options >> 2) & 1)
#define ZGP_PAIRING_SEND_GET_DUP_COMPLETE(req) (((req)->send_options >> 3) & 1)
#define ZGP_PAIRING_SEND_GET_SEND_INCORRECT_LW_PAIR_REMOVE(req) (((req)->send_options >> 4) & 1)
#define ZGP_PAIRING_SEND_CLR_SEND_INCORRECT_LW_PAIR_REMOVE(req) ((req)->send_options &= ~(1 << 4))
#define ZGP_PAIRING_SEND_GET_ASSIGNED_ALIAS(req) (((req)->options >> 8) & 1)
#define ZGP_PAIRING_SEND_GET_COMMUNICATION_MODE(req) (ZGP_PAIRING_SEND_GET_SEND_INCORRECT_LW_PAIR_REMOVE(req) ?\
                                                      ZGP_COMMUNICATION_MODE_LIGHTWEIGHT_UNICAST : (((req)->options >> 3) & 3))
#define ZGP_PAIRING_SEND_GET_APP_ID(req) ((req)->options & 7)
#define ZGP_PAIRING_SEND_GET_SEC_LEVEL(req) (((req)->options & (1<<9)) ? ZGP_SINK_GET_SEC_LEVEL((req)->sec_options): 0)
#define ZGP_PAIRING_SEND_GET_FIXED_LOCATION(req) (((req)->options >> 7) & 1)
#define ZGP_PAIRING_SEND_GET_SEQ_NUM_CAP(req) (((req)->options >> 5) & 1)
#define ZGP_PAIRING_SEND_GET_SEC_KEY_TYPE(req) (((req)->options & (1<<9)) ? (((req)->sec_options >> 2) & 7) : 0)

zb_ret_t zgp_send_gp_pairing(zb_bufid_t param);
void zgp_send_dev_annce_for_alias(zb_cb_param_t cb_param);

zb_bool_t zgp_is_equal_zgpd_id_for_table_entry(zgp_tbl_ent_t *ent, zb_zgpd_id_t *id);

zb_ret_t zgp_tbl_load(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl);
/**
   returns security counter for that zgpd.

   @return counter value of ~0 if no entry
 */
zb_uint32_t zgp_tbl_get_security_counter(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl);
/**
   returns duplicate counter for that zgpd.

   @return counter value of ~0 if no entry or expired.
 */
zb_uint32_t zgp_tbl_get_dup_counter(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl);
zb_ret_t zgp_tbl_restore_security_counter(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl);
zb_ret_t zgp_tbl_set_security_counter(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl, zb_uint32_t counter);
void zgp_tbl_get_lqi_rssi(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl, zb_uint8_t *lqi_p, zb_int8_t *rssi_p);
void zgp_tbl_set_lqi_rssi(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl, zb_uint8_t lqi, zb_int8_t rssi);
zb_ret_t zgp_tbl_read(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl, zgp_tbl_ent_t *ent);
zb_ret_t zgp_tbl_del_all_if_endpoint(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl);

/**
 * @brief Get table entry by index
 *
 * @param idx   [in]   Index of table entry which needed
 * @param tbl   [in]   Pointer to the table for search entry
 * @param ent   [out]  Pointer to allocated memory space for table entry
 *
 */
zb_ret_t zgp_tbl_read_by_idx(zb_uint_t idx, zb_zgp_tbl_t *tbl, zgp_tbl_ent_t *ent);
zb_ret_t zgp_tbl_write(zb_zgpd_id_t *zgpd_id, zb_zgp_tbl_t *tbl, zgp_tbl_ent_t *ent);
zb_uint8_t zb_zgp_tbl_entry_count(zb_zgp_tbl_t *tbl);
zb_uint16_t zb_nvram_zgp_tbl_length(zb_zgp_tbl_t *tbl);
zb_ret_t zgp_any_table_read(zb_zgpd_id_t *zgpd_id, zgp_tbl_ent_t *ent);

#ifdef ZB_USE_NVRAM
/**
   Read ZGP sink/proxy dataset
*/
void zb_nvram_read_zgp_tbl_dataset(zb_zgp_tbl_t *tbl, zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length, zb_nvram_ver_t ver, zb_uint16_t ds_ver);
zb_ret_t zb_nvram_write_zgp_tbl_dataset(zb_zgp_tbl_t *tbl, zb_uint8_t page, zb_uint32_t pos);
void zb_nvram_update_zgp_tbl_offset(zb_zgp_tbl_t *tbl, zb_uint8_t page, zb_uint32_t dataset_pos, zb_uint32_t pos);
#endif  /* ZB_USE_NVRAM */

#endif  /* ! ZB_ZGPD_ROLE */
/*! @} */
#endif /* ZB_ENABLE_ZGP */
#endif /* ZB_ZGP_COMMON_H */
