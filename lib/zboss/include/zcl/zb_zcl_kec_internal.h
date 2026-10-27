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
/* PURPOSE: Key-establishment cluster definitions for internal use
*/

#ifndef ZB_ZCL_KEC_INTERNAL_H
#define ZB_ZCL_KEC_INTERNAL_H 1

#include "zb_types.h"
#include "zcl/zb_zcl_kec.h"

/** @cond DOXYGEN_SE_SECTION */

/** @addtogroup ZB_ZCL_KEC
 *  @{
 *    @details This cluster provides attributes and commands to perform mutual authentication and
 *      establish keys between two Zigbee devices. Figure C-3 depicts a diagram of a successful key
 *      establishment negotiation.
 */


/** @cond internals_doc */
/** @defgroup se_kec_key_cert Keys and Certificates
 *  @brief Data structures and sizes
 *  @{
 */


/** @def ZB_KEC_MAX_PUBLIC_KEY_SIZE
 *  @brief Public key maximum size
 */
#define ZB_KEC_MAX_PUBLIC_KEY_SIZE 37U


/** @def ZB_KEC_MAX_PRIVATE_KEY_SIZE
 *  @brief Private key maximum size
 */
#define ZB_KEC_MAX_PRIVATE_KEY_SIZE 36U


/** @def ZB_KEC_MAX_SHARED_KEY_SIZE
 *  @brief Shared key maximum size
 */
#define ZB_KEC_MAX_SHARED_KEY_SIZE ZB_KEC_MAX_PRIVATE_KEY_SIZE


/** @def ZB_KEC_MAX_CERT_SIZE
 *  @brief Certificate maximum size
 */
#define ZB_KEC_MAX_CERT_SIZE 74U


/** @def ZB_MAX_RND_SIZE
 *  @brief Ephemeral data maximum size
 */
#define ZB_MAX_RND_SIZE 36U

/** @def ZB_KEC_HASH_LEN
 *  @brief KEC hash size
 */
#define ZB_KEC_HASH_LEN 16U

#define ZB_KEC_RETRY_WAIT_TIME_DEFAULT (10U * ZB_TIME_ONE_SECOND)

/* These defines uses to avoid MISRA violations. It is possible that MISRA checker will
 * detect violation if we are using logarithm calculation from KEC_CS1 and KEC_CS2 values,
 * so let's define them directly
 */
#define KEC_CS1_BIT_POS 0U
#define KEC_CS2_BIT_POS 1U

/* For MISRA violation resolution(12.2, 18.1). The value should be equal to KEC_CS1 (0) or KEC_CS2 (1).
 * Using zb_kec_get_suite_num in the macro doesn't solve the violation */
#define ZB_KEC_VALID_SUITE_NUMBER(a) ((a) == KEC_CS1_BIT_POS || (a) == KEC_CS2_BIT_POS)

/*
 * Constants for array sizeof for different CryptoSuites
 */
extern const zb_uint8_t ZB_CERT_SIZE[2];    /**< Certificate sizes map */
extern const zb_uint8_t ZB_QE_SIZE[2];      /**< Public key sizes map */
extern const zb_uint8_t ZB_PR_SIZE[2];      /**< Private key sizes map */


/** @union zb_kec_icu_u
 *  @brief Certificate in different CryptoSuites
 *
 * C.4.2.2.4 ECC Implicit Certificates
 * The exact format of the 48-byte implicit certificate ICU used with CBKE scheme shall be specified as follows:
 *
 * ICU = PublicReconstrKey  || Subject || Issuer || ProfileAttributeData
 * PublicReconstrKey:    22 byte BEU
 * Subject:               8 byte id
 * Issuer:                8 byte CA Id
 * ProfileAttributeData: 10 byte (first 2 bytes are Profile ID)
 * IU (step6) = Subject || Issuer || ProfileAttributeData
 *
 * C.4.2.3.3  ECC Implicit Certificates
 * The exact format of the Cryptographic Suite 2 74-byte implicit certificate ICU used with CBKE
 * scheme follows the definitions given in SEC 4 [19] for the minimal encoding scheme (MES) and shall be specified as follows:
 * !!!
 *
 * ICU = Type || SerialNo  || Curve || Hash || Issuer || ValidFrom || ValidTo || Subject || KeyUsage ||  PublicReconstrKey
 *
 */
