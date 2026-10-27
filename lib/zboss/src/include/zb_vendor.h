/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2026 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ZB_VENDOR_H__
#define ZB_VENDOR_H__

/* NCS Zephyr osif (zb_nrf_transceiver.c) uses 2-arg zb_buf_* macros. */
#define ZB_NCS_NO_DEBUG_BUFFERS 1

#include "zb_vendor_cfg_zoi_base.h"

/**
 * Include additional configuration file, which usually contains the set of
 * symbols defined by ZBOSS source code Kconfig, which can be found in NCS
 * platform repository.
 */
#ifdef LIBZBOSS_CONFIG_FILE
#include LIBZBOSS_CONFIG_FILE
#endif

#ifdef CONFIG_ZB_PSA_CRYPTO
#include "zb_cfg_psa.h"
/* Nordic entropy driver; app PSA config omits PSA_WANT_GENERATE_RANDOM. */
#define ZB_RANDOM_HARDWARE
#endif


#include "zb_config_platform.h"
#include "zb_version.h"
#include "zb_revision_r22.h"

#ifdef CONFIG_ZB_NCP_MODE
#define NCP_MODE
#endif

#ifdef CONFIG_ZB_NCP_TRANSPORT_TYPE_SERIAL
#define ZB_NCP_TRANSPORT_TYPE_SERIAL
#endif

#ifdef CONFIG_ZB_NCP_TRANSPORT_TYPE_SPI
#define ZB_NCP_TRANSPORT_TYPE_SPI
#endif

#ifdef CONFIG_ZB_NCP_TRANSPORT_TYPE_NSNG
#define ZB_NCP_TRANSPORT_TYPE_NSNG
#endif

#ifdef CONFIG_ZB_HAVE_SERIAL
#define ZB_HAVE_SERIAL
#define ZB_HAVE_ASYNC_SERIAL
#define ZB_HAVE_SERIAL_SINGLE
#ifndef ZB_TRACE_SERIAL_BAUDRATE
#define ZB_TRACE_SERIAL_BAUDRATE 115200U
#endif
#endif

#ifdef CONFIG_ZB_SDK_TYPE
#define ZBOSS_SDK_TYPE (CONFIG_ZB_SDK_TYPE - 1)
#endif

#ifdef CONFIG_ZB_TRACE_LOG_LEVEL
#define ZB_TRACE_LEVEL CONFIG_ZB_TRACE_LOG_LEVEL
#endif

#ifdef CONFIG_ZB_TRACE_MASK
#define ZB_TRACE_MASK CONFIG_ZB_TRACE_MASK
#endif

#ifdef CONFIG_ZB_NVRAM_ENABLE_VER_MIGRATION
#define ZB_NVRAM_ENABLE_VER_MIGRATION
#else
#define ZB_NO_NVRAM_VER_MIGRATION
#endif

#ifdef CONFIG_ZB_BDB_ENABLE_FINDING_BINDING
#define ZB_BDB_ENABLE_FINDING_BINDING
#endif

#ifdef CONFIG_ZB_DISTRIBUTED_SECURITY_ON
#define ZB_DISTRIBUTED_SECURITY_ON
#endif

#ifdef CONFIG_ZB_SECURITY_INSTALLCODES
#define ZB_SECURITY_INSTALLCODES
#endif

#ifdef CONFIG_ZB_PRODUCTION_CONFIG
#define ZB_PRODUCTION_CONFIG
#endif

#ifdef CONFIG_ZB_APS_USER_PAYLOAD
#define ZB_APS_USER_PAYLOAD
#endif

#ifdef CONFIG_ZB_LOW_SECURITY_MODE
#define ZB_LOW_SECURITY_MODE
#endif

#ifdef CONFIG_ZB_APS_FRAGMENTATION
#define APS_FRAGMENTATION
#endif

#ifdef CONFIG_ZB_NWK_BLACKLIST
#define ZB_NWK_BLACKLIST

#ifdef CONFIG_ZB_NWK_BLACKLIST_SIZE
#define ZB_NWK_BLACKLIST_SIZE CONFIG_ZB_NWK_BLACKLIST_SIZE
#endif

