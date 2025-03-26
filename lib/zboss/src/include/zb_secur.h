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
/* PURPOSE: Security services and routines - internals
*/

#ifndef ZB_SECUR_H
#define ZB_SECUR_H 1

#include "zb_common.h"
#include "zb_mac.h"
#include "zb_ecc.h"
#include "zb_tlv.h"

/*! @cond internals_doc */
/*! \addtogroup ZB_SECUR */
/*! @{ */


#define ZB_NONCE_LENGTH 13U
#define ZB_KEY_LENGTH 16U

/**
  * @name Secur key attributes
 * @anchor secur_key_attributes
 *
 * KeyAttributes (Table 4-31 Elements of the Key-Pair Descriptor)
 *
 * Note: These values were members of zb_secur_key_attributes_t `enum zb_secur_key_attributes_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_SECUR_PROVISIONAL_KEY 0U
#define ZB_SECUR_UNVERIFIED_KEY  1U
#define ZB_SECUR_VERIFIED_KEY    2U
#define ZB_SECUR_APPLICATION_KEY 3U    /*!< non-standard - for quick check for !TCLK */
#define ZB_SECUR_ANY_KEY_ATTR    0xFFU /*!< Any key attribute - for zb_secur_get_link_key_by_address() */
/** @} */

/**
 * KeyNegotiationState (Table 4-31 Elements of the Key-Pair Descriptor)
 *
 * Note: These values were members of zb_secur_key_negotiation_state_t `enum zb_secur_key_negotiation_state_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
#define ZB_SECUR_NO_KEY_NEGOTIATION       0x00U
#define ZB_SECUR_START_KEY_NEGOTIATION    0x01U
#define ZB_SECUR_VERIFY_KEY_NEGOTIATION   0x02U
#define ZB_SECUR_KEY_NEGOTIATION_COMPLETE 0x03U
#define ZB_SECUR_DEVICE_INTERVIEW         0x04U
#define ZB_SECUR_ANY_KEY_NEGOTIATION      0xFFU

/**
 * @name Secur link key type.
 * @anchor secur_aps_link_key_type
 *
 * Note: These values were members of `enum zb_secur_aps_link_key_type_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_SECUR_UNIQUE_KEY 0U
#define ZB_SECUR_GLOBAL_KEY 1U
/** @} */

/**
 * @name BDB node join link key type.
 * @anchor bdb_node_join_link_key_type
 *
 * Note: These values were members of `enum zb_bdb_node_join_link_key_type_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define BDB_DEFAULT_GLOBAL_TRUST_CENTER_LINK_KEY        0U
#define BDB_DISTRIBUTED_SECURITY_GLOBAL_LINK_KEY        1U
#define BDB_INSTALL_CODE_DERIVED_PRECONFIGURED_LINK_KEY 2U
/** @cond touchlink */
#define BDB_TOUCHLINK_PRECONFIGURED_LINK_KEY            3U
/** @endcond */ /* touchlink */

/* Defines to simplify merge with r22 */

/**
 * @name r22 key src mapped to r23 zb_post_join_key_upd_method_t
 * @anchor zb_secur_key_attributes_t
 *
 * Note: These values were members of `enum zb_secur_aps_link_key_type_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_SECUR_KEY_SRC_UNKNOWN ZB_POST_JOIN_KEY_UPD_METH_NOT_UPDATED
#define ZB_SECUR_KEY_SRC_CBKE ZB_POST_JOIN_KEY_UPD_METH_APP_DEFINED_CERT_BASED_MUTUAL
/** @} */

/**
   APS Device Key Pair Set structure

   Structure contain record Application key with attributes (address, global/unique, etc.)
 */
/* Since that structure used to save into nvram, pack it! */
typedef ZB_PACKED_PRE struct zb_aps_secur_common_data_s
{
  zb_uint8_t coordinator_version;
  zb_uint8_t align[3];
} ZB_PACKED_STRUCT zb_aps_secur_common_data_t;

typedef ZB_PACKED_PRE struct zb_aps_device_key_pair_set_v1_s
{
  zb_ieee_addr_t  device_address;               /*!< Partner address */
  zb_uint8_t      link_key[ZB_CCM_KEY_SIZE];         /*!< Link key, see Spec. Not use in current
                                                      * release */
  /* 4.4.1.1[2] Security Processing of Outgoing[Incoming] Frames */
/* Moved to zb_aps_device_key_pair_array_t */
/* This fields were commented out after discussion with EE.
 * ZB spec does not require to save APS key frame counters into NVRAM.
 */
  zb_bitfield_t   aps_link_key_type:1; /*!< @ref secur_aps_link_key_type  */
  zb_bitfield_t   key_source:1;        /*!< how the key was obtained @ref secur_key_sources */
  zb_bitfield_t   key_attributes:2;    /*!< attributes of the key @ref secur_key_attributes */
  zb_bitfield_t   reserved:4;
  zb_uint8_t      align[3];
} ZB_PACKED_STRUCT zb_aps_device_key_pair_nvram_v1_t;


typedef ZB_PACKED_PRE struct zb_aps_device_key_pair_set_v2_s
{
  zb_ieee_addr_t device_address;              /*!< Partner address */
  zb_uint8_t     link_key[ZB_CCM_KEY_SIZE];   /*!< Link key, see Spec. Not use in current
                                                * release */
  zb_uint8_t     passphrase[ZB_CCM_KEY_SIZE]; /*!< Passphrase */

  /* 4.4.1.1[2] Security Processing of Outgoing[Incoming] Frames */
  /* Moved to zb_aps_device_key_pair_array_t */
  /* This fields were commented out after discussion with EE.
   * ZB spec does not require to save APS key frame counters into NVRAM.
   */

  zb_bitfield_t  key_upd_method:3;            /*!< @ref zb_post_join_key_upd_method_t */
  zb_bitfield_t  initial_join_auth:3;         /*!< @ref zb_initial_join_auth_t */
  zb_bitfield_t  key_attributes:2;            /*!< attributes of the key @ref zb_secur_key_attributes */
  zb_bitfield_t  passphrase_update_allowed:1; /*!< PassphraseUpdateAllowed
                                               *   Update is allowed just after create, disallowed
                                               *   after Get Auth Token done. */
  zb_bitfield_t  aps_link_key_type:1;         /*!< @ref zb_secur_aps_link_key_type_t: unique vs global (r20 stuff) */
  zb_bitfield_t  kn_methods_present:1; /*!< If true, then supported_kn_methods and supported_kn_secrets fields are present */
  zb_bitfield_t  aps_frame_cnt_sync_supported:1;
  zb_bitfield_t  reserved: 4;

  zb_uint8_t     supported_kn_methods; /*!< Supported Key negotiation methods */
  zb_uint8_t     supported_kn_secrets; /*!< Supported Key negotiation secrets */

} ZB_PACKED_STRUCT zb_aps_device_key_pair_set_v2_t;

typedef zb_aps_device_key_pair_set_v2_t zb_aps_device_key_pair_nvram_t;
typedef zb_aps_device_key_pair_set_v2_t zb_aps_device_key_pair_set_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_aps_device_key_pair_nvram_t);

#define ZB_APS_DEVICE_KEY_PAIR_CACHED (zb_uint32_t)0x07ffffffU

/* zb_aps_device_key_pair_array_t moved to zboss_api_internal.h */

typedef struct zb_aps_device_key_pair_storage_s
{
  zb_uint8_t  nvram_page;
  zb_uint8_t  cached_i;
  zb_uint16_t nvram_ds_ver;
  zb_aps_device_key_pair_set_t cached;
#ifndef ZB_CONFIGURABLE_MEM
  zb_aps_device_key_pair_array_t key_pair_set[ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE]; /*!< APS Application Key pair table */
#else
  zb_aps_device_key_pair_array_t *key_pair_set;
#endif
} zb_aps_device_key_pair_storage_t;

typedef ZB_PACKED_PRE struct zb_aps_device_key_pair_set_nvram_1_0_s
{
  zb_ieee_addr_t  device_address;               /*!< Partner address */
  zb_uint8_t master_key[ZB_CCM_KEY_SIZE];       /*!< Master key */
  zb_uint8_t global_link_key;                   /*!< 1 - Global key, 0 - Unique key  */
#ifdef ZB_PLATFORM_CORTEX_M3
  zb_uint8_t  aligned[3];
#endif /* ZB_PLATFORM_CORTEX_M3 */
} ZB_PACKED_STRUCT zb_aps_device_key_pair_set_nvram_1_0_t;


/**
   APS Installcode structure for NVRAM
 */
typedef ZB_PACKED_PRE struct zb_aps_installcode_nvram_s
{
  zb_ieee_addr_t  device_address;               /*!< Partner address */
  /*AEV:Here data was a filler: align[2]; - remove it for storage of IC type for now.
    options lowest 3 bits [0-2]:
      000 - ZB_IC_TYPE_48,
      001 - ZB_IC_TYPE_64,
      010 - ZB_IC_TYPE_96,
      011 - ZB_IC_TYPE_128,
      100 - ZB_IC_TYPE_PASSCODE,
      101 - ZB_IC_TYPE_128_HASHED,
      otherwise is reserved .*/
  zb_uint8_t      options;
  zb_uint8_t      align;
  zb_uint8_t      installcode[ZB_CCM_KEY_SIZE+ZB_CCM_KEY_CRC_SIZE];       /*!< 16b installcode +2b crc */
  /*hint:we can remove 2b crc at the end of installcode, but we must keep ic type, thus align to dword and don't change it*/
} ZB_PACKED_STRUCT zb_aps_installcode_nvram_t;

#ifdef ZB_SECURITY_INSTALLCODES
extern const zb_uint8_t zb_ic_size_by_type[ZB_IC_TYPE_MAX];

#define ZB_IC_TYPE_MAX_SIZE 16U

#define ZB_IC_SIZE_BY_TYPE(t) ( ( t ) < ZB_IC_TYPE_MAX ? zb_ic_size_by_type[( t )]:zb_ic_size_by_type[(ZB_IC_TYPE_MAX - 1U)] )

#define ZB_IC_GET_TYPE_FROM_OPT(opt) ( (opt) & 0x7U )

