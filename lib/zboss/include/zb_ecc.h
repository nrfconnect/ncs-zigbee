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
/* PURPOSE: ECC security services and routines - internals
*/

#ifndef ZB_SECUR_ECC_H
#define ZB_SECUR_ECC_H 1
#include "zboss_api.h"
#include "zb_types.h"

/*! @cond internals_doc */
/*! \addtogroup ZB_SECUR */
/*! @{ */


/* CURVE25519 for r23 */
#define ZB_ENABLE_MONTGOMERY_CURVES
#define ECC_SYNC_MODE_ENABLED
/* CBKE */
#ifdef ZB_SE_ENABLE_KEC_CLUSTER
#define ZB_ENABLE_KOBLITZ_CURVES
#endif

#define ZB_SHA_256_STATEBYTES 32U
#define ZB_SHA_256_BLOCKBYTES 64U

#define ZB_ECC_HASH_LEN_AES_MMO_128 16U
#define ZB_ECC_HASH_LEN_SHA_256 32U

#define ZB_ECC_HASH_LEN 16UL

#define ZB_ECC_SECT163K1 0U
#define ZB_ECC_SECT283K1 1U
#define ZB_ECC_CURVE_25519 2U
#define ZB_ECC_CURVE_P256 3U
#define ZB_ECC_INVALID 0xFFU

typedef enum zb_ecc_dlk_hash_type_e {
  ZB_ECC_DLK_HASH_AES_MMO_128,
  ZB_ECC_DLK_HASH_SHA_256
} zb_ecc_dlk_hash_type_t;

#define ZB_ECC_SECT163K1_PRV_KEY_LEN 21U
#define ZB_ECC_SECT283K1_PRV_KEY_LEN 36U

/**
 * Actually size of private key is 32, but we need a 320-bit (40-byte) random input
 * for the P256 algorithm (see "DSR - Cryptographic Processing for ECDHE"),
 * but it is easier to use 64 bytes input instead of 40-bytes
 *
 * note: that value is only used in RNG currently
 */
#define ZB_ECC_CURVE_P256_PRV_KEY_LEN 64U
#define ZB_ECC_CURVE_P256_BASE_POINT_LEN 64U

/*For the curve sect163k1*/
#ifdef sect163k1
#define IF1_163(X)         (X)[0] == 1U && (X)[1] == 0U && (X)[2] == 0U && (X)[3] == 0U && (X)[4] == 0U && (X)[5] == 0U
#define IFN1_163(X)        (X)[0] != 1U || (X)[1] != 0U || (X)[2] != 0U || (X)[3] != 0U || (X)[4] != 0U || (X)[5] != 0U
#define IF0_163(X)         (X)[0] == 0U && (X)[1] == 0U && (X)[2] == 0U && (X)[3] == 0U && (X)[4] == 0U && (X)[5] == 0U
#define IFN0_163(X)        (X)[0] != 0U || (X)[1] != 0U || (X)[2] != 0U || (X)[3] != 0U || (X)[4] != 0U || (X)[5] != 0U
#endif

#define ZB_ECC_SECT163K1_PUB_KEY_LEN 22U
#define ZB_ECC_SECT283K1_PUB_KEY_LEN 37U

#define ZB_ECC_CURVE_P256_PUB_KEY_LEN 64U
#define ZB_ECC_CURVE25519_BASE_POINT_LEN 32U

#define ZB_DLK_PRV_KEY_LEN(curve_id) (((curve_id) == (ZB_ECC_CURVE_25519)) ? \
                                      (ZB_ECC_CURVE25519_PRV_KEY_LEN) : (ZB_ECC_CURVE_P256_PRV_KEY_LEN))

#define ZB_DLK_PRV_KEY_MAX_LEN ZB_ECC_CURVE_P256_PRV_KEY_LEN

#define ZB_DLK_BASE_POINT_LEN(curve_id) (((curve_id) == (ZB_ECC_CURVE_25519)) ? \
                                         (ZB_ECC_CURVE25519_BASE_POINT_LEN) : (ZB_ECC_CURVE_P256_BASE_POINT_LEN))

