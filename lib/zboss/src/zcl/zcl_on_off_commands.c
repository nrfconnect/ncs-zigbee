/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2025 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ZCL On/Off cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2075

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_ON_OFF)

#include "zb_zcl.h"
#include "zcl/zb_zcl_identify.h"
#include "zb_zdo.h"
#include "zb_aps.h"

/*
CR:MAJOR Current implementation schedules timer multiple times with
0.1 sec timeout. Out timer and scheduler does NOT guarantee this
interval in real work, it guarantees that time interval is equal or
more. It mean in real total timeout maybe much more than specified by
command (x2 - x5). To solve this problem we need to check real timer
value in runtime. This functionality will be done by feature, called
Continuous Value Change, currently under development (DRI Kovtun)
TODO: redesign on/off operation with timer, using Continuous Value
Change feature, when it is ready.
*/

zb_uint8_t gs_on_off_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_ON_OFF_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_on_off_client_cmd_list =
{
  0, NULL,
  sizeof(gs_on_off_server_received_commands), gs_on_off_server_received_commands
};

zb_discover_cmd_list_t gs_on_off_server_cmd_list =
{
  sizeof(gs_on_off_server_received_commands), gs_on_off_server_received_commands,
  0, NULL
};

void zb_zcl_on_off_invoke_user_app(zb_uint8_t param);
static zb_ret_t zb_zcl_call_on_off_attr_device_cb(zb_uint8_t param, zb_uint8_t dst_ep, zb_uint8_t value);
static void zb_zcl_on_off_timer_handler(zb_uint8_t param);

zb_ret_t check_value_on_off_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_bool_t zb_zcl_process_on_off_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_on_off_specific_commands_cli(zb_uint8_t param);

static void cancel_on_off_timer_handler_alarm(void);

void zb_zcl_on_off_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_ON_OFF,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_on_off_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_on_off_specific_commands_srv);
}

void zb_zcl_on_off_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_ON_OFF,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_on_off_specific_commands_cli);
}

zb_ret_t check_value_on_off_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(endpoint);
  ZVUNUSED(attr_id);

  switch (attr_id)
  {
    case ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID: case ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL:
    {
      return ( ZB_ZCL_CHECK_BOOL_VALUE(*value) )?(RET_OK):(RET_ERROR);
    }
    case ZB_ZCL_ATTR_ON_OFF_ON_TIME: case ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME:
    {
      return RET_OK;
    }
    case ZB_ZCL_ATTR_ON_OFF_START_UP_ON_OFF:
    {
      return ( (*value) == ZB_ZCL_ON_OFF_START_UP_ON_OFF_IS_OFF ||
               (*value) == ZB_ZCL_ON_OFF_START_UP_ON_OFF_IS_ON  ||
               (*value) == ZB_ZCL_ON_OFF_START_UP_ON_OFF_IS_TOGGLE ||
               (*value) == ZB_ZCL_ON_OFF_START_UP_ON_OFF_IS_PREVIOUS ) ? RET_OK : RET_ERROR;
    }
    default:
    {
      return RET_ERROR;
    }
  }
}
/*! @internal Schedule command Inform User App about set On/Off attribute
 * @param buffer - buffer for schedule, place data for User App and get result
 * @param pcmd_info - pointer zb_zcl_parsed_hdr_t (see @ref zb_zcl_parsed_hdr_s ), use for
 * contain ZCL data. Use for Send response to client app
 * @param on_off - new value On/Off
 * @param is_run_timer_ - if set True then after invoke User App will be schedule
 * timer zb_zcl_on_off_timer_handler else run finish command "ZB_ZCL_PROCESS_COMMAND_FINISH".
 * zb_zcl_on_off_timer_handler use for "On with timed off" command
 * */
void zb_on_off_schedule_user_app(zb_bufid_t buffer, zb_zcl_parsed_hdr_t *pcmd_info, zb_bool_t on_off, zb_bool_t is_run_timer_)
{
  zb_on_off_user_app_schedule_t* data =
          ZB_BUF_GET_PARAM((buffer), zb_on_off_user_app_schedule_t);
  ZB_MEMMOVE(&(data->cmd_info), (pcmd_info), sizeof(zb_zcl_parsed_hdr_t));
  data->new_value = (on_off);
  data->is_run_timer = (zb_bool_t)(is_run_timer_);
  ZB_SCHEDULE_CALLBACK(zb_zcl_on_off_invoke_user_app, (buffer));
}


