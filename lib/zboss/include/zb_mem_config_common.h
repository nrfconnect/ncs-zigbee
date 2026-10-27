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
/*  PURPOSE: Common rules for ZBOSS application-side memory configuring

That file is to be included from zb_mem_config_xxxx.h after basic
selectors defined.

Do not include that file directly into the application source!
*/
#ifndef ZB_MEM_CONFIG_COMMON_H
#define ZB_MEM_CONFIG_COMMON_H 1

/*
  That file is useful only for ZBOSS build with memory configurable by the user without ZBOSS lib recompile.
 */
#ifdef ZB_CONFIGURABLE_MEM

#ifndef ZB_NWK_CONFIGURABLE_MEM_MAX_NETWORK_SIZE
#error "You should include zboss_api.h or zb_config.h to add custom memory configuration."
#endif /* ZB_NWK_CONFIGURABLE_MEM_MAX_NETWORK_SIZE */

#ifdef ZB_ED_ROLE
/* If ZBOSS library is compiled for ZED only, force ZED config role. */
#ifndef ZB_CONFIG_ROLE_ZED
#define ZB_CONFIG_ROLE_ZED
#endif
#ifdef ZB_CONFIG_ROLE_ZC
#undef ZB_CONFIG_ROLE_ZC
#endif
#ifdef ZB_CONFIG_ROLE_ZR
#undef ZB_CONFIG_ROLE_ZR
#endif
#endif

