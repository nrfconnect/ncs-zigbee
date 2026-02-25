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
/* PURPOSE: Ballast Configuration cluster definitions
*/

#ifndef ZB_ZCL_BALLAST_CONFIG_H
#define ZB_ZCL_BALLAST_CONFIG_H 1

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_BALLAST_CONFIG
 *  @{
 *    @details
 *    The Ballast Configuration cluster provides an interface for configuring a lighting ballast.
 *    No cluster specific commands are received or generated.
 */


/* Cluster ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG */

/** @defgroup ZB_ZCL_BALLAST_CONFIG_ATTRS_GROUP Ballast Configuration cluster attributes
 *  @{
 */

/** @brief Ballast Configuration cluster attribute identifiers
 *  @see ZCL spec, Ballast Configuration Cluster 5.3.2.2
 */
enum zb_zcl_ballast_config_attr_e
{
  /** @brief PhysicalMinLevel attribute, ZCL spec 5.3.2.2.1.1 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_PHYSICAL_MIN_LEVEL_ID         = 0x0000,
  /** @brief PhysicalMaxLevel attribute, ZCL spec 5.3.2.2.1.2 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_PHYSICAL_MAX_LEVEL_ID         = 0x0001,
  /** @brief BallastStatus attribute, ZCL spec 5.3.2.2.1.3 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID             = 0x0002,
  /** @brief MinLevel attribute, ZCL spec 5.3.2.2.2.1 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_MIN_LEVEL_ID                  = 0x0010,
  /** @brief MaxLevel attribute, ZCL spec 5.3.2.2.2.2 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_MAX_LEVEL_ID                  = 0x0011,
  /** @brief IntrinsicBallastFactor attribute, ZCL spec 5.3.2.2.2.3 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_INTRINSIC_BALLAST_FACTOR_ID   = 0x0014,
  /** @brief BallastFactorAdjustment attribute, ZCL spec 5.3.2.2.2.4 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_FACTOR_ADJUSTMENT_ID  = 0x0015,
  /** @brief LampQuantity attribute, ZCL spec 5.3.2.2.3.1 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_QUANTITY_ID              = 0x0020,
  /** @brief LampType attribute, ZCL spec 5.3.2.2.4.1 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_TYPE_ID                  = 0x0030,
  /** @brief LampManufacturer attribute, ZCL spec 5.3.2.2.4.2 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_MANUFACTURER_ID          = 0x0031,
  /** @brief LampRatedHours attribute, ZCL spec 5.3.2.2.4.3 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_RATED_HOURS_ID           = 0x0032,
  /** @brief LampBurnHours attribute, ZCL spec 5.3.2.2.4.4 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_BURN_HOURS_ID            = 0x0033,
  /** @brief LampAlarmMode attribute, ZCL spec 5.3.2.2.4.5 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_ALARM_MODE_ID            = 0x0034,
  /** @brief LampBurnHoursTripPoint attribute, ZCL spec 5.3.2.2.4.6 */
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_BURN_HOURS_TRIP_POINT_ID = 0x0035,
};

/** @brief BallastStatus attribute values
 *  @see ZCL spec 5.3.2.2.1.3 for details
 */
enum zb_zcl_ballast_config_ballast_status_value_e
{
  /** @brief Normal (default) state */
  ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_NORMAL                       = 0x00,
  /** @brief Flag indicating that ballast is not fully operational */
  ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_FLAG_BALLAST_NON_OPERATIONAL = 0x01,
  /** @brief Flag indicating that one or more lamp is not in its socket or is faulty */
  ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_FLAG_LAMP_FAILURE            = 0x02,
  /** @brief Mask for BallastStatus supported range of flags */
  ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_FLAG_MASK                    = 0x03,
};

/** @brief LampAlarmMode attribute values
 *  @see ZCL spec 5.3.2.2.4.5 for details
 */
enum zb_zcl_ballast_config_lamp_alarm_mode_value_e
{
  /** @brief No attributes that may generate an alarm (default) */
  ZB_ZCL_BALLAST_CONFIG_LAMP_ALARM_MODE_NONE            = 0x00,
  /** @brief Flag indicating LampBurnHours attribute may generate an alarm */
  ZB_ZCL_BALLAST_CONFIG_LAMP_ALARM_MODE_LAMP_BURN_HOURS = 0x01,
  /** @brief Mask for LampAlarmMode supported range of flags */
  ZB_ZCL_BALLAST_CONFIG_LAMP_ALARM_MODE_FLAG_MASK       = 0x01,
};

/** @brief Default value for Ballast Configuration cluster revision global attribute */
#define ZB_ZCL_BALLAST_CONFIG_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0003u)

/** @brief Minimal value for level attributes including PhysicalMinLevel, PhysicalMaxLevel, MinLevel, MaxLevel */
#define ZB_ZCL_BALLAST_CONFIG_MIN_LEVEL_VALUE 0x01

/** @brief Maximal value for level attributes including PhysicalMinLevel, PhysicalMaxLevel, MinLevel, MaxLevel */
#define ZB_ZCL_BALLAST_CONFIG_MAX_LEVEL_VALUE 0xfe

/** @brief Default value for PhysicalMinLevel attribute */
#define ZB_ZCL_BALLAST_CONFIG_PHYSICAL_MIN_LEVEL_DEFAULT_VALUE ZB_ZCL_BALLAST_CONFIG_MIN_LEVEL_VALUE

/** @brief Default value for PhysicalMaxLevel attribute */
#define ZB_ZCL_BALLAST_CONFIG_PHYSICAL_MAX_LEVEL_DEFAULT_VALUE ZB_ZCL_BALLAST_CONFIG_MAX_LEVEL_VALUE

/** @brief Default value for BallastStatus attribute */
#define ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_DEFAULT_VALUE ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_NORMAL

/** @brief Default value for MinLevel attribute */
#define ZB_ZCL_BALLAST_CONFIG_MIN_LEVEL_DEFAULT_VALUE ZB_ZCL_BALLAST_CONFIG_PHYSICAL_MIN_LEVEL_DEFAULT_VALUE

/** @brief Default value for MaxLevel attribute */
#define ZB_ZCL_BALLAST_CONFIG_MAX_LEVEL_DEFAULT_VALUE ZB_ZCL_BALLAST_CONFIG_PHYSICAL_MAX_LEVEL_DEFAULT_VALUE

/** @brief Invalid value for IntrinsicBallastFactor attribute */
#define ZB_ZCL_BALLAST_CONFIG_INTRINSIC_BALLAST_FACTOR_INVALID 0xff

/** @brief Default value for BallastFactorAdjustment attribute */
#define ZB_ZCL_BALLAST_CONFIG_BALLAST_FACTOR_ADJUSTMENT_DEFAULT_VALUE 0xff

/** @brief Default value for LampType attribute */
#define ZB_ZCL_BALLAST_CONFIG_LAMP_TYPE_DEFAULT_VALUE {0}

/** @brief Default value for LampManufacturer attribute */
#define ZB_ZCL_BALLAST_CONFIG_LAMP_MANUFACTURER_DEFAULT_VALUE {0}

/** @brief Invalid or unknown value for LampRatedHours and LampBurnedHours attributes */
#define ZB_ZCL_BALLAST_CONFIG_LAMP_UNKNOWN_HOURS MAX_UNSIGNED_24BIT_VAL

/** @brief Default value for LampRatedHours attribute */
#define ZB_ZCL_BALLAST_CONFIG_LAMP_RATED_HOURS_DEFAULT_VALUE ZB_ZCL_BALLAST_CONFIG_BALLAST_LAMP_UNKNOWN_HOURS

/** @brief Default value for LampBurnHours attribute */
#define ZB_ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_DEFAULT_VALUE 0x000000U

/** @brief Default value for LampAlarmMode attribute */
#define ZB_ZCL_BALLAST_CONFIG_LAMP_ALARM_MODE_DEFAULT_VALUE ZB_ZCL_BALLAST_CONFIG_LAMP_ALARM_MODE_NONE

/** @brief Value for LampBurnHoursTripPoint attribute indicating that the alarm for this attribute
 *    shall not be generated */
#define ZB_ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_NOT_GENERATED MAX_UNSIGNED_24BIT_VAL

/** @brief Default value for LampBurnHoursTripPoint attribute */
#define ZB_ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_TRIP_POINT_DEFAULT_VALUE ZB_ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_NOT_GENERATED

/** @cond internals_doc */
/** @name Ballast Configuration cluster internals
 *  Internal structures for Ballast Configuration cluster
 *  @{
 */

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_PHYSICAL_MIN_LEVEL_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_PHYSICAL_MIN_LEVEL_ID,                 \
  ZB_ZCL_ATTR_TYPE_U8,                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                     \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_PHYSICAL_MAX_LEVEL_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_PHYSICAL_MAX_LEVEL_ID,                 \
  ZB_ZCL_ATTR_TYPE_U8,                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                     \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID,                     \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                     \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_MIN_LEVEL_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_MIN_LEVEL_ID,                          \
  ZB_ZCL_ATTR_TYPE_U8,                                              \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_MAX_LEVEL_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_MAX_LEVEL_ID,                          \
  ZB_ZCL_ATTR_TYPE_U8,                                              \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_INTRINSIC_BALLAST_FACTOR_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_INTRINSIC_BALLAST_FACTOR_ID,           \
  ZB_ZCL_ATTR_TYPE_U8,                                              \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_FACTOR_ADJUSTMENT_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_FACTOR_ADJUSTMENT_ID,          \
  ZB_ZCL_ATTR_TYPE_U8,                                              \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_QUANTITY_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_QUANTITY_ID,                      \
  ZB_ZCL_ATTR_TYPE_U8,                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                     \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_TYPE_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_TYPE_ID,                          \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                     \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_MANUFACTURER_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_MANUFACTURER_ID,                  \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                     \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_RATED_HOURS_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_RATED_HOURS_ID,                   \
  ZB_ZCL_ATTR_TYPE_U24,                                             \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_BURN_HOURS_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_BURN_HOURS_ID,                    \
  ZB_ZCL_ATTR_TYPE_U24,                                             \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_ALARM_MODE_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_ALARM_MODE_ID,                    \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_BURN_HOURS_TRIP_POINT_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_BURN_HOURS_TRIP_POINT_ID,         \
  ZB_ZCL_ATTR_TYPE_U24,                                             \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

/** @} */
/** @endcond */ /* Ballast Configuration cluster internals */

/** @brief Declare attribute list for Ballast Configuration cluster
 *  @param attr_list - attribute list name
 *  @param physical_min_level - pointer to variable storing PhysicalMinLevel attribute value
 *  @param physical_max_level - pointer to variable storing PhysicalMaxLevel attribute value
 *  @param ballast_status - pointer to variable storing BallastStatus attribute value
 *  @param min_level - pointer to variable storing MinLevel attribute value
 *  @param max_level - pointer to variable storing MaxLevel attribute value
 *  @param intrinsic_ballast_factor - pointer to variable storing IntrinsicBallastFactor attribute value
 *  @param ballast_factor_adjustment - pointer to variable storing BallastFactorAdjustment attribute value
 *  @param lamp_quantity - pointer to variable storing LampQuantity attribute value
 *  @param lamp_type - pointer to variable storing LampType attribute value
 *  @param lamp_manufacturer - pointer to variable storing LampManufacturer attribute value
 *  @param lamp_rated_hours - pointer to variable storing LampRatedHours attribute value
 *  @param lamp_burn_hours - pointer to variable storing LampBurnHours attribute value
 *  @param lamp_alarm_mode - pointer to variable storing LampAlarmMode attribute value
 *  @param lamp_burn_hours_trip_point - pointer to variable storing LampBurnHoursTripPoint attribute value
 */
#define ZB_ZCL_DECLARE_BALLAST_CONFIG_ATTRIB_LIST(                                                             \
    attr_list,                                                                                                 \
    physical_min_level,                                                                                        \
    physical_max_level,                                                                                        \
    ballast_status,                                                                                            \
    min_level,                                                                                                 \
    max_level,                                                                                                 \
    intrinsic_ballast_factor,                                                                                  \
    ballast_factor_adjustment,                                                                                 \
    lamp_quantity,                                                                                             \
    lamp_type,                                                                                                 \
    lamp_manufacturer,                                                                                         \
    lamp_rated_hours,                                                                                          \
    lamp_burn_hours,                                                                                           \
    lamp_alarm_mode,                                                                                           \
    lamp_burn_hours_trip_point)                                                                                \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_BALLAST_CONFIG)                          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_PHYSICAL_MIN_LEVEL_ID, (physical_min_level))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_PHYSICAL_MAX_LEVEL_ID, (physical_max_level))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID, (ballast_status))                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_MIN_LEVEL_ID, (min_level))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_MAX_LEVEL_ID, (max_level))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_INTRINSIC_BALLAST_FACTOR_ID, (intrinsic_ballast_factor))     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_FACTOR_ADJUSTMENT_ID, (ballast_factor_adjustment))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_QUANTITY_ID, (lamp_quantity))                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_TYPE_ID, (lamp_type))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_MANUFACTURER_ID, (lamp_manufacturer))                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_RATED_HOURS_ID, (lamp_rated_hours))                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_BURN_HOURS_ID, (lamp_burn_hours))                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_ALARM_MODE_ID, (lamp_alarm_mode))                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BALLAST_CONFIG_LAMP_BURN_HOURS_TRIP_POINT_ID, (lamp_burn_hours_trip_point)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @} */ /* end of ZB_ZCL_BALLAST_CONFIG_ATTRS_GROUP group */