#define ZB_DLK_BASE_POINT_MAX_LEN ZB_ECC_CURVE_P256_BASE_POINT_LEN

#define ZB_DLK_PUB_KEY_LEN(curve_id) (((curve_id) == (ZB_ECC_CURVE_25519)) ? \
                                      (ZB_ECC_CURVE25519_PUB_KEY_LEN) : (ZB_ECC_CURVE_P256_PUB_KEY_LEN))

#define ZB_DLK_HASH_LEN(hash_id) (((hash_id) == (ZB_ECC_DLK_HASH_AES_MMO_128)) ? \
                                  (ZB_ECC_HASH_LEN_AES_MMO_128) : (ZB_ECC_HASH_LEN_SHA_256))

/**
 * Maximum values of keys sizes for currently used curves;
 * used in common ecc arrays definitions
 */
#define ZB_ECC_PUB_KEY_MAX_LEN \
  ((ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_25519) > ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_P256)) ? \
   (ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_25519)) :                           \
   (ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_P256)))

#define ZB_ECC_PRV_KEY_MAX_LEN \
  ((ZB_DLK_PRV_KEY_LEN(ZB_ECC_CURVE_25519) > ZB_DLK_PRV_KEY_LEN(ZB_ECC_CURVE_P256)) ? \
   (ZB_DLK_PRV_KEY_LEN(ZB_ECC_CURVE_25519)) :                           \
   (ZB_DLK_PRV_KEY_LEN(ZB_ECC_CURVE_P256)))

/* Length of shared secret, currently does not depend on curve type */
#define ZB_ECC_SHARED_SECRET_LEN 32U

/* Length of derived (session) key, currently does not depend on curve type */
#define ZB_ECC_DERIVED_KEY_LEN 16U

/*For the curve sect283k1*/
#ifdef sect283k1
#define IF1_283(X)         (X)[0] == 1U && (X)[1] == 0U && (X)[2] == 0U && (X)[3] == 0U && (X)[4] == 0U && (X)[5] == 0U && (X)[6] == 0U && (X)[7] == 0U && (X)[8] == 0U // && X[9] == 0
#define IFN1_283(X)        (X)[0] != 1U || (X)[1] != 0U || (X)[2] != 0U || (X)[3] != 0U || (X)[4] != 0U || (X)[5] != 0U || (X)[6] != 0U || (X)[7] != 0U || (X)[8] != 0U // || X[9] != 0
#define IF0_283(X)         (X)[0] == 0U && (X)[1] == 0U && (X)[2] == 0U && (X)[3] == 0U && (X)[4] == 0U && (X)[5] == 0U && (X)[6] == 0U && (X)[7] == 0U && (X)[8] == 0U // && X[9] == 0
#define IFN0_283(X)        (X)[0] != 0U || (X)[1] != 0U || (X)[2] != 0U || (X)[3] != 0U || (X)[4] != 0U || (X)[5] != 0U || (X)[6] != 0U || (X)[7] != 0U || (X)[8] != 0U // || X[9] != 0
#endif

#define ZB_ECC_SECRET_MAX_LEN 32U
#define ZB_ECC_SHARED_SECRET_MAX_LEN 32U

/* See Zigbee Direct Spec 6.8.1.2.3-4 SE Messages 3 & 4, MAC Tag related values */
#define ZB_DIRECT_MAC_PREAMBLE_SIZE 6U  //!< Size of preamble to be put to MAC Tag data
#define ZB_DIRECT_MAC_DATA_MAX_SIZE                                                                \
  ((zb_uint16_t)ZB_DIRECT_MAC_PREAMBLE_SIZE + (zb_uint16_t)(sizeof(zb_ieee_addr_t) * 2U)           \
   + ((zb_uint16_t)ZB_ECC_PUB_KEY_MAX_LEN * 2U))  //!< Max size of input for MAC Tag calculation