#define ZB_ZCL_ON_OFF_TIMER_INTERVAL    100

#define ZB_ZCL_ON_OFF_TIMER_BEACON_INTERVAL    \
    ZB_MILLISECONDS_TO_BEACON_INTERVAL(ZB_ZCL_ON_OFF_TIMER_INTERVAL)

#define ZB_ZCL_ON_OFF_TIMER_INTERVAL_REDUCED  90

#define ZB_ZCL_ON_OFF_TIMER_BEACON_INTERVAL_REDUCED    \
    ZB_MILLISECONDS_TO_BEACON_INTERVAL(ZB_ZCL_ON_OFF_TIMER_INTERVAL_REDUCED)

#define ZB_ZCL_ON_OFF_ZERO_TIMEOUT          0x0000

#define ZB_ZCL_ON_OFF_NON_LIMIT_TIMER       0xffff

#define ZB_ZCL_ON_OFF_GLOBAL_SCENE_STORED   ZB_FALSE

#define ZB_ZCL_ON_OFF_GLOBAL_SCENE_NA       ZB_TRUE

/* Usage of this function.
 * We need to set an alarm each tenth of second.
 * Converting it to beacon interval we get the 15.36ms * 7 = 107.52ms
 * It will increase the real time to 7,52 ms for every step
 * The proposition is to switch alarm timer between 15.36ms * 7 = 107.52ms and 15.36ms * 6 = 92.16ms one by one.
 * In that case the average time is equal to 99.84ms
 */
static zb_bool_t reduced_timer_counter = ZB_FALSE;

static zb_bool_t get_reduced_timer_counter()
{
  reduced_timer_counter = !reduced_timer_counter;
  return reduced_timer_counter;
}

static void cancel_on_off_timer_handler_alarm(void)
{
  zb_ret_t ret;
  zb_bufid_t bufid;

  TRACE_MSG(TRACE_ZCL1, ">>cancel_on_off_timer_handler_alarm", (FMT__0));

  do
  {
    ret = zb_schedule_alarm_cancel(zb_zcl_on_off_timer_handler, ZB_ALARM_ANY_PARAM, &bufid);
    if (ret == RET_OK)
    {
      TRACE_MSG(TRACE_ZCL1, "free bufid %hd", (FMT__H, bufid));
      zb_buf_free(bufid);
    }
  }
  while (ret != RET_NOT_FOUND);

  TRACE_MSG(TRACE_ZCL1, "<<cancel_on_off_timer_handler_alarm", (FMT__0));
}

static zb_ret_t zb_zcl_call_on_off_attr_device_cb(zb_uint8_t param, zb_uint8_t dst_ep, zb_uint8_t value)
{
  zb_zcl_device_callback_param_t *user_app_invoke_data =
    ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_call_on_off_attr_device_cb(), param %hd, dst_ep %hd, value %hd",
            (FMT__H_H_H, param, dst_ep, value));

  user_app_invoke_data->device_cb_id = ZB_ZCL_SET_ATTR_VALUE_CB_ID;
  user_app_invoke_data->endpoint = dst_ep;
  user_app_invoke_data->cb_param.set_attr_value_param.cluster_id = ZB_ZCL_CLUSTER_ID_ON_OFF;
  user_app_invoke_data->cb_param.set_attr_value_param.attr_id = ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID;
  user_app_invoke_data->cb_param.set_attr_value_param.values.data8 = value;
  user_app_invoke_data->status = RET_NOT_IMPLEMENTED;

  ZB_ASSERT(ZCL_CTX().device_cb != NULL);
  (ZCL_CTX().device_cb)(param);

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_call_on_off_attr_device_cb(), status 0x%lx", (FMT__L, user_app_invoke_data->status));
  return user_app_invoke_data->status;
}

