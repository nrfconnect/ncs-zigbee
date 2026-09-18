/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/*
 * Strong replacements for the weak ZBOSS master-key CTR helpers.
 *
 * These belong to the Zephyr build rather than the standalone ZBOSS Makefile
 * build: psa_cipher_operation_t is sized by the active PSA driver, and the two
 * builds resolve different psa/crypto_driver_contexts_*.h headers. Allocating
 * the operation objects on the Makefile side makes them smaller than what
 * nrf_security writes through, overflowing whatever follows them in .bss.
 */

#include <zboss_api.h>

/*
 * Gated on the stack macro rather than CONFIG_ZIGBEE_PSA_CRYPTO_STORAGE: only
 * the former tells us whether libzboss declares and calls these weak symbols.
 * The MAC certification featuresets keep PSA crypto off, so the file has to
 * compile away there.
 */
#if defined(ZB_PSA_CRYPTO_STORAGE)

#include <hw_crypto_api.h>
#include <psa/crypto.h>

#define ZB_PSA_MASTER_KEY_ID 0x1

/* Largest payload ZBOSS passes here: link key followed by passphrase. */
#define ZB_PSA_MASTER_MAX_PAYLOAD_SIZE (2U * ZB_CCM_KEY_SIZE)

/* PSA drivers may emit whole AES blocks, so the output buffer handed to
 * psa_cipher_update()/psa_cipher_finish() needs a block of slack beyond the
 * payload. The caller's buffer is exactly payload-sized and lives on the
 * cooperative ZBOSS stack, so the operation runs into this static scratch and
 * only the payload is copied back out.
 */
#define ZB_PSA_MASTER_SCRATCH_SIZE (ZB_PSA_MASTER_MAX_PAYLOAD_SIZE + ZB_CCM_KEY_SIZE)

static psa_cipher_operation_t gs_zb_psa_master_encrypt_op;
static psa_cipher_operation_t gs_zb_psa_master_decrypt_op;
static zb_uint8_t gs_zb_psa_master_scratch[ZB_PSA_MASTER_SCRATCH_SIZE];

static zb_ret_t zb_psa_master_key_crypt(psa_cipher_operation_t *op,
					zb_bool_t encrypt,
					const zb_uint8_t *input,
					zb_uint8_t input_len,
					const zb_uint8_t *iv,
					zb_uint8_t *output)
{
	psa_status_t status;
	size_t update_len = 0U;
	size_t finish_len = 0U;

	if (input == NULL || output == NULL || iv == NULL ||
	    input_len > (zb_uint8_t)ZB_PSA_MASTER_MAX_PAYLOAD_SIZE) {
		return RET_ERROR;
	}

	zb_psa_init();

	if (encrypt == ZB_TRUE) {
		zb_psa_generate_master_key();
	}

	*op = psa_cipher_operation_init();

	status = (encrypt == ZB_TRUE) ?
		psa_cipher_encrypt_setup(op, ZB_PSA_MASTER_KEY_ID, PSA_ALG_CTR) :
		psa_cipher_decrypt_setup(op, ZB_PSA_MASTER_KEY_ID, PSA_ALG_CTR);
	if (status != PSA_SUCCESS) {
		return RET_ERROR;
	}

	status = psa_cipher_set_iv(op, iv, ZB_PSA_IV_SIZE);
	if (status != PSA_SUCCESS) {
		(void)psa_cipher_abort(op);
		return RET_ERROR;
	}

	status = psa_cipher_update(op, input, (size_t)input_len,
				   gs_zb_psa_master_scratch,
				   sizeof(gs_zb_psa_master_scratch),
				   &update_len);
	if (status != PSA_SUCCESS) {
		(void)psa_cipher_abort(op);
		return RET_ERROR;
	}

	status = psa_cipher_finish(op,
				   gs_zb_psa_master_scratch + update_len,
				   sizeof(gs_zb_psa_master_scratch) - update_len,
				   &finish_len);
	if (status != PSA_SUCCESS) {
		(void)psa_cipher_abort(op);
		return RET_ERROR;
	}

	if ((update_len + finish_len) != (size_t)input_len) {
		return RET_ERROR;
	}

	ZB_MEMCPY(output, gs_zb_psa_master_scratch, input_len);

	return RET_OK;
}

zb_ret_t zb_psa_encrypt_by_master_key(const zb_uint8_t *plaintext,
				      zb_uint8_t plaintext_len,
				      const zb_uint8_t *iv,
				      zb_uint8_t *output)
{
	return zb_psa_master_key_crypt(&gs_zb_psa_master_encrypt_op, ZB_TRUE,
				       plaintext, plaintext_len, iv, output);
}

zb_ret_t zb_psa_decrypt_by_master_key(const zb_uint8_t *ciphertext,
				      zb_uint8_t ciphertext_len,
				      const zb_uint8_t *iv,
				      zb_uint8_t *output)
{
	return zb_psa_master_key_crypt(&gs_zb_psa_master_decrypt_op, ZB_FALSE,
				       ciphertext, ciphertext_len, iv, output);
}

#endif /* ZB_PSA_CRYPTO_STORAGE */
