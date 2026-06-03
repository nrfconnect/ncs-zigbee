/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zigbee/matter_coexistence.h>
#include <zigbee/matter_protocol_state.h>
#include <zigbee/zigbee_app_utils.h>

#include <platform/CHIPDeviceLayer.h>
#include <platform/ThreadStackManager.h>

#include <app/matter_event_handler.h>

#include <net/nrf_802154_callbacks_dispatcher.h>

extern "C" {
#include <zb_nrf_platform.h>
}

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/reboot.h>

LOG_MODULE_REGISTER(zigbee_matter_coexistence, CONFIG_ZIGBEE_MATTER_COEXISTENCE_LOG_LEVEL);

namespace
{

const struct zigbee_matter_coexistence_callbacks *g_cb;

K_SEM_DEFINE(matter_init_done_sem, 0, 1);

atomic_t g_switch_press_active = ATOMIC_INIT(0);

void switch_to_thread_radio(void);
void switch_to_zigbee_radio(void);

void matter_board_init_signal(void)
{
	k_sem_give(&matter_init_done_sem);
}

void protocol_switch_work_handler(struct k_work *work)
{
	ARG_UNUSED(work);

	if (atomic_get(&g_switch_press_active) == 0) {
		return;
	}

	atomic_set(&g_switch_press_active, 0);

	if (protocol_state_get() == PROTOCOL_ZIGBEE) {
		if (zigbee_network_join_commissioning_active()) {
			LOG_WRN("Protocol switch blocked: Zigbee join commissioning in progress");
			return;
		}

		LOG_INF("Switching to Matter");
		switch_to_thread_radio();
		return;
	}

	LOG_INF("Switching to Zigbee, rebooting");
	switch_to_zigbee_radio();
}

K_WORK_DELAYABLE_DEFINE(protocol_switch_work, protocol_switch_work_handler);

void zigbee_thread_fn()
{
	int ret;

	ARG_UNUSED(ret);

	/*
	 * If this device was already commissioned via Matter, a previous boot
	 * stored the persisted protocol state as PROTOCOL_MATTER; we then skip
	 * Zigbee entirely and hand the 802.15.4 radio straight to the Thread
	 * stack so Matter can resume operation.
	 */
	if (protocol_state_get() == PROTOCOL_MATTER) {
		LOG_INF("Persisted protocol is Matter; skipping Zigbee and handing radio to Thread");

		ret = nrf_802154_callbacks_dispatcher_switch("openthread");
		__ASSERT(ret == 0, "Failed to switch 802.15.4 radio to Thread: %d", ret);

		/* Still chain sample button handlers after Matter board init. */
		(void)k_sem_take(&matter_init_done_sem, K_FOREVER);
		if (g_cb->post_matter_board_init != NULL) {
			g_cb->post_matter_board_init();
		}
		return;
	}

	ret = nrf_802154_callbacks_dispatcher_switch("zigbee");
	__ASSERT(ret == 0, "Failed to switch 802.15.4 radio to Zigbee: %d", ret);

	(void)k_sem_take(&matter_init_done_sem, K_FOREVER);

	if (g_cb->post_matter_board_init != NULL) {
		g_cb->post_matter_board_init();
	}

	(void)g_cb->zigbee_start();
}

void matter_thread_fn()
{
	g_cb->matter_start();
}

/* Create threads as dormant, they are started later. */
K_THREAD_DEFINE(zigbee_thread_id, CONFIG_ZIGBEE_MATTER_COEXISTENCE_ZIGBEE_THREAD_STACK_SIZE,
		zigbee_thread_fn, NULL, NULL, NULL,
		CONFIG_ZIGBEE_MATTER_COEXISTENCE_ZIGBEE_THREAD_PRIORITY, 0, K_TICKS_FOREVER);

K_THREAD_DEFINE(matter_thread_id, CONFIG_ZIGBEE_MATTER_COEXISTENCE_MATTER_THREAD_STACK_SIZE,
		matter_thread_fn, NULL, NULL, NULL,
		CONFIG_ZIGBEE_MATTER_COEXISTENCE_MATTER_THREAD_PRIORITY, 0, K_TICKS_FOREVER);

void switch_to_thread_radio(void)
{
	k_thread_abort(zigbee_thread_id);
	zigbee_deinit();

	protocol_state_set(PROTOCOL_MATTER);
	LOG_INF("Protocol switched to Matter");

	int ret = nrf_802154_callbacks_dispatcher_switch("openthread");
	__ASSERT(ret == 0, "Failed to switch 802.15.4 radio to Thread: %d", ret);
}

void switch_to_zigbee_radio(void)
{
	protocol_state_set(PROTOCOL_ZIGBEE);
	LOG_INF("Protocol switched to Zigbee, rebooting");
	sys_reboot(SYS_REBOOT_COLD);
}

void matter_event_handler(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg)
{
	ARG_UNUSED(arg);

	switch (event->Type) {
#ifdef CONFIG_ZIGBEE_MATTER_COEXISTENCE_BT_ADV_WHILE_ZIGBEE
	case chip::DeviceLayer::DeviceEventType::kCHIPoBLEAdvertisingChange:
		if (event->CHIPoBLEAdvertisingChange.Result ==
		    chip::DeviceLayer::kActivity_Started) {
			matter_board_init_signal();
		}
		break;
#endif /* CONFIG_ZIGBEE_MATTER_COEXISTENCE_BT_ADV_WHILE_ZIGBEE */
	case chip::DeviceLayer::DeviceEventType::kSecureSessionEstablished:
		if (protocol_state_get() == PROTOCOL_ZIGBEE &&
		    !chip::DeviceLayer::ThreadStackMgr().IsThreadAttached()) {
			switch_to_thread_radio();
		}
		break;
	case chip::DeviceLayer::DeviceEventType::kFactoryReset: {
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
		 * Reset the persisted protocol to the configured default so the
		 * device reboots in the same state as a factory-fresh unit.
		 * Matter's own factory reset will additionally wipe ZMS (via
		 * CONFIG_CHIP_FACTORY_RESET_ERASE_SETTINGS).
		 */
		protocol_state_set(protocol_state_get_default());
		break;
	}
	default:
		break;
	}
}

} /* namespace */