/* Timer for "On with timed off" command, see Spec 6.6.1.4.6.4
 * @param param - buffer
 *
 * If attr OnTime >0 then decrement OnTime and
 *                          if OnTime stay =0 prepare to set OnOff = false
 * else if attr OffWaitTime >0 then decrement OffWaitTime
 *
 * if prepare to set OnOff = false then invoke User App
 *
 * if OnTime or OffWaitTime >0 then schedule timer else send response command
 *
 * Timer period = 1/10 sec
 * */
static void zb_zcl_on_off_timer_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_uint8_t endpoint;
  zb_uint16_t val;
  zb_zcl_attr_t * attr_desc_on_off;
  zb_zcl_attr_t * attr_desc_on_time;
  zb_zcl_attr_t * attr_desc_off_wait_time;
  zb_bool_t is_set_off = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_on_off_timer_handler %hd", (FMT__H, param));

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;

  attr_desc_on_off = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);
  attr_desc_on_time = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_TIME);
  attr_desc_off_wait_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME);

  TRACE_MSG(TRACE_ZCL1, "dst_ep %hd", (FMT__H, endpoint));

  ZB_ASSERT(attr_desc_on_off);
  ZB_ASSERT(attr_desc_on_time);
  ZB_ASSERT(attr_desc_off_wait_time);

  /* Update OnTime and OffWaitTime attributes and set flag "update OnOff", if need */
  if( ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_on_off))
  {
    val = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_on_time);
    if (val > 0)
    {
      --val;
    }
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_on_time, val);
    if (val == 0)
    {
      is_set_off = ZB_TRUE;
      TRACE_MSG(TRACE_ZCL1, "OnTime is expired", (FMT__0));
    }
  }
  else
  {
    val = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_off_wait_time);
    if (val > 0)
    {
      --val;
    }
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_off_wait_time, val);
    if (val == 0)
    {
      TRACE_MSG(TRACE_ZCL1, "OffWaitTime is expired", (FMT__0));
    }
  }

  if (val == 0)
  {
    if (is_set_off)   // if need set OnOff
    {
      val = ZB_ZCL_ON_OFF_IS_OFF;
      ZB_ZCL_SET_ATTRIBUTE(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                           ZB_ZCL_CLUSTER_ID_ON_OFF,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                           ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                           (zb_uint8_t *)&val,
                           ZB_FALSE);

      if (ZCL_CTX().device_cb != NULL)
      {
        (void)zb_zcl_call_on_off_attr_device_cb(
          param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
          val);
      }
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_off_wait_time, ZB_ZCL_ON_OFF_ZERO_TIMEOUT);
      ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_on_off_timer_handler, ZB_ALARM_ANY_PARAM);
    }

    zb_buf_free(param);
  }
  else
  {
    ZB_SCHEDULE_ALARM(zb_zcl_on_off_timer_handler, param, (get_reduced_timer_counter() ?
                                                           ZB_ZCL_ON_OFF_TIMER_BEACON_INTERVAL : ZB_ZCL_ON_OFF_TIMER_BEACON_INTERVAL_REDUCED));
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_on_off_timer_handler", (FMT__0));
}

void zb_zcl_on_off_send_default_resp(zb_uint8_t cb_param, zb_uint16_t user_param)
{
  if (!cb_param)
  {
    zb_buf_get_out_delayed_ext(zb_zcl_on_off_send_default_resp, user_param, 0);
  }
  else
  {
    zb_zcl_parsed_hdr_t *cmd_info;
    cmd_info = ZB_BUF_GET_PARAM((zb_uint8_t)user_param, zb_zcl_parsed_hdr_t);
    ZB_ZCL_PROCESS_COMMAND_FINISH(cb_param, cmd_info, ZB_ZCL_STATUS_SUCCESS);
  }
}

/* Process On command
 * see Spec 3.8.2.3.2
 * */
