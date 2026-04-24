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

#ifdef CONFIG_ZIGBEE_MATTER_COEXISTENCE
/**
 * @brief Button handler for Zigbee in combined Matter+Zigbee builds.
 *
 * Processes button events when Zigbee is the active protocol. Returns
 * immediately when Matter is active so Matter's own handlers (registered
 * by Board::Init() during AppTask initialisation) take full ownership.
 *
 * @param button_state  Bitmask containing buttons state.
 * @param has_changed   Bitmask containing buttons that changed state.
 */
void zb_button_handler(uint32_t button_state, uint32_t has_changed);

/**
 * @brief Register the Zigbee button handler with the DK library.
 *
 * Must be called after Matter's Board::Init() (which runs dk_buttons_init())
 * so that dk_button_handler_add() finds an initialised subsystem.
 */
void zb_register_button_handler(void);
#endif /* CONFIG_ZIGBEE_MATTER_COEXISTENCE */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_ZIGBEE_H */