typedef union zb_kec_icu_u
{
  ZB_PACKED_PRE struct zb_kec_cs1_s
  {
    zb_uint8_t    public_reconstr_key[22];
    zb_uint8_t    subject[8];
    zb_uint8_t    issuer[8];
    zb_uint8_t    profile_attribute_data[10];
  } ZB_PACKED_STRUCT cs1;                       /**< 48 bytes icu */

  ZB_PACKED_PRE struct zb_kec_cs2_s
  {
    zb_uint8_t    type;              /**< 0x00 for Crypto Suite 2*/
    zb_uint8_t    serial[8];         /**< is an 8-byte representation of the certificate Serial Number; */
    zb_uint8_t    curve;             /**< is a 1-byte elliptic curve identifier. */
                                     /**< For the Crypto Suite 2 Cipher Suite this shall be 0x0D indicating the sect283k1 curve is used; */
    zb_uint8_t    hash;              /**< is a 1-byte hash identifier. */
                                     /**< For the Crypto Suite 2 Cipher Suite, this shall be 0x08 indicating that AES-MMO is used; */
    zb_uint8_t    issuer[8];
    zb_uint8_t    valid_from[5];
    zb_uint8_t    valid_to[4];
    zb_uint8_t    subject[8];
    zb_uint8_t    key_usage;         /**< @ref cs2_key_usage */
    zb_uint8_t    public_reconstr_key[37];
  } ZB_PACKED_STRUCT cs2;                       /**< 74-byte icu */

  zb_uint8_t  u8[74];
} zb_kec_icu_t;


/** @union zb_kec_qe_u
 *  @brief Public key in different CryptoSuites
 */
typedef union zb_kec_qe_u
{
  zb_uint8_t    cs1[22];            /**< For Crypto Suite 1 */
  zb_uint8_t    cs2[37];            /**< For Crypto Suite 2 */
  zb_uint8_t    u8[37];             /**< for access as unsigned 8, max size */
} zb_kec_qe_t;


/** @union zb_kec_pr_u
 *  @brief Private key in different CryptoSuites
 */
typedef union zb_kec_pr_u
{
  zb_uint8_t    cs1[21];            /**< For Crypto Suite 1 */
  zb_uint8_t    cs2[36];            /**< For Crypto Suite 2 */
  zb_uint8_t    u8[36];             /**< for access as unsigned 8, max size */
} zb_kec_pr_t;


/**
 * @name Key Usage flag in Certificate bit field
 * @anchor cs2_key_usage
 *
 * Note: These values were members of `enum zb_cs2_key_usage_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_KU_KEY_AGREEMENT     (1U << 3) /*!< 0x08 */
#define ZB_KU_DIGITAL_SIGNATURE (1U << 7) /*!< 0x80 */
/** @} */


/** @def IS_KEC_KEY_AGREEMENT_FLAG
 *  @brief Checks whether Key Agreement flag is set in certificate
 */
#define IS_KEC_KEY_AGREEMENT_FLAG(x) ( ( (x) & ZB_KU_KEY_AGREEMENT ) > 0U )


/**
 * @name Terminate Key Establishment Command Status (Table C-11)
 * @anchor kec_term_status
 */
/** @{ */
/** The Issuer field within the key establishment partner's certificate is unknown to the sending
 * device, and it has terminated the key establishment. */
#define ZB_SE_KEY_ESTABLISHMENT_TERMINATE_UNKNOWN_ISSUER 0x01U
/** The device could not confirm that it shares the same key with the corresponding device and has
 * terminated the key establishment. */
#define ZB_SE_KEY_ESTABLISHMENT_TERMINATE_BAD_KEY_CONFIRM 0x02U
/** The device received a bad message from the corresponding device (e.g. message with bad data, an
 * out of sequence number, or a message with a bad format) and has terminated the key establishment.
 */
#define ZB_SE_KEY_ESTABLISHMENT_TERMINATE_BAD_MESSAGE 0x03U
/** The device does not currently have the internal resources necessary to perform key establishment
 * and has terminated the exchange. */
#define ZB_SE_KEY_ESTABLISHMENT_TERMINATE_NO_RESOURCES 0x04U
/** The device does not support the specified key establishment suite in the partner's Initiate Key
 * Establishment message. */
#define ZB_SE_KEY_ESTABLISHMENT_TERMINATE_UNSUPPORTED_SUITE 0x05U
/** The received certificate specifies a type, curve, hash, or other parameter that is either
 * unsupported by the device or invalid. */