/** @defgroup ZB_ZCL_BALLAST_CONFIG_ATTR_API Ballast Configuration cluster attribute value manipulation API
 *  @{
 */

/** @brief Set non-operational ballast status
 *  @param ep - endpoint number
 */
#define ZB_ZCL_BALLAST_CONFIG_SET_BALLAST_STATUS_NON_OPERATIONAL(ep)                                           \
{                                                                                                              \
  zb_zcl_attr_t *attr_desc;                                                                                    \
  zb_uint8_t val;                                                                                              \
                                                                                                               \
  attr_desc = zb_zcl_get_attr_desc_a(ep, ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG,                                     \
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,                                               \
                                     ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID);                            \
  if (attr_desc)                                                                                               \
  {                                                                                                            \
    val = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) | ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_FLAG_BALLAST_NON_OPERATIONAL; \
    ZB_ZCL_SET_ATTRIBUTE(ep, ZB_ZCL_CLUSTER_ID_BALLAST_STATUS,                                                 \
                         ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
                         ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID,                                         \
                         &val, ZB_FALSE);                                                                      \
  }                                                                                                            \
}

/** @brief Reset non-operational ballast status
 *  @param ep - endpoint number
 */
#define ZB_ZCL_BALLAST_CONFIG_RESET_BALLAST_STATUS_NON_OPERATIONAL(ep)                                         \
{                                                                                                              \
  zb_zcl_attr_t *attr_desc;                                                                                    \
  zb_uint8_t val;                                                                                              \
                                                                                                               \
  attr_desc = zb_zcl_get_attr_desc_a(ep, ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG,                                     \
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,                                               \
                                     ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID);                            \
  if (attr_desc)                                                                                               \
  {                                                                                                            \
    zb_uint8_t flag = ~ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_FLAG_BALLAST_NON_OPERATIONAL;                      \
    val = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) & flag;                                                        \
    ZB_ZCL_SET_ATTRIBUTE(ep, ZB_ZCL_CLUSTER_ID_BALLAST_STATUS,                                                 \
                         ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
                         ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID,                                         \
                         &val, ZB_FALSE);                                                                      \
  }                                                                                                            \
}