static zb_ret_t zb_zcl_process_on_off_on_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
  zb_zcl_attr_t *attr_desc_on_off = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);
  zb_zcl_attr_t *attr_desc_on_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_TIME);
  zb_zcl_attr_t *attr_desc_off_wait_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME);
  zb_zcl_attr_t *attr_desc_global_scene = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL);

  zb_bool_t is_need_call_user_app = (attr_desc_on_off && ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_on_off)==ZB_ZCL_ON_OFF_IS_OFF) ? ZB_TRUE : ZB_FALSE;

  ZB_ASSERT(attr_desc_on_off);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_on_off_on_handler param %hx",
      (FMT__H, param));

  if (attr_desc_on_time && attr_desc_off_wait_time)
  {
    if (ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_on_time) == ZB_ZCL_ON_OFF_ZERO_TIMEOUT)
    {
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_off_wait_time, ZB_ZCL_ON_OFF_ZERO_TIMEOUT);
      cancel_on_off_timer_handler_alarm();
    }
  }

#ifdef ZB_ZCL_SUPPORT_CLUSTER_SCENES
  /* According to ZCL spec Revision 8. 3.8.2.2.2 GlobalSceneControl Attribute*/
  if (attr_desc_global_scene)
  {
    if( ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_global_scene) == 
        ZB_ZCL_ON_OFF_GLOBAL_SCENE_STORED )
    {
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc_global_scene,
                                    ZB_ZCL_ON_OFF_GLOBAL_SCENE_NA);
    }
  }
#endif
  if(is_need_call_user_app)
  {
    zb_on_off_schedule_user_app(param, cmd_info, ZB_TRUE, ZB_FALSE);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_on_off_on_handler", (FMT__0));

  return (is_need_call_user_app==ZB_TRUE) ? RET_BUSY : RET_OK;
}

/* Process Off command
 * see Spec 3.8.2.3.1
 * */
static zb_ret_t zb_zcl_process_on_off_off_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
  zb_zcl_attr_t *attr_desc_on_off = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);
  zb_zcl_attr_t *attr_desc_on_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_TIME);

  zb_bool_t is_need_call_user_app = (attr_desc_on_off && ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_on_off)!=ZB_ZCL_ON_OFF_IS_OFF) ? ZB_TRUE : ZB_FALSE;

  ZB_ASSERT(attr_desc_on_off);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_on_off_off_handler param %hx",
      (FMT__H, param));

  if (attr_desc_on_time)
  {
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_on_time, ZB_ZCL_ON_OFF_ZERO_TIMEOUT);
  }
  if(is_need_call_user_app)
  {
    zb_on_off_schedule_user_app(param, cmd_info, ZB_FALSE, ZB_FALSE);
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_on_off_off_handler", (FMT__0));

  return (is_need_call_user_app==ZB_TRUE) ? RET_BUSY : RET_OK;
}

/* Process Toggle command
 * see Spec 3.8.2.3.3
 * */
static zb_ret_t zb_zcl_process_on_off_toggle_handler(zb_uint8_t param)
{
  zb_uint8_t new_val; 
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
  zb_zcl_attr_t *attr_desc_on_off = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);
  zb_zcl_attr_t *attr_desc_on_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_TIME);
  zb_zcl_attr_t *attr_desc_off_wait_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME);
  zb_zcl_attr_t *attr_desc_global_scene = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_on_off_toggle_handler param %hx",
      (FMT__H, param));

  ZB_ASSERT(attr_desc_on_off);

  new_val = !ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_on_off);

  if (new_val)
  {
    if (attr_desc_on_time && attr_desc_off_wait_time)
    {
      if (ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_on_time) == ZB_ZCL_ON_OFF_ZERO_TIMEOUT)
      {
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_off_wait_time, ZB_ZCL_ON_OFF_ZERO_TIMEOUT);
        cancel_on_off_timer_handler_alarm();
      }
    }
#ifdef ZB_ZCL_SUPPORT_CLUSTER_SCENES
    /* According to ZCL spec Revision 8. 3.8.2.2.2 GlobalSceneControl Attribute*/
    if (attr_desc_global_scene)
    {
      if( ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_global_scene) == 
          ZB_ZCL_ON_OFF_GLOBAL_SCENE_STORED )
      {
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc_global_scene,
                                      ZB_ZCL_ON_OFF_GLOBAL_SCENE_NA);
      }
    }
#endif
  }
  else
  {
    if (attr_desc_on_time)
    {
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_on_time, ZB_ZCL_ON_OFF_ZERO_TIMEOUT);
    }
  }

  zb_on_off_schedule_user_app(param, cmd_info, (zb_bool_t)new_val, ZB_FALSE);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_on_off_toggle_handler", (FMT__0));

  return RET_BUSY;
}