/* zb_aps_installcode_storage_t moved to zboss_api_internal.h */

typedef struct zb_secur_ic_add_s
{
  zb_uint8_t const *address;
  zb_uint8_t const *ic;
  zb_uint8_t do_update;
  zb_uint8_t type;
} zb_secur_ic_add_t;
#endif


/**
 * @name Network key types
 * @anchor secur_key_types
 * @see r20 4.4.3.1.1
 * @see r21 Table 4.9 StandardKeyType Parameter of the Transport-Key, Verify-Key, and Confirm-Key
 * Primitives
 *
 * Note: These values were members of `enum zb_secur_key_types_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_TC_MASTER_KEY_OBSOLETE          0U   /*!< reserved */
#define ZB_STANDARD_NETWORK_KEY            1U   /*!< NWK key */
#define ZB_APP_MASTER_KEY_OBSOLETE         2U   /*!< reserved */
#define ZB_APP_LINK_KEY                    3U   /*!< Application link key */
#define ZB_TC_LINK_KEY                     4U   /*!< Trust-center link key */
#define ZB_HIGH_SECUR_NETWORK_KEY_OBSOLETE 5U   /*!< reserved */

#define ZB_DIRECT_EPHEMERAL_GLOBAL_AUTHORIZATION_KEY 0xB0U /*!< Ephemeral Global Authorization Key, see the Zigbee Direct Specification for detail  */
#define ZB_DIRECT_EPHEMERAL_UNIQUE_AUTHORIZATION_KEY 0xB1U /*!< Ephemeral Unique Authorization Key, see the Zigbee Direct Specification for detail  */
#define ZB_DIRECT_BASIC_AUTHORIZATION_KEY 0xB2U /*!< Basic Authorization Key, see the Zigbee Direct Specification for detail  */

#define ZB_TC_INVALID_LINK_KEY 0xFFU /*!< Invalid Link Key type value  */
/** @} */

#ifdef ZB_DIRECT_ENABLED
/**
 * @brief Checks whether authorization key type is ZB Direct network key
 *
 */
#define ZB_KEY_IS_DIRECT_NWK_AUTH_KEY(key)                   \
  ((key) == ZB_DIRECT_BASIC_AUTHORIZATION_KEY ||             \
  (key) == ZB_DIRECT_EPHEMERAL_GLOBAL_AUTHORIZATION_KEY ||   \
  (key) == ZB_DIRECT_EPHEMERAL_UNIQUE_AUTHORIZATION_KEY )

#else /* ZB_DIRECT_ENABLED */
#define ZB_KEY_IS_DIRECT_NWK_AUTH_KEY(key) (ZB_FALSE)
#endif /* ZB_DIRECT_ENABLED */

/**
 * @brief Type for Network key types values.
 *
 * Holds one of @ref secur_key_types. Kept only for backward compatibility as
 * they were declared previously as 'enum zb_secur_key_types_e'.
 */
typedef zb_uint8_t zb_secur_key_type_t;

/**
 * @name RequestKey type values
 * @anchor secur_request_key_types
 * @see r21 Table 4-19 RequestKeyType Values
 *
 * Note: These values were members of `enum zb_secur_request_key_types_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_REQUEST_APP_LINK_KEY 2U /*!< Application link key */
#define ZB_REQUEST_TC_LINK_KEY  4U /*!< Trust-center link key */
/** @} */

/**
 * @brief Type for RequestKey type values.
 *
 * Holds one of @ref secur_request_key_types. Kept only for backward compatibility as
 * they were declared previously as 'enum zb_secur_key_attributes_e'.
 */
typedef zb_uint8_t zb_secur_key_attributes_t;

/**
 * @name Key ID
 * @see r20 4.5.1.1.2, r21 Table 4.31 Encoding of the Key Identifier Sub-Field
 *      and 4.5.3 Cryptographic Key Hierarchy
 * @anchor secur_key_id
 */
/** @{ */
#define  ZB_SECUR_DATA_KEY           0U     /*!< Data key  - Link key without a hash */
#define  ZB_SECUR_NWK_KEY            1U     /*!< NWK key - use NWK key*/
#define  ZB_SECUR_KEY_TRANSPORT_KEY  2U     /*!< Transport key - use hash of TC Link key with 2. Used to protect NWK key send. */
#define  ZB_SECUR_KEY_LOAD_KEY       3U     /*!< Load key - use hash of TC key with 3. Used to protect APS LC send */
#define  ZB_SECUR_DATA_KEY_N_UNENCR  0xFEU  /*!< Send 2 copy: encrypted and not encrypted at APS */
#define  ZB_NOT_SECUR                0xFFU  /*!< Not use APS security */
/** @} */

/**
 * @brief Type for key ID.
 *
 * Holds one of @ref secur_key_id. Kept for backward compatibility as
 * @ref secur_key_id were declared previously as enum.
 */
typedef zb_uint8_t zb_secur_key_id_t;

/**
   Auxiliary frame header (4.5.1) for NWK frame and NWK key

   Extended nonce subfield set to 1 (4.3.1.1).
   source_address amd key_seq_number exist.
 */
typedef ZB_PACKED_PRE struct zb_nwk_aux_frame_hdr_s
{
  zb_uint8_t     secur_control;         /*!< Security Control Field */
  zb_uint32_t    frame_counter;         /*!< Frame Counter */
  zb_ieee_addr_t source_address;        /*!< Extended Source */
  zb_uint8_t     key_seq_number;        /*!< Key Sequence Number */
} ZB_PACKED_STRUCT zb_nwk_aux_frame_hdr_t;


/**
   Auxiliary frame header (4.5.1) for APS frame encrypted by NWK key

   Extended nonce subfield set to 0 (4.4.1.1).
   source_address absent, key_seq_number exists.
 */
typedef ZB_PACKED_PRE struct zb_aps_nwk_aux_frame_hdr_s
{
  zb_uint8_t     secur_control;      /*!< Security Control Field */
  zb_uint32_t    frame_counter;      /*!< Frame Counter */
  zb_uint8_t     key_seq_number;     /*!< Key Sequence Number */
} ZB_PACKED_STRUCT zb_aps_nwk_aux_frame_hdr_t;

/**
   Auxiliary frame header (4.5.1) for APS frame encrypted by Data key

   Extended nonce subfield set to 0 (4.4.1.1).
   source_address and key_seq_number are absent.
 */
typedef ZB_PACKED_PRE struct zb_aps_data_aux_frame_hdr_s
{
  zb_uint8_t     secur_control;     /*!< Security Control Field */
  zb_uint32_t    frame_counter;     /*!< Frame Counter */
} ZB_PACKED_STRUCT zb_aps_data_aux_frame_hdr_t;

/**
   Auxiliary frame header (4.5.1) for APS frame encrypted by Data key

   Extended nonce subfield set to 1
   source_address is present, key_seq_number is absent.
 */
typedef ZB_PACKED_PRE struct zb_aps_data_aux_nonce_frame_hdr_s
{
  zb_uint8_t     secur_control;     /*!< Security Control Field */
  zb_uint32_t    frame_counter;     /*!< Frame Counter */
  zb_ieee_addr_t source_address;    /*!< Extended Source */
} ZB_PACKED_STRUCT zb_aps_data_aux_nonce_frame_hdr_t;



/**
   CCM nonce (see 4.5.2.2)
 */
typedef ZB_PACKED_PRE struct zb_secur_ccm_nonce_s
{
  zb_ieee_addr_t source_address;    /*!< Extended Source */
  zb_uint32_t    frame_counter;     /*!< Frame Counter */
  zb_uint8_t     secur_control;     /*!< Security Control Field */
} ZB_PACKED_STRUCT zb_secur_ccm_nonce_t;

typedef enum zb_secur_frame_type_e
{
  ZB_DEFAULT_FRAME = 0,
  ZB_TUNNEL_FRAME,
  ZB_RELAY_FRAME
}zb_secur_frame_type_t;

/**
 * See 4.5.1.1
 */

#define ZB_NWK_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_NWK_KEY << 3U) /* key identifier - NWK key */ | 1U << 5U /* ext nonce */)

#define ZB_APS_NWK_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_NWK_KEY << 3U) /* key identifier - NWK key */ | 0U /* ext nonce */)

#define ZB_APS_MASTEP_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_KEY_TRANSPORT_KEY << 3U) /* key identifier - APS master key */ | 0U /* ext nonce */)

#define ZB_APS_LINK_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_KEY_LOAD_KEY << 3U) /* key identifier - APS link key */ | 0U /* ext nonce */)

#define ZB_APS_DATA_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_DATA_KEY << 3U) /* key identifier - APS data key */ | 0U /* ext nonce */)

#define ZB_APS_SET_SECUR_CONTROL(security_level, key_identifier, ext_nonce) \
  ( (security_level) | ((key_identifier) << 3U) | ((((ext_nonce) != 0u) ? 1U : 0U) << 5U) )

#define ZB_SECUR_SET_ZEROED_LEVEL(ctrl)  (ctrl) = (ctrl) & (~0x07U)

#define ZB_SECUR_AUX_HDR_GET_KEY_TYPE(ctrl) (((ctrl) >> 3U) & 0x3U)

#define ZB_SECUR_AUX_HDR_SET_KEY_TYPE(ctrl, key_type) ( (ctrl) = ((ctrl) & ~(0x3U << 3U)) | (((key_type) & 0x3U) << 3U))

#define ZB_SECUR_GET_SECURITY_LEVEL(ctrl) ((ctrl) & 0x7U)

#define ZB_SECUR_SET_SECURITY_LEVEL(ctrl, level) ( (ctrl) = ((ctrl) & ~0x07U) | ((level) & 0x07U) )

#define ZB_SECUR_GET_SECURITY_NONCE(ctrl) (((ctrl) >> 5U) & 0x1U)

#define ZB_SECUR_APS_FRAME_CNT_SYNC_IS_ENABLED(lk_cap) ((lk_cap) & 0x1U)

#define ZB_SECUR_SET_APS_FRAME_CNT_SYNC(lk_cap) ((lk_cap) |= (1 << 0u))