/*
  Rules of deriving parameters from generic settings.

## device role ZC/ZR/ZED

### Parameters depending on it:
ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE / ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE:
  - OVERALL_NETWORK_SIZE for ZC role
  - 4 for ZR and ZED roles

## Overall Zigbee network size:

- OVERALL_NETWORK_SIZE 2 to 600 for all roles if large network support is enabled.
- OVERALL_NETWORK_SIZE 2 to 250 for all roles if large network support is disabled.
Large networks support is disabled by default for ZED role.

### Parameters depending on it:
- ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE / ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE:
  - OVERALL_NETWORK_SIZE for ZC role
- ZB_IEEE_ADDR_TABLE_SIZE / ZB_CONFIG_IEEE_ADDR_TABLE_SIZE:
    - OVERALL_NETWORK_SIZE + reserve for redirected entries (1/4 of OVERALL_NETWORK_SIZE) for ZC and ZR roles
    - OVERALL_NETWORK_SIZE for ZED role
- ZB_NEIGHBOR_TABLE_SIZE / ZB_CONFIG_NEIGHBOR_TABLE_SIZE:
    - OVERALL_NETWORK_SIZE for ZC and ZR roles but no more than 254 entries
    - 1 for ZED role (for parent device)
- ZB_ZDO_KEY_NEGOTIATIONS_NUM / ZB_CONFIG_ZDO_KEY_NEGOTIATIONS_NUM:
    - OVERALL_NETWORK_SIZE for ZC and ZR roles but not less than 10 entries
    - 1 for ZED role
- ZB_NWK_MAX_SRC_ROUTES / ZB_CONFIG_NWK_MAX_SOURCE_ROUTES
    - OVERALL_NETWORK_SIZE for ZC and ZR roles
    - Undefined for ZED role

## Total network traffic
LIGHT_TRAFFIC / MODERATE_TRAFFIC / HIGH_TRAFFIC / LARGE_NET_TRAFFIC

### Parameters depending on it:
- ZB_IOBUF_POOL_SIZE / ZB_CONFIG_IOBUF_POOL_SIZE
    - 26 - LIGHT_TRAFFIC
    - 40 - MODERATE_TRAFFIC
    - 48 - HIGH_TRAFFIC
    - 96 - LARGE_NET_TRAFFIC
- ZB_NWK_ROUTING_TABLE_SIZE / ZB_CONFIG_NWK_ROUTING_TABLE_SIZE
    - 8 - LIGHT_TRAFFIC
    - MIN(OVERALL_NETWORK_SIZE, 255) - MODERATE_TRAFFIC, HIGH_TRAFFIC, LARGE_NET_TRAFFIC
- ZB_NWK_ROUTE_DISCOVERY_TABLE_SIZE / ZB_CONFIG_NWK_ROUTE_DISC_TABLE_SIZE
    - 6 - LIGHT_TRAFFIC
    - 16 - MODERATE_TRAFFIC
    - 32 - HIGH_TRAFFIC
    - 64 - LARGE_NET_TRAFFIC
- ZB_APS_SRC_BINDING_TABLE_SIZE / ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE
    - 16 - LIGHT_TRAFFIC
    - 24 - MODERATE_TRAFFIC
    - 32 - HIGH_TRAFFIC
    - 32 - LARGE_NET_TRAFFIC
- ZB_APS_DST_BINDING_TABLE_SIZE / ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE
    - 16 - LIGHT_TRAFFIC
    - 24 - MODERATE_TRAFFIC
    - 32 - HIGH_TRAFFIC
    - 32 - LARGE_NET_TRAFFIC


## Complexity of application relationships
APPLICATION_SIMPLE / APPLICATION_MODERATE / APPLICATION_COMPLEX / APPLICATION_LARGE_NET

### Parameters depending on it:
- ZB_IOBUF_POOL_SIZE / ZB_CONFIG_IOBUF_POOL_SIZE - application complexity determines minimal buffers pool size
    - APPLICATION_SIMPLE doesn't set requirement for minimal buffers pool size
    - `>= 24` - APPLICATION_MODERATE
    - `>= 32` - APPLICATION_COMPLEX
    - `>= 96` - APPLICATION_LARGE_NET
- ZB_N_APS_RETRANS_ENTRIES / ZB_CONFIG_N_APS_RETRANS_ENTRIES
    - 6 - APPLICATION_SIMPLE
    - at least 1/3 of ZB_CONFIG_IOBUF_POOL_SIZE - APPLICATION_MODERATE / APPLICATION_COMPLEX / APPLICATION_LARGE_NET
- ZB_APS_DUPS_TABLE_SIZE / ZB_CONFIG_APS_DUPS_TABLE_SIZE
    - 32 - APPLICATION_LARGE_NET
    - 32 - APPLICATION_COMPLEX
    - 16 - APPLICATION_MODERATE
    - 8 - APPLICATION_SIMPLE
- ZB_SCHEDULER_Q_SIZE / ZB_CONFIG_SCHEDULER_Q_SIZE
    - 96 - APPLICATION_LARGE_NET
    - 48 - APPLICATION_COMPLEX
    - 48 with ZB_CONFIG_HIGH_TRAFFIC and 32 otherwise - APPLICATION_MODERATE
    - 24 - APPLICATION_SIMPLE
- ZB_NEIGHBOR_TABLE_SIZE / ZB_CONFIG_NEIGHBOR_TABLE_SIZE - application complexity determines minimal neighbor table size
    - APPLICATION_SIMPLE, APPLICATION_MODERATE, APPLICATION_LARGE_NET don't set requirement for neighbor table size
    - `>= 16` - APPLICATION_COMPLEX for ZC and ZR roles
- ZB_NWK_DISC_TABLE_SIZE / ZB_CONFIG_NWK_DISC_TABLE_SIZE
    - 64 - APPLICATION_LARGE_NET
    - 32 - APPLICATION_COMPLEX
    - 16 - APPLICATION_MODERATE
    - 8 - APPLICATION_SIMPLE

 */

/*
  Total network size: set at upper level, just a verification here.
 */
#ifndef ZB_CONFIG_OVERALL_NETWORK_SIZE
#error Define ZB_CONFIG_OVERALL_NETWORK_SIZE!
#elif ZB_CONFIG_OVERALL_NETWORK_SIZE < 2U || ZB_CONFIG_OVERALL_NETWORK_SIZE > ZB_NWK_CONFIGURABLE_MEM_MAX_NETWORK_SIZE
#error ZB_CONFIG_OVERALL_NETWORK_SIZE must be between 2 and ZB_NWK_CONFIGURABLE_MEM_MAX_NETWORK_SIZE!
#else
/* Derive constands from the network size */

#endif  /* ZB_CONFIG_OVERALL_NETWORK_SIZE */

#define ZB_ADDR_TABLE_SIZE_QUAD(n) ((n + 15U)/16U * 4U)
 /* Reserve in address translation table for aliases: 1/4, at least */
#ifdef ZB_NO_BIG_NET
 /* 4. Table size must be < 255, so let's limit net size to 200 */