#define ZB_SE_KEY_ESTABLISHMENT_TERMINATE_INVALID_CERTIFICATE 0x06U
/** Non-standard ZBOSS extension: SE KE endpoint not found */
#define ZB_SE_KEY_ESTABLISHMENT_TERMINATE_NO_KE_EP 0x07U
/** @} */

/**
 * @brief Type for Terminate Key Establishment Command Status.
 *
 * Holds one of @ref kec_term_status. Kept for backward compatibility as
 * @ref kec_term_status were declared previously as enum.
                                    */
typedef zb_uint8_t zb_kec_term_status_t;


/** @fn zb_bool_t zb_zcl_process_kec_specific_commands(zb_uint8_t param)
 *  @brief Common commands processor for Server and Client
 *  @param[in]    param   buffer reference with input command
 *  @returns ZB_TRUE, if command was processed, ZB_FALSE otherwise
 */
/* TODO: Check if we may pass kec init here. */

/** @fn zb_uint8_t zb_kec_get_suite_num(zb_uint16_t suite)
 *  @brief Translate suite id into sequential crypto suite number to work with arrays
 *  @param[in]    suite   CryptoSuite id (@ref kec_key_suite)
 *  @returns integer representation of CryptoSuite id (-1 if @e suite is incorrect)
 */
zb_uint8_t zb_kec_get_suite_num(zb_uint16_t suite);


/** @fn zb_uint8_t *zb_kec_get_issuer(zb_uint8_t suite_no, zb_kec_icu_t *cert)
 *  @brief Get certificate's issuer
 *  @param[in]    suite_no    CryptoSuite number
 *  @param[in]    cert        pointer to certificate buffer
 *  @returns pointer to issuer field within the @e cert buffer (NULL if @e suite_no is incorrect)
 */
zb_uint8_t *zb_kec_get_issuer(zb_uint8_t suite_no, zb_kec_icu_t *cert);


/** @fn zb_uint8_t *zb_kec_get_subject(zb_uint8_t suite_no, zb_kec_icu_t *cert)
 *  @brief Get certificate's subject
 *  @param[in]    suite_no    CryptoSuite number
 *  @param[in]    cert        pointer to certificate buffer
 *  @returns pointer to subject field within the @e cert buffer (NULL if @e suite_no is incorrect)
 */
zb_uint8_t *zb_kec_get_subject(zb_uint8_t suite_no, zb_kec_icu_t *cert);

/** @} */ /* se_kec_key_cert */
/** @endcond */ /* internals_doc */

/** @cond internals_doc */

/** @defgroup se_kec_cmd_frames Cluster Commands
 *  @brief Command frames and constants definitions
 *  @{
 */

/*
 * Server
 */
#ifdef ZB_SE_ENABLE_KEC_SERVER

/** @defgroup se_kec_srv_cmds Server-side commands
 *  @{
 */

/**
 * @name Stages for KEC client state-machine
 * @anchor ke_stages_client
 *
 * Note: These values were members of `enum ke_stages_client` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define KE_STAGE_CLIENT_START                            0U
#define KE_STAGE_CLIENT_INITIATE_REQUEST_SENT            1U
#define KE_STAGE_CLIENT_INITIATE_RESPONSE_RECEIVED       2U
#define KE_STAGE_CLIENT_EPHEMERAL_DATA_REQUEST_SENT      3U
#define KE_STAGE_CLIENT_EPHEMERAL_DATA_RESPONSE_RECEIVED 4U
#define KE_STAGE_CLIENT_CONFIRM_KEY_REQUEST_SENT         5U
#define KE_STAGE_CLIENT_CONFIRM_KEY_RESPONSE_RECEIVED    6U
#define KE_STAGE_CLIENT_FINISHED                         7U
/** @} */

/**
 * @name Stages for KEC server state-machine
 * @anchor ke_stages_server
 *
 * Note: These values were members of `enum ke_stages_server` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define  KE_STAGE_SERVER_START 0U
#define  KE_STAGE_SERVER_INITIATE_REQUEST_RECEIVED 1U
#define  KE_STAGE_SERVER_INITIATE_RESPONSE_SENT 2U
#define  KE_STAGE_SERVER_EPHEMERAL_DATA_REQUEST_RECEIVED 3U
#define  KE_STAGE_SERVER_EPHEMERAL_DATA_RESPONSE_SENT 4U
#define  KE_STAGE_SERVER_CONFIRM_KEY_REQUEST_RECEIVED 5U
#define  KE_STAGE_SERVER_CONFIRM_KEY_RESPONSE_SENT 6U
#define  KE_STAGE_SERVER_FINISHED 7U
/** @} */