#if 0
#define ZB_SECUR_SET_BUF_MAC_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type |= ZB_SECUR_MAC_ENCR)

#define ZB_SECUR_SET_BUF_NWK_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type |= ZB_SECUR_NWK_ENCR)

#define ZB_SECUR_SET_BUF_APS_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type |= ZB_SECUR_APS_ENCR)

#define ZB_SECUR_CLR_BUF_MAC_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type &= ~ZB_SECUR_MAC_ENCR)

#define ZB_SECUR_CLR_BUF_NWK_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type &= ~ZB_SECUR_NWK_ENCR)

#define ZB_SECUR_CLR_BUF_APS_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type &= ~ZB_SECUR_APS_ENCR)

#define ZB_SECUR_GET_BUF_MAC_ENCR(_pbuf) (((_pbuf)->u.hdr.encrypt_type & ZB_SECUR_MAC_ENCR) >> 2U)

#define ZB_SECUR_GET_BUF_NWK_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type & ZB_SECUR_NWK_ENCR)

#define ZB_SECUR_GET_BUF_APS_ENCR(_pbuf) (((_pbuf)->u.hdr.encrypt_type & ZB_SECUR_APS_ENCR) >> 1U)
#endif

/* Bitmask from Configuration Mode Global TLV*/
#define ZB_SECUR_GET_CONF_MODE(bmask)         ((bmask) & 0x01U)
#define ZB_SECUR_GET_LINK_KEY_ENC(bmask)      (((bmask) >> 1U) & 0x01U)
#define ZB_SECUR_GET_LEAVE_REQ_ALLOWED(bmask) (((bmask) >> 2U) & 0x01U)

#define ZB_SECUR_SET_CONF_MODE(bmask)         ((bmask) |= 0x01U) /* 1 if is restricted mode */
#define ZB_SECUR_SET_LINK_KEY_ENC(bmask)      ((bmask) |= 0x02U)
#define ZB_SECUR_SET_LEAVE_REQ_ALLOWED(bmask) ((bmask) |= 0x04U)

/**
   Initialize APS security data structures
 */
void zb_aps_secur_init(void);

/**
 * @brief Key hash implementation according to FIPS Pub 198 spec.
 *        HMAC(K, text) = H((K0 XOR opad )|| H((K0 XOR ipad) || text))
 *        K0 size is equal to the hashing function block size.
 *        Notes:
 *          - By R22 Zigbee spec, key length is equal to block size (16 bytes),
 *              so there is no need to add any paddings, so these paddings are not implemented.
 *              (Check static assertion above)
 *          - Zigbee uses only one byte as a text.
 *
 * @param key pointer to key, assuming that key length is equal to block size (@see ZB_CCM_KEY_SIZE, @see ZB_SECUR_B6_HASH_BLOCK_SIZE)
 * @param text additional info (ZigBee spec always uses one byte)
 * @param hash_key hash result having length equal to @see ZB_SECUR_B6_HASH_BLOCK_SIZE
 */
void zb_cmm_key_hash(zb_uint8_t *key, zb_uint8_t text, zb_uint8_t *hash_key);


/**
 * Size of one hash block.
 * @note The result of hashing is exactly one hash block.
 */
#define ZB_SECUR_B6_HASH_BLOCK_SIZE 16U

/**
 * Max size of message that can be hashed using the hash described in R22 spec (B.6):
 *  > Note that the cryptographic hash function operates on bit strength of length LESS
 *    than 2^{2n} bits where n is the block size (or key size)
 *    of the established block cipher, in bytes
 *    (2n == 32).
 *
 * Note:
 *   Max length in bytes is equal to (2^{2n - 3} - 1) == (2^{2n} / 8 - 1)
 *    because:
 *      a) Need to convert bits to bytes (divide by 8 or right shift by 3)
 *      b) There is a strong condition (LESS) in spec.
 *
 * Earlier it used to have max length equal to 2^{28} - 1 bytes, which is wrong (described above).
 */
#define ZB_SECUR_B6_HASH_MAX_MSG_LEN_BYTES ((1U << (2U * (ZB_SECUR_B6_HASH_BLOCK_SIZE) - 3U)) - 1U)

/**
 * @brief Computes Matyas-Meyer-Oseas hash (Hash_{j} = E(Hash_{j-1}, M_{j}) XOR M_{j}).
 *
 * @param msg_block[in] Block of message. (Won't be changed)
 * @param hash[in, out] Will be used as Hash_{j-1} upon input.
 *                        Result hash value (Hash_{j}) will be stored here.
 */
void zb_sec_matyas_meyer_oseas_hash(const zb_uint8_t *msg_block, zb_uint8_t *hash);

/**
 * @brief Adds padding for hash described in B.6 of R22 ZigBee spec.
 *        Uses remainder of message if `orig_msg_len % block_size != 0`.
 *
 * @param msg_remainder[in] The last part of msg in case if it isn't multiple of block size.
 *                            Otherwise, it won't be used and may have any value.
 * @param orig_msg_len      Original message length to write into tail.
 * @param hash[in/out]      Previously computed hash to use in Matyas-Meyer-Oseas hash.
 */
void zb_sec_b6_hash_pad_tail(const zb_uint8_t *msg_remainder, const zb_uint32_t orig_msg_len, zb_uint8_t *hash);

/**
 * @brief Computes hash according to B.6 of ZigBee spec.
 *        Notes:
 *          - In spec this algorithm is expressed in bits,
 *              this implementation uses bytes if possible.
 *          - For simplicity, actions - 1.a will be called "short tail", 2.a - "long tail".
 *
 * @param input[in]     input message to get hash from.
 * @param input_len     message length in bytes. (It should be less than 2^29)
 * @param output[out]   hash having length @see ZB_SECUR_B6_HASH_BLOCK_SIZE
 * @return zb_bool_t    Hash have been computed or not.
 */
zb_bool_t zb_sec_b6_hash(const zb_uint8_t *input, const zb_uint32_t input_len, zb_uint8_t *output);

/**
   Search for link key by pair address and attribute.

   @note Up to 2 keys to the same address can exist: one is
   VERIFIED/PROVISIONING, another is UNVERIFIED.
   See BDB 10.3.3 Behavior when a known node joins.

   @param address - IEEE address
   @param attr - key attribute to search.
   @return Key Pair Set structure
 */
zb_aps_device_key_pair_set_t *zb_secur_get_link_key_by_address(zb_ieee_addr_t address,
                                                               zb_secur_key_attributes_t attr);

/**
   Search for link key index by pair address and attribute.

   @note Up to 2 keys to the same address can exist: one is
   VERIFIED/PROVISIONING, another is UNVERIFIED.
   See BDB 10.3.3 Behavior when a known node joins.

   @param address - IEEE address
   @param attr - key attribute to search.
   @return keypair index if link key exists, -1 otherwise
 */
zb_uint16_t zb_aps_keypair_get_index_by_addr(zb_ieee_addr_t dev_addr,
                                             zb_secur_key_attributes_t attr);

#ifndef ZB_HW_ZB_AES128
/*
  Encrypt a single 128-bit block by a 128-bit key using AES
  url: en.wikipedia.org/wiki/Advanced_Encryption_Standard
*/
void zb_sw_aes128(const zb_uint8_t *key, const zb_uint8_t *msg, zb_uint8_t *c);
#endif


#ifndef ZB_HW_ZB_AES128_DEC
void zb_sw_aes128_dec(const zb_uint8_t *key, const zb_uint8_t *msg, zb_uint8_t *c);
#endif

/**
   CCM* encryption and authentication procedure for Standard security

   This function is a wrapper over ZB_CCM_ENCRYPT_N_AUTH_RAW().
   The main goal is to incapsulate working with Zigbee buffers.

   @param key   - 16-bytes key
   @param nonce - 16-bytes nonce - see 4.5.2.2 "CCM nonce" for details
   @param string_a - 'a' parameter - authentication string -
                     NwkHeader||AuxuluaryHeader for Standard security. See 4.3.1.1 for details
   @param string_a_len - l(a)
   @param string_m - 'm' parameter - text data -
                     Payload for Standard security. See 4.3.1.1 for details
   @param string_m_len - l(m)
   @param crypted_text - encryption result - user supplied buffer. Must have size
          string_a_len + string_m_len + ccm_m rounded to 16.
          Result len is always string_a_len + string_m_len + ccm_m.

   @note may use HW or SW backend

   @return RET_OK if success, RET_ERROR in case of error
 */
zb_ret_t
zb_ccm_encrypt_n_auth(
  zb_uint8_t *key,
  const zb_uint8_t *nonce,
  const zb_uint8_t *string_a,
  zb_uint32_t string_a_len,
  const zb_uint8_t *string_m,
  zb_uint32_t string_m_len,
  zb_bufid_t crypted_text);

#ifndef ZB_HW_CCM_ENCRYPT_N_AUTH_RAW
/**
   Internal CCM* encryption and authentication procedure for Standard security

   This implementation may be not optimal. It done for debug only: be able to
   encrypt in Linux to be able to compare results with real HW.
   To be debugged using test vectors from the spec.

   This function encrypts the string_m data only.

   @param key   - 16-bytes key
   @param nonce - 16-bytes nonce - see 4.5.2.2 "CCM nonce" for details
   @param string_a - 'a' parameter - authentication string -
                     NwkHeader||AuxiliaryHeader for Standard security. See 4.3.1.1 for details
   @param string_a_len - l(a)
   @param string_m - 'm' parameter - text data -
                     Payload for Standard security. See 4.3.1.1 for details
   @param string_m_len - l(m)
   @param dest_buf - buffer pointer where to put the result. The result is
                     the encrypted string_m || U
   @param dest_len - size of the result buffer. Must be string_m_len + ccm_m rounded to 16.
          Result len is always string_m_len + ccm_m.


   @return RET_OK if success, RET_ERROR in case of error
 */
zb_ret_t
zb_sw_ccm_encrypt_n_auth_raw(
  zb_uint8_t *key,
  const zb_uint8_t *nonce,
  const zb_uint8_t *string_a,
  zb_uint32_t string_a_len,
  const zb_uint8_t *string_m,
  zb_uint32_t string_m_len,
  zb_uint8_t *dest_buf,
  zb_uint16_t dest_len
);
#endif /* ZB_HW_CCM_ENCRYPT_N_AUTH_RAW */

