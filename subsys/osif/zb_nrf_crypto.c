/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/sys/__assert.h>
#include <zephyr/random/random.h>
#include <zigbee/zigbee_settings_subsys.h>
#include <zboss_api.h>
#if CONFIG_NRF_SECURITY
#include <psa/crypto.h>
#else
#error No crypto suite for Zigbee stack has been selected
#endif

#include "zb_nrf_crypto.h"

#define ECB_AES_KEY_SIZE   16
#define ECB_AES_BLOCK_SIZE 16

void zb_osif_rng_init(void)
{
}

zb_uint32_t zb_osif_random_hw(void)
{
	return zb_random_seed();
}

zb_uint32_t zb_random_seed(void)
{
	zb_uint32_t rnd_val = 0;
	int err_code;

#if defined(CONFIG_ENTROPY_HAS_DRIVER)
	err_code = sys_csrand_get(&rnd_val, sizeof(rnd_val));
#else
#warning Entropy driver required to generate cryptographically secure random numbers
	sys_rand_get(&rnd_val, sizeof(rnd_val));
	err_code = 0;
#endif /* CONFIG_ENTROPY_HAS_DRIVER */
	__ASSERT_NO_MSG(err_code == 0);
	return rnd_val;
}

#if defined(ZB_PSA_CRYPTO)

/*
 * Override weak zb_psa_init() from libzboss (see secur/zb_psa_crypto.c).
 * Settings must be ready before psa_crypto_init() when using persistent
 * PSA storage for the ZBOSS master key (ZOI: override platform PSA hooks).
 */
void zb_psa_init(void)
{
	psa_status_t status;

#if defined(CONFIG_SETTINGS) && defined(CONFIG_MBEDTLS_PSA_CRYPTO_STORAGE_C)
	int err = zigbee_settings_subsys_init();

	__ASSERT(err == 0, "Cannot initialize settings for PSA storage (err: %d)", err);
#endif

	status = psa_crypto_init();
	__ASSERT(status == PSA_SUCCESS, "Cannot initialize PSA crypto");
}

#endif /* ZB_PSA_CRYPTO */

void zb_osif_aes_init(void)
{
#if defined(ZB_PSA_CRYPTO)
	zb_psa_init();
#endif
}

#if defined(ZB_PSA_CRYPTO_STORAGE)

#define ZB_PSA_MASTER_KEY_ID 0x1

static psa_status_t import_master_key(psa_key_lifetime_t lifetime)
{
	psa_status_t status;
	psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_key_id_t key_id;
	uint8_t key_material[16];

	attributes = (psa_key_attributes_t)PSA_KEY_ATTRIBUTES_INIT;
	psa_set_key_id(&attributes, ZB_PSA_MASTER_KEY_ID);
	psa_set_key_lifetime(&attributes, lifetime);
	psa_set_key_algorithm(&attributes, PSA_ALG_CTR);
	psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&attributes, 128);
	psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);

#if defined(CONFIG_ENTROPY_HAS_DRIVER)
	__ASSERT(sys_csrand_get(key_material, sizeof(key_material)) == 0,
		 "Cannot get random material for PSA master key");
#else
	sys_rand_get(key_material, sizeof(key_material));
#endif

	status = psa_import_key(&attributes, key_material, sizeof(key_material), &key_id);
	psa_reset_key_attributes(&attributes);

	return status;
}

void zb_psa_generate_master_key(void)
{
	psa_status_t status;
	psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;

	zb_psa_init();

	if (PSA_SUCCESS == psa_get_key_attributes(ZB_PSA_MASTER_KEY_ID, &attributes)) {
		psa_reset_key_attributes(&attributes);
		return;
	}
	psa_reset_key_attributes(&attributes);

	status = import_master_key(PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
		PSA_KEY_PERSISTENCE_DEFAULT, PSA_KEY_LOCATION_LOCAL_STORAGE));
	if (status != PSA_SUCCESS) {
		(void)import_master_key(PSA_KEY_LIFETIME_VOLATILE);
	}
}

#endif /* ZB_PSA_CRYPTO_STORAGE */

void zb_osif_aes128_hw_encrypt(const zb_uint8_t *key, const zb_uint8_t *msg, zb_uint8_t *c)
{
	if (!(c && msg && key)) {
		__ASSERT(false, "NULL argument passed");
		return;
	}

	psa_status_t status;
	psa_key_id_t key_id;
	uint32_t out_len;

	ZVUNUSED(status);

	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_ECB_NO_PADDING);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&key_attributes, 128);

	status = psa_import_key(&key_attributes, key, ECB_AES_KEY_SIZE, &key_id);
	__ASSERT(status == PSA_SUCCESS, "psa_import failed! (Error: %d)", status);

	psa_reset_key_attributes(&key_attributes);

	status = psa_cipher_encrypt(key_id, PSA_ALG_ECB_NO_PADDING, msg, ECB_AES_KEY_SIZE, c,
				    ECB_AES_KEY_SIZE, &out_len);
	__ASSERT(status == PSA_SUCCESS, "psa_cipher_encrypt failed! (Error: %d)", status);

	psa_destroy_key(key_id);
}

zb_int_t zb_osif_scalarmult(zb_uint8_t *result_point,
                            const zb_uint8_t *scalar,
                            const zb_uint8_t *point)
{
	psa_status_t status;
	mbedtls_svc_key_id_t key_id;
	size_t output_length;

	ZVUNUSED(status);

#if defined(ZB_PSA_CRYPTO)
	zb_psa_init();
#endif

	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_DERIVE);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDH);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_MONTGOMERY));

	status = psa_import_key(&key_attributes, scalar, ZB_ECC_CURVE25519_BASE_POINT_LEN, &key_id);
	__ASSERT(status == PSA_SUCCESS, "psa_import failed! (Error: %d)", status);

	psa_reset_key_attributes(&key_attributes);

	status = psa_raw_key_agreement(PSA_ALG_ECDH, key_id, point, ZB_ECC_CURVE25519_BASE_POINT_LEN,
			 result_point, ZB_ECC_SECRET_MAX_LEN, &output_length);
	__ASSERT(status == PSA_SUCCESS, "psa_raw_key_agreement failed! (Error: %d)", status);

	psa_destroy_key(key_id);

	return 0;
}
