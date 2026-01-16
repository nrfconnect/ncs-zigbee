/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifdef CONFIG_CHIP

#include <app/server/Server.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <platform/CHIPDeviceLayer.h>

LOG_MODULE_REGISTER(matter_init, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace ::chip;
using namespace ::chip::DeviceLayer;
using namespace ::chip::Credentials;

/**
 * @brief Initialize Matter stack for combined Zigbee+Matter application
 * 
 * Simplified initialization for memory estimation purposes.
 * Full Zigbee/Matter integration would require additional synchronization.
 */
extern "C" int matter_init(void)
{
	CHIP_ERROR err = CHIP_NO_ERROR;

	LOG_INF("Initializing Matter stack (simplified for memory estimation)");

	// Initialize Matter platform
	err = PlatformMgr().InitChipStack();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("PlatformMgr().InitChipStack() failed: %s", ErrorStr(err));
		return -ENODEV;
	}

	// Initialize device attestation credentials
	SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());

	// Start Matter event loop
	err = PlatformMgr().StartEventLoopTask();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("PlatformMgr().StartEventLoopTask() failed: %s", ErrorStr(err));
		return -ENODEV;
	}

	LOG_INF("Matter stack initialized successfully");

	return 0;
}

/**
 * @brief Synchronize Zigbee and Matter light states
 * 
 * Simplified version for memory estimation.
 */
extern "C" void matter_zigbee_light_sync(bool on_off, uint8_t level)
{
	// In a full implementation, this would update Matter attributes
	// For now, just log for memory estimation purposes
	LOG_DBG("Light sync: on_off=%d, level=%d", on_off, level);
}

#endif /* CONFIG_CHIP */

/**
 * @brief Update Matter on/off attribute from Zigbee state
 */
extern "C" void matter_update_onoff(bool on)
{
	LOG_DBG("Updating Matter on/off to %d", on);
	// For memory estimation, we skip the actual cluster update
	// In full implementation: chip::app::Clusters::OnOff::Attributes::OnOff::Set(MATTER_LIGHT_ENDPOINT, on);
}

/**
 * @brief Update Matter level control attribute from Zigbee state
 */
extern "C" void matter_update_level(uint8_t level)
{
	LOG_DBG("Updating Matter level to %d", level);
	// For memory estimation, we skip the actual cluster update
	// In full implementation: chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Set(MATTER_LIGHT_ENDPOINT, level);
}