/**
   CCM* decryption and authentication procedure for Standard security

   This function is a wrapper over ZB_CCM_DECRYPT_N_AUTH_RAW().
   The main goal is to incapsulate working with Zigbee buffers.

   @param key   - 16-bytes key
   @param nonce - 16-bytes nonce - see 4.5.2.2 "CCM nonce" for details
   @param buf   - buffer with encrypted_text [IN], for decryption result [OUT]
   @param string_a_len - length string_a - authentication string -
                     NwkHeader||AuxiliaryHeader for Standard security. See 4.3.1.1 for details
   @param string_c_len - length text data -
                     Payload for Standard security. See 4.3.1.1 for details

   @note may use HW or SW backend

   @return RET_OK if success, RET_ERROR in case of error (authentication failure)
 */
zb_ret_t
zb_ccm_decrypt_n_auth(
  zb_uint8_t *key,
  const zb_uint8_t *nonce,
  zb_bufid_t buf,
  zb_uint32_t string_a_len,
  zb_uint32_t string_c_len);

#ifndef ZB_HW_CCM_DECRYPT_N_AUTH_RAW
/**
 * @brief CCM* decryption and authentication procedure for Standard security
 *
 * @param key 16-bytes key
 * @param nonce - 16-bytes nonce - see 4.5.2.2 "CCM nonce" for details
 * @param string_a - pointer to the string_a data
 * @param string_a_len - length string_a - authentication string -
                     NwkHeader||AuxuluaryHeader for Standard security. See 4.3.1.1 for details
 * @param string_c_u - pointer to the encoded string_m and U (string_m || U)
 * @param string_c_u_len - length text data -
                     Payload for Standard security. See 4.3.1.1 for details
 * @param string_dest - Pointer where to store the result in form of decrypted string_m and T after that.
 * @param string_dest_len - [IN]: size of the result buffer. Must be large enough to store len(string_m) + 16
 *                          [OUT]: size of real output data - len(string_m) + CCM_M
 * @return zb_ret_t
 */
zb_ret_t
zb_sw_ccm_decrypt_n_auth_raw(
  zb_uint8_t *key,
  const zb_uint8_t *nonce,
  const zb_uint8_t *string_a,
  zb_uint32_t string_a_len,
  const zb_uint8_t *string_c_u,
  zb_uint16_t string_c_u_len,
  zb_uint8_t *string_dest,
  zb_uint16_t *string_dest_len);
#endif /* ZB_HW_CCM_DECRYPT_N_AUTH_RAW */

/**
   Decryption and authentication according to standard security requirements.

   Decrypt "on place". Use HW if possible.
 */
#define zb_ccm_decrypt_n_auth_stdsecur zb_ccm_decrypt_n_auth

void zb_ccm_auth_trans(
  zb_ushort_t ccm_m,
  zb_uint8_t *key,
  const zb_uint8_t *nonce,
  const zb_uint8_t *string_a,
  zb_uint32_t string_a_len,
  const zb_uint8_t *string_m,
  zb_uint32_t string_m_len,
  zb_uint8_t *t);

void encrypt_trans(
  zb_ushort_t ccm_m,
  zb_uint8_t *key,
  const zb_uint8_t *nonce,
  const zb_uint8_t *string_m,
  zb_uint32_t string_m_len,
  const zb_uint8_t *t,
  zb_uint8_t *encrypted,
  zb_uint16_t encrypted_len);

/**
   Secure frame at NWK level according to the current security mode
   This procedure secures frame 'on place' increasing its size.
   It is supposed that buffer tail has enough free space.

   @param src - nwk packet to secure
   @param mac_hdr_size - size of header placed before mac hdr
   @param dst - buffer for the secured frame


   @return RET_OK if ok, else error code
 */
zb_ret_t zb_nwk_secure_frame(zb_bufid_t src, zb_uint_t mac_hdr_size, zb_bufid_t dst);


/**
   Unsecure NWK frame at NWK level according to the current security mode
   This procedure secures frame 'on place' decreasing its size.

   @param param - ref to nwk packet to unsecure

   @return RET_OK if success or RET_ERROR if unsecure failed
   Attention: if frame unsecure failed, this function reuses packet buffer to
   send NWK status. Don't use the buffer if return code != RET_OK!
 */
zb_ret_t zb_nwk_unsecure_frame(zb_uint8_t param);

/**
   Allocate and fill space for auxiliary security header in the APS data or command frame.

   @param p - pointer to the aux hdr
   @param key_type - key type for aux header see 4.5.1.1.2, @see @ref secur_key_id
   @param ext_nonce - extended nonce flag

   If nwk_key is false and transport_key is false then use APS data key
 */
void zb_secur_aps_aux_hdr_fill(zb_uint8_t *p, zb_secur_key_id_t key_type, zb_bool_t ext_nonce);

/**
   Add security to the APS command frame. Allocate space for APS header. Fill APS Aux security header.

   @param buf - buffer with command payload
   @param command_id - command id to assign into APS command header
   @param secure_aps - secure type APS level
 */
void zb_aps_command_add_secur(zb_bufid_t buf, zb_uint8_t command_id, zb_secur_key_id_t secure_aps);

/**
   Encrypt APS frame before its send.

   Need to put secured frame into another buffer because original buffer will be
   passed back to APS in CONFIRM primitive and, potentially, will be
   retransmitter by APS layer, so must keep it unencrypted.

   @param src - source buffer with all headers filled, including MHR
   @param mac_hdr_size - size of MAC header
   @param dst - destination buffer (for secure result)
   @param frame_type - default, tunnel or relay

   @return RET_OK if successfully encrypted frame, error code otherwise
 */
zb_ret_t zb_aps_secure_frame(zb_bufid_t src, zb_uint_t mac_hdr_size, zb_bufid_t dst, zb_secur_frame_type_t frame_type);

void zb_aps_secure_frame_by_key(zb_bufid_t src, zb_bufid_t dst, zb_uint8_t *aps_hdr, zb_uint8_t key[ZB_CCM_KEY_SIZE]);

/**
   Decrypt APS frame "on place"

   @param buf - (in/out) buffer holding frame
   @param keypair_i_p - (out) index of APS keypair used to decrypt or (zb_uint16_t)-1 if used default TCLK
   @param key_id_p - pointer to variable that contains key used to protect the frame
   @param is_verified_tclk - (out) ZB_TRUE if the frame was decrypted successfully using TCLK, ZB_FALSE otherwise

   @return RET_OK if successfully decrypted frame, error code otherwise
 */
zb_ret_t zb_aps_unsecure_frame(zb_bufid_t buf, zb_uint16_t *keypair_i_p, zb_secur_key_id_t *key_id_p, zb_bool_t *is_verified_tclk);

/**
   Return size of APS aux secure header

   @param secur_control - aux APS header secur_control field
   @return aux header size if ok or -1 if error (unknown key type)
 */
zb_ushort_t zb_aps_secur_aux_size(zb_uint8_t secur_control);

#ifdef ZB_MAC_SECURITY
/**
   Encrypt MAC frame before its send.

   Need to put secured frame into another buffer because original buffer will be
   passed back to APS in CONFIRM primitive and, potentially, will be
   retransmitter by APS layer, so must keep it unencrypted.

   @param src - source buffer with all headers filled, including MHR
   @param mac_hdr_size - size of MAC header
   @param dst - destination buffer (for secure result)

   @return RET_OK if successfully encrypted frame, error code otherwise
 */
zb_ret_t zb_mac_secure_frame(zb_bufid_t src, zb_uint_t mac_hdr_size, zb_bufid_t dst);

/**
   Encrypt MAC frame "on place"

   @param param - (in/out) buffer holding frame

   @return RET_OK if successfully decrypted frame, error code otherwise
 */
zb_ret_t zb_mac_unsecure_frame(zb_uint8_t param);
#endif

/**
   Parameters for APSME-TRANSPORT-KEY.request primitive
 */
typedef struct zb_apsme_transport_key_req_s
{
  zb_addr_u dest_address;  /*!< destination address  */
  zb_uint8_t addr_mode;    /*!< The type of destination address supplied by
                             the DstAddr parameter - see @ref address_modes.
                             This field is non-standard: according to table 4.11
                             dest_address can be 64-bit only.
                             But, according to 4.6.3.4.1  Trust Center Operation,
                             TC must issue APSME-TRANSPORT-KEY.request with
                             broadcast address - means, need 16-bit address here!
                           */
  zb_uint8_t key_type;          /*!< @see @ref secur_key_types  */
  union key_holder_u {
    struct apsme_transport_key_nwk_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];     /*!< Key */
      zb_uint8_t key_seq_number;           /*!< Sequence Number */
      zb_uint8_t use_parent;               /*!< Use parent - see 4.6.3 */
      zb_ieee_addr_t parent_address;       /*!< Parent Address */
    } nwk;                                 /*!< Use for transmit NWK key */
    struct apsme_transport_key_app_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];      /*!< Key */
      zb_ieee_addr_t partner_address;       /*!< Partner address */
      zb_uint8_t initiator;                 /*!< Is initiator.
                                                 @note Exists in r20, no in r21, seems, still necessary. */
    } app;                                  /*!< Use for transmit Application key */
  } key;
  /* mega-hack: zb_aps_secure_frame uses dest_address to seek APS key. But,
     zb_aps_secure_frame is called from MAC when NWK also put parameter into
     buffer tail. Let's reserve a space for mcps data request parameters.
     Agree, this is ugly solution. The alternative is to parse packet contents,
     but is is more complex: must analyze for tunnel/direct send.
 */
  zb_uint8_t filler[sizeof(zb_mcps_data_req_params_t) > sizeof(union key_holder_u) ? sizeof(zb_mcps_data_req_params_t) - sizeof(union key_holder_u) : 4];
} zb_apsme_transport_key_req_t;

/**
   Variable part of Transport key command for translate NWK key
 */
