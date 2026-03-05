/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifdef CONFIG_CHIP

#include <DeviceInfoProviderImpl.h>
#include <app/clusters/network-commissioning/network-commissioning.h>
#include <app/server/Server.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <data-model-providers/codegen/Instance.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/OpenThread/GenericNetworkCommissioningThreadDriver.h>
#include <platform/nrfconnect/DeviceInstanceInfoProviderImpl.h>
#include <setup_payload/OnboardingCodesUtil.h>

#ifdef CONFIG_CHIP_CRYPTO_PSA
#include <crypto/PSAOperationalKeystore.h>
#endif

LOG_MODULE_REGISTER(matter_init, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace ::chip;
using namespace ::chip::DeviceLayer;
using namespace ::chip::Credentials;

chip::app::Clusters::NetworkCommissioning::InstanceAndDriver<
    NetworkCommissioning::GenericThreadDriver>
    sThreadNetworkDriver(0 /*endpointId*/);
static chip::CommonCaseDeviceServerInitParams sServerInitParamsDefault;
static chip::DeviceLayer::DeviceInfoProviderImpl sDeviceInfoProviderDefault;

#ifdef CONFIG_CHIP_CRYPTO_PSA
static chip::Crypto::PSAOperationalKeystore sOperationalKeystoreDefault;
#endif

CHIP_ERROR configure_thread_role() {
  using ThreadRole = ConnectivityManager::ThreadDeviceType;

  // Assuming FTD role for the light bulb sample
  ThreadRole threadRole{ThreadRole::kThreadDeviceType_Router};

  return ConnectivityMgr().SetThreadDeviceType(threadRole);
}

void lock_open_thread_task(void) {
  chip::DeviceLayer::ThreadStackMgr().LockThreadStack();
}

void unlock_open_thread_task(void) {
  chip::DeviceLayer::ThreadStackMgr().UnlockThreadStack();
}

/**
 * @brief Initialize Matter stack for combined Zigbee+Matter application
 *
 * Simplified initialization for memory estimation purposes.
 * Full Zigbee/Matter integration would require additional synchronization.
 */
extern "C" int matter_init(void) {
  CHIP_ERROR err = CHIP_NO_ERROR;

  LOG_INF("Initializing Matter stack (simplified for memory estimation)");

  // Initialize Matter platform
  err = PlatformMgr().InitChipStack();
  if (err != CHIP_NO_ERROR) {
    LOG_ERR("PlatformMgr().InitChipStack() failed: %s", ErrorStr(err));
    return -ENODEV;
  }

  err = ThreadStackMgr().InitThreadStack();
  if (err != CHIP_NO_ERROR) {
    LOG_ERR("ThreadStackMgr().InitThreadStack() failed: %s", ErrorStr(err));
    return -ENODEV;
  }

  err = configure_thread_role();
  if (err != CHIP_NO_ERROR) {
    LOG_ERR("configure_thread_role() failed: %s", ErrorStr(err));
    return -ENODEV;
  }

  sThreadNetworkDriver.Init();

  // Initialize device attestation credentials
  SetDeviceInstanceInfoProvider(&DeviceInstanceInfoProviderMgrImpl());
  SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());

  chip::Inet::EndPointStateOpenThread::OpenThreadEndpointInitParam nativeParams;
  nativeParams.lockCb = lock_open_thread_task;
  nativeParams.unlockCb = unlock_open_thread_task;
  nativeParams.openThreadInstancePtr =
      chip::DeviceLayer::ThreadStackMgrImpl().OTInstance();
  sServerInitParamsDefault.endpointNativeParams =
      static_cast<void *>(&nativeParams);

#ifdef CONFIG_CHIP_CRYPTO_PSA
  sServerInitParamsDefault.operationalKeystore = &sOperationalKeystoreDefault;
#endif

  sServerInitParamsDefault.InitializeStaticResourcesBeforeServerInit();

  SetDeviceInfoProvider(&sDeviceInfoProviderDefault);

  sServerInitParamsDefault.dataModelProvider =
      chip::app::CodegenDataModelProviderInstance(
          sServerInitParamsDefault.persistentStorageDelegate);

  err = Server::GetInstance().Init(sServerInitParamsDefault);
  if (err != CHIP_NO_ERROR) {
    LOG_ERR("Server::GetInstance().Init() failed: %s", ErrorStr(err));
    return -ENODEV;
  }

  ConfigurationMgr().LogDeviceConfig();
  PrintOnboardingCodes(
      RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));

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
extern "C" void matter_zigbee_light_sync(bool on_off, uint8_t level) {
  // In a full implementation, this would update Matter attributes
  // For now, just log for memory estimation purposes
  LOG_DBG("Light sync: on_off=%d, level=%d", on_off, level);
}

#endif /* CONFIG_CHIP */

/**
 * @brief Update Matter on/off attribute from Zigbee state
 */
extern "C" void matter_update_onoff(bool on) {
  LOG_DBG("Updating Matter on/off to %d", on);
  // For memory estimation, we skip the actual cluster update
  // In full implementation:
  // chip::app::Clusters::OnOff::Attributes::OnOff::Set(MATTER_LIGHT_ENDPOINT,
  // on);
}

/**
 * @brief Update Matter level control attribute from Zigbee state
 */
extern "C" void matter_update_level(uint8_t level) {
  LOG_DBG("Updating Matter level to %d", level);
  // For memory estimation, we skip the actual cluster update
  // In full implementation:
  // chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Set(MATTER_LIGHT_ENDPOINT,
  // level);
}