/* Invoke User App for "Off with effect" command
 *
 * Invoke User App with effect parameters : EffectId and EffectVariant
 * if invoke result RET_OK then schedule invoke User App with attribute On/Off
 * else send response command with error
 */
void zb_zcl_on_off_effect_invoke_user_app(zb_uint8_t param)
{
  zb_zcl_on_off_effect_user_app_schedule_t* invoke_data = ZB_BUF_GET_PARAM(param, zb_zcl_on_off_effect_user_app_schedule_t);
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t result;
  zb_zcl_attr_t *attr_desc_on_time;
  zb_uint8_t val = ZB_ZCL_ON_OFF_IS_OFF;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_on_off_effect_invoke_user_app param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, &(invoke_data->cmd_info), sizeof(zb_zcl_parsed_hdr_t));

  if (ZCL_CTX().device_cb!=NULL)
  {

    zb_zcl_device_callback_param_t *user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    ZB_MEMMOVE(&(user_app_data->cb_param.on_off_set_effect_value_param),
        &(invoke_data->param),
        sizeof(zb_zcl_on_off_set_effect_value_param_t));
    user_app_data->device_cb_id = ZB_ZCL_ON_OFF_WITH_EFFECT_VALUE_CB_ID;
    user_app_data->endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    user_app_data->status = RET_OK;
    (ZCL_CTX().device_cb)(param);
    result = user_app_data->status;
    /* TODO: check - free buffer after a call? */
  }
  else
  {
    result = RET_ERROR;
  }

  if (result == RET_OK)
  {
/* 2012-12-27 CR:MAJOR:DISCUSS Is this call really needed?
 * zb_zcl_on_off_invoke_user_app() is called.
 * A: YES. Spec 6.6.1.4.4.3 - both paragraf ended "...shall enter its off state
 * and update the OnOff attribute accordingly."
 * */
/* 2013-01-09 CR:MAJOR:FIXED No need to call user application callback zb_zcl_on_off_invoke_user_app()
 * here - need only set attribute value to Off and OnTime attribute
 * to 0x0000
 * A: Not sure, but change*/

    /* Use SET_ATTRIBUTE to report attribute change if needed. */
    ZB_ZCL_SET_ATTRIBUTE(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
                         ZB_ZCL_CLUSTER_ID_ON_OFF,
                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                         ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                         &val,
                         ZB_FALSE);

    attr_desc_on_time = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
            ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_TIME);
    ZB_ASSERT(attr_desc_on_time);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_on_time, 0x0000);
  }

  ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, result==RET_OK ? ZB_ZCL_STATUS_SUCCESS :
                               (zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE) ?
                                ZB_ZCL_STATUS_FAIL : ZB_ZCL_STATUS_HW_FAIL);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_on_off_effect_invoke_user_app param", (FMT__0));
}
/* Process Off with effect command
 * see Spec 3.8.2.3.4
 * */
static zb_ret_t zb_zcl_process_on_off_off_with_effect_handler(zb_uint8_t param)
{
  zb_zcl_on_off_off_with_effect_req_t off_with_effect_payload;
  zb_zcl_parse_status_t status;

  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
  zb_zcl_attr_t *attr_desc_on_off = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);
  zb_zcl_attr_t *attr_desc_on_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_TIME);
  zb_zcl_attr_t *attr_desc_global_scene = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL);

  zb_ret_t ret = RET_ERROR;

  ZB_ASSERT(attr_desc_on_off);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_on_off_off_with_effect_handler param %hx",
      (FMT__H, param));

  if (attr_desc_on_time && attr_desc_global_scene)
  {
    ZB_ZCL_ON_OFF_GET_OFF_WITH_EFFECT_REQ(&off_with_effect_payload, param, status);
    if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
    {
      TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_ON_OFF_GET_OFF_WITH_EFFECT_REQ",
                (FMT__0));
      ret = RET_INVALID_PARAMETER_1;
    }
    else
    {
      zb_bool_t is_need_call_user_app = (ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_on_off)!=ZB_ZCL_ON_OFF_IS_OFF) ? ZB_TRUE : ZB_FALSE;