typedef void (*zb_kec_client_cb_t)(zb_bufid_t, zb_ret_t);

/** @struct zb_ke_ctx_s
 *  @brief Key Establishment global context
 *  @note Only one CBKE simultaneously allowed!
 */
typedef ZB_PACKED_PRE struct zb_ke_ctx_s
{
  zb_uint8_t        ke_stage;
  zb_uint8_t        ke_status;
  zb_uint8_t        curve_id;
  zb_uint16_t       active_suites;
  zb_uint16_t       local_ke_suite;
  zb_uint16_t       remote_ke_suite;
  zb_uint8_t        ke_suite_num;

  zb_uint8_t        remote_ephemeral_timeout;
  zb_uint8_t        remote_confirm_timeout;

  zb_uint8_t        transaction_sequence_number;
  zb_callback_t     user_cb;
  zb_callback_t     server_cb;
#ifdef ZB_SE_ENABLE_KEC_CLIENT
  zb_kec_client_cb_t  client_cb;
#endif /* ZB_SE_ENABLE_KEC_CLIENT */
  zb_uint8_t        src_ep;
  zb_uint8_t        dst_ep;
  zb_addr_u         dst_addr;

  zb_kec_qe_t       local_ephemeral_public_key;
  zb_kec_pr_t       local_ephemeral_private_key;
  zb_kec_qe_t       remote_ephemeral_public_key;

  zb_kec_icu_t      remote_certificate;
  zb_kec_qe_t       active_certificate_authority_public_key;

  zb_uint16_t       selected_ke_suite;

  /* >>>> Initiate Key Establishment payload: do not modify order of parameters! */
  zb_uint16_t       selected_ke_suite_le; /* selected_ke_suite in the little-endian format */
  zb_uint8_t        time_generate_eph;
  zb_uint8_t        time_confirm_key;
  zb_kec_icu_t      local_certificate;
  /* <<<< Initiate Key Establishment payload */

  zb_kec_pr_t       local_private_key;
  zb_kec_qe_t       local_public_key;
  zb_kec_qe_t       remote_public_key;
  zb_ret_t          remote_public_key_retval;

  zb_kec_pr_t       Z; /* Shared secret (ecc private key) */
  zb_callback_t     kec_cb;

  zb_uint8_t        local_mac_data[ZB_KEC_HASH_LEN];
  zb_uint8_t        mac_key[ZB_KEC_HASH_LEN];
  zb_uint8_t        key_data[ZB_KEC_HASH_LEN];

  zb_uint16_t              frag_total;
  zb_zcl_frame_direction_t zcl_dir;
  zb_zcl_parsed_hdr_t      cmd_info;
} ZB_PACKED_STRUCT zb_ke_ctx_t;



/**
 * @name Key Establishment cluster's server commands IDs
 * @anchor kec_server_recv_cmd
 * @brief
 * C.3.1.2.3 Commands Received
 * The server side of the key establishment cluster is capable of receiving the commands listed in
 * Table C-5. Table C-5 Received Command IDs for the Key Establishment Cluster Server
 * Mandatory/Optional    Command Identifier Field Value     Description
 * M                    0x00                              Initiate Key Establishment Request
 * M                    0x01                              Ephemeral  Data Request
 * M                    0x02                              Confirm Key Data Request
 * M                    0x03                              Terminate Key Establishment
 *                      0x04 - 0xFF                       Reserved
 *
 * Note: These values were members of `enum zb_kec_server_rcv_cmd_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_SE_KEC_CMD_INITIATE_KE_REQUEST    0x00U /**< Initiate Key Establishment Request */
#define ZB_SE_KEC_CMD_EPHEMERAL_DATA_REQUEST 0x01U /**< Ephemeral  Data Request */
#define ZB_SE_KEC_CMD_CONFIRM_KEY_REQUEST    0x02U /**< Confirm Key Data Request */
#define ZB_SE_KEC_CMD_TERMINATE_KE_SERVER    0x03U /**< Terminate Key Establishment */
/** @} */


