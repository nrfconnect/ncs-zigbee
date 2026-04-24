/*
 * Copyright (c) 2024-2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *
 * @brief Matter + Zigbee light bulb entry point.
 */

#include "app_task_zigbee.h"

#if defined(CONFIG_CHIP) && !defined(CONFIG_ZIGBEE_MATTER_COEXISTENCE)
#error "Combined Matter + Zigbee build requires CONFIG_ZIGBEE_MATTER_COEXISTENCE."
#endif

#ifdef CONFIG_ZIGBEE_MATTER_COEXISTENCE

#include "app_task_matter.h"

#include <zigbee/matter_coexistence.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, CONFIG_CHIP_APP_LOG_LEVEL);

namespace
{

void matter_start_cb(void)
{
	AppTask::Instance().StartApp();
}

const struct zigbee_matter_coexistence_callbacks coexistence_cb = {
	matter_start_cb,
	ZigbeeStart,
	zb_register_button_handler,
};

} /* namespace */

#endif /* CONFIG_ZIGBEE_MATTER_COEXISTENCE */

int main(void)
{
#ifdef CONFIG_ZIGBEE_MATTER_COEXISTENCE
	return zigbee_matter_coexistence_run(&coexistence_cb);
#else
	return ZigbeeStart();
#endif
}