#ifdef ZB_ZCL_SUPPORT_CLUSTER_SCENES
      if (ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_global_scene)==
          ZB_ZCL_ON_OFF_GLOBAL_SCENE_NA)
      {
        zb_zcl_parsed_hdr_t cmd_info_copy;
        zb_zcl_scenes_store_scene_req_t req;

        req.group_id = ZB_ZCL_SCENE_GLOBAL_SCENE_GROUP_ID;
        req.scene_id = ZB_ZCL_SCENE_GLOBAL_SCENE_ID;

        ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info_copy);
        zb_zcl_scenes_process_store_scene(param, &req, &cmd_info_copy);
        ZB_MEMCPY(cmd_info, &cmd_info_copy, sizeof(zb_zcl_parsed_hdr_t));

        ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc_global_scene,
                                      ZB_ZCL_ON_OFF_GLOBAL_SCENE_STORED);

        ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_on_time, ZB_ZCL_ON_OFF_ZERO_TIMEOUT);
      }
#endif
      if(is_need_call_user_app)
      {
        zb_on_off_schedule_user_app(param, cmd_info, ZB_FALSE, ZB_FALSE);
      }
       ret = (is_need_call_user_app==ZB_TRUE) ? RET_BUSY : RET_OK;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_on_off_off_with_effect_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/* Process Off with effect command
 * see Spec 3.8.2.3.5
 * */
static zb_ret_t zb_zcl_process_on_off_on_with_recall_global_scene_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
  zb_zcl_attr_t *attr_desc_on_off = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);
  zb_zcl_attr_t *attr_desc_on_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_TIME);
  zb_zcl_attr_t *attr_desc_off_wait_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME);
  zb_zcl_attr_t *attr_desc_global_scene = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL);

  zb_bool_t is_need_call_user_app = (attr_desc_on_off && ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_on_off)==ZB_ZCL_ON_OFF_IS_OFF) ? ZB_TRUE : ZB_FALSE;

  /*According to ZCL On/Off Cluster Test Specification Test OO-TC-03S steps 3a and 6c,
    the OnOff should response with success status in both cases (GlobalSceneControl attribute is equal to TRUE or FALSE)
   */
  zb_ret_t ret = RET_OK;

  ZB_ASSERT(attr_desc_on_off);

  TRACE_MSG(TRACE_ZCL1,
      "> zb_zcl_process_on_off_on_with_recall_global_scene_handler param %hx",
      (FMT__H, param));

  if (attr_desc_global_scene == NULL)
  {
    ret = RET_NOT_IMPLEMENTED;
  }
  else
  {
    if (attr_desc_on_time && attr_desc_off_wait_time)
    {
      if (ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_on_time) == ZB_ZCL_ON_OFF_ZERO_TIMEOUT)
      {
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_off_wait_time, ZB_ZCL_ON_OFF_ZERO_TIMEOUT);
        cancel_on_off_timer_handler_alarm();
      }
    }

#ifdef ZB_ZCL_SUPPORT_CLUSTER_SCENES
    /* Call User App - ON */
    if( ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_global_scene) ==
        ZB_ZCL_ON_OFF_GLOBAL_SCENE_STORED )
    {
      zb_zcl_parsed_hdr_t cmd_info_copy;
      zb_zcl_scenes_recall_scene_req_t req;

      req.group_id = ZB_ZCL_SCENE_GLOBAL_SCENE_GROUP_ID;
      req.scene_id = ZB_ZCL_SCENE_GLOBAL_SCENE_ID;

      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc_global_scene,
                                    ZB_ZCL_ON_OFF_GLOBAL_SCENE_NA);

      ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info_copy);
      zb_zcl_scenes_process_recall_scene(param, &req, &cmd_info_copy);
      ZB_MEMCPY(cmd_info, &cmd_info_copy, sizeof(zb_zcl_parsed_hdr_t));
    }
    else
