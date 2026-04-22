/*
 * Copyright (c) 2024-2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef PROTOCOL_STATE_H
#define PROTOCOL_STATE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	PROTOCOL_ZIGBEE,
	PROTOCOL_MATTER
} active_protocol_t;

/**
 * @brief Load the persisted protocol state from non-volatile storage.
 *
 * Must be called once before the protocol dispatch logic runs. Safe to call
 * from multiple contexts; internal state is guarded and the underlying
 * settings subsystem is initialized at most once.
 *
 * @retval 0 on success (state loaded or default applied)
 * @retval <0 negative errno on unrecoverable storage failure
 */
int protocol_state_init(void);

/**
 * @brief Get the currently active protocol.
 * @return PROTOCOL_ZIGBEE or PROTOCOL_MATTER
 */
active_protocol_t protocol_state_get(void);

/**
 * @brief Set and persist the active protocol.
 *
 * Updates the in-RAM state and writes it to non-volatile storage so that the
 * application resumes with the same protocol after a reboot.
 *
 * @param protocol The protocol to activate.
 */
void protocol_state_set(active_protocol_t protocol);

/**
 * @brief Check if Zigbee is the active protocol.
 * @return true if Zigbee is active, false otherwise.
 */
static inline bool protocol_is_zigbee_active(void)
{
	return protocol_state_get() == PROTOCOL_ZIGBEE;
}

/**
 * @brief Check if Matter is the active protocol.
 * @return true if Matter is active, false otherwise.
 */
static inline bool protocol_is_matter_active(void)
{
	return protocol_state_get() == PROTOCOL_MATTER;
}

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_STATE_H */