#define ZB_IEEE_ADDR_TABLE_SIZE_RESERVE(n) ((ZB_ADDR_TABLE_SIZE_QUAD(n) + (n) < 255u) ? ZB_ADDR_TABLE_SIZE_QUAD(n) : (255u - (n)))
#else
#define ZB_IEEE_ADDR_TABLE_SIZE_RESERVE(n) ZB_ADDR_TABLE_SIZE_QUAD(n)
#endif

/*
  Device role
*/
#ifdef ZB_CONFIG_ROLE_ZC

#if defined ZB_CONFIG_ROLE_ZR || defined ZB_CONFIG_ROLE_ZED
#error Only one ZB_CONFIG_ROLE_xxx can be defined!
#endif

/* ZC need to store one TCLK per device in the overall network */
#define ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE ZB_CONFIG_OVERALL_NETWORK_SIZE
/* Address table for entire network + reserve */
#define ZB_CONFIG_IEEE_ADDR_TABLE_SIZE (ZB_CONFIG_OVERALL_NETWORK_SIZE + ZB_IEEE_ADDR_TABLE_SIZE_RESERVE(ZB_CONFIG_OVERALL_NETWORK_SIZE))
/* Let's have enough space to have the entire network in neighbors - Star topology */
/* Limit Neighbor table size to 254, due to Zigbee spec implicitly (?) defining it to be 255.
 * 254 since ZB_NWK_NEIGHBOR_REF_NONE equals 255 (max val for zb_uint8_t) */
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE (ZB_CONFIG_OVERALL_NETWORK_SIZE < 255 ? ZB_CONFIG_OVERALL_NETWORK_SIZE : 254U)
#define ZB_CONFIG_NWK_MAX_SOURCE_ROUTES ZB_CONFIG_OVERALL_NETWORK_SIZE
#define ZB_CONFIG_ZDO_KEY_NEGOTIATIONS_NUM (ZB_CONFIG_OVERALL_NETWORK_SIZE < 10u ? ZB_CONFIG_OVERALL_NETWORK_SIZE : 10u)

#elif defined ZB_CONFIG_ROLE_ZR

#if defined ZB_CONFIG_ROLE_ZC || defined ZB_CONFIG_ROLE_ZED
#error Only one ZB_CONFIG_ROLE_xxx can be defined!
#endif

/* Only own keys need to be stored */
#define ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE 4U
/* The same as ZC: let's be able to work in Star. */
#define ZB_CONFIG_IEEE_ADDR_TABLE_SIZE (ZB_CONFIG_OVERALL_NETWORK_SIZE + ZB_IEEE_ADDR_TABLE_SIZE_RESERVE(ZB_CONFIG_OVERALL_NETWORK_SIZE))
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE (ZB_CONFIG_OVERALL_NETWORK_SIZE < 255 ? ZB_CONFIG_OVERALL_NETWORK_SIZE : 254U)
/* 10/21/2019 EE CR:MINOR Why we ever need that constant for ZR? Only ZC is a concentrator. */
#define ZB_CONFIG_NWK_MAX_SOURCE_ROUTES ZB_CONFIG_OVERALL_NETWORK_SIZE
#define ZB_CONFIG_ZDO_KEY_NEGOTIATIONS_NUM (ZB_CONFIG_OVERALL_NETWORK_SIZE < 10u ? ZB_CONFIG_OVERALL_NETWORK_SIZE : 10u)

#elif defined ZB_CONFIG_ROLE_ZED

#if defined ZB_CONFIG_ROLE_ZC || defined ZB_CONFIG_ROLE_ZR
#error Only one ZB_CONFIG_ROLE_xxx can be defined!
#endif

/* 2 is needed to perform BDB TCLK, 1 more is needed to request new TCLK */
#define ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE 4U
/* Set it here big enough; may decrease it later */
#define ZB_CONFIG_IEEE_ADDR_TABLE_SIZE ZB_CONFIG_OVERALL_NETWORK_SIZE

/* ZED has single nbt entry - for its parent */
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE 1U
#define ZB_CONFIG_ZDO_KEY_NEGOTIATIONS_NUM 1U

#else

#error Define exactly one ZB_CONFIG_ROLE_xxx!

#endif  /* ZB_CONFIG_ROLE_ZC */

/*
  Total network traffic (including NWK routing)
 */
