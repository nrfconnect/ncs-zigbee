/*
 * Copyright (c) 2024-2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *
 * @brief Zigbee light bulb entry point.
 */

#include "app_task_zigbee.h"

int main(void)
{
	return ZigbeeStart();
}
