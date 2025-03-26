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
/* PURPOSE:
*/

#ifndef ZDO_WWAH_PARENT_CLASSIFICATION_H
#define ZDO_WWAH_PARENT_CLASSIFICATION_H 1

#include "zb_config.h"
#include "zb_common.h"
#include "zcl/zb_zcl_wwah.h"


/** @cond DOXYGEN_ZCL_SECTION */

/**
 *  @addtogroup ZB_ZCL_WWAH
 *  @{
 */

/**
 *  @addtogroup ZDO_WWAH_PARENT_CLASSIFICATION
 *  @{
 */

/**
 *  @brief Attribute to determine min RSSI for receiving packets
 *
 *         See Zigbee WWAH Requirements spec,
 *         6.2 Commissioning Requirements, C-20
 *
 *         @ZDO_WWAH_RSSI_FOR_GOOD_LINK_QUALITY_GROUP
 */
/*
 * TODO: ZDO_WWAH_MIN_RSSI_FOR_RECEIVING_PACKETS is platform dependent!!!
 *
 * Now ZDO_WWAH_MIN_RSSI_FOR_RECEIVING_PACKETS and
 * ZDO_WWAH_RSSI_FOR_RECEIVING_PACKETS_ADDING are zero to put all
 * beacons in the Good Link Quality group and check only TC Connectivity
 * and Long Uptime flags.
 */
#define ZDO_WWAH_MIN_RSSI_FOR_RECEIVING_PACKETS 0

/**
 *  @brief Additional RSSI value to determine beacons by two groups:
 *         Good and Marginal Link Quality
 *
 *         See Zigbee WWAH Requirements spec,
 *         6.2 Commissioning Requirements, C-20
 *
 *         @ZDO_WWAH_RSSI_FOR_GOOD_LINK_QUALITY_GROUP
 */
/* TODO: maybe need to move it into the MAC module */
#define ZDO_WWAH_RSSI_FOR_RECEIVING_PACKETS_ADDING 0

/**
 *  @brief Min RSSI to determine a beacon in the Good Link Quality Group
 *
 *         Received beacons with an RSSI less that value,
 *         are put in the Marginal Link Quality group
 *
 *         See Zigbee WWAH Requirements spec,
 *         6.2 Commissioning Requirements, C-20
 */
#define ZDO_WWAH_RSSI_FOR_GOOD_LINK_QUALITY_GROUP  \
  (ZDO_WWAH_MIN_RSSI_FOR_RECEIVING_PACKETS         \
   + ZDO_WWAH_RSSI_FOR_RECEIVING_PACKETS_ADDING)

/**
 *  @brief Return ZB_TRUE if RSSI value is valid for Good Link Quality group
 */
#define ZDO_WWAH_GET_LINK_QUALITY_GROUP_BY_RSSI(rssi) \
  (zb_bool_t)((rssi) >= ZDO_WWAH_RSSI_FOR_GOOD_LINK_QUALITY_GROUP)

/* Let's compile that macro always */

/* low bit for TC Connectivity; high bit for Long Uptime */
#define ZB_ZDO_TC_CONNECTIVITY_BIT_POS 0U
#define ZB_ZDO_LONG_UPTIME_BIT_POS 1U

#define ZB_ZDO_SET_TC_CONNECTIVITY_BIT_VALUE(mask, val)               \
  if (ZB_U2B(val))                                                    \
  {                                                                   \
    ZB_SET_BIT_IN_BIT_VECTOR((mask), ZB_ZDO_TC_CONNECTIVITY_BIT_POS); \
  }                                                                   \
  else                                                                \
  {                                                                   \
    ZB_CLR_BIT_IN_BIT_VECTOR((mask), ZB_ZDO_TC_CONNECTIVITY_BIT_POS); \
  }

#define ZB_ZDO_SET_LONG_UPTIME_BIT_VALUE(mask, val)                   \
  if (ZB_U2B(val))                                                    \
  {                                                                   \
    ZB_SET_BIT_IN_BIT_VECTOR((mask), ZB_ZDO_LONG_UPTIME_BIT_POS);     \
  }                                                                   \
  else                                                                \
  {                                                                   \
    ZB_CLR_BIT_IN_BIT_VECTOR((mask), ZB_ZDO_LONG_UPTIME_BIT_POS);     \
  }


/* 0b00 */
#define ZB_ZCL_WWAH_NO_TC_CONNECTIVITY_AND_SHORT_UPTIME_MASK \
  (zb_uint8_t)(0)