#endif /* CONFIG_ZB_NWK_BLACKLIST */

#ifdef CONFIG_ZB_CONTROL4_NETWORK_SUPPORT
#define ZB_CONTROL4_NETWORK_SUPPORT
#endif

#ifdef CONFIG_ZB_R22_MULTIMAC_MODE
#define ZB_R22_MULTIMAC_MODE
#endif

#ifdef CONFIG_ZB_MAC_INTERFACE_SINGLE
#define ZB_MAC_INTERFACE_SINGLE
#endif

#ifdef CONFIG_ZB_MAC_TESTING_MODE
#define ZB_MAC_TESTING_MODE
#define ZB_MAC_API_TRACE_PRIMITIVES
#endif

#ifdef CONFIG_ZB_MAC_DUTY_CYCLE_MONITORING
#define ZB_MAC_DUTY_CYCLE_MONITORING
#endif

#ifdef CONFIG_ZB_MAC_POWER_CONTROL
#define ZB_MAC_POWER_CONTROL
#endif

#ifdef CONFIG_ZB_MAC_CONFIGURABLE_TX_POWER
#define ZB_MAC_CONFIGURABLE_TX_POWER
#endif

#ifdef CONFIG_ZB_MAC_QUEUE_SIZE
#define ZB_MAC_QUEUE_SIZE CONFIG_ZB_MAC_QUEUE_SIZE
#endif

#ifdef CONFIG_ZB_NWK_CHANNEL_ACCEPT_LEVEL
#define ZB_NWK_CHANNEL_ACCEPT_LEVEL CONFIG_ZB_NWK_CHANNEL_ACCEPT_LEVEL
#endif

#ifdef CONFIG_ZB_FILTER_OUT_CLUSTERS
#define ZB_FILTER_OUT_CLUSTERS
#endif

#ifdef CONFIG_ZB_ENHANCED_BEACON_SUPPORT
#define ZB_ENHANCED_BEACON_SUPPORT
#endif

#ifdef CONFIG_ZB_JOINING_LIST_SUPPORT
#define ZB_JOINING_LIST_SUPPORT
#endif

#ifdef CONFIG_ZB_ZCL_SUPPORT_CLUSTER_WWAH
#define ZB_ZCL_SUPPORT_CLUSTER_WWAH

#ifdef CONFIG_ZB_ZCL_ENABLE_WWAH_CLIENT
#define ZB_ZCL_ENABLE_WWAH_CLIENT
#endif

#ifdef CONFIG_ZB_ZCL_ENABLE_WWAH_SERVER
#define ZB_ZCL_ENABLE_WWAH_SERVER

#ifdef CONFIG_ZB_NWK_RETRY_COUNT
#define ZB_NWK_RETRY_COUNT
#endif

#ifdef CONFIG_ZB_BEACON_SURVEY
#define ZB_BEACON_SURVEY
#endif

#ifdef CONFIG_ZB_PARENT_CLASSIFICATION
#define ZB_PARENT_CLASSIFICATION
#endif

#elif defined(CONFIG_ZB_NWK_RETRY_COUNT) || defined(CONFIG_ZB_BEACON_SURVEY) || defined(CONFIG_ZB_PARENT_CLASSIFICATION)
#error ZBOSS configuration mismatch: You have to enable WWAHu cluster server implementation to use additional WWAHu features.

#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */

#endif /* CONFIG_ZB_ZCL_SUPPORT_CLUSTER_WWAH */

#ifdef CONFIG_ZB_CHECK_OOM_STATUS
#define ZB_CHECK_OOM_STATUS
#endif

#ifdef CONFIG_ZB_SEND_OOM_STATUS
#define ZB_SEND_OOM_STATUS
#endif

#ifdef CONFIG_ZB_PROMISCUOUS_MODE
#define ZB_PROMISCUOUS_MODE
#endif

#ifdef CONFIG_ZB_CERTIFICATION_HACKS
#define ZB_CERTIFICATION_HACKS
#endif

#ifdef CONFIG_ZB_TEST_PROFILE
#define ZB_TEST_PROFILE
#endif

