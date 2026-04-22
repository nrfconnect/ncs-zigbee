/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef APP_TASK_ZIGBEE_H
#define APP_TASK_ZIGBEE_H 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int ZigbeeStart(void);

#ifdef CONFIG_CHIP
/**
 * @brief Button handler for Zigbee in combined Matter+Zigbee builds.
 *
 * Processes button events when Zigbee protocol is active.
 * Checks protocol state and returns early if Matter is active.
 *
 * @param button_state  Bitmask containing buttons state.
 * @param has_changed   Bitmask containing buttons that changed state.
 */
void zb_button_handler(uint32_t button_state, uint32_t has_changed);

/**
 * @brief Register the Zigbee button handler with DK library.
 *
 * Must be called after Matter's Board::Init() to chain the Zigbee handler.
 */
void zb_register_button_handler(void);
#endif /* CONFIG_CHIP */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_ZIGBEE_H */