typedef ZB_PACKED_PRE struct zb_transport_key_nwk_pkt_s
{
  zb_uint8_t     seq_number;        /*!< Sequence Number */
  zb_ieee_addr_t dest_address;      /*!< Destination address */
  zb_ieee_addr_t source_address;    /*!< Source address */
} ZB_PACKED_STRUCT zb_transport_key_nwk_pkt;

/**
   Variable part of Transport key command for translate TC key
 */
typedef ZB_PACKED_PRE struct zb_transport_key_tc_pkt_s
{
  zb_ieee_addr_t dest_address;      /*!< Destination address */
  zb_ieee_addr_t source_address;    /*!< Source address */
} ZB_PACKED_STRUCT zb_transport_key_tc_pkt;

/**
   Variable part of Transport key command for translate Application key
 */
typedef ZB_PACKED_PRE struct zb_transport_key_app_pkt_s
{
  zb_ieee_addr_t partner_address;   /*!< Partner address */
  zb_uint8_t initiator;             /*!< Is initiator */
}  ZB_PACKED_STRUCT zb_transport_key_app_pkt;

/**
   Transport key command data structure


 */
typedef ZB_PACKED_PRE struct zb_transport_key_dsc_pkt_s
{
  zb_uint8_t     key_type;       /*!< indeed, not part of the nwk key descriptor,
                                  * but it simplifies code  */

  zb_uint8_t     key[ZB_CCM_KEY_SIZE];  /*!< Key*/
  ZB_PACKED_PRE union {
    zb_transport_key_nwk_pkt nwk;       /*!< Variable part for translate NWK key */
    zb_transport_key_tc_pkt tc;         /*!< Variable part for translate TC key */
    zb_transport_key_app_pkt app;       /*!< Variable part for translate Application key */
  } key_data;
} ZB_PACKED_STRUCT zb_transport_key_dsc_pkt_t;

/**
   Update device command data structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_update_device_pkt_s
{
  zb_ieee_addr_t device_address;            /*!< 64-bit address of update device */
  zb_uint16_t    device_short_address;      /*!< 16-bit address of update device */
  zb_uint8_t     status;                    /*!< @see @ref secur_upd_device_status  */
} ZB_PACKED_STRUCT zb_apsme_update_device_pkt_t;

#ifdef ZB_FORMATION
/**
   APSME-TRANSPORT-KEY.request primitive

   @param param - packet buffer filled be \see zb_transport_key_dsc_pkt_t
 */
void zb_apsme_transport_key_request(zb_uint8_t param);
#endif

/**
   Parameter for APSME-UPDATE-DEVICE.request
 */
typedef struct zb_apsme_update_device_req_s
{
  zb_ieee_addr_t dest_address;               /*!< 64-bit Destination address */
  zb_ieee_addr_t device_address;             /*!< 64-bit address of update device */
  zb_uint16_t    device_short_address;       /*!< 16-bit address of update device */
  zb_uint8_t     status;                     /*!< @see @ref secur_upd_device_status  */
} zb_apsme_update_device_req_t;



/**
   Parameter for APSME-UPDATE-DEVICE.indication
 */
typedef struct zb_apsme_update_device_ind_s
{
  zb_ieee_addr_t src_address;               /*!< 64-bit Destination address */
  zb_ieee_addr_t device_address;            /*!< 64-bit address of update device */
  zb_uint16_t    device_short_address;      /*!< 16-bit address of update device */
  zb_uint8_t     status;                    /*!< @see @ref secur_upd_device_status  */
  zb_bitfield_t  tlv_found:1;
  zb_bitfield_t  zvd_capability:1;          /*!< See section about Device Capability Extension in Zigbee Direct spec. */
  zb_bitfield_t  reserved:6;
} zb_apsme_update_device_ind_t;


/**
   Parameter for APSME-SWITCH-KEY.request

   Note about addr_mode: it is spec violation, but it necessary to satisfy TC.
 */
typedef struct zb_apsme_switch_key_req_s
{
  zb_ieee_addr_t dest_address;   /*!< Destination address  */
  zb_uint8_t key_seq_number;     /*!< Sequence Number */
} zb_apsme_switch_key_req_t;


#ifdef ZB_COORDINATOR_ROLE
/**
   APSME-SWITCH-KEY.request primitive

   @param param - packet buffer filled be \see zb_apsme_switch_key_req_t
 */
void zb_apsme_switch_key_request(zb_uint8_t param);
#endif

/**
   APSME-SWITCH-KEY APS command body
 */
typedef ZB_PACKED_PRE struct zb_apsme_switch_key_pkt_s
{
  zb_uint8_t key_seq_number;     /*!< Key Number */
} ZB_PACKED_STRUCT zb_apsme_switch_key_pkt_t;


/**
   APSME-SWITCH-KEY.indication
 */
typedef struct zb_apsme_switch_key_ind_s
{
  zb_ieee_addr_t src_address;       /*!< Source Address*/
  zb_uint8_t key_seq_number;        /*!< Key Number */
} zb_apsme_switch_key_ind_t;


void zb_aps_in_switch_key(zb_uint8_t param);


/**
   APSME-REMOVE-DEVICE.request primitive parameters structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_remove_device_req_s
{
  zb_ieee_addr_t parent_address;    /*!< Parent Address*/
  zb_ieee_addr_t child_address;     /*!< Child Address*/
} ZB_PACKED_STRUCT zb_apsme_remove_device_req_t;


/**
   Remove device command data structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_remove_device_pkt_s
{
  zb_ieee_addr_t child_address;     /*!< Child Address*/
} ZB_PACKED_STRUCT zb_apsme_remove_device_pkt_t;


/**
   APSME-REMOVE-DEVICE.indication primitive parameters structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_remove_device_ind_s
{
  zb_ieee_addr_t src_address;       /*!< Source Address */
  zb_ieee_addr_t child_address;     /*!< Child Address */
} ZB_PACKED_STRUCT zb_apsme_remove_device_ind_t;

#ifdef ZB_COORDINATOR_ROLE
/**
   APSME-REMOVE-DEVICE.request command

   @param param - packet buffer filled be \see zb_apsme_request_key_req_t
 */
void zb_secur_apsme_remove_device(zb_uint8_t param);
#endif

/**
   APSME-REMOVE-DEVICE.indication primitive

   @param param - packet buffer filled be \see zb_apsme_remove_device_pkt_t
 */
void zb_aps_in_remove_device(zb_uint8_t param);

/**
   APSME-REQUEST-KEY.request primitive parameters structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_request_key_req_s
{
  zb_ieee_addr_t dest_address;      /*!< Destination address */
  zb_uint8_t     key_type;          /*!< \see secur_request_key_types  */
  zb_ieee_addr_t partner_address;   /*!< Partner address */
} ZB_PACKED_STRUCT zb_apsme_request_key_req_t;

typedef ZB_PACKED_PRE struct zb_apsme_request_key_pkt_s
{
  zb_uint8_t     key_type;          /*!< \see secur_request_key_types  */
  zb_ieee_addr_t partner_address;   /*!< Partner address */
} ZB_PACKED_STRUCT zb_apsme_request_key_pkt_t;

/**
   APSME-REQUEST-KEY.indication primitive parameters structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_request_key_ind_s
{
  zb_ieee_addr_t src_address;       /*!< Source address */
  zb_uint8_t     key_type;          /*!< \see secur_request_key_types  */
  zb_ieee_addr_t partner_address;   /*!< Partner address */

  zb_uint16_t    keypair_i;     /*!< index of keypair used to decrypt cmd */
} ZB_PACKED_STRUCT zb_apsme_request_key_ind_t;

/**
   APSME-VERIFY-KEY.request

   See 4.4.7.1 APSME-VERIFY-KEY.request
 */
typedef struct zb_apsme_verify_key_req_s
{
  zb_ieee_addr_t dest_address;  /*!< Destination address. ONly TC is valid  */
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_uint8_t use_relay;
} zb_apsme_verify_key_req_t;


/**
4.4.10.7 Verify-Key Command
 */
typedef ZB_PACKED_PRE struct zb_apsme_verify_key_pkt_s
{
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_ieee_addr_t src_address;  /*!< Source address. */
  zb_uint8_t key_hash[ZB_CCM_KEY_SIZE]; /*!< Initiator Verify-Key Hash Value  */
} ZB_PACKED_STRUCT zb_apsme_verify_key_pkt_t;


/**
4.4.7.2 APSME-VERIFY-KEY.indication
 */
typedef struct zb_apsme_verify_key_ind_s
{
  zb_ieee_addr_t src_address;  /*!< Source address. */
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_uint8_t key_hash[ZB_CCM_KEY_SIZE]; /*!< Initiator Verify-Key Hash Value  */
  zb_uint16_t relay_via;
} ZB_PACKED_STRUCT zb_apsme_verify_key_ind_t;

/**
4.4.8.1 APSME-CONFIRM-KEY.request
 */
typedef struct zb_apsme_confirm_key_req_s
{
  zb_uint8_t status;          /*!< A value indicating the success or failure of a previous attempt to verify the trust center link key. See Table 2.27*/
  zb_ieee_addr_t dest_address;  /*!< Destination address.   */
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_uint8_t use_relay;
  zb_uint16_t relay_via;
} zb_apsme_confirm_key_req_t;


/**
4.4.10.8 Confirm-Key Command
 */
typedef ZB_PACKED_PRE struct zb_apsme_confirm_key_pkt_s
{
  zb_uint8_t status; /*!< A value indicating the success or failure of a previous attempt to
                             verify the trust center link key. See Table 2.27*/
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_ieee_addr_t dest_address;  /*!< Dest address. */
} ZB_PACKED_STRUCT zb_apsme_confirm_key_pkt_t;


/**
4.4.8.2 APSME-CONFIRM-KEY.indication
 */
typedef struct zb_apsme_confirm_key_ind_s
{
  zb_uint8_t status; /*!< A value indicating the success or failure of a previous attempt to
                             verify the trust center link key. See Table 2.27*/
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_ieee_addr_t src_address; /*!< The extended 64-bit address of the device that sent the
                                 verify-key command. */
} zb_apsme_confirm_key_ind_t;