#ifdef ZB_CONFIG_LARGE_NET_TRAFFIC

#if defined(ZB_CONFIG_HIGH_TRAFFIC) || defined(ZB_CONFIG_MODERATE_TRAFFIC) || defined(ZB_CONFIG_LIGHT_TRAFFIC)
#error Only one ZB_CONFIG_xxx_TRAFFIC can be defined!
#endif

/* More NWK traffic we route or send/recv from our app - more packet buffers required. */
#define ZB_CONFIG_IOBUF_POOL_SIZE 96U
#define ZB_CONFIG_NWK_ROUTE_DISC_TABLE_SIZE 64U
#define ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE 32U
#define ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE 32U

#elif defined(ZB_CONFIG_HIGH_TRAFFIC)

#if defined(ZB_CONFIG_LARGE_NET_TRAFFIC) || defined(ZB_CONFIG_MODERATE_TRAFFIC) || defined(ZB_CONFIG_LIGHT_TRAFFIC)
#error Only one ZB_CONFIG_xxx_TRAFFIC can be defined!
#endif

/* More NWK traffic we route or send/recv from our app - more packet buffers required. */
#define ZB_CONFIG_IOBUF_POOL_SIZE 48U
#define ZB_CONFIG_NWK_ROUTE_DISC_TABLE_SIZE 32U
#define ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE 32U
#define ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE 32U

#elif defined(ZB_CONFIG_MODERATE_TRAFFIC)

#if defined(ZB_CONFIG_LARGE_NET_TRAFFIC) || defined(ZB_CONFIG_HIGH_TRAFFIC) || defined(ZB_CONFIG_LIGHT_TRAFFIC)
#error Only one ZB_CONFIG_xxx_TRAFFIC can be defined!
#endif

#define ZB_CONFIG_IOBUF_POOL_SIZE 40U
#define ZB_CONFIG_NWK_ROUTE_DISC_TABLE_SIZE 16U
#define ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE 24U
#define ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE 24U

#elif defined(ZB_CONFIG_LIGHT_TRAFFIC)

#if defined(ZB_CONFIG_LARGE_NET_TRAFFIC) || defined(ZB_CONFIG_HIGH_TRAFFIC) || defined(ZB_CONFIG_MODERATE_TRAFFIC)
#error Only one ZB_CONFIG_xxx_TRAFFIC can be defined!
#endif

#define ZB_CONFIG_IOBUF_POOL_SIZE 26U
#define ZB_CONFIG_NWK_ROUTING_TABLE_SIZE 8U
#define ZB_CONFIG_NWK_ROUTE_DISC_TABLE_SIZE 6U
#define ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE 16U
#define ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE 16U

#else

#error Define exactly one ZB_CONFIG_xxx_TRAFFIC!

#endif  /* ZB_CONFIG_HIGH_TRAFFIC */

/*
  Complexity of the application interconnection to other Zigbee devices.
 */
#if defined(ZB_CONFIG_APPLICATION_LARGE_NET)

#if defined(ZB_CONFIG_APPLICATION_COMPLEX) || defined(ZB_CONFIG_APPLICATION_MODERATE) || defined(ZB_CONFIG_APPLICATION_SIMPLE)
#error Only one ZB_CONFIG_APPLICATION_xxx can be defined!
#endif

#define ZB_CONFIG_SCHEDULER_Q_SIZE 96U

/* Increase buffers pool size for large net applications */
#if ZB_CONFIG_IOBUF_POOL_SIZE < 96U
#undef ZB_CONFIG_IOBUF_POOL_SIZE
#define ZB_CONFIG_IOBUF_POOL_SIZE 96U
#endif

#define ZB_CONFIG_APS_DUPS_TABLE_SIZE 32U
#define ZB_CONFIG_N_APS_RETRANS_ENTRIES ((ZB_CONFIG_IOBUF_POOL_SIZE + 8U)/9U * 3U) /* 1/3, at least 3 */

/* More devices and nets around - bigger nwk discovery table required. Let's use some heuristics. */
#define ZB_CONFIG_NWK_DISC_TABLE_SIZE 64U

#elif defined ZB_CONFIG_APPLICATION_COMPLEX

