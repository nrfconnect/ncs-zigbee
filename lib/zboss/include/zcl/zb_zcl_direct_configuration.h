/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Zigbee Direct Configuration cluster definitions
*/

#ifndef ZB_ZCL_DIRECT_CONFIGURATION_H
#define ZB_ZCL_DIRECT_CONFIGURATION_H 1

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZB_DIRECT_SECTION */

/** @addtogroup ZB_ZCL_DIRECT_CONFIGURATION
 *  @{
 */

/**
 * @brief Hook on Write attribute
 * @param endpoint - endpoint number
 * @param attr_id - ID of attribute being written
 * @param new_value - pointer to new value of attribute
 */
void zb_zcl_direct_configuration_write_attr_hook(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value);

/* Cluster ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION */

/*! @name Zigbee Direct Configuration cluster attributes
    @{
*/

/*! @brief Zigbee Direct Configuration cluster attribute identifiers
    @see ZB Direct spec, Zigbee Direct Configuration Cluster, 13.3
*/
enum zb_zcl_direct_configuration_attr_e
{
  /** @brief Interface State attribute, ZB Direct spec 13.3.5.2 */
  ZB_ZCL_ATTR_DIRECT_CONFIGURATION_INTERFACE_STATE_ID = 0x0000,
  /** @brief Interface State attribute, ZB Direct spec 13.3.5.2 */
  ZB_ZCL_ATTR_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_ID = 0x0001,
};

/** @brief Interface State Bitmap definition. Bitmap part.
 *  @note This enumeration defines bit position (bit-shift) of specified value
 *  in bitmap
 *  @see ZB Direct spec, 13.3.5.3.1.
 */
typedef enum zb_zcl_direct_configuration_interface_state_bits_e
{
  ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_CURRENT_STATE = 0,  /**< Bit 0 specifies the current state of the Zigbee Direct
                                                                   * interface as follows: 1 = enabled, 0 = disabled. */
} zb_zcl_direct_configuration_interface_state_bits_t;

/** @brief Interface State Bitmap definition.
 *  @see ZB Direct spec, 13.3.5.3.1
 *
 *  Bit 0 specifies the current state of the Zigbee Direct interface as follows: 1 = enabled, 0 = disabled.
 *  All other bits are reserved.
 *  @see zb_zcl_direct_configuration_interface_state_bits_t enum.
 */
typedef zb_uint8_t zb_zcl_direct_configuration_interface_state_attr_t;

/**
 * @brief Empty value of Interface State attribute
 */
#define ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_EMPTY_VALUE 0x00

/*  Macros that should be used for get/set the Interface State Bitmap value */
/** @brief Get Current State from the Interface State Bitmap.
 *  @see zb_zcl_direct_configuration_interface_state_bits_t definition.
 *  @param state_bitmap - @ref zb_zcl_direct_configuration_interface_state_attr_t value.
 *  @returns true or false.
 */
#define ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_GET_CURRENT_STATE(state_bitmap)                                     \
               ZB_CHECK_BIT_IN_BIT_VECTOR((&(state_bitmap)), ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_CURRENT_STATE)

/** @brief Set Current State from the Interface State Bitmap.
 *  @see zb_zcl_direct_configuration_interface_state_bits_t definition.
 *  @param state_bitmap - @ref zb_zcl_direct_configuration_interface_state_attr_t value.
 *  @param current_state Current interface state (1U = enabled, 0U = disabled)
 */
#define ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_SET_CURRENT_STATE(state_bitmap, current_state)                      \
               ZB_ASSIGN_BIT_IN_BIT_VECTOR((&(state_bitmap)), ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_CURRENT_STATE, current_state)

/** @brief Anonymous Join Timeout definition.
 *  @see ZB Direct spec, 13.3.5.3.2
 */
typedef zb_uint24_t zb_zcl_direct_configuration_anonymous_join_timeout_attr_t;

/**
 * @brief Empty value of Anonymous Join Timeout, 13.3.5.3.2.
 */
#define ZB_ZCL_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_EMPTY_VALUE 60U

/**
 * @brief Get Current Anonymous Join Timeout value as zb_uint32_t
 * @param attr_p pointer to @ref zb_zcl_direct_configuration_anonymous_join_timeout_attr_t
 * @return  zb_uint32_t representing current Anonymous Join Timeout value
 */
#define ZB_ZCL_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_GET_CURRENT_VALUE_U32(attr_p)           \
  ZB_GET_UINT32_FROM_UINT24(*((zb_uint24_t *)(attr_p)->data_p))

/**
 * @brief Set Current Anonymous Join Timeout value
 * @param attr_p    attribute pointer
 * @param value_u32 desired value to be set
 */