/**
   APSME-REQUEST-KEY.request command

   @param param - packet buffer filled be \see zb_apsme_request_key_req_t
 */
void zb_secur_apsme_request_key(zb_uint8_t param);

/**
   APSME-REQUEST-KEY.indication primitive

   @param param - packet buffer filled be \see zb_apsme_request_key_pkt_t
   @param keypair_i - index of APS key used to decrypt APS packet of -1 if default TCLK
   @param key_id - the key used to protect the frame
 */
#ifdef ZB_COORDINATOR_ROLE
void zb_aps_in_request_key(zb_uint8_t param, zb_uint16_t keypair_i
                           , zb_secur_key_id_t key_id
                          );
#endif

/**
   APSME-UPDATE-DEVICE.request command

   @param param - packet buffer filled be \see zb_apsme_update_device_req_t
 */
void zb_apsme_update_device_request(zb_uint8_t param);

#ifdef ZB_FORMATION
/**
   Initialize Trust Center functionality
 */
void secur_tc_init(void);


/**
   Generate NWK keys as random numbers
 */
void secur_nwk_generate_keys(void);
#endif

/**
   Authenticate child after join
 */
void secur_authenticate_child(zb_uint8_t param);

/**
   Remove from NBT unauthenticated device
 */
void secur_forget_unauth_dev(zb_uint8_t addr_ref);


/**
   Generate NWK key by key sequence number

   @param key_seq_number - key sequence number
   @return ref to NWK key from NWK keys table
 */
zb_uint8_t *secur_nwk_key_by_seq(zb_ushort_t key_seq_number);

/**
  Update/create AIB Key-Pair table - legacy r22 call

  Calls zb_secur_update_key_pair_ex() setting defaults:
  - Passphrase              : set to null,
  - passphrase_update_allowed : TRUE
  - initial_join_auth         : ZB_JOIN_INSTALLCODE_KEY if ZB_SECUR_KEY_SRC_CBKE else ZB_JOIN_NO_AUTHENTICATION
  - key_upd_method            : @see zb_secur_key_attributes_t
  - apc_cnt_sync_supported    : FALSE
  - PassphraseUpdateAllowed : ZB_TRUE

  @param address - partner address
  @param key - application key
  @param key_type - @see zb_secur_key_types_t
  @param key_attr - @see zb_secur_key_attributes_t

  @return pointer to the keypair updated/created or NULL in case of error

 */
zb_aps_device_key_pair_set_t * zb_secur_update_key_pair(zb_ieee_addr_t address,
                                                        zb_uint8_t* key,
                                                        zb_uint8_t key_type,
                                                        zb_uint8_t key_attr,
                                                        zb_uint8_t key_source);

/**
  Update/create AIB Key-Pair table - extended >= r23 implementation

  @param address - partner address
  @param key - application key
  @param key_type - @see @ref secur_key_types
  @param key_attr - @see secur_key_attributes
  @param initial_join_auth - @see zb_initial_join_auth_t
  @param key_upd_meth - @see zb_post_join_key_upd_method_t
  @param aps_cnt_sync_supported - if TRUE, then aps frame counter sync in enabled for this key pair
  @param passphrase - if not NULL, PSK for Key Negotiation process; enabled if passphrase is null
  @param passphrase_update_allowed - PassphraseUpdateAllowed (enabled or disabled)

  @return pointer to the keypair updated/created or NULL in case of error

 */
zb_aps_device_key_pair_set_t * zb_secur_update_key_pair_ex(zb_ieee_addr_t address,
                                                           const zb_uint8_t* key,
                                                           zb_uint8_t key_type,
                                                           zb_uint8_t key_attr,
                                                           zb_uint8_t initial_join_auth,
                                                           zb_uint8_t key_upd_meth,
                                                           zb_bool_t  aps_cnt_sync_supported,
                                                           zb_uint8_t* passphrase,
                                                           zb_bool_t passphrase_update_allowed);

/**
   Get address from link key pair by specified index.

   @param idx - index from link key pair set
   @param dev_addr - pointer to store device address (addr from key pair if found, zero addr otherwise)
 */
void zb_aps_keypair_get_addr_by_idx(zb_uint_t idx, zb_uint8_t *dev_addr);

/**
   Get link key pair by specified index.

   @param idx - index from link key pair set
   @return pointer to the keypair if it is found and NULL otherwise
 */
zb_aps_device_key_pair_set_t* zb_aps_keypair_get_ent_by_idx(zb_uint_t idx);

/**
  Write/add key pair to AIB Key-Pair table

  @param ent - pointer to key pair entry
  @param idx - index from link key pair set to write (or -1 to add new element)

  @return status of operation - RET_OK for success, RET_ERROR otherwise
 */
zb_ret_t zb_aps_keypair_write(zb_aps_device_key_pair_set_t *ent, zb_uint32_t idx);

/**
   Load link key pair by specified index.

   @param idx - index from link key pair set
   @return status of operation - RET_OK for success, RET_NOT_FOUND if key pair is not found
 */
zb_ret_t zb_aps_keypair_load_by_idx(zb_uint_t idx);

/**
   Read link key pair by specified index.

   @param idx - index from link key pair set
   @param ent - pointer to the loaded key pair
   @return status of operation - RET_OK for success, RET_NOT_FOUND if key pair is not found
 */
zb_ret_t zb_aps_keypair_read_by_idx(zb_uint_t idx, zb_aps_device_key_pair_nvram_t *ent);

/**
   Parameters for APSME-TRANSPORT-KEY.indication primitive
 */
typedef struct zb_apsme_transport_key_indication_s
{
  zb_ieee_addr_t src_address;               /*!< Source address */
  zb_uint8_t key_type;                      /*!< @see @ref secur_key_types  */
  union {
    struct apsme_transport_key_nwk_ind_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];      /*!< Key */
      zb_uint8_t key_seq_number;            /*!< Sequence number */
    } nwk;
    struct apsme_transport_key_app_ind_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];      /*!< app link Key */
      zb_ieee_addr_t partner_address;       /*!< The extended 64-bit address of the device that was also sent this link key. */
    } app;
    struct apsme_transport_key_tc_ind_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];      /*!< TC LiKey */
    } tc;
  } key;

  /* link-key capabilities */
  zb_bitfield_t lk_capabilities_tlv_present:1;
  zb_uint8_t    lk_capabilities;
} zb_apsme_transport_key_indication_t;

/**
   APSME-TRANSPORT-KEY.indication primitive

   @param param - packet buffer filled be \see zb_apsme_transport_key_indication_t
 */
void zb_apsme_transport_key_indication(zb_uint8_t param);


/**
 * @brief Initiates ZDO authentication procedure (Device Announcement sending, keys establishment)
 *
 * @param param - output buffer
 */
void zb_initiate_zdo_commissioning_authenticated(zb_uint8_t param);


/**
   Switch NWK key

   @param key_number - NWK key number
 */
void secur_nwk_key_switch(zb_uint8_t key_number);

/**
   Switch NWK key and send it via broadcast
 */
#ifdef ZB_COORDINATOR_ROLE
void zb_secur_switch_nwk_key_br(zb_uint8_t param);
#endif

/**
   Return true if network key is empty (all zeroes).

   @param key - key
   @return ZB_TRUE if key is empty, ZB_FALSE otherwise
 */
zb_bool_t secur_nwk_key_is_empty(zb_uint8_t *key);


/**
   APSME-REMOVE-DEVICE.indication primitive

   @param param - packet buffer filled be \see zb_apsme_remove_device_ind_t
 */
void zb_apsme_remove_device_indication(zb_uint8_t param);


/**
   APSME-REQUEST-KEY.indication primitive

   @param param - packet buffer filled be \see zb_apsme_request_key_ind_t
 */
#ifdef ZB_COORDINATOR_ROLE
void zb_apsme_request_key_indication(zb_uint8_t param);
#endif

/**
   APSME-SWITCH-KEY.indication primitive

   @param param - packet buffer filled be \see zb_apsme_switch_key_ind_t
 */
void zb_apsme_switch_key_indication(zb_uint8_t param);


/**
   Reaction on TRANSPORT-KEY APS command (indication)

   @param param - buffer index with transport key command data
   @param keypair_i - index of APS key used to decrypt APS packet or -1 if used default TCLK
   @param key_id - the key used to protect the frame
   @return nothing
 */
void zb_aps_in_transport_key(zb_uint8_t param, zb_uint16_t keypair_i, zb_secur_key_id_t key_id);

/**
   Analyze trust frame by command ID, security level and key type

    @param cmd_id - command ID
    @param secured - frame is crypt
    @param is_global_key - key is global

    @return 'frame is trust'
 */
zb_bool_t zb_secur_aps_accept_policy(zb_uint8_t cmd_id, zb_bool_t secured, zb_uint16_t keypair_i);

/**
   APSME-TUNNEL.indication primitive
   Wrap APS command from tunnel command

   @param param - packet buffer filled be and APS command
 */
void zb_aps_in_tunnel_cmd(zb_uint8_t param);

/**
   Handle Start Key Update Request.

   @param param - packet buffer with request
 */
void zb_zdo_secur_start_key_update_req_handle(zb_uint8_t param);

/**
   Fills ctx with private key and public point key for CURVE25519 ECDHE process.

   @param curve_id            - curve 25519 or p-256 identifier
   @param pre_shared_key      - PSK (see. J2.ECDHE using Curve25519)
   @param initiator           - ZB_TRUE if this function called on the initiators side
   @param key_negotiation_ctx - (out) key_negotiation_context
   @return RET_OK    - success,
           RET_ERROR - some error occurs
 */
zb_ret_t zb_zdo_secur_ecdhe_public_point_fill(zb_uint8_t curve_id,
                                              zb_uint8_t *pre_shared_key,
                                              zb_bool_t initiator,
                                              zb_secur_ecdhe_common_ctx_t *key_negotiation_ctx);

/**
   Calculates derived key for the session (see. J2.ECDHE using Curve25519).

   @param curve_id            - curve 25519 or p-256 identifier
   @param hash_type           - sha256 or aes mmo 128 hash identifier (@ref zb_ecc_dlk_hash_type_t)
   @param initiator           - ZB_TRUE if this function called on the initiators side
   @param key_negotiation_ctx - (out) key_negotiation_context
   @param derived_key         - (out) derived key
   @return RET_OK    - success,
           RET_ERROR - some error occurs
 */