#endif
    {
      /* ZCL8, 3.8.2.3.5.1
         On receipt of the On With Recall Global Scene command, if the GlobalSceneControl attribute is equal to TRUE, the
         application on the associated endpoint SHALL discard the command.
       */
    }
  }

  if (ret == RET_OK && is_need_call_user_app)
  {
    zb_on_off_schedule_user_app(param, cmd_info, ZB_TRUE, ZB_FALSE);
    ret = RET_BUSY;
  }

  TRACE_MSG(TRACE_ZCL1,
      "< zb_zcl_process_on_off_on_with_recall_global_scene_handler ret %d",
      (FMT__H, ret));
  return ret;
}

/* Process On with timed off command
 * see Spec 3.8.2.3.6
 * */
static zb_ret_t zb_zcl_process_on_off_on_with_timed_off_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_ret_t ret = RET_ERROR;

  zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
  zb_zcl_attr_t *attr_desc_on_off = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);
  zb_zcl_attr_t *attr_desc_on_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_TIME);
  zb_zcl_attr_t *attr_desc_off_wait_time = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME);
  zb_bool_t is_need_timer;
  zb_bool_t is_need_set_on = ZB_FALSE;

  zb_zcl_on_off_on_with_timed_off_req_t on_timed_off_payload;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_on_off_on_with_timed_off_handler param %hx",
      (FMT__H, param));

  ZB_ASSERT(attr_desc_on_off);

  if (attr_desc_on_time && attr_desc_off_wait_time)
  {
    ZB_ZCL_ON_OFF_GET_ON_WITH_TIMED_OFF_REQ(&on_timed_off_payload, param, status);
    if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
    {
      ret = RET_INVALID_PARAMETER_1;
    }
    else
    {
      // if payload.on_off==1 and attribute On/Off == 0 then
      // discarded
      /* Condition is checked on the certification On/Off Cluster tests */
      if ( ((on_timed_off_payload.on_off & ZB_ZCL_ON_OFF_ACCEPT_ONLY_WHEN_ON) && ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_on_off)) ||
           !(on_timed_off_payload.on_off & ZB_ZCL_ON_OFF_ACCEPT_ONLY_WHEN_ON ) )
      {
        // stop timer
        cancel_on_off_timer_handler_alarm();

        // update attributes
        /* We are in Delayed Off state. */
        if ( ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_off_wait_time) > ZB_ZCL_ON_OFF_ZERO_TIMEOUT &&
             !ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_on_off))
        {
          // set minimum
          if ( on_timed_off_payload.off_wait_time <
               ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_off_wait_time) )
          {
            ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_off_wait_time,
                                           on_timed_off_payload.off_wait_time);
          }
        }
        /* We are in other states */
        else
        {
          // set maximum
          if ( on_timed_off_payload.on_time >
               ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_on_time) )
          {
            ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_on_time,
                                           on_timed_off_payload.on_time);
          }

          ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc_off_wait_time,
                                         on_timed_off_payload.off_wait_time);

          is_need_set_on = ZB_TRUE;
        }

        is_need_timer = (( ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_off_wait_time) <
                           ZB_ZCL_ON_OFF_NON_LIMIT_TIMER ) &&
                         ( ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_on_time) <
                           ZB_ZCL_ON_OFF_NON_LIMIT_TIMER)) ? ZB_TRUE : ZB_FALSE;

        if (is_need_set_on)
        {
          // schedule User App and next, if need, start timer
          zb_on_off_schedule_user_app(param, cmd_info, ZB_TRUE, is_need_timer);
        }
        /* 2013-01-09 CR:MEDIUM:DISCUSS check, "else" clause is needed here or
         * not. According to previous comment, "else" is not needed
        * A: "else" run if this command working and client change timers delay */
        else if (is_need_timer)
        {
          //start timer
          ZB_SCHEDULE_ALARM(zb_zcl_on_off_timer_handler,
                            param,
                            ZB_ZCL_ON_OFF_TIMER_BEACON_INTERVAL);
          /* Server sends Default Response immediately after receiving the command*/
          zb_zcl_on_off_send_default_resp(0, param);
        }
        ret = RET_BUSY;
      }
