/*
 * Copyright (c) 2024-2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *
 * @brief Matter + Zigbee light bulb (802.15.4 dispatcher hands radio to Zigbee or Thread).
 */

#include "app_task_zigbee.h"

#ifdef CONFIG_CHIP

#include "app_task_matter.h"
#include <app/matter_event_handler.h>
#include <nrf_802154_callbacks_dispatcher.h>
#include <openthread.h>
#include <zigbee/matter_protocol_state.h>

extern "C" {
#include <zb_nrf_platform.h>
}

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, CONFIG_CHIP_APP_LOG_LEVEL);

#define ZIGBEE_THREAD_STACK_SIZE 2048
#define ZIGBEE_THREAD_PRIORITY	 3
#define MATTER_THREAD_STACK_SIZE 3072
#define MATTER_THREAD_PRIORITY	 3

K_SEM_DEFINE(matter_init_done_sem, 0, 1);

namespace
{

void zigbee_thread_fn()
{
	/*
	 * Load the persisted protocol before claiming the radio. If this device
	 * was already commissioned via Matter, a previous boot stored
	 * PROTOCOL_MATTER; we then skip Zigbee entirely and hand the 802.15.4
	 * radio straight to the Thread stack so Matter can resume operation.
	 */
	(void)protocol_state_init();

	if (protocol_state_get() == PROTOCOL_MATTER) {
		LOG_INF("Persisted protocol is Matter; skipping Zigbee and handing radio to Thread");

		int ret = nrf_802154_callbacks_dispatcher_switch("openthread");
		__ASSERT(ret == 0, "Failed to switch 802.15.4 radio to Thread: %d", ret);
		return;
	}

	int ret = nrf_802154_callbacks_dispatcher_switch("zigbee");
	__ASSERT(ret == 0, "Failed to switch 802.15.4 radio to Zigbee: %d", ret);

	/* Block until the Matter board has initialised the DK buttons. */
	(void)k_sem_take(&matter_init_done_sem, K_FOREVER);
	zb_register_button_handler();
	LOG_INF("Zigbee button handler registered");

	ZigbeeStart();
}

void matter_thread_fn()
{
	AppTask::Instance().StartApp();
}

/* Created dormant (delay = K_TICKS_FOREVER). main() starts them explicitly */
K_THREAD_DEFINE(zigbee_thread_id, ZIGBEE_THREAD_STACK_SIZE, zigbee_thread_fn, NULL, NULL, NULL,
		ZIGBEE_THREAD_PRIORITY, 0, K_TICKS_FOREVER);

K_THREAD_DEFINE(matter_thread_id, MATTER_THREAD_STACK_SIZE, matter_thread_fn, NULL, NULL, NULL,
		MATTER_THREAD_PRIORITY, 0, K_TICKS_FOREVER);

void switch_to_thread_radio(void)
{
	k_thread_abort(zigbee_thread_id);
	zigbee_deinit();

	protocol_state_set(PROTOCOL_MATTER);
	LOG_INF("Protocol switched to Matter");

	int ret = nrf_802154_callbacks_dispatcher_switch("openthread");
	__ASSERT(ret == 0, "Failed to switch 802.15.4 radio to Thread: %d", ret);
}

void matter_zigbee_event_handler(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg)
{
	switch (event->Type) {
	case chip::DeviceLayer::DeviceEventType::kCHIPoBLEAdvertisingChange:
		if (event->CHIPoBLEAdvertisingChange.Result == chip::DeviceLayer::kActivity_Started) {
			k_sem_give(&matter_init_done_sem);
		}
		break;
	case chip::DeviceLayer::DeviceEventType::kSecureSessionEstablished:
		if (!chip::DeviceLayer::ThreadStackMgr().IsThreadAttached()) {
			switch_to_thread_radio();
		}
		break;
	case chip::DeviceLayer::DeviceEventType::kFactoryReset:
		/*
		 * Only touch Zigbee storage if the stack was actually brought
		 * up on this boot. When we skipped Zigbee init (persisted
		 * Matter state), calling these would operate on an
		 * uninitialised stack.
		 */
		if (protocol_state_get() == PROTOCOL_ZIGBEE) {
			zigbee_pibcache_pan_id_clear();
#ifdef ZB_USE_NVRAM
			zb_nvram_erase();
#endif
		}
		/*
		 * Reset the persisted protocol so the device reboots as a
		 * fresh, commissioning-ready Zigbee device. Matter's own
		 * factory reset will additionally wipe ZMS (via
		 * CONFIG_CHIP_FACTORY_RESET_ERASE_SETTINGS), which is
		 * consistent with this default.
		 */
		protocol_state_set(PROTOCOL_ZIGBEE);
		break;
	default:
		break;
	}
}
} /* namespace */

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