#define ZB_ZCL_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_SET_CURRENT_VALUE_U32(attr_p,           \
                                                                                 value_u32)        \
  do                                                                                               \
  {                                                                                                \
    *((zb_uint24_t *)((attr_p)->data_p))                                                           \
        = (zb_zcl_direct_configuration_anonymous_join_timeout_attr_t)ZB_ASSIGN_UINT24_FROM_UINT32( \
            value_u32);                                                                            \
  } while (ZB_FALSE)

/** @cond internals_doc */
/*! @internal @name Zigbee Direct Configuration cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIRECT_CONFIGURATION_INTERFACE_STATE_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIRECT_CONFIGURATION_INTERFACE_STATE_ID,          \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                     \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_ID,   \
  ZB_ZCL_ATTR_TYPE_U24,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

/*! @internal Number of attributes mandatory for reporting in Zigbee Direct Configuration cluster */
#define ZB_ZCL_DIRECT_CONFIGURATION_REPORT_ATTR_COUNT 0

/*! @} */ /* Zigbee Direct Configuration cluster internals */
/*! @endcond */ /* internals_doc */

/** @cond internals_doc */
/*! @internal @name Zigbee Direct Configuration cluster attributes
    Internal attributes.
    @{
*/
/** @brief Declare attribute list for Zigbee Direct Configuration cluster - server side (Information Attribute set)
    @param attr_list - attribute list name
    @param interface_state        - pointer to variable to store Interface State attribute
    @param anonymous_join_timeout - pointer to variable to store Anonymous Join Timeout
*/
#define ZB_ZCL_DECLARE_DIRECT_CONFIGURATION_INFORMATION_ATTRIB_LIST(attr_list, interface_state, anonymous_join_timeout) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                                           \
  ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIRECT_CONFIGURATION_INTERFACE_STATE_ID(interface_state),                          \
  ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_ID(anonymous_join_timeout),            \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Zigbee Direct Configuration cluster attributes */
/*! @endcond */ /* internals_doc */

/*! @name Zigbee Direct Configuration cluster commands
    @{
*/

/*! @brief Command identifiers for Zigbee Direct Configuration cluster
    @see ZB Direct spec, subclause 13.3.5.4
*/
enum zb_zcl_direct_configuration_cmd_e
{
  /* Configure Zigbee Direct Interface command */
  ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_INTERFACE_ID = 0x00, /**< "Turn off" command. */
  /* Configure Zigbee Direct Anonymous Timeout Join */
  ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_ANONYMOUS_TIMEOUT_JOIN_ID = 0x01
};

/*! @brief Command response identifiers for Zigbee Direct Configuration cluster
    @see ZB Direct spec, subclause 13.3.5.4
*/
enum zb_zcl_direct_configuration_cmd_resp_e
{
  /* Configure Zigbee Direct Interface Response command */
  ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_INTERFACE_RESP_ID = 0x00,
};

/** @cond internals_doc */
/*! @{ */
/* Scenes cluster commands list : only for information - do not modify */
#define ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_SERVER_ROLE_GENERATED_CMD_LIST \
                                      ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_INTERFACE_RESP_ID


#define ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_CLIENT_ROLE_RECEIVED_CMD_LIST \
  ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_SERVER_ROLE_GENERATED_CMD_LIST

#define ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_CLIENT_ROLE_GENERATED_CMD_LIST                     \
                                      ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_INTERFACE_ID, \
                                      ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_ANONYMOUS_TIMEOUT_JOIN_ID

#define ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_SERVER_ROLE_RECEIVED_CMD_LIST \
  ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_CLIENT_ROLE_GENERATED_CMD_LIST

/*! @} */
/**  @endcond */ /* internals_doc */

/** @cond DOXYGEN_ZCL_SECTION */

/**
 * @name Interface State Field
 * @see ZB Direct spec B.3.5.4.2.2
 * @anchor zb_zcl_direct_configuration_interface_state
 */
/** @{ */
#define ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_OFF      0u /*!< Disable BLE interface  */
#define ZB_ZCL_DIRECT_CONFIGURATION_INTERFACE_STATE_ON       1u /*!< Enable BLE interface   */
/** @} */

/**
 * @brief Configure Interface Request parameters
 */
typedef ZB_PACKED_PRE struct zb_zcl_direct_configuration_configure_interface_req_s
{
  /** Requested Interface State, @see zb_zcl_direct_configuration_interface_state */
  zb_uint8_t interface_state;
} ZB_PACKED_STRUCT zb_zcl_direct_configuration_configure_interface_req_t;


/**
 * @brief Configure Interface Response parameters
 */