/*2019-04-10 In this case we have nothing to do, just send Default Response with "Success" status*/
      else
      {
        ret = RET_OK;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_on_off_on_with_timed_off_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/*
 * Process specific command ZCL OnOff cluster
 * See Spec 6.6.1
 */
zb_bool_t  zb_zcl_process_on_off_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_on_off_specific_commands: param %hd, cmd_info.cmd_id 0x%hx",
             (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_ON_OFF == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch( cmd_info.cmd_id )
  {
  case ZB_ZCL_CMD_ON_OFF_ON_ID:
    status = zb_zcl_process_on_off_on_handler(param);
    break;
  case ZB_ZCL_CMD_ON_OFF_OFF_ID:
    status = zb_zcl_process_on_off_off_handler(param);
    break;
  case ZB_ZCL_CMD_ON_OFF_TOGGLE_ID:
    status = zb_zcl_process_on_off_toggle_handler(param);
    break;
  case ZB_ZCL_CMD_ON_OFF_OFF_WITH_EFFECT_ID:
    status = zb_zcl_process_on_off_off_with_effect_handler(param);
    break;
  case ZB_ZCL_CMD_ON_OFF_ON_WITH_RECALL_GLOBAL_SCENE_ID:
    status = zb_zcl_process_on_off_on_with_recall_global_scene_handler(param);
    break;
  case ZB_ZCL_CMD_ON_OFF_ON_WITH_TIMED_OFF_ID:
    status = zb_zcl_process_on_off_on_with_timed_off_handler(param);
    break;

  default:
    processed = ZB_FALSE;
    break;
  }

  if( processed )
  {
    if( cmd_info.disable_default_response && status == RET_OK)
    {
      TRACE_MSG( TRACE_ZCL3,
                 "Default response disabled",
                 (FMT__0));
      zb_buf_free(param);
    }
    else if (status == RET_NOT_IMPLEMENTED) /* case of absence global scene attribute instance for
                                             * zb_zcl_process_on_off_on_with_recall_global_scene_handler
                                             */
    {
      ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    }
    else if (status != RET_BUSY)
    {
      ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, status==RET_OK ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_INVALID_FIELD);
    }
  }

  TRACE_MSG( TRACE_ZCL1,
             "< zb_zcl_process_on_off_specific_commands: processed %d",
             (FMT__D, processed));
  return processed;
}


zb_bool_t zb_zcl_process_on_off_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_on_off_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_on_off_specific_commands(param);
}

zb_bool_t zb_zcl_process_on_off_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_on_off_client_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_on_off_specific_commands(param);
}

/*
 * Invoke User App with On/Off attribute, set attribute and
 * finish command or schedule on_off_timer.
 *
 * On_off_timer used "On with timed off" command.
 */
void zb_zcl_on_off_invoke_user_app(zb_uint8_t param)
{
  zb_on_off_user_app_schedule_t* invoke_data =
      ZB_BUF_GET_PARAM(param, zb_on_off_user_app_schedule_t);
  zb_uint8_t on_off = invoke_data->new_value;
  zb_uint8_t is_run_timer = invoke_data->is_run_timer;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t result;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_on_off_invoke_user_app param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, &(invoke_data->cmd_info), sizeof(zb_zcl_parsed_hdr_t));

  if (ZCL_CTX().device_cb != NULL)
  {
    result = zb_zcl_call_on_off_attr_device_cb(
      param,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
      on_off);
  }
  else
  {
    result = RET_OK;
  }

  /* Use SET_ATTRIBUTE to report attribute change if needed. */
  ZB_ZCL_SET_ATTRIBUTE(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
                       ZB_ZCL_CLUSTER_ID_ON_OFF,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                       &on_off,
                       ZB_FALSE);

  if(is_run_timer && result == RET_OK)
  {
    ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), &cmd_info, sizeof(cmd_info));
    ZB_SCHEDULE_ALARM(zb_zcl_on_off_timer_handler, param, ZB_ZCL_ON_OFF_TIMER_BEACON_INTERVAL);
/*2019-04-10 Now Server sends Default Response immediately after receiving the command*/
    if (result == RET_OK)
    {
      zb_zcl_on_off_send_default_resp(0, param);
    }
  }
  else
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, zb_zcl_map_ret_code_to_zcl_status(result));
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_on_off_invoke_user_app", (FMT__0));
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_ON_OFF */