/** @struct zb_kec_init_ke_req_cmd_s
 *  @brief Initiate Key Establishment Request Command frame
 *
 *  C.3.1.2.3.1 Initiate Key Establishment Request Command
 *  The Initiate Key Establishment Request command allows a device to initiate key establishment
 *  with another device. The sender shall indicate the identity information and key establishment
 *  protocol information that it wishes to use to the receiving device.
 *
 *  C.3.1.2.3.1.1 Payload Format
 *  The Initiate Key Establishment Request command payload shall be formatted as illustrated below
 * <table>
 *  <caption>Figure C-4 Initiate Key Establishment Request Command Payload</caption>
 *   <tr>
 *     <th> Octets </th>
 *      <td> @b 2 </td>
 *      <td> @b 1 </td>
 *      <td> @b 1 </td>
 *      <td> @b Variable </td>
 *   </tr>
 *   <tr>
 *     <th> DataType </th>
 *     <td> 16-bit BitMap </td>
 *     <td> Unsigned  8-bit </td>
 *     <td> Unsigned  8-bit </td>
 *     <td> Octets(non-ZCL Data Type) </td>
 *   </tr>
 *   <tr>
 *     <th> FieldName </th>
 *     <td> Key Establishment suite </td>
 *     <td> Ephemeral Data Generate Time </td>
 *     <td> Confirm Key Generate Time </td>
 *     <td> Identity (IDU) </td>
 *   </tr>
 * </table>
 */
typedef ZB_PACKED_PRE struct zb_kec_init_ke_req_cmd_s
{
  zb_uint16_t     ke_suite;            /**< @ref kec_key_suite - Only one suite shall be indicated in the command. */
  zb_uint8_t      eph_data_gen_time;
  zb_uint8_t      conf_key_gen_time;
  zb_kec_icu_t    idu;
} ZB_PACKED_STRUCT zb_kec_init_ke_req_cmd_t;


/** @struct zb_kec_eph_data_req_cmd_s
 *  @brief Ephemeral Data Request Command frame
 *
 *  C.3.1.2.3.2 Ephemeral Data Request Command
 *  The Ephemeral Data Request command allows a device to communicate its ephemeral data
 *  to another device and request that the device send back its own ephemeral data.
 *  C.3.1.2.3.2.1 Payload Format
 *  Octets            Variable
 *  DataType          Octets (non-ZCL Data Type)
 *  FieldName         Ephemeral Data (QEU)
 *  Figure C-5 Ephemeral Data Request Command Payload
 */
typedef ZB_PACKED_PRE struct zb_kec_eph_data_req_cmd_s
{
  zb_uint8_t      qeu[ZB_KEC_MAX_PUBLIC_KEY_SIZE+1];
} ZB_PACKED_STRUCT zb_kec_eph_data_req_cmd_t;


/** @struct zb_kec_confirm_key_req_cmd_s
 *  @brief Confirm Key Request Command frame
 *
 * C.3.1.2.3.3 Confirm Key Request Command
 * The Confirm Key Request command allows the initiator sending device to confirm the key
 * established with the responder receiving device based on performing a cryptographic hash
 * using part of the generated keying material and the identities and ephemeral data of both parties.
 *
 * C.3.1.2.3.3.1 Payload Format
 * The Confirm KeyRequest command payload shall be formatted as illustrated in Figure C-6.
 * Octets      16
 * DataType    Octets (non-ZCL Data Type)
 * FieldName   Secure Message Authentication Code (MACU)
 * Figure C-6 Confirm Key Request Command Payload
 * Secure Message Authentication Code field: The Secure Message Authentication Code field
 * shall be the octet representation  of MACU as specified in sub-clause C.4.2.
 */
typedef ZB_PACKED_PRE struct zb_kec_confirm_key_req_cmd_s
{
  zb_uint8_t      macu[16];
} ZB_PACKED_STRUCT zb_kec_confirm_key_req_cmd_t;


/** @struct zb_kec_term_cmd_s
 *  @brief Terminate Key Establishment Command frame
 *
 *  C.3.1.2.3.4 Terminate Key Establishment Command
 *  The Terminate Key Establishment command may be sent by either the initiator or responder to
 *  indicate a failure in the key establishment exchange.
 *
 *  C.3.1.2.3.4.1 Payload Format
 *  The Terminate Key Establishment command payload shall be formatted as illustrated in Figure
 *  C-7.
 *  Octets      1                      1                              2
 *  DataType    8-bit Enumeration     Unsigned  8-bit Integer        16-bit BitMap
 *  FieldName    Status Code          Wait Time                      KeyEstablishmentSuite
 *  Figure C-7 Terminate Key Establishment Command Payload
 */
typedef ZB_PACKED_PRE struct zb_kec_term_cmd_s
{
  zb_uint8_t      status_code;    /**< @ref kec_term_status*/
  zb_uint8_t      wait_time;
  zb_uint16_t     key_suite;      /**< @ref kec_key_suite */
} ZB_PACKED_STRUCT zb_kec_term_cmd_t;

