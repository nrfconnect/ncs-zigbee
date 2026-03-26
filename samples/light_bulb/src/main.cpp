/*
 * Copyright (c) 2024-2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *
 * @brief Simple Matter + Zigbee light bulb implementation.
 */

#include "app_task_zigbee.h"

#ifdef CONFIG_CHIP

#include "app_task_matter.h"
#include <app/matter_event_handler.h>
#include <nrf_802154_callbacks_dispatcher.h>
#include <openthread.h>

extern "C" {
#include <zb_nrf_platform.h>
}

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, CONFIG_CHIP_APP_LOG_LEVEL);

/* Zephyr thread stack size and priority */
#define ZIGBEE_THREAD_STACK_SIZE 2048
#define ZIGBEE_THREAD_PRIORITY	 3
#define MATTER_THREAD_STACK_SIZE 3072
#define MATTER_THREAD_PRIORITY	 3

namespace
{

void zigbee_thread_fn()
{
	int ret = nrf_802154_callbacks_dispatcher_switch("zigbee");

  __ASSERT(ret == 0, "Failed to switch 802.15.4 radio to Zigbee: %d", ret);

	ZigbeeStart();
}
void matter_thread_fn()
{
	AppTask::Instance().StartApp();
}

K_THREAD_DEFINE(zigbee_thread_id, ZIGBEE_THREAD_STACK_SIZE, zigbee_thread_fn,
		NULL, NULL, NULL, ZIGBEE_THREAD_PRIORITY, 0, 0);

K_THREAD_DEFINE(matter_thread_id, MATTER_THREAD_STACK_SIZE, matter_thread_fn,
		NULL, NULL, NULL, MATTER_THREAD_PRIORITY, 0, 0);

void switch_to_thread_radio(void)
{
	k_thread_abort(zigbee_thread_id);
	zigbee_deinit();
	int ret = nrf_802154_callbacks_dispatcher_switch("openthread");

  __ASSERT(ret == 0, "Failed to switch 802.15.4 radio to Thread: %d", ret);
}

void matter_zigbee_event_handler(
	const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg)
{
	switch (event->Type) {
	case chip::DeviceLayer::DeviceEventType::kSecureSessionEstablished:
		/* Switch to Thread Radio and disable Zigbee Stack*/
		if (!chip::DeviceLayer::ThreadStackMgr().IsThreadAttached()) {
			switch_to_thread_radio();
		}
		break;
	case chip::DeviceLayer::DeviceEventType::kFactoryReset:
		/* Remove also the zigbee stack data */
		zigbee_pibcache_pan_id_clear();
#ifdef ZB_USE_NVRAM
		zb_nvram_erase();
#endif
		break;
	default:
		break;
	}
}
} // namespace

#endif /* CONFIG_CHIP */

int main(void)
{

#ifdef CONFIG_CHIP

	Nrf::Matter::RegisterEventHandler(matter_zigbee_event_handler, 0);

	k_thread_start(zigbee_thread_id);
	k_thread_start(matter_thread_id);

#else

	ZigbeeStart();

#endif

	return 0;
}