#if defined(ZB_CONFIG_APPLICATION_LARGE_NET) || defined(ZB_CONFIG_APPLICATION_MODERATE) || defined(ZB_CONFIG_APPLICATION_SIMPLE)
#error Only one ZB_CONFIG_APPLICATION_xxx can be defined!
#endif

#define ZB_CONFIG_SCHEDULER_Q_SIZE 48U

/* Increase pool and neighbor table sizes of complex application is supposed. */

#if ZB_CONFIG_IOBUF_POOL_SIZE < 32U
#undef ZB_CONFIG_IOBUF_POOL_SIZE
#define ZB_CONFIG_IOBUF_POOL_SIZE 32U
#endif

/* Neighbor table for ED build should have a single entry according to R23 PICS - "NWK-NT-SIZE-ZED - Neighbor Table Size is equal to 1 (for End Devices)" */
#if !defined(ZB_CONFIG_ROLE_ZED) && (ZB_CONFIG_NEIGHBOR_TABLE_SIZE < 16U)
#undef ZB_CONFIG_NEIGHBOR_TABLE_SIZE
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE 16U
#endif

#define ZB_CONFIG_APS_DUPS_TABLE_SIZE 32U
#define ZB_CONFIG_N_APS_RETRANS_ENTRIES ((ZB_CONFIG_IOBUF_POOL_SIZE + 8U)/9U * 3U) /* 1/3, at least 3 */

/* More devices and nets around - bigger nwk discovery table required. Let's use some heuristics. */
#define ZB_CONFIG_NWK_DISC_TABLE_SIZE 32U

#elif defined ZB_CONFIG_APPLICATION_MODERATE

#if defined(ZB_CONFIG_APPLICATION_LARGE_NET) || defined(ZB_CONFIG_APPLICATION_COMPLEX) || defined(ZB_CONFIG_APPLICATION_SIMPLE)
#error Only one ZB_CONFIG_APPLICATION_xxx can be defined!
#endif

#ifdef ZB_CONFIG_HIGH_TRAFFIC
#define ZB_CONFIG_SCHEDULER_Q_SIZE 48U
#else
#define ZB_CONFIG_SCHEDULER_Q_SIZE 32U
#endif

#if ZB_CONFIG_IOBUF_POOL_SIZE < 24U
#undef ZB_CONFIG_IOBUF_POOL_SIZE
#define ZB_CONFIG_IOBUF_POOL_SIZE 24U
#endif

#define ZB_CONFIG_APS_DUPS_TABLE_SIZE 16U
#define ZB_CONFIG_N_APS_RETRANS_ENTRIES ((ZB_CONFIG_IOBUF_POOL_SIZE + 8U)/9U * 3U) /* 1/3, at least 3 */

#define ZB_CONFIG_NWK_DISC_TABLE_SIZE 16U

#elif defined ZB_CONFIG_APPLICATION_SIMPLE

#if defined(ZB_CONFIG_APPLICATION_LARGE_NET) || defined(ZB_CONFIG_APPLICATION_MODERATE) || defined(ZB_CONFIG_APPLICATION_COMPLEX)
#error Only one ZB_CONFIG_APPLICATION_xxx can be defined!
#endif

#define ZB_CONFIG_SCHEDULER_Q_SIZE 24U

#define ZB_CONFIG_APS_DUPS_TABLE_SIZE 8U
#define ZB_CONFIG_N_APS_RETRANS_ENTRIES 6U

#define ZB_CONFIG_NWK_DISC_TABLE_SIZE 8U

#else

#error Define exactly one ZB_CONFIG_APPLICATION_xxx!

#endif  /* ZB_CONFIG_APPLICATION_COMPLEX */

/* Common definitions across all Application complexities and Expected traffic loads */

#ifndef ZB_CONFIG_NWK_ROUTING_TABLE_SIZE
/* In case of ZB_CONFIG_MODERATE_TRAFFIC or higher traffic configuration set routing table size for the most complex case
 * (route for each device in the network is needed), but no more than 255 entries as ZDO Mgmt RTG Request
 * has 1 byte start_index. One entry is also reserved for stack internal purposes. */
#define ZB_CONFIG_NWK_ROUTING_TABLE_SIZE  ((ZB_CONFIG_OVERALL_NETWORK_SIZE <= 255U) ? ZB_CONFIG_OVERALL_NETWORK_SIZE : (255U))
#endif