extern "C" void zigbee_matter_coexistence_signal_matter_board_init(void)
{
	matter_board_init_signal();
}

extern "C" int zigbee_matter_coexistence_run(const struct zigbee_matter_coexistence_callbacks *cb)
{
	__ASSERT(cb != NULL, "Null callback table");
	__ASSERT(cb->matter_start != NULL, "matter_start callback is NULL");
	__ASSERT(cb->zigbee_start != NULL, "zigbee_start callback is NULL");

	g_cb = cb;

	int ps_ret = protocol_state_init();
	__ASSERT(ps_ret == 0, "Failed to initialise protocol state persistence: %d", ps_ret);
	ARG_UNUSED(ps_ret);

	CHIP_ERROR err = Nrf::Matter::RegisterEventHandler(matter_event_handler, 0);
	__ASSERT(err == CHIP_NO_ERROR, "Failed to register Matter event handler: %" CHIP_ERROR_FORMAT,
		 err.Format());
	ARG_UNUSED(err);

	k_thread_start(zigbee_thread_id);
	k_thread_start(matter_thread_id);

	return 0;
}

extern "C" void zigbee_matter_coexistence_process_switch_button(uint32_t button_state,
								uint32_t has_changed,
								uint32_t switch_button)
{
	if (switch_button == 0U) {
		return;
	}

	const bool pressed = (has_changed & switch_button) && (button_state & switch_button);
	const bool released = (has_changed & switch_button) && !(button_state & switch_button);
	const bool press_active = atomic_get(&g_switch_press_active) != 0;

	if (!press_active && pressed) {
		atomic_set(&g_switch_press_active, 1);
		LOG_INF("Protocol switch press started (mask 0x%08x)", switch_button);
		k_work_reschedule(&protocol_switch_work,
				  K_SECONDS(CONFIG_ZIGBEE_MATTER_COEXISTENCE_SWITCH_BUTTON_PRESS_TIME_SECONDS));
	} else if (press_active && released) {
		LOG_INF("Protocol switch press canceled before timeout");
		atomic_set(&g_switch_press_active, 0);
		(void)k_work_cancel_delayable(&protocol_switch_work);
	}
}