/** @} */ /* se_kec_srv_cmds */

#endif /* ZB_SE_ENABLE_KEC_SERVER */


/*
 * Client
 */
#ifdef ZB_SE_ENABLE_KEC_CLIENT
/** @defgroup se_kec_client_cmds Client-side commands
 *  @{
 */

/**
 * @name Key Establishment cluster's client-side commands IDs
 * @anchor kec_client_recv_cmd
 * @brief
 *  C.3.1.3.3 Commands Received
 *  The client side of the Key Establishment cluster is capable of receiving the commands
 *  listed in Table C-10.
 *  Table C-10 Received Command IDs for the Key Establishment Cluster Client
 *  Mandatory/Optional    Command Identifier Field Value     Description
 *  M                    0x00                              Initiate Key Establishment Response
 *  M                    0x01                              Ephemeral  Data Response
 *  M                    0x02                              Confirm Key Data Response
 *  M                    0x03                              Terminate Key Establishment
 *  0x04  0xFF  Reserved
 *
 * Note: These values were members of `enum zb_kec_client_recv_cmd_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_SE_KEC_CMD_INITIATE_KE_RESPONSE    0x00U /*!< Initiate Key Establishment Response */
#define ZB_SE_KEC_CMD_EPHEMERAL_DATA_RESPONSE 0x01U /*!< Ephemeral  Data Response */
#define ZB_SE_KEC_CMD_CONFIRM_KEY_RESPONSE    0x02U /*!< Confirm Key Data Response */
#define ZB_SE_KEC_CMD_TERMINATE_KE_CLIENT     0x03U  /*!< Terminate Key Establishment */
/** @} */


/** @struct zb_kec_init_ke_resp_cmd_s
 *  @brief Initiate Key Establishment Response Command frame
 *
 *  C.3.1.3.3.1 Initiate Key Establishment Response Command
 *  The Initiate Key Establishment Response command allows a device to respond to a device
 *  requesting the initiation of key establishment with it. The sender will transmit its identity
 *  information and key establishment protocol information to the receiving device.
 *
 *  C.3.1.3.3.1.1 Payload Format
 *  The Initiate Key Establishment Response command payload shall be formatted as illustrated in
 *  Figure C-8.
 *  Octets      2                  1                              1                              Variable
 *  DataType    16-bit BitMap      Unsigned  8-bit    Integer        Unsigned  8-bit Integer        Octets (non-ZCL Data Type)
 *  FieldName    Requested Key     Ephemeral Data                Confirm Key                    Identity (IDU)
 *              Establ. suite    Generate Time                Generate Time
 *  Figure C-8 Initiate Key Establishment Response Command Payload
 */
typedef ZB_PACKED_PRE struct zb_kec_init_ke_resp_cmd_s
{
    zb_uint16_t     ke_suite;            /**< @ref kec_key_suite - Only one suite shall be indicated in the command.*/
    zb_uint8_t      eph_data_gen_time;
    zb_uint8_t      conf_key_gen_time;
    zb_kec_icu_t    icu;
} ZB_PACKED_STRUCT zb_kec_init_ke_resp_cmd_t;


/** @struct zb_kec_eph_data_resp_cmd_s
 *  @brief Ephemeral Data Response Command
 *
 * C.3.1.3.3.2 Ephemeral Data Response Command
 * The Ephemeral Data Response command allows a device to communicate its ephemeral data
 * to another device that previously requested it.
 *
 * C.3.1.3.3.2.1 Payload Format
 * Octets          Variable
 * DataType          Octets (non-ZCL Data Type)
 * FieldName          Ephemeral Data (QEV)
 * Figure C-9 Ephemeral Data Response Command Payload
 */
typedef ZB_PACKED_PRE struct zb_kec_eph_data_resp_cmd_s
{
    zb_uint8_t      qev[ZB_KEC_MAX_PUBLIC_KEY_SIZE+1];
} ZB_PACKED_STRUCT zb_kec_eph_data_resp_cmd_t;


