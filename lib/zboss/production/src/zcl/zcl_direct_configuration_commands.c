/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2022 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Zigbee Direct Configuration cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 17608
#include "zb_common.h"


#if defined ZB_ZCL_SUPPORT_CLUSTER_DIRECT_CONFIGURATION

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_direct_configuration.h"
#include "zb_nvram.h"
#include "zb_osif.h"
#include "zb_aps.h"
#include "zb_zdd.h"

/** @internal
    @{
*/

/* Enable Anonymous Join Timeout by default in case Zigbee Direct Configuration is not enabled */
static zb_bool_t anonymous_join_enabled = ZB_TRUE;

static zb_uint8_t gs_direct_configuration_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_CLIENT_ROLE_RECEIVED_CMD_LIST
};

static zb_uint8_t gs_direct_configuration_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_direct_configuration_server_cmd_list =
{
  sizeof(gs_direct_configuration_server_received_commands), gs_direct_configuration_server_received_commands,
  sizeof(gs_direct_configuration_client_received_commands), gs_direct_configuration_client_received_commands
};

zb_discover_cmd_list_t gs_direct_configuration_client_cmd_list =
{
  sizeof(gs_direct_configuration_client_received_commands), gs_direct_configuration_client_received_commands,
  sizeof(gs_direct_configuration_server_received_commands), gs_direct_configuration_server_received_commands
};


zb_ret_t check_value_direct_configuration_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
void zb_zcl_direct_configuration_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code);
zb_bool_t zb_zcl_process_direct_configuration_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_direct_configuration_specific_commands_cli(zb_uint8_t param);


void zb_zcl_direct_configuration_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_direct_configuration_server,
                              zb_zcl_direct_configuration_write_attr_hook_server,
                              zb_zcl_process_direct_configuration_specific_commands_srv);
}


void zb_zcl_direct_configuration_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_direct_configuration_specific_commands_cli);
}


zb_ret_t check_value_direct_configuration_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(endpoint);
  ZVUNUSED(value);

  TRACE_MSG(TRACE_ZCL1, ">> check_value_direct_configuration_server, attr_id 0x%x, endpoint %hd",
    (FMT__D_H, attr_id, endpoint));

  switch (attr_id)
  {
#if 0
    case ZB_ZCL_ATTR_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_ID:
      ret = (ZB_GET_UINT32_FROM_UINT24(ZB_ZCL_ATTR_GET24(value))
             <= ZB_ZCL_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_MAX_VALUE)
                ? RET_OK
                : RET_ERROR;
      break;
#endif

    default:
      /* No implementation needed */
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< check_value_direct_configuration_server, ret %d", (FMT__D, ret));
  return ret;
}


static zb_ret_t zbd_configure_interface_req_handler(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_direct_configuration_configure_interface_req_t zbd_req;
  zb_zcl_parse_status_t parse_payload_status;
  zb_zcl_status_t resp_status = ZB_ZCL_STATUS_SUCCESS;
  zb_zcl_attr_t *attr_interface_state;
  zb_zcl_direct_configuration_interface_state_attr_t interface_state_bitfield;

  TRACE_MSG(TRACE_ZCL1, ">> zbd_configure_interface_req_handler, param %hx", (FMT__H, param));

  if (ZB_NWK_IS_ADDRESS_BROADCAST(cmd_info->addr_data.common_data.dst_addr))
  {
    TRACE_MSG(TRACE_ZCL1,
              "zbd_configure_interface_req_handler: dst_addr 0x%hx is not a unicast",
              (FMT__H, cmd_info->addr_data.common_data.dst_addr));

    cmd_info->disable_default_response = ZB_TRUE;

    ret = RET_IGNORE;
  }

  if (ret == RET_OK)
  {
    if (!zb_is_network_distributed() && (zb_aib_get_trust_center_short_address() != cmd_info->addr_data.common_data.source.u.short_addr))
    {
      TRACE_MSG(TRACE_ZCL1,
                "zbd_configure_interface_req_handler: source address 0x%hx is not authorized (not a TC)",
                (FMT__H, cmd_info->addr_data.common_data.source.u.short_addr));
      ret = RET_UNAUTHORIZED;
    }
  }

  if (ret == RET_OK)
  {
    ZB_BZERO(&zbd_req, sizeof(zbd_req));
    ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_INTERFACE_REQ(&zbd_req, param, parse_payload_status);

    if (parse_payload_status != ZB_ZCL_PARSE_STATUS_SUCCESS)
    {
      TRACE_MSG(TRACE_ERROR, "Invalid payload of ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_INTERFACE_REQ", (FMT__0));
      ret = RET_INVALID_PARAMETER;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL2, "Command payload is parsed: interface_state %hd", (FMT__H, zbd_req.interface_state));
    }
  }

  if (ret == RET_OK)
  {
    if (zbd_req.interface_state != ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_ON &&
      zbd_req.interface_state != ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_OFF)
    {
      TRACE_MSG(TRACE_ZCL2, "Requested Interface State value is invalid", (FMT__0));
      ret = RET_INVALID_PARAMETER_1;
    }
  }

  if (ret == RET_OK)
  {
    attr_interface_state = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DIRECT_CONFIGURATION_INTERFACE_STATE_ID);
    interface_state_bitfield = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_interface_state);

    if (zbd_req.interface_state == ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_GET_CURRENT_STATE(interface_state_bitfield))
    {
      TRACE_MSG(TRACE_ZCL2, "Requested Interface State is equal to current Interface State 0x%hx",
        (FMT__H, interface_state_bitfield));
      resp_status = ZB_ZCL_STATUS_SUCCESS;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL2, "Try to apply requested Interface State value",  (FMT__0));

      if (ZCL_CTX().device_cb == NULL)
      {
        TRACE_MSG(TRACE_ERROR, "Application should register Device CB to properly process Configure Interface command",  (FMT__0));
        resp_status = ZB_ZCL_STATUS_FAIL;
      }
      else
      {
        ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
          ZB_ZCL_DIRECT_CONFIGURATION_CONFIGURE_INTERFACE_CB_ID, RET_OK, cmd_info, &zbd_req, NULL);

        (ZCL_CTX().device_cb)(param);

        if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
        {
          resp_status = ZB_ZCL_STATUS_SUCCESS;

          ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_SET_CURRENT_STATE(interface_state_bitfield, zbd_req.interface_state);
          ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_interface_state, interface_state_bitfield);

