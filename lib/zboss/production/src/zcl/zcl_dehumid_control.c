/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
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
/* PURPOSE:
*/

#define ZB_TRACE_FILE_ID 76

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_DEHUMIDIFICATION_CONTROL)

zb_ret_t check_value_dehumidification_control_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_dehumidification_control_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_dehumidification_control_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

void zb_zcl_dehumidification_control_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

zb_ret_t check_value_dehumidification_control_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = ZB_TRUE;

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_COOLING_ID:
    {
      zb_zcl_attr_t *attr_desc;

      ret = RET_ERROR;
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_ID);
      if ( attr_desc != NULL )
      {
        zb_uint8_t max_value = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
        ret = (*value <= max_value) ? RET_OK : RET_ERROR;
      }
    }
    break;
    case ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_ID:
      ret = (*value >= ZB_ZCL_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_MIN_VALUE &&
             *value <= ZB_ZCL_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_ID:
      ret = (*value >= ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_MIN_VALUE &&
             *value <= ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_ID:
      ret = (*value >= ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_MIN_VALUE &&
             *value <= ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "check_value_fan_control ret %hd", (FMT__H, ret));
  return ret;
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_DEHUMIDIFICATION_CONTROL */