/** @struct zb_se_kec_confirm_key_resp_cmd_s
 *  @brief Confirm Key Response Command frame
 *
 * C.3.1.3.3.3 Confirm Key Response Command
 * The Confirm Key Response command allows the responder to verify the initiator has derived the
 * same secret key. This is done by sending the initiator a cryptographic hash generated using the
 * keying material and the identities and ephemeral data of both parties.
 *
 * C.3.1.3.3.3.1 Payload Format
 * The Confirm Key Response command payload shall be formatted as illustrated in Figure C-10.
 * Octets      16
 * DataType    Octets (non-ZCL Data Type)
 * FieldName   Secure Message Authentication Code (MACV)
 * Figure C-10 Confirm Key Response Command Payload
 * Secure Message Authentication Code field: The Secure Message Authentication Code field
 * shall be the octet representation of MACV as specified in sub-clause C.4.2.
 */
typedef ZB_PACKED_PRE struct zb_kec_confirm_key_resp_cmd_s
{
    zb_uint8_t      macv[16];
} ZB_PACKED_STRUCT zb_kec_confirm_key_resp_cmd_t;


/** @struct zb_kec_term_cmd_s
 *  @brief Terminate Key Establishment Command frame
 *
 * C.3.1.3.3.4 Terminate Key Establishment Command
 * The Terminate Key Establishment command may be sent by either the initiator or responder to
 * indicate a failure in the key establishment exchange.
 * C.3.1.3.3.4.1 Payload Format
 * The Terminate Key Establishment command payload shall be formatted as illustrated in Figure
 * C-11.
 * Octets      1                      1                              2
 * DataType    8-bit Enumeration     Unsigned  8-bit Integer        16-bit BitMap
 * FieldName    Status Code          Wait Time                      KeyEstablishmentSuite
 * Figure C-11- Terminate Key Establishment Command Payload
 *
 * --the same as in server--
 * typedef ZB_PACKED_PRE struct zb_kec_term_cmd_s
 * {
 *   zb_uint8_t      status_code;
 *   zb_uint8_t      wait_time;
 *   zb_uint16_t     key_suite;
 * } ZB_PACKED_STRUCT zb_kec_term_cmd_t;
 */


/** @fn zb_uint8_t zb_kec_activate_suite(zb_uint16_t suite)
 *  @brief Activate loaded keys
 *
 *  @param[in]    suite   CryptoSuite Id
 *  @return RET_OK, RET_ERROR
 */
zb_ret_t zb_kec_activate_suite(zb_uint16_t suite);


/** @fn zb_ret_t zb_kec_load_keys(zb_callback_t cb_done, zb_uint8_t param, zb_uint16_t suite, zb_uint8_t *ca_public_key, zb_uint8_t *certificate, zb_uint8_t *private_key)
 *  @brief Load (make a copy) and activate passed keys into local context
 *
 *  @param[in] cb_done - callback that will be executed after keys loading
 *  @param[in] param   - parameter that will be passed to @cb_done callback
 *  @param[in] suite - CryptoSuite Id
 *  @param[in] ca_public_key - Certification Authority (CA) public key
 *  @param[in] certificate - certificate, issued by CA
 *  @param[in] private_key - private key of device, issued together with certificate
 *  @return RET_OK, RET_ERROR
 */
zb_ret_t zb_kec_load_keys(zb_callback_t cb_done,
                             zb_bufid_t param,
                             zb_uint16_t suite,
                             zb_uint8_t *ca_public_key,
                             zb_uint8_t *certificate,
                             zb_uint8_t *private_key);


/** @fn void zse_kec_set_server_cb(zb_callback_t cb)
 *  @brief Sets server callback in global context
 *
 *  @param[in]    cb    pointer to function
 */
void zse_kec_set_server_cb(zb_callback_t cb);


/** @fn void zse_kec_set_client_cb(zb_callback_t cb)
 *  @brief Sets client callback in global context
 *
 *  @param[in]    cb    pointer to function
 */
void zse_kec_set_client_cb(zb_kec_client_cb_t cb);


/** @fn zb_ret_t zb_se_kec_initiate_key_establishment(zb_uint8_t param, zb_uint8_t src_ep, zb_uint8_t dst_ep, zb_callback_t cb)
 *  @brief Client command to initiate key Establishment
 *
 * @param[in]   param     buffer reference for command send
 * @param[in]   src_ep    source endpoint to send command from
 * @param[in]   dst_ep    destination endpoint to send command to
 * @param[in]   dst_addr_short destination short address to send command to
 * @param[in]   cb        callback function to call after key establishment completion
 * @returns ZB error code @ref zb_ret_t
 */
zb_ret_t zb_se_kec_initiate_key_establishment( zb_bufid_t param,
                                               zb_uint8_t src_ep,
                                               zb_uint8_t dst_ep,
                                               zb_uint16_t dst_addr_short,
                                               zb_callback_t cb
                                              );