#ifdef ZB_USE_NVRAM
          /* If we fail, trace is given and assertion is triggered */
          (void)zb_nvram_write_dataset(ZB_NVRAM_DATASET_ZB_DIRECT);
#endif
        }
        else
        {
          resp_status = ZB_ZCL_STATUS_FAIL;
        }
      }
    }

    ZB_ZCL_DIRECT_CONFIGURATION_SEND_CONFIGURE_ZBD_INTERFACE_RES(
      param,
      cmd_info->seq_number,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      NULL,
      resp_status,
      interface_state_bitfield
    );
  }

  if (ret == RET_OK)
  {
    ret = RET_BUSY;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zbd_configure_interface_req_handler, ret %d", (FMT__D, ret));

  return ret;
}


static zb_ret_t zbd_configure_interface_resp_handler(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_direct_configuration_configure_interface_resp_t zbd_resp;
  zb_zcl_parse_status_t parse_payload_status;

  ZVUNUSED(cmd_info);

  TRACE_MSG(TRACE_ZCL1, ">> zbd_configure_interface_resp_handler, param %hx", (FMT__H, param));

  ZB_BZERO(&zbd_resp, sizeof(zbd_resp));
  ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_INTERFACE_RESP(&zbd_resp, param, parse_payload_status);

  if (parse_payload_status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ERROR, "Invalid payload of ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_INTERFACE_RESP", (FMT__0));
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "Command payload is parsed: status %hd, interface_state %hd", (FMT__H_H, zbd_resp.status, zbd_resp.interface_state));
  }

  TRACE_MSG(TRACE_ZCL1, "<< zbd_configure_interface_resp_handler, ret %d", (FMT__D, ret));

  return ret;
}

