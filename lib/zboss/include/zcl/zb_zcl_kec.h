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
/* PURPOSE: Key-establishment cluster definitions
*/

#ifndef ZB_ZCL_KEC_H
#define ZB_ZCL_KEC_H 1

#include "zb_types.h"

/** @addtogroup ZB_ZCL_KEC
 *  @{
 */

/** @brief Key Establishment cluster's attributes IDs
 *
 *  The Information attribute set contains the attributes summarized in table below
 * <table>
 *  <caption> Key Establishment Attribute Sets </caption>
 *   <tr>
 *     <th> Identifier </th>
 *     <th> Name </th>
 *     <th> Type </th>
 *     <th> Range </th>
 *     <th> Access </th>
 *     <th> Default </th>
 *   </tr>
 *   <tr>
 *     <td> 0x0000 </td>
 *     <td> KeyEstablishmentSuite </td>
 *     <td> 16-bit Enumeration </td>
 *     <td> 0x0000-0xFFFF </td>
 *     <td> Readonly </td>
 *     <td> 0x0000 </td>
 *   </tr>
 * </table>
 * @see SE spec, C.3.1.2.2.1
 */
#define ZB_ZCL_ATTR_KEY_ESTABLISHMENT_SUITE_ID 0x0000U   /**< KeyEstablishmentSuite attribute */

/** @brief Default value for Key Establishment cluster revision global attribute */
#define ZB_ZCL_KEY_ESTABLISHMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

/**
 * @name KeyEstablishmentSuite attribute values
 * @anchor kec_key_suite
 * @brief Table Values of the KeyEstablishmentSuite Attribute (Table C-4)
 */
/** @{ */
#define KEC_CS1 (1U << 0) /*!< Certificate-based Key Establishment Cryptographic Suite 1 (Crypto Suite 1)*/
#define KEC_CS2 (1U << 1) /*!< Certificate-based Key Establishment Cryptographic Suite 2 (Crypto Suite 2)*/
/** @} */

/**
 * @brief Type for KeyEstablishmentSuite attribute values.
 *
 * @deprecated holds one of @ref kec_key_suite. Kept only for backward compatibility as
 * @ref kec_key_suite were declared previously as enum. Can be removed in future releases.
 */
typedef zb_uint8_t zb_kec_key_suite_t;


/** @def ZB_KEC_SUPPORTED_CRYPTO_ATTR
 *  @brief Attribute value const (supported CryptoSuites)
 */
#define ZB_KEC_SUPPORTED_CRYPTO_ATTR (KEC_CS1 | KEC_CS2)

/** @cond internals_doc */

/** @brief Declare attribute list for Key Establishment cluster
 *  @param[in]  attr_list - attribute list variable name
 *  @param[in]  kec_key_establishment_suite - pointer to variable to store KeyEstablishmentSuite
 *              value
 */
#define ZB_ZCL_DECLARE_KEC_ATTRIB_LIST(attr_list, kec_key_establishment_suite)                     \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION_STATIC(attr_list, ZB_ZCL_KEY_ESTABLISHMENT)    \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_KEY_ESTABLISHMENT_SUITE_ID, (kec_key_establishment_suite),    \
                         ZB_ZCL_ATTR_TYPE_16BIT_ENUM, ZB_ZCL_ATTR_ACCESS_READ_ONLY)                \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @endcond */
/**
 *  @brief Key Establishment cluster attributes
 */
 typedef struct zb_zcl_kec_attrs_s
 {
   /** @copydoc ZB_ZCL_ATTR_KEY_ESTABLISHMENT_SUITE_ID
    * @see ZB_ZCL_ATTR_KEY_ESTABLISHMENT_SUITE_ID
    */
   zb_uint16_t kec_suite;
 } zb_zcl_kec_attrs_t;


 /** @brief Declare attribute list for Key Establishment cluster
 *  @param[in]  attr_list - attribute list variable name
 *  @param[in]  attrs - pointer to @ref zb_zcl_kec_attrs_s structure
 */
#define ZB_ZCL_DECLARE_KEC_ATTR_LIST(attr_list, attrs)  \
  ZB_ZCL_DECLARE_KEC_ATTRIB_LIST(attr_list, &attrs.kec_suite)

/** @} */ /* ZB_ZCL_KEC */

/** @cond internals_doc */
/** Internal handler for Key Establishment Cluster commands */
void zb_zcl_kec_init_server(void);
void zb_zcl_kec_init_client(void);
#define ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT_SERVER_ROLE_INIT zb_zcl_kec_init_server
#define ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT_CLIENT_ROLE_INIT zb_zcl_kec_init_client

/** @endcond */ /* internals_doc */

/**
 * @brief Loads device's certificate to NVRAM.
 * @details This function is used to store a private key and a digital certificate, which is signed by a Certificate Authority (CA).
 *
 * @param[in]  suite - CryptoSuite ID (@ref kec_key_suite)
 * @param[in]  ca_public_key - buffer with Certification Authority's public key
 * @param[in]  certificate - buffer with device's certificate
 * @param[in]  private_key - buffer with device's private key
 *
 * @retval RET_OK - on success
 * @retval RET_CONVERSION_ERROR - invalid certificate for the issuer
 *
 * @note This function is designed mainly for Trust Center devices as an additional method of
 *   adding certificates from several CAs.
 *
 * @par Example
 * Loading certificates into NVRAM with both CryptoSuites:
 * @snippet se/energy_service_interface/se_esi_zc_debug.c SIGNAL_HANDLER_LOAD_CERT
 *
 * @see <b> Certificate-Based Key Establishment 10.7.6.2 (ZCL8) </b>
 *
 * @see ZB_SE_SIGNAL_CBKE_FAILED
 */
zb_ret_t zb_zcl_kec_load_ecc_cert(zb_uint16_t suite,
                                  zb_uint8_t *ca_public_key,
                                  zb_uint8_t *certificate,
                                  zb_uint8_t *private_key);


/**
 * @brief Erases device's certificate from NVRAM.
 *
 * @param[in] suite_no - CryptoSuite number
 * @param[in] issuer - buffer with certificate's issuer
 * @param[in] subject - buffer MAC address (IEEE 802.15.4)
 *
 * @retval RET_OK - entry was found and successfully deleted
 * @retval RET_NOT_FOUND - there was no such entry
 *
 * @note This function is designed primarily for Trust Center devices to erase
 *   certificates from NVRAM by suite, issuer and subject (MAC address).
 *
 * @note Error codes might originate from NVRAM operations.
 *
 * @see zb_se_load_ecc_cert()
 */
zb_ret_t zb_zcl_kec_erase_ecc_cert(zb_uint8_t suite_no,
                                   zb_uint8_t *issuer,
                                   zb_uint8_t *subject);

#endif /* ZB_ZCL_KEC_H */