typedef ZB_PACKED_PRE struct zb_zcl_direct_configuration_configure_interface_resp_s
{
  /** Command processing status */
  zb_uint8_t status;
  /** Current Interface State, @see zb_zcl_direct_configuration_interface_state */
  zb_uint8_t interface_state;
} ZB_PACKED_STRUCT zb_zcl_direct_configuration_configure_interface_resp_t;

/** @endcond */ /* internals_doc */

/**
 * @brief Configure Anonymous Join Timeout Request parameters
 */
typedef ZB_PACKED_PRE struct zb_zcl_direct_configuration_configure_anonymous_join_timeout_req_s
{
  zb_uint24_t anonymous_join_timeout;
} ZB_PACKED_STRUCT zb_zcl_direct_configuration_configure_anonymous_join_timeout_req_t;

#define ZB_ZCL_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_MIN_VALUE 0x000000U

#define ZB_ZCL_DIRECT_CONFIGURATION_ANONYMOUS_JOIN_TIMEOUT_MAX_VALUE 0x100000U

/*! @brief Send Configure Zigbee Direct Interface command
    The command can be sent only with unicast addressing mode
    @param buffer to put packet to
    @param dst_addr - address to send packet to
    @param dst_addr_mode - addressing mode. @ref ZB_APS_ADDR_MODE_16_ENDP_PRESENT will fit
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - enable/disable default response
    @param callback for getting command send status
    @param interface_state - requested state of ZBD interface
*/
#define ZB_ZCL_DIRECT_CONFIGURATION_SEND_CONFIGURE_ZBD_INTERFACE_REQ( \
    buffer,                                                           \
    dst_addr,                                                         \
    dst_addr_mode,                                                    \
    dst_ep,                                                           \
    ep,                                                               \
    prof_id,                                                          \
    dis_default_resp,                                                 \
    callback,                                                         \
    interface_state)                                                  \
  {                                                                   \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                 \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(              \
        ptr,                                                          \
        dis_default_resp)                                             \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                              \
        ptr,                                                          \
        ZB_ZCL_GET_SEQ_NUM(),                                         \
        ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_INTERFACE_ID);  \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (interface_state));                  \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                                 \
    ZB_ZCL_SEND_COMMAND_SHORT(                                        \
        buffer,                                                       \
        dst_addr,                                                     \
        dst_addr_mode,                                                \
        dst_ep,                                                       \
        ep,                                                           \
        prof_id,                                                      \
        ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION,                       \
        callback);                                                    \
  }


/*! @brief Send Configure Zigbee Direct Interface Response command

    @param buffer to put packet to
    @param seq_num - ZCL sequence number
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param callback for getting command send status
    @param status of the corresponding Configure Zigbee Direct Interface command execution. Appropriate
    values (defined in @ref zcl_status enumeration)are:
    @li @ref ZB_ZCL_STATUS_SUCCESS for successful result
    @li @ref ZB_ZCL_STATUS_FAIL if command was not executed successfully
    @param interface_state - current state of ZBD interface
*/
#define ZB_ZCL_DIRECT_CONFIGURATION_SEND_CONFIGURE_ZBD_INTERFACE_RES(      \
    buffer,                                                                \
    seq_num,                                                               \
    dst_addr,                                                              \
    dst_ep,                                                                \
    ep,                                                                    \
    prof_id,                                                               \
    callback,                                                              \
    status,                                                                \
    interface_state)                                                       \
  {                                                                        \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                         \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);              \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER(                                       \
        ptr,                                                               \
        seq_num,                                                           \
        ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_INTERFACE_RESP_ID);  \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (status));                                \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (interface_state));                       \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                                      \
    ZB_ZCL_SEND_COMMAND_SHORT(                                             \
        buffer,                                                            \
        dst_addr,                                                          \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,                                  \
        dst_ep,                                                            \
        ep,                                                                \
        prof_id,                                                           \
        ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION,                            \
        callback);                                                         \
  }