#ifdef CONFIG_ZB_MULTITEST_CONSOLE_SLEEP_TIMEOUT
#define ZB_MULTITEST_CONSOLE_SLEEP_TIMEOUT CONFIG_ZB_MULTITEST_CONSOLE_SLEEP_TIMEOUT
#endif

#ifdef CONFIG_ZB_DONT_NEED_TRACE_FILE_ID
#define ZB_DONT_NEED_TRACE_FILE_ID
#endif

#ifdef CONFIG_ZB_MAC_AUTO_DELAY_IN_MAC_GP_SEND
#define MAC_AUTO_DELAY_IN_MAC_GP_SEND
#endif

#ifdef CONFIG_ZB_USEALIAS
#define ZB_USEALIAS
#endif

#ifdef CONFIG_ZB_LIMIT_VISIBILITY
#define ZB_LIMIT_VISIBILITY
#endif

#ifdef CONFIG_ZB_DISABLE_AUTO_REBOOT_FOR_CI
#define DISABLE_AUTO_REBOOT_FOR_CI
#endif

#ifdef CONFIG_ZB_BDB_TOUCHLINK
#define ZB_ENABLE_INTER_PAN_EXCHANGE
#define ZB_ENABLE_ZLL
#define ZB_BDB_TOUCHLINK
#endif

#ifdef CONFIG_ZB_ENABLE_INTER_PAN_NON_DEFAULT_CHANNEL
#define ZB_ENABLE_INTER_PAN_NON_DEFAULT_CHANNEL
#endif

#ifdef CONFIG_ZB_CONFIGURABLE_MEM
#define ZB_CONFIGURABLE_MEM
#endif

/* Default memory storage configuration, used while dynamic memory configuration features is enabled. */
#ifdef ZB_CONFIG_DEFAULT_KERNEL_DEFINITION

/* Select memory configuration according to the implemented Zigbee role. */
#ifndef ZB_ED_ROLE
#define ZB_CONFIG_ROLE_ZC
#else
#define ZB_CONFIG_ROLE_ZED
#endif

#ifdef CONFIG_ZB_CONFIG_OVERALL_NETWORK_SIZE
#undef ZB_CONFIG_OVERALL_NETWORK_SIZE
#define ZB_CONFIG_OVERALL_NETWORK_SIZE CONFIG_ZB_CONFIG_OVERALL_NETWORK_SIZE
#endif

#if defined(CONFIG_ZB_CONFIG_HIGH_TRAFFIC) || defined(CONFIG_ZB_CONFIG_MODERATE_TRAFFIC) || \
	defined(CONFIG_ZB_CONFIG_LIGHT_TRAFFIC) || defined(CONFIG_ZB_CONFIG_LARGE_NET_TRAFFIC)
#undef ZB_CONFIG_HIGH_TRAFFIC
#undef ZB_CONFIG_MODERATE_TRAFFIC
#undef ZB_CONFIG_LIGHT_TRAFFIC
#undef ZB_CONFIG_LARGE_NET_TRAFFIC
#endif

#ifdef CONFIG_ZB_CONFIG_HIGH_TRAFFIC
#define ZB_CONFIG_HIGH_TRAFFIC
#endif

#ifdef CONFIG_ZB_CONFIG_MODERATE_TRAFFIC
#define ZB_CONFIG_MODERATE_TRAFFIC
#endif

#ifdef CONFIG_ZB_CONFIG_LIGHT_TRAFFIC
#define ZB_CONFIG_LIGHT_TRAFFIC
#endif

#ifdef CONFIG_ZB_CONFIG_LARGE_NET_TRAFFIC
#define ZB_CONFIG_LARGE_NET_TRAFFIC
#endif

#if defined(CONFIG_ZB_CONFIG_APPLICATION_COMPLEX) || defined(CONFIG_ZB_CONFIG_APPLICATION_MODERATE) || \
	defined(CONFIG_ZB_CONFIG_APPLICATION_SIMPLE) || defined(CONFIG_ZB_CONFIG_APPLICATION_LARGE_NET)
#undef ZB_CONFIG_APPLICATION_COMPLEX
#undef ZB_CONFIG_APPLICATION_MODERATE
#undef ZB_CONFIG_APPLICATION_SIMPLE
#undef ZB_CONFIG_APPLICATION_LARGE_NET
#endif

