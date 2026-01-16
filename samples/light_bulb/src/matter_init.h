/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MATTER_INIT_H
#define MATTER_INIT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize Matter stack
 * 
 * @return 0 on success, negative errno on failure
 */
int matter_init(void);

/**
 * @brief Update Matter on/off attribute
 * 
 * @param on true for on, false for off
 */
void matter_update_onoff(bool on);

/**
 * @brief Update Matter level control attribute
 * 
 * @param level Level value (0-255)
 */
void matter_update_level(uint8_t level);

#ifdef __cplusplus
}
#endif

#endif /* MATTER_INIT_H */
