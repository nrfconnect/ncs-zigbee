/*
 * Copyright (c) 2026 Nordic Semiconductor
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 *
 * ZBOSS 5 base vendor configuration for Nordic NCS platform builds.
 */

#ifndef ZB_VENDOR_CFG_ZOI_BASE_H
#define ZB_VENDOR_CFG_ZOI_BASE_H 1

#define ZB_LIMIT_VISIBILITY

/* ZB_USE_SLEEP: set from CONFIG_ZB_USE_SLEEP in zb_vendor.h (respects Kconfig). */
#define APS_FRAGMENTATION
#define ZB_ALL_DEVICE_SUPPORT
#define ZB_PRODUCTION_CONFIG
#define ZB_SECURITY_INSTALLCODES
#define ZB_MAC_CONFIGURABLE_TX_POWER
#define ZB_APS_USER_PAYLOAD
#define ZB_USE_OSIF_OTA_ROUTINES

#define ZB_RESET_AUTORESTART
#define ZB_REDUCE_NWK_LOAD_ON_LOW_MEMORY

#ifdef ZB_CONFIG_DEFAULT_KERNEL_DEFINITION

#ifndef ZB_ED_ROLE
#define ZB_CONFIG_ROLE_ZC
#else
#define ZB_CONFIG_ROLE_ZED
#endif

#define ZB_CONFIG_OVERALL_NETWORK_SIZE 128
#define ZB_CONFIG_HIGH_TRAFFIC
#define ZB_CONFIG_APPLICATION_COMPLEX

#endif /* ZB_CONFIG_DEFAULT_KERNEL_DEFINITION */

#ifdef DEBUG
#ifndef ZB_NCS_NO_DEBUG_BUFFERS
#define ZB_DEBUG_BUFFERS
#endif
#define ZB_TRAFFIC_DUMP_ON
#define ZB_CHECK_OOM_STATUS

#if !defined(USE_ASSERT)
#define USE_ASSERT
#endif
#endif /* DEBUG */

#endif /* ZB_VENDOR_CFG_ZOI_BASE_H */
