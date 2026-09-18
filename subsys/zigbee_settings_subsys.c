/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zigbee/zigbee_settings_subsys.h>

#if defined(CONFIG_SETTINGS)

#include <zephyr/settings/settings.h>

int zigbee_settings_subsys_init(void)
{
	static bool initialized;
	int err;

	if (initialized) {
		return 0;
	}

	err = settings_subsys_init();
	if (err != 0) {
		return err;
	}

	err = settings_load();
	if (err == 0) {
		initialized = true;
	}

	return err;
}

#endif /* CONFIG_SETTINGS */