#define ZB_PSEUDOPSK_MAX_LENGTH (8U + 64U + 8U + 64U + 16U)
/**
 * Set base point (generator) for curve
 * @param base_point - base point (32-byte for curve25519)
 */
zb_ret_t zb_ecc_set_curve_base_point(zb_uint8_t curve_id, zb_uint8_t *base_point);

/**
 * Get base point (generator) for curve
 * @return pointer on the registered base_point (32-byte for curve25519)
 * or NULL if no base point available
 */
zb_uint8_t *crypto_get_curve25519_base(void);

/**
 * Set inactive previous registered base point if new one was generated.
 */
void crypto_new_curve25519_base(void);

/**
 * Get error from last executed function
 * @return           last ecc error
 */
zb_ret_t zb_ecc_get_last_error(zb_uint8_t curve_id);

/**
 * Keyed hash function. It uses AES-MMO-128 hash function
 *
 * @param mac_key    - security key in length.
 * @param input      - input data to compute MAC.
 * @param input_len  - length of input data
 * @param hash       - MAC computed over security key and given input data
 */
void zb_ecc_key_hmac(const zb_uint8_t *mac_key, const zb_uint8_t *input, const zb_uint16_t input_len, zb_bufid_t hash);

#ifdef ECC_SYNC_MODE_ENABLED
/**
* Key pair generation procedure
* @param rnd     - rnd a random number (21-byte for sect163k1 and 36-byte for sect283k1 and 32-byte for curve25519)
* @param prv_key - generated private key
* @param pub_key - generated public key
*/
zb_ret_t zb_ecc_key_generation(zb_uint8_t curve_id, const zb_uint8_t *rnd, zb_uint8_t *prv_key, zb_uint8_t *pub_key);

#endif

/**
 * Key pair generation procedure. Async mode.
 * @param async_cb       - callback which will be executed after generation procedure.
 * @param async_cb_param - parameter which will be passed to @async_cb function. It is unchanged during execution
 * @param rnd            - rnd a random number (21-byte for sect163k1 and 36-byte for sect283k1)
 * @param prv_key        - generated private key
 * @param pub_key        - generated public key
 */
zb_ret_t zb_ecc_key_generation_async(zb_uint8_t curve_id, zb_callback_t async_cb, const zb_uint8_t async_cb_param, const zb_uint8_t *rnd, zb_uint8_t *prv_key, zb_uint8_t *pub_key);

#ifdef ECC_SYNC_MODE_ENABLED
/**
* Elliptic Curve Qu-Vanstone implicit certificate scheme. Public key extraction procedure.
*
* @param QCAx           - the CA's public key in compressed form
* @param pub_rec        - reconstruction public key in original form
* @param CertA          - an implicit certificate for user idA
* @param QAx            - a user A's public key in compressed form
* @return               - return RET_OK in success and return RET_ERROR in fail.
*/
zb_ret_t zb_ecc_ecqv_extraction(zb_uint8_t curve_id, zb_uint8_t *QCAx, zb_uint8_t *pub_rec, zb_uint8_t *CertA, zb_uint8_t *QAx);
#endif

/**
 * Elliptic Curve Qu-Vanstone implicit certificate scheme. Public key extraction procedure. Async mode
 *
 * @param async_cb       - callback which will be executed after extraction procedure.
 * @param async_cb_param - parameter which will be passed to @async_cb function. It is unchanged during execution.
 * @param QCAx           - the CA's public key in compressed form
 * @param pub_rec        - reconstruction public key in original form
 * @param CertA          - an implicit certificate for user idA
 * @param QAx            - a user A's public key in compressed form
 */
void zb_ecc_ecqv_extraction_async(zb_uint8_t curve_id, zb_callback_t async_cb, const zb_uint8_t async_cb_param, zb_uint8_t *QCAx, zb_uint8_t *pub_rec, zb_uint8_t *CertA, zb_uint8_t *QAx);