static zb_ret_t zbd_configure_anonymous_join_timeout_req_handler(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_direct_configuration_configure_anonymous_join_timeout_req_t zbd_req;
  zb_zcl_parse_status_t parse_payload_status;
  zb_zcl_attr_t *attr_anonymous_join_timeout;
  zb_uint32_t anonymous_join_timeout;

  TRACE_MSG(TRACE_ZCL1, ">> zbd_configure_anonymous_join_timeout_req_handler, param %hd", (FMT__H, param));

  if (ZB_NWK_IS_ADDRESS_BROADCAST(cmd_info->addr_data.common_data.dst_addr))
  {
    TRACE_MSG(TRACE_ZCL1,
              "zbd_configure_anonymous_join_timeout_req_handler: dst_addr 0x%hx is not a unicast",
              (FMT__H, cmd_info->addr_data.common_data.dst_addr));

    cmd_info->disable_default_response = ZB_TRUE;

    ret = RET_IGNORE;

  }

  if (ret == RET_OK)
  {
    if (!zb_is_network_distributed() && (zb_aib_get_trust_center_short_address() != cmd_info->addr_data.common_data.source.u.short_addr))
    {
      TRACE_MSG(TRACE_ZCL1,
                "zbd_configure_anonymous_join_timeout_req_handler: source address 0x%hx is not authorized (not a TC)",
                (FMT__H, cmd_info->addr_data.common_data.source.u.short_addr));
      ret = RET_UNAUTHORIZED;
    }
  }

  if (ret == RET_OK)
  {
    ZB_BZERO(&zbd_req, sizeof(zbd_req));
    ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_ANONYMOUS_JOIN_TIMEOUT_REQ(&zbd_req, param, parse_payload_status);

    if (parse_payload_status != ZB_ZCL_PARSE_STATUS_SUCCESS)
    {
      TRACE_MSG(TRACE_ERROR,
                "zbd_configure_anonymous_join_timeout_req_handler: invalid payload of ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_ANONYMOUS_JOIN_TIMEOUT_REQ",
                (FMT__0));
      ret = RET_INVALID_PARAMETER;
    }
    else
    {
      anonymous_join_timeout = ZB_GET_UINT32_FROM_UINT24(zbd_req.anonymous_join_timeout);
      TRACE_MSG(TRACE_ZCL2,
                "zbd_configure_anonymous_join_timeout_req_handler: command payload is parsed, anonymous_join_timeout %d",
                (FMT__D, anonymous_join_timeout));
    }
  }

  if (ret == RET_OK)
  {
    ret = (anonymous_join_timeout <= ZB_ZCL_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_MAX_VALUE)
              ? RET_OK
              : RET_INVALID_PARAMETER;
  }

  if (ret == RET_OK)
  {
    attr_anonymous_join_timeout = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_ID);

    ZB_ASSERT(attr_anonymous_join_timeout != NULL);

    if (ZB_ZCL_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_GET_CURRENT_VALUE_U32(attr_anonymous_join_timeout) == anonymous_join_timeout)
    {
      TRACE_MSG(TRACE_ZCL2,
                "zbd_configure_anonymous_join_timeout_req_handler: requested Anonymous Join Timeout is equal to current Anonymous Join Timeout %d",
                (FMT__D, anonymous_join_timeout));
    }
    else
    {
      TRACE_MSG(TRACE_ZCL2, "zbd_configure_anonymous_join_timeout_req_handler: try to apply requested Anonymous Join Timeout value",  (FMT__0));

      ZB_COPY_UINT24(attr_anonymous_join_timeout->data_p, &zbd_req.anonymous_join_timeout);

#ifdef ZB_USE_NVRAM
      /* If we fail, trace is given and assertion is triggered */
      (void)zb_nvram_write_dataset(ZB_NVRAM_DATASET_ZB_DIRECT);
#endif

      /** 13.3.5.4.4.
       * After a power-cycle or local manufacturer specific stimulus of the provisioned ZDD,
       * or when the Anonymous Join Timeout value is set using this command,
       * an Anonymous Join Countdown Timer SHALL start with the value set to Anonymous Join Timeout seconds
       */
      ZB_ASSERT(ZB_DIRECT_CTX().is_zbd_provisioned != NULL);

      if(ZB_DIRECT_CTX().is_zbd_provisioned())
      {
        zb_zcl_direct_configuration_anonymous_join_timer_start(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zbd_configure_anonymous_join_timeout_req_handler, ret %d", (FMT__D, ret));

  return ret;
}

zb_bool_t zb_zcl_process_direct_configuration_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_direct_configuration_specific_commands: param %d, cmd %d",
    (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION);

  if(cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV)
  {
    switch(cmd_info.cmd_id)
    {
    case ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_INTERFACE_ID:
      status = zbd_configure_interface_req_handler(param, &cmd_info);
      break;
    case ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_ANONYMOUS_TIMEOUT_JOIN_ID:
      status = zbd_configure_anonymous_join_timeout_req_handler(param, &cmd_info);
      break;

    default:
      processed = ZB_FALSE;
      break;
    }
  }
  else  // ZB_ZCL_FRAME_DIRECTION_TO_CLI
  {
    switch(cmd_info.cmd_id)
    {
    case ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_INTERFACE_RESP_ID:
      status = zbd_configure_interface_resp_handler(param, &cmd_info);
      break;

    default:
      processed = ZB_FALSE;
      break;
    }
  }

  if(processed)
  {
    if(cmd_info.disable_default_response && (status == RET_OK || status == RET_IGNORE))
    {
      TRACE_MSG(TRACE_ZCL3, "Default response disabled", (FMT__0));
      zb_buf_free(param);
    }
    else if (status != RET_BUSY)
    {
      zb_uint8_t resp_status;

      switch (status)
      {
      case RET_OK:
        resp_status = ZB_ZCL_STATUS_SUCCESS;
        break;

      case RET_UNAUTHORIZED:
        resp_status = ZB_ZCL_STATUS_NOT_AUTHORIZED;
        break;

      default:
        resp_status = ZB_ZCL_STATUS_INVALID_VALUE;
        break;
      }

      ZB_ZCL_SEND_DEFAULT_RESP_DIRECTION(param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
        cmd_info.profile_id,
        ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION,
        cmd_info.seq_number,
        cmd_info.cmd_id,
        resp_status,
        cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV
          ? ZB_ZCL_FRAME_DIRECTION_TO_CLI
          : ZB_ZCL_FRAME_DIRECTION_TO_SRV);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_direct_configuration_specific_commands: processed %d",
    (FMT__D, processed));

  return processed;
}


zb_bool_t zb_zcl_process_direct_configuration_specific_commands_srv(zb_uint8_t param)
{
  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_direct_configuration_server_cmd_list;

    return ZB_TRUE;
  }

  return zb_zcl_process_direct_configuration_specific_commands(param);
}

zb_bool_t zb_zcl_process_direct_configuration_specific_commands_cli(zb_uint8_t param)
{
  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_direct_configuration_client_cmd_list;

    return ZB_TRUE;
  }

  return zb_zcl_process_direct_configuration_specific_commands(param);
}

void zb_zcl_direct_configuration_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code)
{
  ZVUNUSED(endpoint);
  ZVUNUSED(new_value);
  ZVUNUSED(manuf_code);
  
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_direct_configuration_write_attr_hook_server, attr_id 0x%x, endpoint %hd, manuf_code %d",
      (FMT__D_H_D, attr_id, endpoint, manuf_code));

  switch (attr_id)
  {
    case ZB_ZCL_ATTR_DIRECT_CONFIGURATION_INTERFACE_STATE_ID:
      TRACE_MSG(TRACE_ZCL1, "Interface state is changed, new_state 0x%hx", (FMT__H, *new_value));
      break;

    case ZB_ZCL_ATTR_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_ID:
      TRACE_MSG(TRACE_ZCL1, "Anonymous Join Timeout is changed, new_state 0x%d", (FMT__D, ZB_GET_UINT32_FROM_UINT24(*((zb_uint24_t*)new_value))));
      break;

    default:
      ZB_ASSERT(ZB_FALSE);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_direct_configuration_write_attr_hook_server", (FMT__0));
}

