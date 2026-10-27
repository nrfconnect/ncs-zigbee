/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZB_NRF_CRYPTO_H__
#define ZB_NRF_CRYPTO_H__

void zb_osif_rng_init(void);
void zb_osif_aes_init(void);

#if defined(ZB_PSA_CRYPTO_STORAGE)
/* Master-key CTR helpers (libzboss contract; see hw_crypto_api.h in ZBOSS sources). */
#define ZB_PSA_IV_SIZE 16U

void zb_psa_init(void);
void zb_psa_generate_master_key(void);
#endif

#endif /* ZB_NRF_CRYPTO_H__ */