/** @enum zb_kec_status_e
 *  @brief aaa
 */
typedef enum zb_kec_status_e
{
  ZB_SE_KEY_ESTABLISHMENT_STATE_ERROR = 0x80,
  ZB_SE_KEY_ESTABLISHMENT_BAD_MESSAGE,
  ZB_SE_KEY_ESTABLISHMENT_NO_RESOURCES,
  ZB_SE_KEY_ESTABLISHMENT_UNSUPPORTED_SUITE,
  ZB_SE_KEY_ESTABLISHMENT_TSN_ERROR,
  ZB_SE_KEY_ESTABLISHMENT_MAC_VERIFY_FAILED,
  ZB_SE_KEY_ESTABLISHMENT_KEYS_NOT_LOADED
} zb_kec_status_t;


/** @} */ /* se_kec_client_cmds */
#endif // ZB_SE_ENABLE_KEC_CLIENT

/** @} */ /* se_kec_cmd_frames */

/** @endcond */

/** @} */ /* ZB_ZCL_KEC */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#ifdef ZB_TH_ENABLED

void zse_kec_set_ch_srv_delays (zb_uint8_t x, zb_uint8_t y, zb_uint8_t z);
void zse_kec_set_ch_cli_delays (zb_uint8_t x, zb_uint8_t y);
void zse_kec_ch_skip_mac_check (void);

#endif /* ZB_TH_ENABLED */


/**
   CERT structure for NVRAM
 */
typedef ZB_PACKED_PRE struct zb_kec_cert_nvram_s
{
  zb_uint8_t      suite_no;
  zb_kec_qe_t     ca;
  zb_kec_pr_t     pr;
  zb_kec_icu_t    cert;
} ZB_PACKED_STRUCT zb_kec_cert_nvram_t;

zb_kec_cert_nvram_t * zb_zcl_kec_certdb_get_from_tc_storage(zb_uint8_t suite_no, zb_uint8_t *issuer, zb_kec_cert_nvram_t * buf);

/**
   Certificates hash for TC

   Do not store installcode itself in RAM - keep only 4 bytes per entry in RAM.
 */
typedef ZB_PACKED_PRE struct zb_kec_cert_storage_s
{
  zb_uint32_t    nvram_offset:28;      /*!< offset of zb_aps_installcode_nvram_t
                                          record in nvram. 28 bit supports 256k
                                          page - hope, it is enough  */
  zb_bitfield_t  nvram_page:4;         /*!< nvram page. Hope 16 pages are enough  */
} ZB_PACKED_STRUCT zb_kec_cert_storage_t;

typedef struct zb_kec_cert_add_s
{
  zb_kec_cert_nvram_t full_cert;
  zb_uint8_t*      issuer;
  zb_uint8_t*      subject;
  zb_uint8_t       suite_no;
  zb_uint8_t       do_update;
} zb_kec_cert_add_t;

zb_ret_t zb_kec_certdb_set(zb_uint8_t *ic);

typedef enum zb_kec_whitelist_op_e {
  ZSE_KE_WHITELIST_ADD,
  ZSE_KE_WHITELIST_DEL,
  ZSE_KE_WHITELIST_DEL_ALL,
  ZSE_KE_WHITELIST_NONE
} zb_kec_whitelist_op_t;

typedef struct zb_kec_whitelist_s {
  /* current page/position of page in nvram */
  zb_uint8_t page;
  zb_uint32_t pos;
  zb_uint16_t num_entries;

  /* Outstanding operation to perform */
  zb_ieee_addr_t addr;
  zb_kec_whitelist_op_t op;
} zb_kec_whitelist_t;

#define ZB_KEC_CERTDB_TBL_SIZE 20U

/** @brief KEC context data */
typedef struct zb_zcl_kec_ctx_s
{
  zb_ke_ctx_t ke;
  zb_kec_cert_storage_t certdb[ZB_KEC_CERTDB_TBL_SIZE];
  zb_kec_cert_add_t *cert_to_add;
#ifdef ZB_SE_KE_WHITELIST
  zb_kec_whitelist_t ke_whitelist;
#endif /* ZB_SE_KE_WHITELIST */
} zb_zcl_kec_ctx_t;

extern zb_zcl_kec_ctx_t g_kec_ctx;

/**
   Direct access to KEC context.

   To be used internally only.
 */
#define KEC_CTX() (g_kec_ctx)

void zb_kec_cluster_nvram_init(void);

#endif /* ZB_ZCL_KEC_INTERNAL_H */
