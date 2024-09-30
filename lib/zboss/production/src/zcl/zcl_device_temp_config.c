/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2023 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Device Temperature Configuration cluster implementation. */


#define ZB_TRACE_FILE_ID 142

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_DEVICE_TEMP_CONFIG)

#include "zboss_api.h"
#include "zcl/zb_zcl_device_temp_config.h"

static zb_ret_t check_value_device_temp_config_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_device_temp_config_init_server(void)
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DEVICE_TEMP_CONFIG,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_device_temp_config_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}


void zb_zcl_device_temp_config_init_client(void)
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DEVICE_TEMP_CONFIG,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

static zb_ret_t check_value_device_temp_config_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;

  ZVUNUSED(endpoint);

#define ATTR_GET8(value)  *value
#define ATTR_GET24(value) ZB_GET_INT32_FROM_INT24(ZB_ZCL_ATTR_GET24(value))

#define RANGED_ATTRIB__FOREACH(XX)                                             \
  XX(CURRENT_TEMP,               ZB_ZCL_ATTR_GETS16, ZB_ZCL_NON_VALUE_INT16)   \
  XX(MIN_TEMP_EXPERIENCED,       ZB_ZCL_ATTR_GETS16, ZB_ZCL_NON_VALUE_INT16)   \
  XX(MAX_TEMP_EXPERIENCED,       ZB_ZCL_ATTR_GETS16, ZB_ZCL_NON_VALUE_INT16)   \
  XX(OVER_TEMP_TOTAL_DWELL,      ZB_ZCL_ATTR_GET16 , ZB_ZCL_NON_VALUE_UINT16)  \
  XX(DEVICE_TEMP_ALARM_MASK,            ATTR_GET8  , ZB_ZCL_NON_VALUE_UINT8)   \
  XX(LOW_TEMP_THRESHOLD,         ZB_ZCL_ATTR_GETS16, ZB_ZCL_NON_VALUE_INT16)   \
  XX(HIGH_TEMP_THRESHOLD,        ZB_ZCL_ATTR_GETS16, ZB_ZCL_NON_VALUE_INT16)   \
  XX(LOW_TEMP_DWELL_TRIP_POINT,         ATTR_GET24 , ZB_GET_INT32_FROM_INT24(ZB_ZCL_NON_VALUE_INT24))  \
  XX(HIGH_TEMP_DWELL_TRIP_POINT,        ATTR_GET24 , ZB_GET_INT32_FROM_INT24(ZB_ZCL_NON_VALUE_INT24))  \

#define ATTRIB_FULL_NAME(_short) ZB_ZCL_ATTR_DEVICE_TEMP_CONFIG_ ## _short ## _ID
#define ATTRIB_MIN_VALUE(_short) ZB_ZCL_DEVICE_TEMP_CONFIG_ ## _short ## _MIN_VALUE
#define ATTRIB_MAX_VALUE(_short) ZB_ZCL_DEVICE_TEMP_CONFIG_ ## _short ## _MAX_VALUE

// Defaults are non-values, or a value within the range
#define ATTRIB_NON_VALUE(_short) ZB_ZCL_DEVICE_TEMP_CONFIG_ ## _short ## _DEFAULT_VALUE

// Inside a case, value is considered value if it equals non or if it is inside the range
#define SWITCH_CASE(_short, _getter, _non_or_ok_value)                    \
  case ATTRIB_FULL_NAME(_short):                                          \
  {                                                                       \
    zb_bool_t ok = ZB_TRUE;                                               \
    zb_bool_t sure_ok = _getter(value) == (_non_or_ok_value);             \
    ok = sure_ok || (ok && (_getter(value) >= ATTRIB_MIN_VALUE(_short))); \
    ok = sure_ok || (ok && (_getter(value) <= ATTRIB_MAX_VALUE(_short))); \
    ret = ok ? RET_OK : RET_ERROR;                                        \
    break;                                                                \
  }

  switch (attr_id)
  {
    RANGED_ATTRIB__FOREACH(SWITCH_CASE)

    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "check_value_device_temp_config ret %hd", (FMT__H, ret));
  return ret;
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_DEVICE_TEMP_CONFIG */