zb_ret_t zb_zdo_secur_ecdhe_key_generate(zb_bool_t initiator,
                                         zb_secur_ecdhe_common_ctx_t *key_negotiation_ctx,
                                         zb_uint8_t *derived_key);

#if defined ZB_JOIN_CLIENT

/**
   Security Start Key Negotiation Reguest context
 */
typedef struct zb_zdo_secur_start_key_negotiation_req_send_param_s
{
  zb_uint16_t short_addr; /* short address for the apsde_data_request */
  zb_uint8_t use_relay;
} zb_zdo_secur_start_key_negotiation_req_send_param_t;

/**
   Security Get Authentication Token Reguest context
 */
typedef struct zb_zdo_secur_get_authentication_token_req_send_param_s
{
  zb_uint16_t short_addr; /* short address for the apsde_data_request */
} zb_zdo_secur_get_authentication_token_req_send_param_t;

/**
   Start ECDHE process

   @param param - packet buffer for request
 */
void zb_zdo_secur_ecdhe_start(zb_uint8_t param);

/**
   Send Security Start Key Negotiation Reguest

   @param param - packet buffer for request
 */
void zb_zdo_sec_st_key_neg_req_send(zb_uint8_t param);
#define zb_zdo_secur_start_key_negotiation_req_send zb_zdo_sec_st_key_neg_req_send

/**
   Handle Start Key Negotiation Response.
   See. 2.4.4.5.1 Security_Start_Key_Negotiation_Rsp (2.4.4.5.1.5 Effect on Receipt)

   @param param - packet buffer with response
 */
void start_key_neg_rsp_handle(zb_uint8_t param);
#define zb_zdo_secur_start_key_negotiation_rsp_handle start_key_neg_rsp_handle

/**
   Send Security Get Authentication Token Reguest

   @param param - packet buffer for request
 */
void zb_zdo_sec_get_auth_tok_req_snd(zb_uint8_t param);
#define zb_zdo_secur_get_authentication_token_req_send zb_zdo_sec_get_auth_tok_req_snd

/**
   Handle Get Authentication Token Response.
   See. 2.4.4.5.2 Security_Get_Authentication_Token_Rsp (2.4.4.5.2.2 Effect on receipt)

   @param param - packet buffer with response
 */
void get_auth_token_rsp_handle(zb_uint8_t param);
#define zb_zdo_secur_get_authentication_token_rsp_handle get_auth_token_rsp_handle

#endif /* ZB_JOIN_CLIENT */

typedef struct zb_zdo_secur_challenge_req_send_param_s
{
  zb_ieee_addr_t target_ieee;
} zb_zdo_secur_challenge_req_send_param_t;

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE

/**
   Security Start Key Negotiation Response context
 */
typedef struct zb_zdo_secur_start_key_negotiation_rsp_send_param_s
{
  zb_uint16_t short_addr; /* request short address */
  zb_uint8_t tsn;         /* request TSN */
  zb_uint8_t status;      /* status for the response */
  zb_uint8_t use_relay;
  zb_ieee_addr_t joiner_ieee;
} zb_zdo_secur_start_key_negotiation_rsp_send_param_t;

/**
   Security Get Authentication Token Response context
 */
typedef struct zb_zdo_secur_get_authentication_token_rsp_send_param_s
{
  zb_uint16_t short_addr; /* request short address */
  zb_uint8_t tsn;         /* request TSN */
  zb_uint8_t status;      /* status for the response */
} zb_zdo_secur_get_authentication_token_rsp_send_param_t;

/**
   Security Start Key Update Reguest context
 */
typedef struct zb_zdo_secur_start_key_update_req_send_param_s
{
  zb_ieee_addr_t joiner_ieee;
  zb_uint16_t    dst_short_addr;
  zb_uint8_t     selected_kn_method;
  zb_uint8_t     selected_psk_secret;
  zb_bool_t      aps_security;
} zb_zdo_secur_start_key_update_req_send_param_t;

/**
   Handle Start Key Negotiation Request.
   See. 2.4.3.4.1 Security Start Key Negotiation Req (2.4.3.4.1.4 Effect on Receipt)

   @param param - packet buffer with request
 */
void zb_zdo_secur_start_key_negotiation_req_handle(zb_uint8_t param);

/**
 * Internal processing for the Handle Start Key Negotiation Request.
 * Used both by ZDO and Zigbee Direct
 *
 * @param key_neg_ctx_ptr context for current key negotiation
 * @param resp_param      response context
 * @param is_dlk          whether processing must be done for dlk or zigbee direct
 */
zb_ret_t zb_zdo_sec_st_key_neg_req_proc(
  zb_secur_ecdhe_common_ctx_t* key_neg_ctx_ptr,
  zb_uint8_t* status,
  zb_bool_t is_dlk
);

#define zb_zdo_secur_start_key_negotiation_req_process zb_zdo_sec_st_key_neg_req_proc

/**
   Send Security Start Key Negotiation Response

   @param param - packet buffer for response
 */
void zb_zdo_sec_st_key_neg_rsp_send(zb_uint8_t param);
#define zb_zdo_secur_start_key_negotiation_rsp_send zb_zdo_sec_st_key_neg_rsp_send

/**
   Derive APS link key.
   See J.2 ECDHE/SPEKE using Curve25519.

   @param key_neg_ctx_ptr - joiner's dlk context.
 */
void zb_zdo_secur_tc_derive_key(zb_secur_ecdhe_common_ctx_t *key_neg_ctx_ptr);

/**
   Handle Get Authentication Token Request.
   See. 2.4.3.4.2 Security_Get_Authentication_Token_Req (2.4.3.4.2.4 Effect on receipt)

   @param param - packet buffer with request
 */
void zb_zdo_secur_get_authentication_token_req_handle(zb_uint8_t param);

/**
   Send Security Get Authentication Token Response

   @param param - packet buffer for response
 */
void zb_zdo_sec_get_auth_tok_rsp_snd(zb_uint8_t param);
#define zb_zdo_secur_get_authentication_token_rsp_send zb_zdo_sec_get_auth_tok_rsp_snd

/**
   Send Security Start Key Update Reguest

   @param param - packet buffer for request
 */
void zb_zdo_sec_st_key_upd_req_snd(zb_uint8_t param);
#define zb_zdo_secur_start_key_update_req_send zb_zdo_sec_st_key_upd_req_snd

/**
   Set passphrase_update_allowed to TRUE if have not received aps ack for get_auth_token_rsp

   @param ref - address ref
 */
void zb_zdo_get_auth_token_rsp_timeout(zb_uint8_t ref);

#endif /* ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE */

/*
   Handler for APS Upstream/Downstream Relay command.

   @param param - packet buffer filled be and APS command
 */
void zb_aps_in_relay(zb_uint8_t param, zb_uint16_t relay_src, zb_uint8_t cmd_id);

/**

 */
void zb_secur_rejoin_after_security_failure(zb_uint8_t param);

/* Include unconditionally. Needed declarations will be placed according to ZB_HW_... macro definitions */
#include "hw_crypto_api.h"

#ifdef ZB_COORDINATOR_ROLE
void zb_aps_in_verify_key(zb_uint8_t param);
#endif

void zb_aps_in_confirm_key(zb_uint8_t param);

void zb_apsme_verify_key_req(zb_uint8_t param);

void zb_zdo_update_tclk(zb_uint8_t param);

void zb_zdo_verify_tclk_conf(zb_ret_t status);

void zb_zdo_verify_tclk(zb_uint8_t param);

#ifdef ZB_COORDINATOR_ROLE
void zb_apsme_verify_key_indication(zb_uint8_t param);

void zb_apsme_confirm_key_request(zb_uint8_t param);
#endif

void zb_apsme_confirm_key_indication(zb_uint8_t param);

void zb_secur_send_get_auth_token_req(zb_uint8_t param);

void zdo_secur_init(void);

void bdb_initiate_key_exchange(zb_uint8_t param);

void bdb_initiate_key_verify(zb_uint8_t param);
zb_bool_t bdb_verify_tclk_in_progress(void);

#if defined ZB_FORMATION && !defined ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
void bdb_link_key_transport_with_alarm(zb_uint8_t param2, zb_uint16_t param);
#endif

#if defined ZB_COORDINATOR_ROLE && !defined ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE

/**
 * Closes link key refresh alarm by ref.
 * Frees buffer in case it is not needed.
 * Returns buf if found scheduled alarm and buf needed.
 *
 * @param func alarm callback
 * @param param address ref to find device for that alarm was scheduled.
 * @param free_buf free buffer if alarm was found or not
 * @return zb_uint8_t ZB_BUF_INVALID if alarm wasn't found or `free_buf` flag was specified, bufid otherwise.
 */
zb_bufid_t bdb_cancel_link_key_refresh_alarm(zb_callback_t func, zb_address_ieee_ref_t param, zb_bool_t free_buf);

void bdb_link_key_refresh_alarm(zb_uint8_t param);
#endif

void zdo_authentication_failed(zb_uint8_t param);

/* CR : 04/19/2016 [VP]: Start: put it here to call this functions from apsme_secur.c */
void bdb_request_tclk_alarm(zb_uint8_t param);

void bdb_verify_tclk_alarm(zb_uint8_t param);

void bdb_update_tclk_failed(zb_uint8_t param);
/* CR : 04/19/2016 [VP]: Stop */

void bdb_update_tclk_stop(void);

void zb_secur_delete_link_keys_by_addr_ref(zb_address_ieee_ref_t addr_ref);

void zb_secur_delete_link_keys_by_long_addr(zb_ieee_addr_t ieee_address);

void zb_secur_delete_link_key_by_idx(zb_uint16_t idx);

zb_ret_t zb_secur_changing_tc_policy_check(void);

zb_aps_device_key_pair_set_t *zb_secur_get_link_key_pair_set(zb_ieee_addr_t address, zb_bool_t valid_only);