#ifdef ECC_SYNC_MODE_ENABLED
/**
* Elliptic curve MQV scheme for shared secret generation
*
* @param prvA1    - A's private key
* @param prvA2    - A's ephemeral private key
* @param pubA2    - A's ephemeral public key
* @param pubB1    - B's public key
* @param pubB2    - B's ephemeral public key
* @param skey     - a shared key between A and B
* @return         - return RET_OK in success and return RET_ERROR in fail.
*/
zb_ret_t zb_ecc_ecmqv(zb_uint8_t curve_id, zb_uint8_t *prvA1, zb_uint8_t *prvA2,
                      zb_uint8_t *pubA2, zb_uint8_t *pubB1, zb_uint8_t *pubB2, zb_uint8_t *skey);
#endif

/**
 * Elliptic curve MQV scheme for shared secret generation. Async mode
 *
 * @param async_cb       - callback which will be executed after extraction procedure.
 * @param async_cb_param - parameter which will be passed to @async_cb function. It is unchanged during execution.
 * @param prvA1          - A's private key
 * @param prvA2          - A's ephemeral private key
 * @param pubA2          - A's ephemeral public key
 * @param pubB1          - B's public key
 * @param pubB2          - B's ephemeral public key
 * @param skey           - a shared key between A and B
 * @return               - return RET_OK in success and return RET_ERROR in fail.
 */
zb_ret_t zb_ecc_ecmqv_async(zb_uint8_t curve_id, zb_callback_t async_cb, const zb_uint8_t async_cb_param, zb_uint8_t *prvA1,
                            zb_uint8_t *prvA2, zb_uint8_t *pubA2,
                            zb_uint8_t *pubB1, zb_uint8_t *pubB2, zb_uint8_t *skey);

/*! @} */
/*! @endcond */

/**
 * Elliptic curve Diffie-Helman for shared secret generation
 * @param curve_id      - id of a curve for current selected key generation method
 * @param shared_secret - shared secret between two devices (responder and initiator)
 * @param pub_key       - the public key from another party
 * @param prv_key       - our private key
 * @return              - RET_OK on success and return RET_ERROR otherwise
 */
zb_ret_t zb_ecc_ecdh_compute_shared(zb_uint8_t curve_id, zb_uint8_t *shared_secret, const zb_uint8_t *pub_key, const zb_uint8_t *prv_key, const zb_uint8_t *psk);

/**
 * Elliptic curve Diffie-Helman for derived key generation
 * @param curve_id      - id of a curve for current selected key generation method
 * @param derived_key   - derived key to be used for ccm
 * @param shared_secret - shared secret between two devices (responder and initiator)
 * @param buffer        - buffer id, needed for calculations
 * @return              - RET_OK on success and return RET_ERROR otherwise
 */
zb_ret_t zb_ecc_ecdh_compute_derived(zb_uint8_t curve_id, zb_uint8_t* derived_key, const zb_uint8_t *shared_secret);

/**
 * AES-MMO-128 hash function
 *
 * @param input     - buffer holding the input data. This must be a buffer of length \p input_len
 * @param input_len - The length of input data in bytes
 * @param output    - The AES-MMO-128 checksum result. This must be a buffer of length \c 16 bytes.
 */
zb_ret_t zb_aes_mmo_128(const zb_uint8_t *input, const zb_size_t input_len, zb_uint8_t *output);

/**
 * HMAC-AES-MMO-128 function
 *
 * @param key          - security key in length. It must be a buffer of length \p key_len.
 * @param key_len      - length of key
 * @param input        - input data to compute MAC. It must be a buffer of length \p input_len
 * @param input_len    - length of input data
 * @param output       - MAC computed over security key and given input data
 */
zb_ret_t zb_hmac_aes_mmo_128(const zb_uint8_t *key, const zb_size_t key_len,
                         const zb_uint8_t *input, const zb_size_t input_len, zb_uint8_t *output);

/**
 * SHA-256 hash function
 *
 * @param input     - buffer holding the input data. This must be a buffer of length \p input_len
 * @param input_len - The length of input data in bytes
 * @param output    - The SHA-256 checksum result. This must be a buffer of length \c 32 bytes.
 */
