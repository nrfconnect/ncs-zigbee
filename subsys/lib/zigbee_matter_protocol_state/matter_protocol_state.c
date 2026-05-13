/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zigbee/matter_protocol_state.h>
#include <zigbee/zigbee_settings_subsys.h>

#include <stdint.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/atomic.h>

LOG_MODULE_REGISTER(matter_protocol_state, CONFIG_ZIGBEE_MATTER_PROTOCOL_STATE_LOG_LEVEL);

static atomic_t active_protocol = ATOMIC_INIT(PROTOCOL_ZIGBEE);
static atomic_t state_initialized = ATOMIC_INIT(0);
static K_MUTEX_DEFINE(init_mutex);

static int protocol_state_load_direct_cb(const char *key, size_t len,
					 settings_read_cb read_cb, void *cb_arg,
					 void *param)
{
	const char *next;
	uint8_t value;
	ssize_t rc;

	ARG_UNUSED(param);

	if (!settings_name_steq(key, ZIGBEE_SETTINGS_KEY_MATTER_PROTOCOL_STATE, &next) || next) {
		return 0;
	}

	if (len != sizeof(value)) {
		LOG_WRN("Unexpected persisted protocol state size: %zu", len);
		return 0;
	}

	rc = read_cb(cb_arg, &value, sizeof(value));
	if (rc < 0) {
		LOG_ERR("Failed to read persisted protocol state: %d", (int)rc);
		return 0;
	}

	if (value != PROTOCOL_ZIGBEE && value != PROTOCOL_MATTER) {
		LOG_WRN("Discarding invalid persisted protocol value: %u", value);
		return 0;
	}

	atomic_set(&active_protocol, (atomic_val_t)value);
	return 0;
}

int protocol_state_init(void)
{
	if (atomic_get(&state_initialized)) {
		return 0;
	}

	k_mutex_lock(&init_mutex, K_FOREVER);

	if (atomic_get(&state_initialized)) {
		k_mutex_unlock(&init_mutex);
		return 0;
	}

	int rc = settings_subsys_init();

	if (rc) {
		LOG_ERR("settings_subsys_init failed: %d", rc);
		k_mutex_unlock(&init_mutex);
		return rc;
	}

	(void)settings_load_subtree_direct(ZIGBEE_SETTINGS_SUBSYS_NAME,
					   protocol_state_load_direct_cb, NULL);

	atomic_set(&state_initialized, 1);

	LOG_INF("Boot protocol: %s",
		atomic_get(&active_protocol) == PROTOCOL_MATTER ? "matter" : "zigbee");

	k_mutex_unlock(&init_mutex);
	return 0;
}

active_protocol_t protocol_state_get(void)
{
	return (active_protocol_t)atomic_get(&active_protocol);
}

void protocol_state_set(active_protocol_t protocol)
{
	atomic_val_t previous = atomic_set(&active_protocol, (atomic_val_t)protocol);

	if (previous == (atomic_val_t)protocol) {
		/* No change: avoid an unnecessary flash write. */
		return;
	}

	if (!atomic_get(&state_initialized)) {
		/* A save before init would not reach the backend reliably.
		 * The only legitimate callers run after protocol_state_init(),
		 * so treat this as a programming error but keep the RAM state.
		 */
		LOG_WRN("protocol_state_set called before init; state not persisted");
		return;
	}

	uint8_t value = (uint8_t)protocol;
	int rc = settings_save_one(ZIGBEE_SETTINGS_FULL_NAME_MATTER_PROTOCOL_STATE, &value, sizeof(value));

	if (rc) {
		LOG_ERR("Failed to persist protocol state: %d", rc);
	}
}