/* 0b10 */
#define ZB_ZCL_WWAH_NO_TC_CONNECTIVITY_AND_LONG_UPTIME_MASK \
  (zb_uint8_t)(1 << ZB_ZDO_LONG_UPTIME_BIT_POS)

/* 0b01 */
#define ZB_ZCL_WWAH_TC_CONNECTIVITY_AND_SHORT_UPTIME_MASK \
  (zb_uint8_t)(1 << ZB_ZDO_TC_CONNECTIVITY_BIT_POS)

/* 0b11 */
#define ZB_ZCL_WWAH_TC_CONNECTIVITY_AND_LONG_UPTIME_MASK \
  ((zb_uint8_t)(1U << ZB_ZDO_TC_CONNECTIVITY_BIT_POS | 1U << ZB_ZDO_LONG_UPTIME_BIT_POS))


/** @internal Structure for beaconSurvey data type
 */

/**
 *  @brief Determine a parent choose priority (WWAH-Requirements C-20)
 *
 *  End Devices examine all beacons in the Good Link Quality group
 *  (if received beacon with an RSSI above @see minRssiForReceivingPackets
 *  +30 dbm) and choose the parent with highest parent priority, as shown below.
 *  If no suitable parents exist in the Good Link Quality group, then the End
 *  Device shall examine all beacons in the Marginal Link Quality group.
 */

/*! Invalid value for parent priority */
#define ZB_ZCL_WWAH_PARENT_PRIORITY_INVALID   0U
/*! 0b00 - no TC connectivity and Short Uptime or no WWAH parent */
#define ZB_ZCL_WWAH_PARENT_PRIORITY_VERY_LOW  1U
/*! 0b01 - no TC connectivity and Long Uptime */
#define ZB_ZCL_WWAH_PARENT_PRIORITY_LOW       2U
/*! 0b10 - TC connectivity and Short Uptime */
#define ZB_ZCL_WWAH_PARENT_PRIORITY_HIGH      3U
/*! 0b11 - TC connectivity and Long Uptime */
#define ZB_ZCL_WWAH_PARENT_PRIORITY_VERY_HIGH 4U

typedef zb_uint8_t zb_zcl_wwah_parent_priority_t;

/**
 *  @brief Returns parent priority by parent classification mask
 *
 *  @param mask Classification mask
 *  @return zb_zcl_wwah_parent_priority_t Parent priority
 */
zb_zcl_wwah_parent_priority_t zdo_wwah_get_parent_priority_by_classification_mask(zb_uint16_t mask);


/** After this interval ZB_NIB().nwk_long_uptime bit will be set to 1 */
#define ZDO_WWAH_LONG_UPTIME_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(24u*60u*60u)

/**
 *  @param Set 'WWAH Parent Classification is enabled' in ZDO ctx
 *
 *  @param enable ZB_TRUE if WWAH Parent Classification is enabled; ZB_FALSE otherwise
 */
void zdo_wwah_parent_classification_set(zb_bool_t enable);


/**
 *  @brief Compare neighbors by RSSI and WWAH Parent Priorities
 *         (see @zb_zcl_wwah_parent_priority_t) if WWAH Parent
 *         Classification is enabled; otherwise will be used only LQI value
 *
 *  @param nbte_first - pointer to the first neighbor table entry
 *  @param nbte_second - pointer to the second neighbor table entry
 */
zb_bool_t zdo_wwah_compare_neighbors(
  zb_ext_neighbor_tbl_ent_t *nbte_first, zb_ext_neighbor_tbl_ent_t *nbte_second);

/**
 *  @brief Compare neighbors by R23 metrics
 *         (see 3.6.1.5.2 Parent Selection)
 *
 *  @param nbte_first - pointer to the first neighbor table entry
 *  @param nbte_second - pointer to the second neighbor table entry
 *  @param parent_preference - bitmask with preference
 */
zb_bool_t zdo_nwk_compare_neighbors(zb_ext_neighbor_tbl_ent_t *nbte_first,
    zb_ext_neighbor_tbl_ent_t *nbte_second, zb_uint8_t parent_preference);

/** @} */ /* @addtogroup ZDO_WWAH_PARENT_CLASSIFICATION */

/** @} */ /* @addtogroup ZB_ZCL_WWAH */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */


#endif /* ZDO_WWAH_PARENT_CLASSIFICATION_H */