/** @brief Macro for getting Configure Zigbee Direct Interface command payload, see ZB Direct spec 13.3.5.4.2
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_direct_configuration_configure_interface_req_t.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_INTERFACE_REQ(data_ptr, buffer, status)  \
{                                                                                          \
  zb_uint8_t *data = zb_buf_begin(buffer);                                                 \
  if (zb_buf_len((buffer)) !=                                                              \
      sizeof(zb_uint8_t))                                                                  \
  {                                                                                        \
    (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                                \
  }                                                                                        \
  else                                                                                     \
  {                                                                                        \
    ZB_ZCL_PACKET_GET_DATA8(&(data_ptr)->interface_state, data);                           \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                                \
  }                                                                                        \
}


/** @brief Macro for getting Configure Zigbee Direct Interface Response command payload, see ZB Direct spec 13.3.5.5.1
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_direct_configuration_configure_interface_resp_t.
  * @param buffer containing the packet (by pointer).
  * @param parse_status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_INTERFACE_RESP(data_ptr, buffer, parse_status) \
{                                                                                                \
  zb_uint8_t *data = zb_buf_begin(buffer);                                                       \
  if (zb_buf_len((buffer)) !=                                                                    \
      sizeof(zb_uint8_t) + sizeof(zb_uint8_t))                                                   \
  {                                                                                              \
    (parse_status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                                \
  }                                                                                              \
  else                                                                                           \
  {                                                                                              \
    ZB_ZCL_PACKET_GET_DATA8(&(data_ptr)->status, data);                                          \
    ZB_ZCL_PACKET_GET_DATA8(&(data_ptr)->interface_state, data);                                 \
    (parse_status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                                \
  }                                                                                              \
}

/*! @brief Send Configure Zigbee Direct Anonymous Join Timeout command
    The command can be sent only with unicast addressing mode
    @param buffer to put packet to
    @param dst_addr - address to send packet to
    @param dst_addr_mode - addressing mode. @ref ZB_APS_ADDR_MODE_16_ENDP_PRESENT will fit
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - enable/disable default response
    @param callback for getting command send status
    @param anonymous_join_timeout - requested state of ZBD interface
*/
#define ZB_ZCL_DIRECT_CONFIGURATION_SEND_CONFIGURE_ANONYMOUS_JOIN_TIMEOUT_REQ(    \
    buffer,                                                                       \
    dst_addr,                                                                     \
    dst_addr_mode,                                                                \
    dst_ep,                                                                       \
    ep,                                                                           \
    prof_id,                                                                      \
    dis_default_resp,                                                             \
    callback,                                                                     \
    anonymous_join_timeout)                                                       \
  {                                                                               \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                             \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(                          \
        ptr,                                                                      \
        dis_default_resp)                                                         \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                                          \
        ptr,                                                                      \
        ZB_ZCL_GET_SEQ_NUM(),                                                     \
        ZB_ZCL_CMD_DIRECT_CONFIGURATION_CONFIGURE_ZBD_ANONYMOUS_TIMEOUT_JOIN_ID); \
    ZB_ZCL_PACKET_PUT_DATA24(ptr, &(anonymous_join_timeout));                     \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                                             \
    ZB_ZCL_SEND_COMMAND_SHORT(                                                    \
        buffer,                                                                   \
        dst_addr,                                                                 \
        dst_addr_mode,                                                            \
        dst_ep,                                                                   \
        ep,                                                                       \
        prof_id,                                                                  \
        ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION,                                   \
        callback);                                                                \
  }

/** @brief Macro for getting Configure Zigbee Direct Anonymous Join Timeout command payload, see ZB Direct spec 13.3.5.4.4
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_direct_configuration_configure_anonymous_join_timeout_req_t.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_DIRECT_CONFIGURATION_GET_CONFIGURE_ANONYMOUS_JOIN_TIMEOUT_REQ(data_ptr, buffer, status)  \
{                                                                                                       \
  zb_uint8_t *data = zb_buf_begin(buffer);                                                              \
  if (zb_buf_len((buffer)) !=                                                                           \
      sizeof(zb_uint24_t))                                                                              \
  {                                                                                                     \
    (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                                             \
  }                                                                                                     \
  else                                                                                                  \
  {                                                                                                     \
    ZB_ZCL_PACKET_GET_DATA24(&(data_ptr)->anonymous_join_timeout, data);                                \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                                             \
  }                                                                                                     \
}

void zb_zcl_direct_configuration_init_server(void);
void zb_zcl_direct_configuration_init_client(void);
#define ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_SERVER_ROLE_INIT zb_zcl_direct_configuration_init_server
#define ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION_CLIENT_ROLE_INIT zb_zcl_direct_configuration_init_client

/**
 * @brief Start or restart Anonymous Join timer from Anonymous Join Timeout value
 *
 * @param endpoint endpoint of Zigbee Direct Configuration cluster
 */
void zb_zcl_direct_configuration_anonymous_join_timer_start(zb_uint8_t endpoint);

/**
 * @brief Check if current value of Anonymous Join timer is bigger than 0
 *
 * @return ZB_TRUE if Anonymous Join for provisioned ZDD is enabled
 */
zb_bool_t zb_zcl_direct_configuration_anonymous_join_enabled(void);

zb_bool_t zb_zcl_direct_configuration_is_client_authorized(zb_uint16_t client_short_addr);

/*! @} */ /* Zigbee Direct Configuration cluster commands */

/** @endcond */ /* DOXYGEN_ZCL_SECTION && DOXYGEN_DIRECT_FEATURE */

#endif /* ZB_ZCL_DIRECT_CONFIGURATION_H */