zb_aps_device_key_pair_set_t *zb_secur_get_verified_or_provisional_link_key(zb_ieee_addr_t address);

zb_aps_device_key_pair_set_t *zb_secur_create_best_suitable_link_key_pair_set(zb_ieee_addr_t address);

zb_ret_t zb_secur_ic_get_key_by_address(zb_ieee_addr_t address, zb_uint8_t *key);

zb_ret_t zb_secur_ic_get_key_from_passcode (zb_uint8_t *key);

#if defined ZB_COORDINATOR_ROLE && defined ZB_SECURITY_INSTALLCODES && defined ZB_USE_NVRAM && !defined APP_ONLY_NVRAM && !defined NCP_MODE_HOST
zb_int_t zb_secur_ic_get_from_tc_storage(const zb_ieee_addr_t address, zb_uint8_t *ic_type, zb_uint8_t *buf);
#endif

zb_uint8_t *zb_secur_ic_get_from_client_storage(zb_uint8_t* ic_type);

zb_ret_t zb_secur_ic_check_crc(zb_uint8_t ic_type, zb_uint8_t const *ic);

zb_ret_t zb_secur_ic_from_string(char *ic_str, zb_uint8_t *ic_type, zb_uint8_t *ic);

zb_bool_t zb_secur_aps_cmd_is_encrypted_by_good_key(zb_uint8_t cmd_id, zb_uint16_t src_addr, zb_uint16_t keypair_i);

void zdo_secur_update_tclk_done(zb_uint8_t param);

void secur_generate_key(zb_uint8_t *key, zb_uint8_t key_size);

zb_bool_t zb_secur_has_verified_key_by_short(zb_uint16_t addr_short);

void zb_secur_trace_all_key_pairs(zb_uint8_t param);

#ifdef DEBUG
#define ZB_SECUR_TRACE_ALL_KEY_PAIRS() zb_secur_trace_all_key_pairs(0)
#else
#define ZB_SECUR_TRACE_ALL_KEY_PAIRS()
#endif

#ifdef ZB_SE_KE_WHITELIST
void zb_secur_ke_whitelist_add(zb_ieee_addr_t addr);

void zb_secur_ke_whitelist_del(zb_ieee_addr_t addr);

void zb_secur_ke_whitelist_del_all(void);

zb_int16_t zb_secur_search_ke_whitelist(zb_ieee_addr_t addr);
#endif /* ZB_SE_KE_WHITELIST */

/*! @} */
/*! @endcond */

#ifdef ZB_USE_OSIF_OTA_ROUTINES
zb_bool_t zb_sec_b6_hash_iter_start(void *dev, zb_uint32_t input_addr, zb_uint32_t input_len);
void zb_sec_b6_hash_iter_done(void *dev, zb_uint32_t input_len, zb_uint8_t *calc_hash);
#endif

void zdo_initiate_tclk_gen_over_aps(zb_uint8_t param);

zb_uint8_t zb_secur_gen_upd_dev_status(zb_ushort_t rejoin_network, zb_ushort_t secure_rejoin);

#ifdef ZB_FORMATION
void secur_nwk_generate_key(zb_uint8_t i, zb_uint_t key_seq);

void secur_authenticate_child_directly(zb_uint8_t param);
#endif

zb_secur_key_id_t zb_secur_aps_send_policy(zb_uint_t command, zb_uint16_t dest_addr, zb_uint8_t key_type);

#ifdef ZB_FORMATION
zb_ret_t zb_authenticate_dev(zb_bufid_t param, zb_apsme_update_device_ind_t *ind);
#endif

void zb_zdo_ecdhe_common_ctx_init(void);

zb_secur_ecdhe_common_ctx_t *ecdhe_common_ctx_get_by_ref(zb_address_ieee_ref_t ref);
#define zb_zdo_ecdhe_common_ctx_get_by_ref ecdhe_common_ctx_get_by_ref

zb_secur_ecdhe_common_ctx_t *zb_zdo_ecdhe_common_ctx_get_by_ieee(zb_ieee_addr_t ieee_address);

zb_secur_ecdhe_common_ctx_t *ecdhe_cmn_ctx_get_by_short(zb_uint16_t short_addr);
#define zb_zdo_ecdhe_common_ctx_get_by_short ecdhe_cmn_ctx_get_by_short

zb_secur_ecdhe_common_ctx_t *zb_zdo_ecdhe_common_ctx_alloc_by_ref(zb_address_ieee_ref_t ref);

zb_ret_t zb_zdo_ecdhe_common_ctx_delete_by_ref(zb_address_ieee_ref_t ref);

void zb_zdo_set_configuration_res(zb_uint8_t param);

void zb_zdo_get_configuration_res(zb_uint8_t param);

void zb_zdo_decommission_res(zb_uint8_t param);

void zb_zdo_decommission_by_ieee(zb_ieee_addr_t ieee_addr);

zb_bool_t zdo_need_aps_enc(zb_uint16_t cmd_id, zb_uint16_t dst_addr);

#ifdef ZB_COORDINATOR_ROLE
zb_ret_t zb_zdo_ecdhe_common_ctx_find_confirm_key_param(zb_uint8_t param, zb_secur_ecdhe_common_ctx_t **dlk_ctx);
zb_ret_t zb_zdo_ecdhe_common_ctx_find_start_key_neg_rsp_param(zb_uint8_t param, zb_secur_ecdhe_common_ctx_t **dlk_ctx);

void zdo_secur_confirm_key_legacy_confirm(zb_ieee_addr_t ieee_addr);
void zdo_secur_confirm_key_dlk_confirm(zb_secur_ecdhe_common_ctx_t *dlk_ctx);
void zb_confirm_key_ack_received(zb_secur_ecdhe_common_ctx_t *dlk_ctx_p);
#endif

#ifndef ZB_COORDINATOR_ONLY
zb_uint8_t zb_zdo_sec_get_auth_lvl_rq_snd(zb_uint8_t param, zb_callback_t cb);
#endif /* ZB_COORDINATOR_ONLY */

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
/**
 * Get curve_id, using current key negotiation method
 *
 * @param key_negotiation_method  key negotiation method for current session
 *
 * @return id of a curve, used in current key establishment
 */
zb_uint8_t zb_secur_curve_id_get_by_key_neg_method(zb_uint8_t key_negotiation_method);

/**
 * Get curve_id, using current key negotiation context
 *
 * @param key_negotiation_ctx   key negotiation context for current session
 *
 * @return id of a curve, used in current key establishment
 */
zb_uint8_t zb_secur_curve_id_get_by_ctx(zb_secur_ecdhe_common_ctx_t *key_negotiation_ctx);
#endif /* ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE */

/**
 * Pull info about key negotiation method, taken from TLV
 *
 * @param key_neg_method    a value from TLV
 * @param curve_id          [OUT] id of a curve, used in current method
 * @param hash_type         [OUT] hash, used in current method
 *
 * @note any of [OUT] parameters can be NULL
 */
zb_ret_t zb_secur_parse_key_neg_method(zb_uint8_t key_neg_method,
                                       zb_uint8_t *curve_id,
                                       zb_ecc_dlk_hash_type_t *hash_type);

/**
  Check whether key was established using CBKE
 */
zb_bool_t secur_key_is_cbke(zb_aps_device_key_pair_set_t *pair);

zb_bool_t zb_zdo_secur_cbke_key_exists(zb_uint16_t addr);


#ifdef DEBUG
/**
   Broadcast a key using non-standard NWK report (command id 6).

   To be used for debug purposes only.

   @param peer_addr - address of remote device, or NULL
   @param key - APS or NWK key
 */
void zb_debug_bcast_key(zb_uint8_t *peer_addr, zb_uint8_t key[ZB_CCM_KEY_SIZE]);
#endif /* DEBUG */


#ifdef ZB_DIRECT_ENABLED
/**
 * @brief Calculate authorization key
 *
 * @param       psk_type  which type of authorization key is being generated @warning only ADMIN and BASIC key can be provided
 * @param [in]  zvd_ieee  IEEE address of a ZVD being connected
 * @param [in]  key       security material (TC Link Key for admin key generation, NWK Key for basic key)
 * @param [out] auth_key  a buffer of CCM_KEY_SIZE bytes for authorization key to be stored in
 *
 * @return RET_OK if key was generate successfully RET_ERROR otherwise
 */
zb_ret_t zb_secur_generate_auth_key(zb_tlv_psk_secrets_t psk_type, const zb_ieee_addr_t zvd_ieee, const zb_uint8_t key[ZB_CCM_KEY_SIZE], zb_uint8_t auth_key[ZB_CCM_KEY_SIZE]);

/**
 * @brief Calculate admin key
 *
 * @param [in]  zvd_ieee  IEEE address of a ZVD being connected
 * @param [in]  tclk      ZDD's TC Link Key
 * @param [out] admin_key a buffer of CCM_KEY_SIZE bytes for admin key to be stored in
 *
 * @return RET_OK if key was generate successfully RET_ERROR otherwise
 */
#define zb_secur_generate_admin_key(zvd_ieee, tclk, admin_key) zb_secur_generate_auth_key(ZB_TLV_PSK_SECRET_ADMINISTRATIVE_ACCESS_KEY, zvd_ieee, tclk, admin_key)

/**
 * @brief Calculate basic key
 *
 * @param [in]  zvd_ieee  IEEE address of a ZVD being connected
 * @param [in]  nwk_key   ZDD's NWK Key
 * @param [out] basic_key a buffer of CCM_KEY_SIZE bytes for admin key to be stored in
 *
 * @return RET_OK if key was generate successfully RET_ERROR otherwise
 */
#define zb_secur_generate_basic_key(zvd_ieee, nwk_key, basic_key) zb_secur_generate_auth_key(ZB_TLV_PSK_SECRET_BASIC_ACCESS_KEY, zvd_ieee, nwk_key, basic_key)
#endif /* ZB_DIRECT_ENABLED */

#ifndef ZB_COORDINATOR_ONLY
void zb_zdo_parse_get_auth_level_resp(zb_uint8_t param);
#endif

void zb_zdo_parse_get_configuration_resp(zb_uint8_t param);

#endif /* ZB_SECUR_H */