#define ZB_CONFIG_MAC_PENDING_QUEUE_SIZE (ZB_CONFIG_IOBUF_POOL_SIZE / 4U)
#define ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE ((ZB_CONFIG_IOBUF_POOL_SIZE + 15U)/16U * 4U) /* 1/4, at least 4 */
#define ZB_CONFIG_SINGLE_TRANS_INDEX_SIZE ((ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE + 7U) / 8U)

/* check that 5 bits of src_table_index is enough */
ZB_ASSERT_COMPILE_DECL(ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE <= (1U<<5U));

#ifdef ZB_CONFIG_SCHEDULER_Q_SIZE
/**
   The purpose of the define. Ret code handling implementation on the application side
   (via ZB_SCHEDULE_USER_APP_ALARM and ZB_SCHEDULE_USER_APP_CALLBACK) implies that we have some part
   of the callback and alarm queues which can not be used from the user app and always should be reserved
   for stack schedule purposes. So, let's define this part as 12 (for both immediate callbacks and alarms)
   for all configurations.
 */
#define ZB_CONFIG_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL 12U
#if (ZB_CONFIG_SCHEDULER_Q_SIZE - ZB_CONFIG_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL) < 6U
#error The size of application scheduler queue is very small! Please, change ZB_CONFIG_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL, ZB_CONFIG_SCHEDULER_Q_SIZE  to set it at least 6
#endif
#endif

#ifdef ZB_CONFIG_ROLE_ZED
/* That parameters will not be used in ZED, but just in case - let
 * compiler fail if routing parameter used by mistake. */
#undef ZB_CONFIG_NWK_ROUTING_TABLE_SIZE
#undef ZB_CONFIG_MAC_PENDING_QUEUE_SIZE
#undef ZB_CONFIG_NWK_ROUTE_DISC_TABLE_SIZE
#define ZB_CONFIG_NWK_ROUTING_TABLE_SIZE 0U
#define ZB_CONFIG_MAC_PENDING_QUEUE_SIZE 0U
#define ZB_CONFIG_NWK_ROUTE_DISC_TABLE_SIZE 0U

#if (ZB_CONFIG_NEIGHBOR_TABLE_SIZE != 1)
#error Invalid configuration, neighbor table for ED build should have a single entry according to R23 PICS - "NWK-NT-SIZE-ZED - Neighbor Table Size is equal to 1 (for End Devices)"
#endif

#endif /* ZB_CONFIG_ROLE_ZED */

#ifdef ZB_MAC_SOFTWARE_PB_MATCHING
/* Reserve 50% of neighbor table for end devices, but no more than `ZB_MAX_ED_CAPACITY_DEFAULT_LIMIT`.
 *
 * The value should be greater than or equal to runtime max ED capacity.
 * The runtime value can be configured by `zb_nwk_set_max_ed_capacity()`
 * (by default it is `ZB_MAX_ED_CAPACITY_DEFAULT`).
 * Otherwise, it will not be possible to fill entire ED capacity by sleepy ED.
 *
 * It is also possible to set the value greater than the `ZB_MAX_ED_CAPACITY_DEFAULT_LIMIT`.
 * In that case it will be needed to call `zb_nwk_set_max_ed_capacity()` to configure appropriate ED capacity.
 */
#define ZB_CONFIG_CHILD_HASH_TABLE_SIZE \
  (((ZB_CONFIG_NEIGHBOR_TABLE_SIZE / 2U) > ZB_MAX_ED_CAPACITY_DEFAULT_LIMIT) ? ZB_MAX_ED_CAPACITY_DEFAULT_LIMIT : (ZB_CONFIG_NEIGHBOR_TABLE_SIZE / 2U))

/* Pending bitmap size. Each bit corresponds to "child_hash_table" */
#define ZB_CONFIG_PENDING_BITMAP_SIZE ((ZB_CONFIG_CHILD_HASH_TABLE_SIZE + 31U) / 32U)
#endif

/* This value must not be changed. Initialization is based on the ZB_CONFIG_IOBUF_POOL_SIZE value. */
#define ZB_CONFIG_BUF_POOL_BITMAP_SIZE ((ZB_CONFIG_IOBUF_POOL_SIZE + 7U) / 8U)

#endif  /* ZB_CONFIGURABLE_MEM */

#endif /* ZB_MEM_CONFIG_COMMON_H */
