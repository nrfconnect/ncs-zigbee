/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZBOSS_PSA_MAKEFILE_CONFIG_H
#define ZBOSS_PSA_MAKEFILE_CONFIG_H

/*
 * Wrapper for the Makefile build of zb_psa_crypto.c. Includes the Zephyr child
 * image PSA config, then clears options that require headers or drivers not
 * available in the standalone ZBOSS Makefile build (RNG and threading are
 * provided by the application nRF Security link at runtime).
 */
#include "nrf-psa-crypto-config.h"
#undef PSA_WANT_GENERATE_RANDOM
#undef MBEDTLS_THREADING_C
#undef MBEDTLS_THREADING_ALT

#endif /* ZBOSS_PSA_MAKEFILE_CONFIG_H */