/**
 * @brief Set anonymous_join_enabled flag to ZB_FALSE
 *
 * @param param unused
 */
static void zb_zcl_direct_configuration_anonymous_join_disable(zb_uint8_t param)
{
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZBDIRECT1, "zb_zcl_direct_configuration_anonymous_join_disable()", (FMT__0));
  anonymous_join_enabled = ZB_FALSE;
}

/**
 * @brief Start or restart Anonymous Join timer from Anonymous Join Timeout value
 *
 * @param endpoint endpoint of Zigbee Direct Configuration cluster
 */
void zb_zcl_direct_configuration_anonymous_join_timer_start(zb_uint8_t endpoint)
{
  zb_zcl_attr_t* attr_desc;
  zb_uint32_t time;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_direct_configuration_anonymous_join_timer_start, endpoint %hd", (FMT__H, endpoint));

  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_ID);

  if(attr_desc == NULL)
  {
    TRACE_MSG(TRACE_ERROR,
              "zb_zcl_direct_configuration_anonymous_join_timer_start: Zigbee Direct Configuration cluster is not defined on the endpoint %hd",
              (FMT__H, endpoint));
  }
  else
  {
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_direct_configuration_anonymous_join_disable,
                             ZB_ALARM_ANY_PARAM);

    time = ZB_GET_UINT32_FROM_UINT24(ZB_ZCL_ATTR_GET24(attr_desc->data_p));
    if (time == 0)
    {
      zb_zcl_direct_configuration_anonymous_join_disable(ZB_UNUSED_PARAM);
    }
    else
    {
      anonymous_join_enabled = ZB_TRUE;
      ZB_SCHEDULE_ALARM(zb_zcl_direct_configuration_anonymous_join_disable, ZB_UNUSED_PARAM, time * ZB_TIME_ONE_SECOND);
      TRACE_MSG(TRACE_ZBDIRECT1, "zb_zcl_direct_configuration_anonymous_join_timer_start: enabled Anonymous Join for %d seconds", (FMT__D, time));
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_direct_configuration_anonymous_join_timer_start", (FMT__0));
}

/**
 * @brief Check if current value of Anonymous Join timer is bigger than 0
 *
 * @return ZB_TRUE if Anonymous Join for provisioned ZDD is enabled
 */
zb_bool_t zb_zcl_direct_configuration_anonymous_join_enabled(void)
{
  return anonymous_join_enabled;
}


zb_bool_t zb_zcl_direct_configuration_is_client_authorized(zb_uint16_t client_short_addr)
{
  return zb_is_network_distributed() ||
    zb_aib_get_trust_center_short_address() == client_short_addr;
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_DIRECT_CONFIGURATION */