void zb_sha_256(const zb_uint8_t *input, const zb_size_t input_len, zb_uint8_t output[32]);

#ifndef ZB_HW_HMAC_SHA_256
/**
 * HMAC-SHA256 function
 *
 * @param key          - security key in length. It must be a buffer of length \p key_len.
 * @param key_len      - length of key
 * @param input        - input data to compute MAC. It must be a buffer of length \p input_len
 * @param input_len    - length of input data
 * @param output       - MAC computed over security key and given input data
 */
zb_ret_t zb_sw_hmac_sha_256(const zb_uint8_t *key,
                            const zb_size_t key_len,
                            const zb_uint8_t *input,
                            const zb_size_t input_len,
                            zb_uint8_t *output);
#endif /* ZB_HW_HMAC_SHA_256 */

/**
 * @brief Key Derivation Function based on a hash-based
 * message authentication code (HMAC).
 *
 * NOTE: This implementation uses SHA-256 and HMAC-SHA256 algorithms accordingly
 *
 * @param prk      - a pseudorandom key of at least HashLen octets (usually, the output from the extract step).
 *                   This must be a buffer of \p prk_len bytes
 * @param prk_len  - length of prk in bytes
 * @param info     - optional context and application specific information (can be a zero-length string)
 * @param info_len - length of info data in bytes
 * @param okm      - output keying material (of \p okm_len bytes)
 * @param okm_len  - length of output keying material in bytes (<= 255*HashLen)
 * @return         - @RET_OK if there was no error, @RET_ERROR if error occurred and
 *                   @RET_NO_MEMORY if there was problem with buffer allocation
 */
zb_ret_t zb_hkdf_expand(const zb_uint8_t *prk, zb_uint16_t prk_len, zb_uint8_t *info, zb_uint16_t info_len,
                        zb_uint8_t *okm, const zb_uint16_t okm_len, zb_bufid_t buffer);

zb_uint8_t zb_ecc_public_key_len(const zb_uint8_t ecc_curve_id);
zb_uint8_t zb_ecc_public_key_len_dwords(const zb_uint8_t ecc_curve_id);
zb_uint8_t zb_ecc_private_key_len(const zb_uint8_t ecc_curve_id);
void zb_ecc_set_last_error(zb_uint8_t curve_id, zb_ret_t ret);

zb_ret_t zb_ecc_dlk_create_base_point(zb_uint8_t curve_id, zb_ecc_dlk_hash_type_t hash_type, zb_uint8_t *psk, zb_uint_t psk_len, zb_uint8_t *output_bp);

/**
 * Create session identifier
 *
 * @param Ai         - initiator identity, IEEE EUI-64 of initiator
 * @param Qi         - initiator public key point
 * @param Ar         - responder identity, IEEE EUI-64 of responder
 * @param Qr         - responder public key point
 * @param output_identifier - generated session identifier
 */
zb_ret_t zb_ecc_dlk_create_session_identifier(zb_ecc_dlk_hash_type_t hash_type, zb_uint8_t *Ai, zb_uint8_t *Qi, zb_uint8_t *Ar, zb_uint8_t *Qr, zb_uint8_t *output_identifier);

/**
 * Create dlk shared secret
 *
 * @param xk         - shared secret between two devices (responder and initiator)
 * @param I          - session identifier
 * @param G          - generator (curve25519 base point)
 * @param output_secret - generated secret
 */
zb_ret_t zb_ecc_dlk_create_shared_secret(zb_ecc_dlk_hash_type_t hash_type, zb_uint8_t *xk, zb_uint8_t *I, zb_uint8_t *G, zb_uint8_t *output_secret);

/**
 * Create derived key
 *
 * @param s          - dlk shared secret
 * @param s_len      - dlk shared secret length
 * @param output_key - generated key
 */
zb_ret_t zb_ecc_dlk_create_derived_key(zb_ecc_dlk_hash_type_t hash_type, const zb_uint8_t *s, zb_size_t s_len, zb_uint8_t *output_key);

#endif  /* ZB_SECUR_ECC_H */
