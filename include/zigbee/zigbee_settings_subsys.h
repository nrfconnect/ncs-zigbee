/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file zigbee_settings_subsys.h
 *  @brief Zephyr Settings keys shared by Zigbee add-on libraries.
 */

#ifndef ZIGBEE_SETTINGS_SUBSYS_H__
#define ZIGBEE_SETTINGS_SUBSYS_H__

/** Handler subtree for persisted Zigbee add-on data. */
#define ZIGBEE_SETTINGS_SUBSYS_NAME "zb"

/* Leaf segments under the subtree above. */
#define ZIGBEE_SETTINGS_KEY_FOTA_RESUME "fr"
#define ZIGBEE_SETTINGS_KEY_SCENES_TABLE "st"

#define ZIGBEE_SETTINGS_FULL_NAME_FOTA_RESUME \
	ZIGBEE_SETTINGS_SUBSYS_NAME "/" ZIGBEE_SETTINGS_KEY_FOTA_RESUME
#define ZIGBEE_SETTINGS_FULL_NAME_SCENES_TABLE \
	ZIGBEE_SETTINGS_SUBSYS_NAME "/" ZIGBEE_SETTINGS_KEY_SCENES_TABLE

#endif /* ZIGBEE_SETTINGS_SUBSYS_H__ */