#ifdef CONFIG_ZB_CONFIG_APPLICATION_COMPLEX
#define ZB_CONFIG_APPLICATION_COMPLEX
#endif

#ifdef CONFIG_ZB_CONFIG_APPLICATION_MODERATE
#define ZB_CONFIG_APPLICATION_MODERATE
#endif

#ifdef CONFIG_ZB_CONFIG_APPLICATION_SIMPLE
#define ZB_CONFIG_APPLICATION_SIMPLE
#endif

#ifdef CONFIG_ZB_CONFIG_APPLICATION_LARGE_NET
#define ZB_CONFIG_APPLICATION_LARGE_NET
#endif

#endif /* ZB_CONFIG_DEFAULT_KERNEL_DEFINITION */

/* Compile-time parameters, affecting memory usage in all configurations. */
#define ZB_PANID_TABLE_SIZE              CONFIG_ZB_PANID_TABLE_SIZE
#define ZB_DEV_MANUFACTORER_TABLE_SIZE   CONFIG_ZB_DEV_MANUFACTURER_TABLE_SIZE
#define ZB_BUF_Q_SIZE                    CONFIG_ZB_BUF_Q_SIZE
#define ZDO_TRAN_TABLE_SIZE              CONFIG_ZB_ZDO_TRAN_TABLE_SIZE
#define ZB_APS_ENDPOINTS_IN_GROUP_TABLE  CONFIG_ZB_APS_ENDPOINTS_IN_GROUP_TABLE
#define ZB_NWK_BTR_TABLE_SIZE            CONFIG_ZB_NWK_BTR_TABLE_SIZE
#define ZB_NWK_BRR_TABLE_SIZE            CONFIG_ZB_NWK_BRR_TABLE_SIZE
#define ZB_MAX_EP_NUMBER                 CONFIG_ZB_MAX_EP_NUMBER
#define ZB_APS_GROUP_TABLE_SIZE          CONFIG_ZB_APS_GROUP_TABLE_SIZE
#define ZB_ZGP_SINK_TBL_SIZE             CONFIG_ZB_ZGP_SINK_TBL_SIZE
#define ZB_ZGP_PROXY_TBL_SIZE            CONFIG_ZB_ZGP_PROXY_TBL_SIZE

#ifdef CONFIG_ZB_APS_SRC_BINDING_TABLE_SIZE
#define ZB_APS_SRC_BINDING_TABLE_SIZE CONFIG_ZB_APS_SRC_BINDING_TABLE_SIZE
#endif

#ifdef CONFIG_ZB_APS_DST_BINDING_TABLE_SIZE
#define ZB_APS_DST_BINDING_TABLE_SIZE CONFIG_ZB_APS_DST_BINDING_TABLE_SIZE
#endif

#ifdef CONFIG_ZB_IOBUF_POOL_SIZE
#define ZB_IOBUF_POOL_SIZE CONFIG_ZB_IOBUF_POOL_SIZE
#endif

#ifdef CONFIG_ZB_SCHEDULER_Q_SIZE
#define ZB_SCHEDULER_Q_SIZE CONFIG_ZB_SCHEDULER_Q_SIZE
#endif

#ifndef ZB_CONFIGURABLE_MEM
#define ZB_CHILD_HASH_TABLE_SIZE (((ZB_IEEE_ADDR_TABLE_SIZE + ZB_IEEE_ADDR_TABLE_SIZE / 3) + 31) / 32 * 32)
#endif /* ZB_CONFIGURABLE_MEM */

/* ZBOSS stack options non-configurable via Kconfig. */

/* Include Zigbee BDB implementation (always defined, except macsplit). */
#define ZB_BDB_MODE

#if defined NCP_MODE && !defined NCP_MODE_HOST
/* Enable NCP SoC -specific BDB signal generation and handler. */
#define ZB_NCP_SOC_BDB
/* Include additional API, that allows to write any dataset structure into the ZBOSS NVRAM. */
#define ZB_NVRAM_ENABLE_DIRECT_API
/* Include NCP commands for entering bootloader. */
#define ZB_NCP_ENABLE_OTA_CMD
#endif /* NCP_MODE && !NCP_MODE_HOST */