/** @brief Set lamp failure status
 *  @param ep - endpoint number
 */
#define ZB_ZCL_BALLAST_CONFIG_SET_BALLAST_STATUS_LAMP_FAILURE(ep)                                              \
{                                                                                                              \
  zb_zcl_attr_t *attr_desc;                                                                                    \
  zb_uint8_t val;                                                                                              \
                                                                                                               \
  attr_desc = zb_zcl_get_attr_desc_a(ep, ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG,                                     \
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,                                               \
                                     ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID);                            \
  if (attr_desc)                                                                                               \
  {                                                                                                            \
    val = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) | ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_FLAG_LAMP_FAILURE;      \
    ZB_ZCL_SET_ATTRIBUTE(ep, ZB_ZCL_CLUSTER_ID_BALLAST_STATUS,                                                 \
                         ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
                         ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID,                                         \
                         &val, ZB_FALSE);                                                                      \
  }                                                                                                            \
}

/** @brief Reset lamp failure status
 *  @param ep - endpoint number
 */
#define ZB_ZCL_BALLAST_CONFIG_RESET_BALLAST_STATUS_LAMP_FAILURE(ep)                                            \
{                                                                                                              \
  zb_zcl_attr_t *attr_desc;                                                                                    \
  zb_uint8_t val;                                                                                              \
                                                                                                               \
  attr_desc = zb_zcl_get_attr_desc_a(ep, ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG,                                     \
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,                                               \
                                     ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID);                            \
  if (attr_desc)                                                                                               \
  {                                                                                                            \
    zb_uint8_t flag = ~ZB_ZCL_BALLAST_CONFIG_BALLAST_STATUS_FLAG_LAMP_FAILURE;                                 \
    val = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) & flag;                                                        \
    ZB_ZCL_SET_ATTRIBUTE(ep, ZB_ZCL_CLUSTER_ID_BALLAST_STATUS,                                                 \
                         ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
                         ZB_ZCL_ATTR_BALLAST_CONFIG_BALLAST_STATUS_ID,                                         \
                         &val, ZB_FALSE);                                                                      \
  }                                                                                                            \
}

/** @} */ /* end of ZB_ZCL_BALLAST_CONFIG_ATTR_API group */

/** @} */ /* ZCL Ballast Configuration cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

void zb_zcl_ballast_config_init_server(void);
void zb_zcl_ballast_config_init_client(void);
#define ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG_SERVER_ROLE_INIT zb_zcl_ballast_config_init_server
#define ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG_CLIENT_ROLE_INIT zb_zcl_ballast_config_init_client

#endif /* ZB_ZCL_BALLAST_CONFIG_H */