/* Include Zigbee HA clusters implementation (always defined, except macsplit). */
#define ZB_ENABLE_HA

#ifdef CONFIG_ZB_STACK_REGRESSION_TESTING_API

/* Declarations needed for RTP_SEC_* tests */
#define ZDO_DIAGNOSTICS
#define ZB_HA_DEFINE_DEVICE_ON_OFF_SWITCH
#define ZB_HA_DEFINE_DEVICE_ON_OFF_OUTPUT
#define ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN
#define ZB_ZLL_ADDR_ASSIGN_CAPABLE

#endif /* CONFIG_ZB_STACK_REGRESSION_TESTING_API */

/* HA cluster support is selected in zb_ha_device_config.h via ZB_ALL_DEVICE_SUPPORT. */

/* Include rejoin backoff procedure (always included, except macsplit). */
#define ZB_REJOIN_BACKOFF

/* Enable ZGP profile  (always defined, except macsplit). */
#define ZB_ENABLE_ZGP

/* Enable Green Power Combo implementation. */
#if defined(CONFIG_ZIGBEE_GP_CB) && !defined(ZB_ENABLE_ZGP_COMBO)
#define ZB_ENABLE_ZGP_COMBO
#endif

/* Enable sleepy behaviour on all devices. The sleep signal will allow other, lower-priority task to take over. */
#ifdef CONFIG_ZB_USE_SLEEP
#define ZB_USE_SLEEP
#else
/* Suppress the End Device sleep default in zb_config.h so that Kconfig stays
 * authoritative for ZB_ED_ROLE builds too. */
#define ZB_NO_USE_SLEEP
#endif

/* Enter TX mode directly from radio sleep sate. */
#define ZB_TRANSCEIVER_ON_BEFORE_TX  0

/* Start Energy Detection from radio sleep state. */
#define ZB_TRANSCEIVER_ON_BEFORE_ED  0

/* Include test cases in the certification firmware builds. */
#ifdef CONFIG_ZB_TEST_GROUP_ALL
#define ZB_TEST_GROUP_ALL
#endif

#ifdef CONFIG_ZB_TEST_GROUP_ALL_DUT
#ifndef ZB_ED_ROLE
#define ZB_TEST_GROUP_ALL_DUT
#else
/* The zed_test_table.h doe not divide test cases into TH and DUT roles. */
#define ZB_TEST_GROUP_ALL
#endif /* ZB_ED_ROLE */
#endif /* CONFIG_ZB_TEST_GROUP_ALL_DUT */

#ifdef CONFIG_ZB_TEST_GROUP_ALL_TH
#define ZB_TEST_GROUP_ALL_TH
#endif

#ifdef CONFIG_ZB_STACK_REGRESSION_TESTING_API
#define ZB_STACK_REGRESSION_TESTING_API CONFIG_ZB_STACK_REGRESSION_TESTING_API
#endif

#ifdef CONFIG_ZB_MAC_ONLY_STACK
#define ZB_MAC_ONLY_STACK CONFIG_ZB_MAC_ONLY_STACK
#endif

/* Set the ZBOSS buffer size according to the set of enabled MAC features. */
#ifdef ZB_MAC_SECURITY
#define ZB_IO_BUF_SIZE 164
#else /* ZB_MAC_SECURITY */
#define ZB_IO_BUF_SIZE 152
#endif /* ZB_MAC_SECURITY */

/* ZBOSS stack version, represented as an unit32 number. */
#define ZB_STACK_VERSION ((ZBOSS_MAJOR << 24) | (ZBOSS_MINOR << 16) | (ZBOSS_PATCH << 8) | (ZBOSS_SDK_REVISION))

/* ZBOSS NCP protocol version. */
#define ZB_NCP_PROTOCOL_VERSION 0

/* Send beacon immediately according to R23 spec */
#define ZB_SEND_BEACON_IMMEDIATELY

#ifdef ZB_NCS_NO_DEBUG_BUFFERS
#undef ZB_DEBUG_BUFFERS
#endif

#endif /* ZB_VENDOR_H__ */
