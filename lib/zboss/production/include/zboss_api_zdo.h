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
/*  PURPOSE: Public ZDO layer API
*/
#ifndef ZB_ZBOSS_API_ZDO_H
#define ZB_ZBOSS_API_ZDO_H 1

#ifdef ZB_ENABLE_ZGP_SINK
#include "zboss_api_zgp.h"
#endif

/*! \addtogroup zb_comm_signals */
/*! @{ */

/**
 * @name ZDP status values
 * @anchor zdp_status
 * @see 2.4.5 - ZDP Enumeration Description
 */
/** @{ */
/** The requested operation or transmission was completed successfully. */
#define ZB_ZDP_STATUS_SUCCESS 0x00U

/*!< The supplied request type was invalid. */
#define ZB_ZDP_STATUS_INV_REQUESTTYPE 0x80U

/*!< The requested device did not exist on a device following a child descriptor request to a
 * parent.*/
#define ZB_ZDP_STATUS_DEVICE_NOT_FOUND 0x81U

/*!< The supplied endpoint was equal to 0x00 or between 0xf1 and 0xff. */
#define ZB_ZDP_STATUS_INVALID_EP 0x82U

/*!< The requested endpoint is not described by simple descriptor. */
#define ZB_ZDP_STATUS_NOT_ACTIVE 0x83U

/*!< The requested optional feature is not supported on the target device. */
#define ZB_ZDP_STATUS_NOT_SUPPORTED 0x84U

/*!< A timeout has occurred with the requested operation. */
#define ZB_ZDP_STATUS_TIMEOUT 0x85U

/*!< The end device bind request was unsuccessful due to a failure to match any suitable clusters.*/
#define ZB_ZDP_STATUS_NO_MATCH 0x86U

/*!< The unbind request was unsuccessful due to the coordinator or source device not having an
 * entry in its binding table to unbind.*/
#define ZB_ZDP_STATUS_NO_ENTRY 0x88U

/*!< A child descriptor was not available following a discovery request to a parent. */
#define ZB_ZDP_STATUS_NO_DESCRIPTOR 0x89U

/*!< The device does not have storage space to support the requested operation. */
#define ZB_ZDP_STATUS_INSUFFICIENT_SPACE 0x8aU

/*!< The device is not in the proper state to support the requested operation. */
#define ZB_ZDP_STATUS_NOT_PERMITTED 0x8bU

/*!< The device does not have table space to support the operation. */
#define ZB_ZDP_STATUS_TABLE_FULL 0x8cU

/*!< The permissions configuration table on the target indicates that the request is not authorized
 * from this device.*/
#define ZB_ZDP_STATUS_NOT_AUTHORIZED 0x8dU

/*!< The index in the received command is out of bounds. */
#define ZB_ZDP_STATUS_INVALID_INDEX 0x8fU

/**< The response was too large to fit in a single unfragmented message. */
#define ZB_ZDP_STATUS_FRAME_TOO_LARGE 0x90U

/**< A TLV required for processing the ZDO Request was absent from the
                                     * received ZDO message. */
#define ZB_ZDP_STATUS_BAD_KEY_NEG_METHOD 0x91U

/**< The request encountered a temporary failure but a retry at a later time should be attempted and may succeed. */
#define ZB_ZDP_STATUS_TEMPORARY_FAILURE 0x92U

/**< Custom internal statuses. */
#define ZB_ZDP_STATUS_DEV_ANNCE_SENDING_FAILED 0x0feU
#define ZB_ZDP_STATUS_TIMEOUT_BY_STACK 0xffU
/** @} */

/**
 * @brief Type for ZDP status values.
 *
 * Holds one of @ref zdp_status. Kept for backward compatibility as
 * @ref zdp_status were declared previously as enum.
 */
typedef zb_uint8_t zb_zdp_status_t;

/**
 * @name Signals passed to application signal handler
 * @anchor zdo_app_signal_type
 *
 * @note If an application requires the parameters to be passed to the @ref zboss_signal_handler, it
 * is recommended to introduce the structure according to the pattern below:
 *
 * @code
 * typedef struct zb_zdo_app_my_custom_signal_s
 * {
 *   zb_zdo_app_signal_type_t super;
 *   zb_my_custom_data_type_t my_custom_data;
 * }
 * @endcode
 */
/** @{ */
/** Obsolete signal, used for pre-R21 ZBOSS API compatibility. Not recommended to use.
                                 * @parblock
                                 * When generated:
 *  - After the device has been started and the non-BDB commissioning completed.
                                 *  - In case of the commissioning error.
                                 *
                                 * Status codes:
                                 *  - RET_OK: Device has started and joined the network.
                                 *  - RET_ERROR: An error of any type.
                                 *
                                 * Signal parameters:
 				 *  - none
 * @endparblock */
 #define ZB_ZDO_SIGNAL_DEFAULT_START 0U

/** Notifies the application that ZBOSS framework (scheduler, buffer pool, etc.) has started, but no
 * join/rejoin/formation/BDB initialization has been done yet.
 *
                                 * @parblock
                                 * When generated:
 *  - When the application calls zboss_start_no_autostart() instead of zboss_start() before the
 *    zboss_main_loop(). Used when some hardware must be initialized before the Zigbee commissioning,
 *    but already has ZBOSS scheduler running. Example: sensor connected through UART.
 *  - On macsplit architecture, when the application calls zboss_start_no_autostart() and both
 *    host, SoC components completed initialization procedure.
 *    This signal is always generated after ZB_MACSPLIT_DEVICE_BOOT in such case.
                                 *
                                 * Status codes:
                                 *  - RET_OK: Only ZBOSS framework has been initialized.
                                 *  - Does not return error status.
                                 *
                                 * Signal parameters:
 				                         *  - none
                                 *
                                 * @snippet smart_plug_v2/sp_device/sp_device.c signal_skip_startup
                                 * @endparblock */
#define ZB_ZDO_SIGNAL_SKIP_STARTUP 1U

/** Notifies the application about the new device appearance.
                                 * @parblock
                                 * When generated:
                                 *  - Upon receiving the Device_annce command.
                                 *
                                 * Status codes:
                                 *  - RET_OK:  Device_annce command was received.
                                 *  - Does not return error status.
                                 *
                                 * Signal parameters:
                                 * - @ref zb_zdo_signal_device_annce_params_t
                                 *
                                 * @snippet simple_gw/simple_gw.c signal_device_annce
                                 * @endparblock */
#define ZB_ZDO_SIGNAL_DEVICE_ANNCE 2U

/** Notifies the application that the device itself has left the network.
                                 * @parblock
                                 * When generated:
                                 *  - Upon sending the "Leave" command.
                                 *
                                 * Status codes:
                                 *  - RET_OK: Device has sent the "Leave" command.
                                 *  - Does not return error status.
                                 *
                                 * Signal parameters:
                                 *  - @ref zb_zdo_signal_leave_params_t
                                 *
 * @note All callbacks for ZDO requests will be cleared and will be never called.
 *       For example, if zb_zdo_simple_desc_req has been sent with cb argument set
 *       and leave received before ZDO cb called, then this cb will be never called later.
 *
                                 * @snippet light_sample/light_control/light_control.c signal_leave
 * @endparblock */
#define ZB_ZDO_SIGNAL_LEAVE 3U

/** Corrupted or incorrect signal information.
                                 * @parblock
                                 * When generated:
                                 *  - Incorrect buffer length detected by zb_get_app_signal (less then zb_zdo_app_signal_hdr_t)
                                 *
                                 * Status codes:
                                 *  - none
                                 *
                                 * Signal parameters:
                                 *  - none
                                 *  @endparblock */
#define ZB_ZDO_SIGNAL_ERROR 4U

/** Device started for the first time after the NVRAM erase.
                                     * @parblock
                                     * When generated:
 *  - Upon completing the typical device startup (network formation for ZC or
 *    rejoin/discovery/association for ZR and ZED).
                                     *  - This signal is exclusive to @ref ZB_BDB_SIGNAL_DEVICE_REBOOT
                                     *
                                     * @note No @ref ZB_BDB_SIGNAL_STEERING or @ref ZB_BDB_SIGNAL_FORMATION signals will be generated.
                                     *
                                     * Status codes:
                                     *  - RET_OK: Device started after the NVRAM erase
 *  - RET_INTERRUPTED: The operation was cancelled with zb_bdb_reset_via_local_action()
                                     *  - RET_ERROR: An error of any type.
                                     *
                                     * Signal parameters:
                                     *  - none
                                     *
                                     * @snippet HA_samples/on_off_switch/sample_zed.c signal_first
                                     *  @endparblock */
#define ZB_BDB_SIGNAL_DEVICE_FIRST_START 5U

/** Device started using the NVRAM contents.
                                     * @parblock
                                     * When generated:
                                     *  - Upon the device joining/rejoining Zigbee network using restored parameters.
                                     *  - This signal is exclusive to @ref ZB_BDB_SIGNAL_DEVICE_FIRST_START
                                     *
                                     * @note No @ref ZB_BDB_SIGNAL_STEERING or @ref ZB_BDB_SIGNAL_FORMATION signals will be generated.
                                     *
                                     * Status codes:
                                     *  - RET_OK: Device started using configuration stored in NVRAM
 *  - RET_INTERRUPTED: The operation was cancelled with zb_bdb_reset_via_local_action()
                                     *  - RET_ERROR: An error of any type.
                                     *
                                     * Signal parameters:
                                     *  - none
                                     *
                                     * @snippet HA_samples/on_off_switch/sample_zed.c signal_reboot
                                     * @endparblock */
#define ZB_BDB_SIGNAL_DEVICE_REBOOT 6U

/** @cond DOXYGEN_BDB_SECTION */
#define ZB_SIGNAL_DEVICE_FIRST_START ZB_BDB_SIGNAL_DEVICE_FIRST_START
#define ZB_SIGNAL_DEVICE_REBOOT ZB_BDB_SIGNAL_DEVICE_REBOOT
#ifdef ZB_ENABLE_ZLL
/**
 * @cond DOXYGEN_TOUCHLINK_FEATURE
 *    @addtogroup touchlink
 * @{ */

/** Informs the Touchlink initiator about the new network has been created.
                                     * @parblock
                                     * When generated:
                                     *  - Upon generating the Network Start Response during the Touchlink commissioning procedure.
                                     *
                                     * Status codes:
                                     *  - RET_OK: New Zigbee network created.
                                     *  - Does not return error status.
                                     *
                                     * Signal parameters:
                                     *  - @ref zb_bdb_signal_touchlink_nwk_started_params_t
                                     *
                                     * @snippet lighting/dimmable_light_tl/light_controller_zed.c signal_touchlink_nwk_started
                                     *  @endparblock */
#define ZB_BDB_SIGNAL_TOUCHLINK_NWK_STARTED 7U

/** Inform the Touchlink initiator that the new router joined the network.
                                     * @parblock
                                     * When generated:
                                     *  - Upon receiving the Commissioning Network Join Router Response.
                                     *
                                     * Status codes:
                                     *  - RET_OK: New router joined the network.
                                     *  - Does not return error status.
                                     *
                                     * Signal parameters:
                                     *  - @ref zb_bdb_signal_touchlink_nwk_started_params_t
                                     *
                                     * @snippet lighting/dimmable_light_tl/light_controller_zed.c signal_touchlink_nwk_joined_router
                                     * @endparblock */
#define ZB_BDB_SIGNAL_TOUCHLINK_NWK_JOINED_ROUTER 8U

/** Touchlink commissioning done.
                                     * @parblock
                                     * When generated:
                                     *  - Touchlink initiator device joined the network.
                                     *
                                     * Status codes:
                                     *  - ZB_BDB_STATUS_SUCCESS: Commissioning successful.
                                     *  - ZB_BDB_STATUS_NO_SCAN_RESPONSE:
 *    1. No Touchlink commissioning cluster scan response inter-PAN commands were received
 *    2. No touchlink commissioning cluster scan response inter-PAN commands were received with the
 *       inter-PAN transaction identifier field equal to that used by the initiator in its scan request
                                     *    command.
 *  - RET_INTERRUPTED: The operation was cancelled with zb_bdb_reset_via_local_action()
                                     *
                                     * Signal parameters:
                                     *  - none
                                     * @endparblock */
#define ZB_BDB_SIGNAL_TOUCHLINK 9U
/** @} */  /* touchlink */
/** @endcond */ /* DOXYGEN_TOUCHLINK_FEATURE */
#endif /*ZB_ENABLE_ZLL*/

/** BDB network steering completed (Network steering only).
 *
 * @parblock
 * When generated:
 *  - Upon completing Network steering initiated by
 *    bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING).
 *
 * Status codes:
 *  - RET_OK: Network steering completed.
 *  - RET_INTERRUPTED: was cancelled with bdb_cancel_joining()
 *    or with zb_bdb_reset_via_local_action()
 *
 * Has additional data of type zb_zdo_signal_leave_indication_params_t.
 *
 * Signal parameters:
 *  - none
 * @endparblock */
#define ZB_BDB_SIGNAL_STEERING 10U

/** BDB network formation completed (Network formation only).
 *
 * @parblock
 * When generated:
 *  - Upon completing Network formation initiated by
 *    bdb_start_top_level_commissioning(ZB_BDB_NETWORK_FORMATION).
 *
 * Status codes:
 *  - RET_OK: Network formation completed.
 *  - RET_INTERRUPTED: was cancelled with bdb_cancel_formation()
 *    or with zb_bdb_reset_via_local_action()
 *
 * Signal parameters:
 *  - none
 * @endparblock */
#define ZB_BDB_SIGNAL_FORMATION 11U

/** BDB finding and binding for a target endpoint completed.
 *
 * @parblock
 * When generated:
 *  - F&B target timeout expires.
 *
 * Status codes:
 *  - RET_OK: F&B target identifying time is expired.
 *  - RET_CANCELLED: F&B target identifying is cancelled during the IdentifyTime.
 *  - RET_ERROR: An error of any type.
 *
 * Signal parameters:
 * - @ref zb_uint8_t - endpoint ID
 * @endparblock */
#define ZB_BDB_SIGNAL_FINDING_AND_BINDING_TARGET_FINISHED 12U

/** BDB finding and binding for an initiator endpoint completed.
 *
 * @parblock
 * When generated:
 *  - F&B with a Target succeeded or F&B initiator timeout expired or cancelled.
 *
 * Status codes:
 *  - @ref zdo_fb_initiator_finished_status
 *
 * Signal parameters:
 *  - @ref zb_zdo_signal_fb_initiator_finished_params_t
 * @endparblock */
#define ZB_BDB_SIGNAL_FINDING_AND_BINDING_INITIATOR_FINISHED 13U

#ifdef ZB_ENABLE_ZLL
/**
 * @cond DOXYGEN_TOUCHLINK_FEATURE
 *   @addtogroup touchlink
 * @{ */

/** Touchlink procedure started.
 *
 * @parblock
 * When generated:
 *  - Touchlink procedure started on the Target device.
 *
 * Status codes:
 *  - RET_OK: Procedure started on the Target device
 *  - RET_INTERRUPTED: The operation was cancelled with zb_bdb_reset_via_local_action()
 *
 * Signal parameters:
 *  - none
 * @endparblock */
#define ZB_BDB_SIGNAL_TOUCHLINK_TARGET 14U

/** Touchlink Target network started (Target only).
 *
 * @parblock
 * When generated:
 *  - Touchlink target initiated by bdb_touchlink_target_start().
 *
 * Status codes:
 *  - RET_OK: Touchlink network started successfully.
 *  - RET_INTERRUPTED: The operation was cancelled with zb_bdb_reset_via_local_action()
 *
 * Signal parameters:
 *  - none
 * @endparblock */
#define ZB_BDB_SIGNAL_TOUCHLINK_NWK 15U

/** Touchlink Target finished (Target only).
 *
 * @parblock
 * When generated:
 *  - Touchlink target finished
 *
 * Status codes:
 *  - RET_OK: Touchlink target finished successfully.
 *  - Does not return error status.
 *
 * Signal parameters:
 *  - none
 * @endparblock */
#define ZB_BDB_SIGNAL_TOUCHLINK_TARGET_FINISHED 16U

#define ZB_BDB_SIGNAL_TOUCHLINK_ADD_DEVICE_TO_NWK 17U

/** @} */ /* touchlink */
/** @endcond */ /* DOXYGEN_TOUCHLINK_FEATURE */
#endif /*ZB_ENABLE_ZLL*/

/** @endcond */ /* DOXYGEN_BDB_SECTION */

/** Obsolete signal, used for pre-R21 ZBOSS API compatibility. Use ZB_ZDO_SIGNAL_DEVICE_ANNCE signal
 * instead!
 *
 * @parblock
 * When generated:
 *  - Indicates that there is a new device associated with Zigbee Coordinator or router.
 *
 * Status codes:
 *  - RET_OK: New device associated.
 *  - Does not return error status.
 *
 * Signal parameters:
 *  - @ref zb_nwk_signal_device_associated_params_t
 * @endparblock */
#define ZB_NWK_SIGNAL_DEVICE_ASSOCIATED 18U

/** Network leave indication.
 *
 * @parblock
 * When generated:
 *  - The child device has left the network.
 *
 * Status codes:
 *  - RET_OK: Leave command received from the child device.
 *  - RET_ERROR: An error of any type.
 *
 * Signal parameters:
 *  - @ref zb_zdo_signal_leave_indication_params_t
 *
 * @snippet simple_gw/simple_gw.c signal_leave_ind
 * @endparblock */
#define ZB_ZDO_SIGNAL_LEAVE_INDICATION 19U

/** WWAH Rejoin start indication.
 *
 * @parblock
 * When generated:
 *  - Device lost communication with the parent and started WWAH Rejoin procedure.
 *
 * Status codes:
 *  - RET_OK: Leave command received from the child device.
 *  - Does not return error status.
 *
 * Signal parameters: no
 * @endparblock */
#define ZB_BDB_SIGNAL_WWAH_REJOIN_STARTED 20U

/** @cond DOXYGEN_ZGP_SECTION */
/** ZGP commissioning signal.
 *
 * @parblock
 * When generated:
 *  - GPCB commissioned/decommissioned a device.
 *
 * Status codes:
 * - RET_OK: commissioning/decommissioned complete.
 *  - Does not return error status.
 *
 * Signal parameters:
 *  - zb_zgp_signal_commissioning_params_s
 *
 * @snippet light_sample/light_coordinator_combo/light_zc.c signal_zgp_commissioning
 * @endparblock */
#define ZB_ZGP_SIGNAL_COMMISSIONING 21U
/** @endcond */ /* DOXYGEN_ZGP_SECTION */

/** Notifies the application that the MCU can sleep for the specified period of time.
 * @note Signal will not be generated if the sleep interval is less than sleep threshold
 * ZB_SCHED_SLEEP_THRESHOLD_MS. To change the sleep threshold see zb_ret_t zb_sleep_set_threshold().
 *
 *
 * @parblock
 *
 * When generated:
 *  - ZBOSS scheduler detects that the sleep mode can be enabled on the device.
 *
 * Status codes:
 * - RET_OK: The sleep mode can be enabled on the device.
 * - Does not return error status.
 *
 * Signal parameters:
 *  - @ref zb_zdo_signal_can_sleep_params_t
 *
 * @snippet onoff_server/on_off_switch_zed.c signal_can_sleep
 *
 * @endparblock
 */
#define ZB_COMMON_SIGNAL_CAN_SLEEP 22U

/** Notifies the application that the application specific part of the production configuration was
 * found and restored and provides the application with this data.
 *
 * @parblock
 * When generated:
 *  - After restoring the production configuration.
 *
 * Status codes:
 *  - RET_OK: The application specific part of the production configuration exists.
 *  - RET_ERROR: An error of any type.
 *
 * Signal parameters:
 *  - Passed up with application specific data from production configuration.
 *
 * @endparblock
 */
#define ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY 23U

/** Notifies the application about the Neighbor Table expiration.
 * It means that the neighbor devices did not send the Link Status messages for @ref
 * ZB_NWK_ROUTER_AGE_LIMIT * @ref ZB_NWK_LINK_STATUS_PERIOD seconds.
 *
 * @parblock
 * When generated:
 *  - All routes expired (usually when the link is down).
 *
 * Status codes:
 *  - RET_OK: All routers expired.
 *  - Does not return error status.
 *
 * Signal parameters:
 *  - none
 *  @endparblock */
#define ZB_NWK_SIGNAL_NO_ACTIVE_LINKS_LEFT 24U

#if defined ZB_ENABLE_SE_MIN_CONFIG || defined DOXYGEN
  /* SE signals */
/** @cond DOXYGEN_SE_SECTION */

/** Device started. No auto-join/rejoin configured, no join attempts were performed, control is
                                                 * passed to application.
                                                 * @par Signal parameter
 *  - N/A
                                                 */
#define ZB_SE_SIGNAL_SKIP_JOIN 25U

/** SE Rejoin start indication.
 *
 * @parblock
 * When generated:
 *  - Device lost communication with the parent and started SE rejoin procedure.
 *
 * Status codes:
 *  - RET_OK: Leave command received from the child device.
 *  - Does not return error status.
 *
 * Signal parameters: no
 * @endparblock */
#define ZB_SE_SIGNAL_REJOIN_STARTED ZB_BDB_SIGNAL_WWAH_REJOIN_STARTED

/** We are rejoined
                                                 * @par Signal parameter
 *  - N/A
                                                 */
#define ZB_SE_SIGNAL_REJOIN 26U

/** Our child rejoined.
                                                 * @par Signal parameter
                                                 * @ref zb_ieee_addr_t - long remote device address
                                                 */
#define ZB_SE_SIGNAL_CHILD_REJOIN 27U

/** Some device joined & authenticated in the net, established TCLK using CBKE procedure. Only TC
 * can receive that signal.
                                                 * @par Signal parameter
                                                 * @ref zb_ieee_addr_t - long remote device address
                                                 * @par Example
                                                 * @snippet se/energy_service_interface/se_esi_zc.c SIGNAL_HANDLER_GET_SIGNAL
                                                 * @snippet se/energy_service_interface/se_esi_zc.c SIGNAL_HANDLER_TC_SIGNAL_CHILD_JOIN
                                                 */
#define ZB_SE_TC_SIGNAL_CHILD_JOIN_CBKE 28U

/** Some device joined & authenticated in the net, established TCLK, but using non-CBKE procedure
 * (BDB). Only TC can receive that signal.
                                                 * @par Signal parameter
                                                 * @ref zb_ieee_addr_t - long remote device address
                                                 */
#define ZB_SE_TC_SIGNAL_CHILD_JOIN_NON_CBKE 29U

/** CBKE procedure failed. Application may set another certificate & key and retry CBKE procedure.
                                                 * @par Signal parameter
                                                 * N/A
                                                 * @par Example
 * Signal processing code for @ref ZB_SE_SIGNAL_CBKE_OK can be used as a template
                                                 * @see zb_se_load_ecc_cert - set certificate
                                                 * @see zb_se_retry_cbke_with_tc - retry CBKE procedure
                                                 */
#define ZB_SE_SIGNAL_CBKE_FAILED 30U

/** CBKE procedure with TC succeed.
                                                 * Note: TC can't receive that signal.
                                                 * @par Signal parameter
                                                 * N/A
                                                 * @par Example
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_GET_SIGNAL
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_CBKE_OK
                                                 */
#define ZB_SE_SIGNAL_CBKE_OK 31U

/** Ready to start Service Discovery. Application can issue @ref zb_se_service_discovery_start to
 * actually start discovery
                                                 * @par Signal parameter
                                                 * N/A
                                                 * @par Example
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_GET_SIGNAL
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_START_DISCOVERY
                                                 */
#define ZB_SE_SIGNAL_SERVICE_DISCOVERY_START 32U

/** Service Discovery found a device that can be bound.
                                                 * @par Signal parameter
                                                 * @ref zb_se_signal_service_discovery_bind_params_t
                                                 * @par Example
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_GET_SIGNAL
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_DO_BIND
                                                 * @see zb_se_signal_service_discovery_bind_params_t
                                                 * @see ZB_ZDO_SIGNAL_GET_PARAMS
                                                 * @see zb_se_service_discovery_bind_req
                                                 */
#define ZB_SE_SIGNAL_SERVICE_DISCOVERY_DO_BIND 33U

/** Bind request initiated by @ref zb_se_service_discovery_bind_req() succeeded.
                                                 * @par Signal parameter
                                                 * @ref zb_uint16_t - short remote device address
                                                 * @par Example
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_GET_SIGNAL
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_BIND_OK
                                                 */
#define ZB_SE_SIGNAL_SERVICE_DISCOVERY_BIND_OK 34U

/** Bind request failed.
                                                 * @par Signal parameter
                                                 * @ref zb_uint16_t - short remote device address
                                                 * @par Example
 * The same code to @ref ZB_SE_SIGNAL_SERVICE_DISCOVERY_BIND_OK can be used for signal processing.
                                                 */
#define ZB_SE_SIGNAL_SERVICE_DISCOVERY_BIND_FAILED 35U

/** Successful bind initiated by other device.
                                                   * @par Signal parameter
                                                   * @ref zb_se_signal_service_discovery_bind_params_t
                                                   * @par Example
                                                   * @snippet se/energy_service_interface/se_esi_zc.c SIGNAL_HANDLER_GET_SIGNAL
                                                   * @snippet se/energy_service_interface/se_esi_zc.c SIGNAL_HANDLER_BIND_INDICATION
                                                   */
#define ZB_SE_SIGNAL_SERVICE_DISCOVERY_BIND_INDICATION 36U

/** Service Discovery succeeded
                                                 * @par Signal parameter
                                                 * N/A
                                                 * @par Example
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_GET_SIGNAL
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_DISCOVERY_OK
                                                 */
#define ZB_SE_SIGNAL_SERVICE_DISCOVERY_OK 37U

/** Service Discovery failed (no devices found)
                                                 * @par Signal parameter
                                                 * N/A
                                                 * @par Example
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_GET_SIGNAL
                                                 * @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_DISCOVERY_FAILED
                                                 */
#define ZB_SE_SIGNAL_SERVICE_DISCOVERY_FAILED 38U

/** Partner link key initiated by @ref zb_se_start_aps_key_establishment is done. Keypair with some
 * device is ready.
                                                 * @par Signal parameter
                                                 * @ref zb_ieee_addr_t - long remote device address
                                                 * @see zb_se_start_aps_key_establishment
                                                 */
#define ZB_SE_SIGNAL_APS_KEY_READY 39U

/** Failed to establish a keypair
                                                 * @par Signal parameter
                                                 * @ref zb_ieee_addr_t - long remote device address
                                                 * @see zb_se_start_aps_key_establishment
                                                 */
#define ZB_SE_SIGNAL_APS_KEY_FAIL 40U

#endif /* ZB_ENABLE_SE_MIN_CONFIG */

/** ZC / our MAC suspended our device tx: out of Duty cycle (sub-ghz only)
                                                 * @par Signal parameter - number of minutes to suspend, -1 if unknown
                                                 */
#define ZB_SIGNAL_SUBGHZ_SUSPEND 41U

/** ZC resumed our device tx (sub-ghz only) */
#define ZB_SIGNAL_SUBGHZ_RESUME 42U

/** @endcond */ /* DOXYGEN_SE_SECTION */
#ifdef ZB_MACSPLIT
/** Notifies the application about macsplit mac device boot.
 *
 * When generated:
 *  - When MAC device completed initialization procedure.
 *  - When the application calls zboss_start_no_autostart() and both
 *    host, SoC components have completed initialization procedure.
 *    This signal is always generated before @ref ZB_ZDO_SIGNAL_SKIP_STARTUP in such case.
 *
 * Status codes:
 *  - RET_OK: Device information updated.
 *  - Does not return error status.
 * Signal parameters:
 *  - @ref zb_zdo_signal_macsplit_dev_boot_params_t
 */
#define ZB_MACSPLIT_DEVICE_BOOT              43U

#define ZB_MACSPLIT_DEVICE_READY_FOR_UPGRADE 44U /*!< macsplit mac device is ready for upgrade */

/** macsplit device upgrade event: device ready for upgrade or error indication */
#define ZB_MACSPLIT_DEVICE_FW_UPGRADE_EVENT  45U

#endif /*ZB_MACSPLIT*/

#ifdef NCP_MODE

#define ZB_SIGNAL_NWK_INIT_DONE 46U

#endif /* NCP_MODE */

/** Notifies the Zigbee Trust center application about a new device is authorized in the network.
 * For Zigbee R21 devices it means that the TCLK exchange procedure was finished, for the legacy
 * devices it means that the Transport key was send to the device.
                                          *
                                          * When generated:
                                          *  - Authorization success
                                          *  - Authorization failed
                                          *  - Authorization timeout
                                          *
                                          * Status codes:
                                          *  - RET_OK: Device information updated.
                                          *  - Does not return error status.
                                          * Signal parameters:
                                          *  - @ref zb_zdo_signal_device_authorized_params_t
                                          */
#define ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED 47U

/** Notifies the Zigbee Trust center or parent router application about a new device joined,
 * rejoined or left the network.
                                          *
                                          * When generated:
                                          *  - Standard device secured rejoin
                                          *  - Standard device unsecured join
                                          *  - Device left
                                          *  - Standard device trust center rejoin
                                          *
                                          * Status codes:
                                          *  - RET_OK: Device information updated.
                                          *  - Does not return error status.
                                          *
                                          * Signal parameters:
                                          *  - @ref zb_zdo_signal_device_update_params_t
                                          */
#define ZB_ZDO_SIGNAL_DEVICE_UPDATE 48U

/** Notifies the Zigbee Trust center that new device is ready for Device Interview.
 *
 * @param[in] param_in @ref zb_zdo_signal_device_ready_for_interview_params_t
 *
 * TC can exchange APS encrypted APS Data (including ZDO) with Joiner during Device Interview.
 * TC can finish Device Interview by calling either zb_tc_auth_device_after_interview() or zb_tc_reject_device_after_interview ().
 *
 * Status codes:
 *  - RET_OK: Device is ready and can be authorized.
 *
 * Signal parameters:
 *  - @ref zb_zdo_signal_device_ready_for_interview_params_t
 *
 * @snippet r23_new_api/r23_zc.c device_interview1_snippet
 * @snippet r23_new_api/r23_zc.c device_interview2_snippet
 * @snippet r23_new_api/r23_zc.c device_interview3_snippet
 */
#define ZB_ZDO_SIGNAL_DEVICE_READY_FOR_INTERVIEW 49U

/** Notifies the Joiner device that Device Interview was started
 *
 * Status codes:
 *  - RET_OK: Device Interview is started.
 *
 * Device Interview is finished when device is authorized.
 *
 * @snippet r23_new_api/r23_zed.c device_interview4_snippet
 */
#define ZB_ZDO_SIGNAL_DEVICE_INTERVIEW_STARTED 50U

/** PAN ID conflict has been detected. Application has to decide whether to initiate its resolution
                                                 * or not.
                                                 * @par Signal parameter
                                                 * @see zb_start_pan_id_conflict_resolution
                                                */
#define ZB_NWK_SIGNAL_PANID_CONFLICT_DETECTED 51U

/** NLME status indication. Application may implement additional logic based on receiving some
                                       * statuses.
                                       * @par Signal parameter
                                       * @ref zb_zdo_signal_nlme_status_indication_params_t
 * @see zb_nwk_command_status_t is the full list of NLME statuses
                                       */
#define ZB_NLME_STATUS_INDICATION 52U

/**
   @addtogroup tc_swap
   @{
*/

  /**
   it is time to backup TC database (signal at TC)

   @snippet tc_swap_out/tcso_zc.c tcsw_backup1_snippet
   @snippet tc_swap_out/tcso_zc.c tcsw_backup2_snippet
  */
#define ZB_TCSWAP_DB_BACKUP_REQUIRED_SIGNAL 53U

  /**
     TC is swapped (signal at Joiner)

     @snippet tc_swap_out/tcso_zed.c tcsw_cli_sig_snippet
  */
#define ZB_TC_SWAPPED_SIGNAL 54U

/*! @} tc_swap  */


  /**
     TCLK update completed (signal at Joiner)

     @snippet tc_swap_out/tcso_zed.c tclk_updated_snippet
  */
#define ZB_TCLK_UPDATED_SIGNAL 55U

/**
Signal indicating that device just joined/rejoined, but not authorized yet

To be used mainly for certification testing, when run-time visibility setting is required

@snippet tc_swap_out/tcso_zed.c join_done_snippet
*/
#define ZB_SIGNAL_JOIN_DONE 56U

/** Test Profile 2 Buffer Test Req received. To be used in certification tests .

    @param zb_buffer_test_response_t
*/
#define ZB_BUFFER_TEST_REQ_SIGNAL 57U

/** TC rejoin is completed
 *
 * @parblock
 * When generated:
 *  - Device completes TC rejoin procedure.
 *
 * Status codes:
 *  - RET_OK: TC rejoin completed successful
 *  - RET_ERROR: An error of any type.
 *
 * Signal parameters:
 *  - none
 *
 * @snippet onoff_server/on_off_switch_zed.c signal_bdb_tc_rejoin_done
 * @endparblock */
#define ZB_BDB_SIGNAL_TC_REJOIN_DONE 58U


/** Notifies the Zigbee Trust center or router application about permit join status changed.
 * @par When generated:
 *  - Network was just open
 *  - Network was just closed
 *
 * @par Status codes:
 *  - RET_OK: Device information updated.
 *  - Does not return error status.
 *
 * @par Signal parameters:
 *  - @ref zb_uint8_t - Open time or zero if network closed. Value of @ref zb_zdo_mgmt_permit_joining_req_param_t.permit_duration
 *
 * @par Signal handling:
 * @snippet thermostat/thermostat_zr/thermostat_zr.c zb_zdo_signal_permit_join_status_example
 *
 * @par Signal generation:
 * - To self:
 * @snippet thermostat/thermostat_zr/thermostat_zr.c permit_join_request_local_example
 * - To other devices:
 * @snippet thermostat/thermostat_zr/thermostat_zr.c permit_join_request_broadcast_example
 *
 */
#define ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS 59U

 /**
  * Notifies the TC that Device Interview was finished
  *
  * Status codes:
  *  - RET_OK: DDevice interview finished succeed
  *  - RET_ERROR: Device interview failed and the device was removed.
  *
  * Signal parameters:
  *  - @ref zb_zdo_signal_device_ready_for_interview_params_t
  *
  * @snippet r23_new_api/r23_zc.c device_interview1_snippet
  * @snippet r23_new_api/r23_zc.c device_interview2_snippet
  * @snippet r23_new_api/r23_zc.c device_interview3_snippet
  */
 #define ZB_ZDO_SIGNAL_DEVICE_INTERVIEW_FINISHED 60U

/** BDB steering cancel request processed
 *
 * @parblock
 * When generated:
 *  - after the cancel request called with bdb_cancel_steering() is processed
 *
 * Status codes:
 * - RET_ILLEGAL_REQUEST: device is a ZC
 * - RET_INVALID_STATE: steering for a node not a network is not in progress
 * - RET_PENDING: it is too late to cancel a steering, it will be completed soon
 * - RET_IGNORE: cancellation was already requested
 * - RET_OK: steering is cancelled successfully
 *
 * Signal parameters:
 *  - none
 *
 * @endparblock */
#define ZB_BDB_SIGNAL_STEERING_CANCELLED 61U


/** BDB formation cancel request processed
 *
 * @parblock
 * When generated:
 *  - after the cancel request called with bdb_cancel_formation() is processed
 *
 * Status codes:
 * - RET_INVALID_STATE: formation is not in progress
 * - RET_PENDING: it is too late to cancel a formation, it will be completed soon
 * - RET_IGNORE: cancellation was already requested
 * - RET_OK: formation is cancelled successfully
 *
 * Signal parameters:
 *  - none
 *
 * @endparblock */
#define ZB_BDB_SIGNAL_FORMATION_CANCELLED 62U

/** ZBOSS is ready to shutdown signal
 *
 * @parblock
 * When generated:
 *  - after ZBOSS preparations to shutdown initiated by zboss_start_shut() is done
 *
 * After receiving that signal application MUST complete ZBOSS shutdown by
 * calling zboss_complete_shut(). It is impossible to continue ZBOSS work
 * without a restart after calling zboss_start_shut().
 *
 * Signal parameters:
 *  - none
 *
 * @endparblock */
#define ZB_SIGNAL_READY_TO_SHUT           63U

/** ZBOSS interpan preinit done signal
 *
 * @parblock
 * When generated:
 *  - after ZBOSS preinit enough to send interpan initiated by zboss_preinit_for_interpan() is done
 *
 * After receiving that signal application can use zb_intrp_data_request_with_chan_change() API
 *
 * Signal parameters:
 *  - none
 *
 * @endparblock */
#define ZB_SIGNAL_INTERPAN_PREINIT        64U

/** @cond DOXYGEN_ZGP_SECTION */
/** ZGP Mode change signal.
 *
 * @parblock
 * When generated:
 *  - GPCB Sink change mode between operational mode and commissioning mode.
 *
 * Status codes:
 * - RET_OK: mode changed.
 *  - Does not return error status.
 *
 * Signal parameters:
 *  - zb_zgp_signal_mode_change_params_s
 *
 * @snippet simple_combo/zc_combo.c zgp_signal_mode_change
 *
 * @endparblock */
#define ZB_ZGP_SIGNAL_MODE_CHANGE 65U
/** @endcond */ /* DOXYGEN_ZGP_SECTION */

/** Notifies an application a device unavailable.
 *
 * When generated:
 *  - ZBOSS could not send a packet over NWK, for example:
 *      there is no ACK on the MAC layer;
 *      there is no response to a network address request;
 *  - ZBOSS could not send a packet over APS, for example:
 *      there is no APS-ACK to an APS packet.
 *
 * Signal parameters:
 *  - @ref zb_zdo_device_unavailable_params_t
 */
#define ZB_ZDO_DEVICE_UNAVAILABLE 66U

/** @cond DOXYGEN_ZGP_SECTION */
/** ZGP Approve Commissioning signal.
 * @parblock
 * When generated:
 *  - ZGP subsystem is ready to create new pairing but APP should check
 *    if GPD application functionality matches to continue pairing.
 *
 * Status codes:
 * - RET_OK.
 *
 * After receiving that signal application SHALL decide whether to connect
 * GP device or not. After that, application SHALL send acceptance status
 * by calling zb_zgps_accept_commissioning(). It is impossible to continue
 * GP commissioning operation without a calling zb_zgps_accept_commissioning(),
 * and the sink will exit commissioning mode after commissioning timeout
 * expired.
 *
 * Signal parameters:
 *  - @ref zb_zgp_signal_approve_comm_params_t
 *
 * @snippet simple_combo/zc_combo.c zgp_signal_approve_comm
 * @endparblock */
#define ZB_ZGP_SIGNAL_APPROVE_COMMISSIONING 67U
/** @endcond */ /* DOXYGEN_ZGP_SECTION */

/**
 * When generated:
 *   Unverified aps key created, and verify_key_req has been sent or received.
 *
 * Status codes:
 * - RET_OK.
 *
 * This signal uses during debugging to broadcast aps key by calling zb_debug_broadcast_aps_key().
 *
 * Signal parameters:
 *  - @ref zb_debug_signal_tclk_ready_params_t
*/
#define ZB_DEBUG_SIGNAL_TCLK_READY 68U
/** @} */

/**
 * @brief Type for signals passed to an application signal handler.
 *
 * Holds one of @ref zdo_app_signal_type.
 * Previously it was defined as enum but was converted to a set of defines due to MISRA violations.
 */
typedef zb_uint8_t zb_zdo_app_signal_type_t;

/* Legacy API: zb_zdo_app_signal_type_t == zb_zdo_app_signal_t */
typedef zb_zdo_app_signal_type_t zb_zdo_app_signal_t;

/**
   Obtains pointer to parameters passed with application signal.

   @param[in] sg_p - pointer to application signal
   @param[in] type - parameter type
   @return pointer to signal parameters

@b Example
@snippet linky_sample/erl_interface/erl_interface_zed.c zb_zdo_signal_get_params_snippet
  */
#define ZB_ZDO_SIGNAL_GET_PARAMS(sg_p, type) ((type *)(void *)((((zb_uint8_t *)sg_p) + sizeof(zb_zdo_app_signal_hdr_t))))


/**
   @brief Cuts ZBOSS signal headers while keeping the signal parameter (if it was in the buffer).

   @param buf - pointer to a zb_buf_t structure. The buffer must be the one passed into zb_signal_handler_t.
 */
#define ZB_ZDO_SIGNAL_CUT_HEADER(buf) zb_buf_cut_left(buf, sizeof(zb_zdo_app_signal_hdr_t))

/**
   @brief Leave parameters

   Stack passes this parameter to application when device leave the network.
  */
typedef struct zb_zdo_signal_leave_params_s
{
  zb_nwk_leave_type_t leave_type; /** Leave type. @ref nwk_leave_type */
} zb_zdo_signal_leave_params_t;

/**
   @brief NLME status indication parameters

   Stack passes this parameter to application when NLME status indication is generated.
  */
typedef struct zb_zdo_signal_nlme_status_indication_params_s
{
  zb_nlme_status_indication_t nlme_status; /** NLME status indication payload */
} zb_zdo_signal_nlme_status_indication_params_t;

/**
   @brief Leave Indication parameters

   Stack passes this parameter to application upon receipt of leave indication
   primitive.
  */
typedef struct zb_zdo_signal_leave_indication_params_s
{
  zb_uint16_t short_addr;     /*!< Short address of device requested to leave or leaving device*/
  zb_ieee_addr_t device_addr; /*!< Long address of device requested to leave or leaving device*/
  zb_uint8_t rejoin;          /*!< 1 if this was leave with rejoin; 0 - otherwise */
} zb_zdo_signal_leave_indication_params_t;

/** @cond DOXYGEN_SE_SECTION */
/**
 *  @brief Service Discovery Bind signal parameters
 *  @details Contains info about the device found during Service Discovery. Stack passes this
 *           parameter to the application with signal @ref ZB_SE_SIGNAL_SERVICE_DISCOVERY_DO_BIND
 */
typedef struct zb_se_signal_service_discovery_bind_params_s
{
  zb_ieee_addr_t device_addr;   /**< Long address of the device */
  zb_uint8_t endpoint;          /**< Device endpoint */
  zb_uint16_t cluster_id;       /**< Discovered cluster */
  zb_uint8_t commodity_type;    /**< Commodity type (for multiple commodity networks) */
} zb_se_signal_service_discovery_bind_params_t;
/** @endcond */

#if defined ZB_ENABLE_ZGP_SINK || defined DOXYGEN
/** @cond DOXYGEN_ZGP_SECTION */
/**
   @brief ZGP commissioning parameters

   Stack passes this parameter to application to notify about GPD commissioning
   result.
  */
typedef struct zb_zgp_signal_commissioning_params_s
{
  zb_zgpd_id_t zgpd_id;       /*!< Pointer to GPD ID */
  zb_zgp_comm_status_t result; /*!< commissioning result, see @ref zb_zgp_comm_status_t */
} zb_zgp_signal_commissioning_params_t;

/**
   @brief ZGP approve commissioning parameters

   Stack passes this parameter to application to notify about attempt GPD commissioning.
   Due to limited buffer memory, a pointer to the structure is passed.
  */
typedef struct zb_zgp_signal_approve_comm_params_s
{
  zgp_approve_comm_params_t *params;
} zb_zgp_signal_approve_comm_params_t;

/**
   @brief ZGP mode changed parameters

   Stack passes this parameter to application to notify about GP mode change.
  */
typedef struct zb_zgp_signal_mode_change_params_s
{
  zb_zgp_mode_change_reason_t reason; /*!< mode change reason, see @ref zb_zgp_mode_change_reason_t */
  zb_zgp_mode_t new_mode;
} zb_zgp_signal_mode_change_params_t;
/** @endcond */
#endif  /* ZB_ENABLE_ZGP_SINK */

/**
   @brief Device unavailable signal parameters
 */
typedef struct zb_zdo_device_unavailable_params_s
{
  zb_ieee_addr_t long_addr; /*!< Long address of a device */
  zb_uint16_t short_addr;   /*!< Short address of a device */
} zb_zdo_device_unavailable_params_t;

/**
   @brief Association parameters

   Stack passes this parameter to application when some device associates to that.
  */
typedef struct zb_nwk_signal_device_associated_params_s
{
  zb_ieee_addr_t device_addr; /*!< address of associated device */
} zb_nwk_signal_device_associated_params_t;

/**
 *@brief Association parameters
 *
 * Stack passes this parameter to application when some device joins/rejoins to
 * network.
 */
typedef struct zb_zdo_signal_device_annce_params_s
{
  zb_uint16_t device_short_addr; /*!< address of device that recently joined to network */
  zb_ieee_addr_t   ieee_addr;
  zb_uint8_t       capability;
} zb_zdo_signal_device_annce_params_t;

/** @cond DOXYGEN_TOUCHLINK_FEATURE */
typedef struct zb_bdb_signal_touchlink_nwk_started_params_s
{
  zb_ieee_addr_t device_ieee_addr; /*!< address of device that started the network */
  zb_uint8_t endpoint;
  zb_uint16_t profile_id;
} zb_bdb_signal_touchlink_nwk_started_params_t;

typedef struct zb_bdb_signal_touchlink_nwk_joined_router_s
{
  zb_ieee_addr_t device_ieee_addr; /*!< address of device that started the network */
  zb_uint8_t endpoint;
  zb_uint16_t profile_id;
} zb_bdb_signal_touchlink_nwk_joined_router_t;
/** @endcond */ /* DOXYGEN_TOUCHLINK_FEATURE */

/**
   @brief Sleep signal parameters.

   Stack passes this parameter to application when device ready to sleep.
  */
typedef struct zb_zdo_signal_can_sleep_params_s
{
  zb_uint32_t sleep_tmo; /*!< sleep duration in milliseconds */
} zb_zdo_signal_can_sleep_params_t;

/**
   @brief Macsplit device boot parameters.

   Macsplit passes this parameter to application when device is booted.
  */
typedef struct zb_zdo_signal_macsplit_dev_boot_params_s
{
  zb_uint32_t    dev_version; /*!< macsplit device version */
  zb_ieee_addr_t extended_address;          /*!< The 64-bit (IEEE) address assigned to the device. */
} zb_zdo_signal_macsplit_dev_boot_params_t;

/**
 * @name Status codes for F&B Initiator Finished signal
 * @anchor zdo_fb_initiator_finished_status
 */
/** @{ */
#define ZB_ZDO_FB_INITIATOR_STATUS_SUCCESS 0U /*!< F&B with a Target on the Initiator side is success completed */
#define ZB_ZDO_FB_INITIATOR_STATUS_CANCEL  1U /*!< F&B on the Initiator side is canceled */
#define ZB_ZDO_FB_INITIATOR_STATUS_ALARM   2U /*!< F&B on the Initiator side is finished by timeout */
#define ZB_ZDO_FB_INITIATOR_STATUS_ERROR   3U /*!< F&B on the Initiator side is failure finished */
/** @} */

/**
 * @brief Type for Status codes for F&B Initiator Finished signal.
 *
 * Holds one of @ref zdo_fb_initiator_finished_status. Kept only for backward
 * compatibility as
 * @ref zdo_fb_initiator_finished_status were declared previously as enum. Can be removed in future
 * releases.
 */
typedef zb_uint8_t zb_zdo_fb_initiator_finished_status_t;

/**
 * @brief F&B Initiator Finished signal parameters
 */
typedef struct zb_zdo_signal_fb_initiator_finished_params_s
{
  /*!< Status of the F&B procedure on the Initiator side */
  zb_zdo_fb_initiator_finished_status_t status;
} zb_zdo_signal_fb_initiator_finished_params_t;


/*
 * Note: These values were members of `enum zb_secur_upd_device_status_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/**
 * @name Security/rejoin states of the 'status' field of APSME-Update-Device
 * @see Table 4.40
 * @anchor secur_upd_device_status
 *
 */
/** @{ */
#define ZB_STD_SEQ_SECURED_REJOIN    0U /*!< Device rejoin with standard security */
#define ZB_STD_SEQ_UNSECURED_JOIN    1U /*!< Device join without security */
#define ZB_DEVICE_LEFT               2U /*!< Device left */
#define ZB_STD_SEQ_UNSECURED_REJOIN  3U /*!< Device rejoin without standard security */
#define ZB_JOIN_WITH_DLK             6U /*!< Device join after network commissioning (r23). Note: non-standard status; do not send it. */
/** @} */
/* Obsolete values */
#define ZB_HIGH_SEQ_SECURED_REJOIN   4U /*!< Device rejoin with high security */
#define ZB_HIGH_SEQ_UNSECURED_JOIN   5U /*!< Device join without high security */
#define ZB_HIGH_SEQ_UNSECURED_REJOIN 7U /*!< Device rejoin without high security */


/**
 * @name TC action on incoming Update Device
 * @anchor secur_tc_action
 */
/** @{ */
#define ZB_TC_ACTION_AUTHORIZE      0u /*!< authorize device  */
#define ZB_TC_ACTION_DENY           1u /*!< deby authorization - msend Remove device  */
#define ZB_TC_ACTION_IGNORE         2u /*!< ignore Update Device - that meay lead to authorization deny  */
/** @} */


/**
 * @brief Device Updated signal parameters
 */
typedef struct zb_zdo_signal_device_update_params_s
{
  /*!< Long Address of the updated device */
  zb_ieee_addr_t long_addr;

  /*!< Short Address of the updated device */
  zb_uint16_t short_addr;

  /*!< Indicates the updated status of the device
   *
   * 0x00 = Standard device secured rejoin
   * 0x01 = Standard device unsecured join
   * 0x02 = Device left
   * 0x03 = Standard device trust center rejoin
   * 0x04 - 0x07 = Reserved
   *
   * @see secur_upd_device_status
   *
   * see r21 spec, 4.4.3.2 APSME-UPDATE-DEVICE.indication,
   * Table 4.15 APSME-UPDATE-DEVICE.indication Parameters
   */
  zb_uint8_t status;
  /*!<
    Action by TC: authorize, send remove dev, ignore
    @see secur_tc_action
   */
  zb_uint8_t tc_action;
  /*!< Short Address of the updated device parent, 0xffff is unknown */
  zb_uint16_t parent_short;
} zb_zdo_signal_device_update_params_t;


/**
 * @brief Device Ready For Interview signal parameters
 */
typedef struct zb_zdo_signal_device_ready_for_interview_params_s
{
  /*!< Long Address of the device */
  zb_ieee_addr_t long_addr;

  /*!< Short Address of the device */
  zb_uint16_t short_addr;
} zb_zdo_signal_device_ready_for_interview_params_t;

/**
 * @name Authorization types for @ref ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED
 * @anchor zdo_authorization_type
 */
/** @{ */
#define ZB_ZDO_AUTHORIZATION_TYPE_LEGACY   0U /*!< Authorization type for legacy devices ( < r21) */
#define ZB_ZDO_AUTHORIZATION_TYPE_R21_TCLK 1U /*!< Authorization type for r21 device through TCLK */
#define ZB_ZDO_AUTHORIZATION_TYPE_SE_CBKE  2U /*!< Authorization type for SE through CBKE */
#define ZB_ZDO_AUTHORIZATION_TYPE_DLK      3U /*!< Authorization type for r23 dynamic link key negotiation */
/** @} */

/**
 * @brief Type for Authorization types for @ref ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED.
 *
 * @deprecated holds one of @ref zdo_authorization_type. Kept only for backward compatibility as
 * @ref zdo_authorization_type were declared previously as enum. Can be removed in future releases.
 */
typedef zb_uint8_t zb_zdo_authorization_type_t;

/**
 * @name Authorization statuses for ZB_ZDO_AUTHORIZATION_TYPE_LEGACY value of @ref
 * zdo_authorization_type type.
 * @anchor zdo_legacy_device_authorization_status
 */
/** @{ */
#define ZB_ZDO_LEGACY_DEVICE_AUTHORIZATION_SUCCESS 0U /*!< Authorization success */
#define ZB_ZDO_LEGACY_DEVICE_AUTHORIZATION_FAILED  1U /*!< Authorization failed */
/** @} */

/**
 * @brief Type for Authorization statuses for ZB_ZDO_AUTHORIZATION_TYPE_LEGACY value of @ref
 * zdo_authorization_type type.
 *
 * @deprecated holds one of @ref zdo_legacy_device_authorization_status. Kept only for backward
 * compatibility as @ref zdo_legacy_device_authorization_status were declared previously as enum.
 * Can be removed in future releases.
 */
typedef zb_uint8_t zb_zdo_legacy_device_authorization_status_t;

/**
 * @name Authorization statuses for ZB_ZDO_AUTHORIZATION_TYPE_R21_TCLK and ZB_ZDO_AUTHORIZATION_TYPE_DLK
 * values @ref zdo_authorization_type
 * @anchor zdo_authorization_type_r21_tclk_status
 */
/** @{ */
#define ZB_ZDO_TCLK_AUTHORIZATION_SUCCESS 0U /*!< Authorization success */
#define ZB_ZDO_TCLK_AUTHORIZATION_TIMEOUT 1U /*!< Authorization timeout */
#define ZB_ZDO_TCLK_AUTHORIZATION_FAILED  2U /*!< Authorization failed */
/** @} */

/**
 * @name Authorization statuses for ZB_ZDO_AUTHORIZATION_TYPE_SE_CBKE value @ref zdo_authorization_type
 * For non-success statuses @see zb_kec_term_status_t
 */
/** @{ */
#define ZB_ZDO_CBKE_AUTHORIZATION_SUCCESS 0U /*!< Authorization success */
/** @} */

/**
 * @brief Type for Authorization types for @ref ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED.
 *
 * @deprecated holds one of @ref zdo_authorization_type_r21_tclk_status. Kept only for backward compatibility
 * as @ref zdo_authorization_type_r21_tclk_status were declared previously as enum. Can be removed in future
 * releases.
 */
typedef zb_uint8_t zb_zdo_tclk_authorization_status_t;

/**
 * @brief Device Authorized signal parameters
 */
typedef struct zb_zdo_signal_device_authorized_params_s
{
  /*!< Long Address of the updated device */
  zb_ieee_addr_t long_addr;

  /*!< Short Address of the updated device */
  zb_uint16_t short_addr;

  /*!< Type of the authorization procedure,
   *   @ref zdo_authorization_type
   */
  zb_uint8_t authorization_type;

  /*!< Status of the authorization procedure
   *   (depends on @ref zdo_authorization_type)
   */
  zb_uint8_t authorization_status;
} zb_zdo_signal_device_authorized_params_t;

/**
 * @brief TCLK Read debug signal parameters
 */
typedef struct zb_debug_signal_tclk_ready_params_s
{
  /*!< Long Address of the remote device */
  zb_ieee_addr_t long_addr;

} zb_debug_signal_tclk_ready_params_t;

/** Application signal header */
typedef struct zb_zdo_app_signal_hdr_s
{
  /* 32 bit to have data section aligned well */
  zb_uint32_t sig_type;            /** Application signal type, @ref zdo_app_signal_type */
} zb_zdo_app_signal_hdr_t;
/* Legacy API: zb_zdo_app_signal_hdr_t == zb_zdo_app_event_t */
typedef zb_zdo_app_signal_hdr_t zb_zdo_app_event_t;

/** @} */ /* comm_signals*/
/*! \addtogroup zdo_base */
/*! @{ */

#define ZB_ZDO_INVALID_TSN 0xFFU

typedef ZB_PACKED_PRE struct zb_zdo_callback_info_s
{
  zb_uint8_t tsn;
  zb_uint8_t status;
} ZB_PACKED_STRUCT zb_zdo_callback_info_t;

typedef ZB_PACKED_PRE struct zb_zdo_default_resp_s
{
  zb_uint8_t tsn;
  zb_uint8_t status;
} ZB_PACKED_STRUCT zb_zdo_default_resp_t;

/** @} */
/*! \addtogroup zb_comm_signals */
/*! @{ */

/**
   @brief Unpack application signal buffer in zboss_signal_handler()

   @param param - parameter of zboss_signal_handler()
   @param sg_p - pointer to the extended signal info, if application wants it. Can be NULL.

   @return Application signal
 */

zb_zdo_app_signal_type_t zb_get_app_signal(zb_uint8_t param, zb_zdo_app_signal_hdr_t **sg_p);
/** @cond internals_doc */
/* Legacy API: zb_get_app_event(param, sg_p) == zb_get_app_signal(param, sg_p) */
#define zb_get_app_event(param, sg_p) zb_get_app_signal(param, sg_p)
/** @endcond */ /* internals_doc */

/**
   @brief Get status from the application signal
   @param param - buffer reference
   @return status zb_ret_t
  */
#define ZB_GET_APP_SIGNAL_STATUS(param) zb_buf_get_status(param)

/** @} */
/*! \addtogroup zdo_base */
/*! @{ */


/**
   @brief Obtains last known LQA and RSSI values from device with specified short address

   @param[in] short_address - address of device
   @param[in,out] lqa - pointer to @ref zb_uint8_t variable to store lqa value
   @param[in,out] rssi - pointer to @ref zb_uint8_t variable to store rssi value

@b Example:
@snippet thermostat/thermostat_zr/thermostat_zr.c zb_zdo_get_diag_data_snippet
 */
void zb_zdo_get_diag_data(zb_uint16_t short_address, zb_uint8_t *lqa, zb_int8_t *rssi);

typedef struct zb_zdo_get_diag_data_req_params_s
{
  zb_uint16_t short_address;
} zb_zdo_get_diag_data_req_params_t;

typedef struct zb_zdo_get_diag_data_resp_params_s
{
  zb_ret_t status; /* RET_OK if lqi and rssi values are available for the requested address,
                      RET_NOT_FOUND if no values are available for the requested address */
  zb_uint16_t short_address;
  zb_uint8_t lqi;
  zb_int8_t rssi;
} zb_zdo_get_diag_data_resp_params_t;

/**
 * @brief Obtains last known LQI and RSSI values from the device with the specified short address
 *
 * @param buf - a buffer with request params, see zb_zdo_get_diag_data_req_params_t
 * @param cb - a user's function to call when the response is ready.
 *             See zb_zdo_get_diag_data_resp_params_t for response params description.
 *
 * @return status, RET_OK - request successfully sent,
 *                 RET_INVALID_PARAMETER_1 if buf is ZB_BUF_INVALID,
 *                 RET_INVALID_PARAMETER_2 if cb is NULL,
 *                 RET_BUSY if this request can't be served at the moment
 *
 * Example:
 * @snippet onoff_server/on_off_output_zc.c zb_zdo_get_diag_data_async_example
 */
zb_ret_t zb_zdo_get_diag_data_async(zb_bufid_t buf, zb_callback_t cb);

/*! @} */


/*! \addtogroup zdo_addr */
/*! @{ */



#define ZB_ZDO_SINGLE_DEVICE_RESP   0U /*!<  Request Type parameter value of @ref zb_zdo_nwk_addr_req_t and @ref zb_zdo_ieee_addr_req_t for receiving a Single device response used in the following commands accordingly:
                                       * - @ref zb_zdo_nwk_addr_req
                                       * - @ref zb_zdo_ieee_addr_req
                                       *   @see spec 2.4.3.1, 2.4.4.1
                                       */
#define ZB_ZDO_EXTENDED_DEVICE_RESP 1U /*!< Request Type parameter value of @ref zb_zdo_nwk_addr_req_t and @ref zb_zdo_ieee_addr_req_t for receiving an Extended device response used in the following commands accordingly:
                                       * - @ref zb_zdo_nwk_addr_req
                                       * - @ref zb_zdo_ieee_addr_req_t
                                       *   @see spec 2.4.3.1, 2.4.4.1
                                       */

/** @brief NWK_addr_req command primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_req_s
{
  zb_ieee_addr_t   ieee_addr;    /*!< IEEE address to be matched by the
                                   Remote Device  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
} ZB_PACKED_STRUCT zb_zdo_nwk_addr_req_t;

/** @brief Parameters for nwk_addr_req command */
typedef struct zb_zdo_nwk_addr_req_param_s
{
  zb_uint16_t      dst_addr;     /*!< Destination address */
  zb_ieee_addr_t   ieee_addr;    /*!< IEEE address to be matched by the
                                   Remote Device  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
}
zb_zdo_nwk_addr_req_param_t;

/** @brief NWK_addr_req response frame. */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_head_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the NWK_addr_req command. */
  zb_ieee_addr_t ieee_addr; /*!< 64-bit address for the Remote Device. */
  zb_uint16_t nwk_addr; /*!< 16-bit address for the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_head_t;

/** @brief NWK_addr_req response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_ext_s
{
  zb_uint8_t num_assoc_dev; /*!< Count of the ED List. */
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_ext_t;

/** @brief NWK_addr_req response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_ext2_s
{
  zb_uint8_t start_index; /*!< Starting index to begin reporting for the ED List.*/
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_ext2_t;

/** @brief Sends NWK_addr_req primitive.

   @param param - index of buffer with primitive parameters - \ref zb_zdo_nwk_addr_req_param_s
   @param cb    - user's function to call when got response from the
                  remote. \ref zb_zdo_nwk_addr_resp_head_s passed to cb as parameter.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

@b Example:
@snippet lighting/dimmable_light_tl/light_controller_zed.c zb_zdo_nwk_addr_req_snippet

*/
zb_uint8_t zb_zdo_nwk_addr_req(zb_uint8_t param, zb_callback_t cb);

/** @cond internals_doc */
/**
   @brief Broadcast NWK_addr_req primitive.
   Used internally in stack.

   @param param - index of buffer with primitive parameters - \ref zb_zdo_nwk_addr_req_param_s
   @param ieee_addr - IEEE address to be matched by the Remote Device
*/
zb_uint8_t zb_zdo_initiate_nwk_addr_req(zb_uint8_t param, zb_ieee_addr_t ieee_addr);
/* Used internally in stack. */
void zb_zdo_initiate_nwk_addr_req_2param(zb_uint8_t param, zb_uint16_t user_param);
void zb_zdo_init_node_desc_req_2param(zb_uint8_t param, zb_uint16_t user_param);
zb_ret_t zb_zdo_init_node_desc_req_direct(zb_uint16_t addr, zb_callback_t user_cb);
/** @endcond */ /* internals_doc */

/** @brief Parameters of IEEE_addr_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
} ZB_PACKED_STRUCT zb_zdo_ieee_addr_req_t;

/** @brief Parameters for ieee_addr_req command */
typedef struct zb_zdo_ieee_addr_req_param_s
{
  zb_uint16_t      dst_addr;     /*!< Destination address */
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
}
zb_zdo_ieee_addr_req_param_t;

/** brief ZDO IEEE address response frame */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the NWK_addr_req command. */
  zb_ieee_addr_t ieee_addr_remote_dev; /*!< 64-bit address for the Remote Device. */
  zb_uint16_t nwk_addr_remote_dev; /*!< 16-bit address for the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_t;

/** @brief ZDO IEEE address response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_ext_s
{
  zb_uint8_t num_assoc_dev; /*!< Count of the ED List. */
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_ext_t;

/** @brief ZDO IEEE address response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_ext2_s
{
  zb_uint8_t start_index; /*!< Starting index to begin reporting for the ED List.*/
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_ext2_t;

/** @brief IEEE_addr_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_ieee_addr_req_s.
  *                Parameters must be put into the buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return      - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet simple_gw/simple_gw.c zb_zdo_ieee_addr_req_snippet
  *
  */
zb_uint8_t zb_zdo_ieee_addr_req(zb_uint8_t param, zb_callback_t cb);

/** @cond internals_doc */
/* Used internally in stack. */
zb_uint8_t zb_zdo_initiate_ieee_addr_req_broadcast(zb_uint8_t param, zb_uint16_t nwk_addr);
/** @endcond */ /* internals_doc */

/** @} */

/**
   @addtogroup zdo_disc
   @{
*/

/** @brief Parameters of Node_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_node_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_node_desc_req_t;


/** @brief Header of Node_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_desc_resp_hdr_s
{
  zb_uint8_t      tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @ref zdp_status */
  zb_uint16_t     nwk_addr; /*!< NWK address for the request  */
} ZB_PACKED_STRUCT
zb_zdo_desc_resp_hdr_t;

/** @brief Parameters of Node_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_node_desc_resp_s
{
  zb_zdo_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_node_desc_t node_desc; /*!< Node Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_node_desc_resp_t;

/** @brief Header of simple_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_resp_hdr_s
{
  zb_uint8_t      tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @ref zdp_status */
  zb_uint16_t     nwk_addr; /*!< NWK address for the request  */
  zb_uint8_t      length;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_simple_desc_resp_hdr_t;

#ifdef ZB_FIXED_OPTIONAL_DESC_RESPONSES
typedef zb_zdo_simple_desc_resp_hdr_t     zb_zdo_complex_desc_resp_hdr_t;
typedef zb_zdo_simple_desc_resp_hdr_t     zb_zdo_user_desc_resp_hdr_t;
typedef zb_zdo_desc_resp_hdr_t            zb_zdo_user_desc_conf_hdr_t;
#endif

/** @brief Parameters of simple_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_resp_s
{
  zb_zdo_simple_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_simple_desc_1_1_t simple_desc; /*!< Simple Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_simple_desc_resp_t;


typedef ZB_PACKED_PRE struct zb_zdo_child_info_s
{
  zb_ieee_addr_t  ext_addr;     /*!< 64-bit IEEE address that is
                                 * unique to every device.*/
}ZB_PACKED_STRUCT
zb_zdo_child_info_t;

/** @brief Jitter used for sending Parent Annce */
#define ZB_PARENT_ANNCE_JITTER() (ZB_APS_PARENT_ANNOUNCE_BASE_TIMER + ZB_RANDOM_JTR(ZB_APS_PARENT_ANNOUNCE_JITTER_MAX))


/** @brief Header of parent_annce primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_s
{
  zb_uint8_t      tsn;               /*!< ZDO sequence number */
  zb_uint8_t      num_of_children;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_t;


/** @brief Header of parent_annce_rsp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_rsp_hdr_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @ref zdp_status */
  zb_uint8_t      num_of_children;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_rsp_hdr_t;

/** @brief Parameters of parent_annce_rsp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_rsp_s
{
  zb_zdo_parent_annce_rsp_hdr_t hdr;  /*!< header for response */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_rsp_t;

/** @brief Parameters of Power_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_power_desc_resp_s
{
  zb_zdo_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_node_power_desc_t power_desc; /*!< Power Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_power_desc_resp_t;

/** @brief Node_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_node_desc_req_s.
  * Parameters must be put into buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet onoff_server/on_off_switch_zed.c send_node_desc_req
  * @snippet onoff_server/on_off_switch_zed.c node_req_cb
  *
  */
zb_uint8_t zb_zdo_node_desc_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Parameters of Power_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_power_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_power_desc_req_t;

/** @brief Power_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_power_desc_req_s.
  * Parameters must be put into buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet onoff_server/on_off_switch_zed.c send_power_desc_req
  * @snippet onoff_server/on_off_switch_zed.c power_desc_cb
  *
  */
zb_uint8_t zb_zdo_power_desc_req(zb_uint8_t param, zb_callback_t cb);



/** @brief Parameters of Simple_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint8_t       endpoint;    /*!< The endpoint on the destination  */
} ZB_PACKED_STRUCT zb_zdo_simple_desc_req_t;

/** @brief Simple_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_simple_desc_req_s.
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet onoff_server/on_off_switch_zed.c send_simple_desc_req
  * @snippet onoff_server/on_off_switch_zed.c simple_desc_cb
  *
  */
zb_uint8_t zb_zdo_simple_desc_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Parameters of Active_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_active_ep_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_active_ep_req_t;


/** @brief Active EP response */
typedef ZB_PACKED_PRE struct zb_zdo_ep_resp_s
{
  zb_uint8_t tsn;       /*!< ZDP transaction sequence number */
  zb_uint8_t status;    /*!< The status of the Active_EP_req command. */
  zb_uint16_t nwk_addr; /*!< NWK address for the request. */
  zb_uint8_t ep_count;  /*!< The count of active endpoints on the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_ep_resp_t;

/**
 * @brief Active_desc_req primitive.
 *
 * @param param - index of buffer with primitive parameters \ref zb_zdo_active_ep_req_s. Parameters must be
 * put into buffer as data (allocated).
 * @param cb    - user's function to call when got response from the remote.
 * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
 *                performed now (nor enough memory, resources, etc.)
 *
 * @b Example:
 * @snippet onoff_server/on_off_switch_zed.c send_active_ep_req
 * @snippet onoff_server/on_off_switch_zed.c active_ep_cb
 *
 */
zb_uint8_t zb_zdo_active_ep_req(zb_uint8_t param, zb_callback_t cb);

/** @brief Parameters of match_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_param_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint16_t     addr_of_interest; /*!< NWK address of interest */

  zb_uint16_t      profile_id;  /*!< Profile ID to be matched at the
                                  destination.  */
  zb_uint8_t       num_in_clusters; /*!< The number of Input Clusters
                                      provided for matching within the
                                      InClusterList.  */
  zb_uint8_t       num_out_clusters; /*!< The number of Output Clusters
                                       provided for matching within
                                       OutClusterList.  */
  zb_uint16_t      cluster_list[1]; /*!< Variable size: [num_in_clusters] +  [num_out_clusters]
                                         List of Input ClusterIDs to be used
                                         for matching; the InClusterList is
                                         the desired list to be matched by
                                         the Remote Device (the elements
                                         of the InClusterList are the
                                         supported output clusters of the
                                         Local Device).
                                         List of Output ClusterIDs to be
                                         used for matching; the
                                         OutClusterList is the desired list to
                                         be matched by the Remote Device
                                         (the elements of the
                                         OutClusterList are the supported
                                         input clusters of the Local
                                         Device). */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_param_t;

/** @brief Match_desc_req head */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_req_head_s
{
  zb_uint16_t      nwk_addr;        /*!< NWK address that is used for IEEE
                                         address mapping.  */
  zb_uint16_t      profile_id;      /*!< Profile ID to be matched at the
                                         destination.  */
  zb_uint8_t       num_in_clusters; /*!< The number of Input Clusters
                                         provided for matching within the
                                         InClusterList.  */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_req_head_t;

/** @brief Match_desc_req tail */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_req_tail_s
{
  zb_uint8_t       num_out_clusters; /*!< The number of Output Clusters
                                       provided for matching within
                                       OutClusterList.  */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_req_tail_t;

/** @brief Match_Desc_rsp response structure
  * @see ZB spec, subclause 2.4.4.1.7
  */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_resp_s
{
  zb_uint8_t tsn;       /*!< ZDP transaction sequence number */
  zb_uint8_t status;    /*!< The status of the Match_Desc_req command.*/
  zb_uint16_t nwk_addr; /*!< NWK address for the request. */
  zb_uint8_t match_len; /*!< The count of endpoints on the Remote Device that match the
                          request criteria.*/
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_resp_t;


/** @brief Match_desc_req primitive.

   @param param - index of buffer with primitive parameters \ref zb_zdo_match_desc_param_s.
   @param cb    - user's function to call when got response from the remote. If command is \n
                  broadcast, then user's function will be called as many times as number of \n
                  responses received plus one more time with status \ref ZB_ZDP_STATUS_TIMEOUT
                  to indicate that no more responses will be received.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
   @snippet light_sample/light_control/light_control.c zdo_match_desc_req
*/
zb_uint8_t zb_zdo_match_desc_req(zb_uint8_t param, zb_callback_t cb);


#ifndef ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
/** @brief Request parameters for System_Server_Discovery_req.
  * @see ZB spec, subclause 2.4.3.1.13.
  */
typedef ZB_PACKED_PRE struct zb_zdo_system_server_discovery_req_s
{
  zb_uint16_t server_mask; /*!< Server mask for device discovery */
}
ZB_PACKED_STRUCT
zb_zdo_system_server_discovery_req_t;

/** @brief Parameters for System_Server_Discovery_req call.
  * @see ZB spec, subclause 2.4.3.1.13.
  */
typedef zb_zdo_system_server_discovery_req_t zb_zdo_system_server_discovery_param_t;


/** @brief Response parameters for System_Server_Discovery_rsp.
  * @see ZB spec, subclause 2.4.4.1.10.
  */
typedef ZB_PACKED_PRE struct zb_zdo_system_server_discovery_resp_s
{
  zb_uint8_t tsn;          /*!< ZDP transaction sequence number */
  zb_uint8_t status;       /*!< Status of the operation */
  zb_uint16_t server_mask; /*!< Mask of the supported features */
}
ZB_PACKED_STRUCT
zb_zdo_system_server_discovery_resp_t;


/**
 * @brief Performs System_Server_Discovery_req
 * @param param - index of buffer with request parameters
 * @ref zb_zdo_system_server_discovery_param_t
 * @param cb - user's function to call when got response from the remote. \ref
 *                zb_zdo_system_server_discovery_resp_s
 *
 * @return ZDP transaction sequence number
 * @return 0xFF if operation cannot be
 *          performed now (nor enough memory, resources, etc.)
 *
 *  @b Example:
 *  @snippet onoff_server/on_off_switch_zed.c system_server_discovery_req
 *  @snippet onoff_server/on_off_switch_zed.c system_server_discovery_cb
 *
 */
zb_uint8_t zb_zdo_system_server_discovery_req(zb_uint8_t param, zb_callback_t cb);
#endif  /*ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY */

/*! @} */

/*! \addtogroup zdo_mgmt */
/*! @{ */

#ifdef ZB_JOIN_CLIENT
/**
 * @brief Set the number of network scan attempts
 *  
 * Sets the number of scan attempts to make before the NWK layer decides which Zigbee 
 * coordinator or router to associate with.
 * The default value is 5, see @ref ZB_ZDO_NWK_SCAN_ATTEMPTS.
 * 
 * @param [IN] attempts - value of scan attempts. Valid values between 1 and 255.
 * 
 * @return RET_OK - if success,
 *         RET_INVALID_PARAMETER_1 - if the attempts value is incorrect.
 */
zb_ret_t zb_zdo_set_nwk_scan_attempts(zb_uint8_t attempts);
#endif /* ZB_JOIN_CLIENT */

/** @brief Header of parameters for Mgmt_NWK_Update_req */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_req_hdr_s
{
  zb_uint32_t scan_channels;   /*!< Channels bitmask */
  zb_uint8_t scan_duration;    /*!< A value used to calculate the
                               * length of time to spend scanning
                               * each channel. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_req_hdr_t;

/** @brief Parameters for Mgmt_NWK_Update_req */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_req_s
{
  zb_zdo_mgmt_nwk_update_req_hdr_t hdr; /*!< Request header */
  zb_uint8_t scan_count;       /*!< This field represents the number
                                * of energy scans to be conducted and reported */
  zb_uint16_t manager_addr; /*!< This field shall be present only
                               * if the ScanDuration is set to 0xff,
                               * and, where present, indicates the
                               * NWK address for the device with the
                               * Network Manager bit set in its Node Descriptor. */
  zb_uint16_t dst_addr;     /*!< Destination address */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_req_t;


/** @brief Parameters for Mgmt_NWK_Update_req internal call*/
typedef struct zb_zdo_mgmt_nwk_enhanced_update_req_param_s
{
  zb_channel_list_t channel_list; /*!< ZBOSS internal channel list */
  zb_uint8_t scan_duration;       /*!< A value used to calculate the
                                   * length of time to spend scanning
                                   * each channel. */
  zb_uint8_t scan_count;          /*!< This field represents the number
                                   * of energy scans to be conducted and reported */
  zb_uint16_t manager_addr;       /*!< This field shall be present only
                                   * if the ScanDuration is set to 0xff,
                                   * and, where present, indicates the
                                   * NWK address for the device with the
                                   * Network Manager bit set in its Node Descriptor. */
  zb_uint16_t dst_addr;     /*!< Destination address */
} zb_zdo_mgmt_nwk_enhanced_update_req_param_t;

/** @brief Header of parameters for Mgmt_NWK_Update_req */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_enhanced_update_req_hdr_s
{
  zb_uint8_t channel_page_count;     /* The number of Channel Page Structures
                                      * contained within the Channel List Structure */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_enhanced_update_req_hdr_t;


/** @brief Header parameters for mgmt_nwk_update_notify */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_notify_hdr_s
{
  zb_uint8_t tsn;        /*!< tsn value */
  zb_uint8_t status;     /*!< The status of the Mgmt_NWK_Update_notify command. */
  zb_uint32_t scanned_channels;  /*!< List of channels scanned by the request */
  zb_uint16_t total_transmissions;  /*!< Count of the total transmissions reported by the device */
  zb_uint16_t transmission_failures;  /*!< Sum of the total transmission failures reported by the
                                            device */
  zb_uint8_t scanned_channels_list_count;  /*!< The list shall contain the number of records
                                            * contained in the EnergyValues parameter. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_notify_hdr_t;

/** @brief Parameters for mgmt_nwk_update_notify */
typedef struct zb_zdo_mgmt_nwk_update_notify_param_s
{
  zb_zdo_mgmt_nwk_update_notify_hdr_t hdr;             /*!< Fixed parameters set */
  zb_uint8_t energy_values[ZB_MAC_SUPPORTED_CHANNELS]; /*!< ed scan values */
  zb_uint16_t dst_addr;  /*!< destination address */
  zb_uint8_t enhanced;   /*!< If set to one, send Enhanced notify command  */
}
zb_zdo_mgmt_nwk_update_notify_param_t;


typedef zb_zdo_mgmt_nwk_update_notify_param_t zb_zdo_mgmt_nwk_enhanced_update_notify_param_t;


/** @brief Performs Mgmt_NWK_Update_req request

   @param param - index of buffer with call parameters. Parameters must be
   put into buffer as parameters. \ref zb_zdo_mgmt_nwk_update_req_s
   @param cb    - user's function to call when got response from the remote.
   \ref zb_zdo_mgmt_nwk_update_notify_hdr_s
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
@code
{
  zb_uint8_t tsn;
  zb_zdo_mgmt_nwk_update_req_t *req;

  req = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_nwk_update_req_t);

  req->hdr.scan_channels = ZB_MAC_ALL_CHANNELS_MASK;
  req->hdr.scan_duration = TEST_SCAN_DURATION;
  req->scan_count = TEST_SCAN_COUNT;

  req->dst_addr = 0;

  tsn = zb_zdo_mgmt_nwk_update_req(param, mgmt_nwk_update_ok_cb);
}


void mgmt_nwk_update_ok_cb(zb_uint8_t param)
{
  zb_bufid_t buf = param;
  zb_uint8_t *zdp_cmd = zb_buf_begin(buf);
  zb_zdo_mgmt_nwk_update_notify_hdr_t *notify_resp = (zb_zdo_mgmt_nwk_update_notify_hdr_t *)zdp_cmd;

  TRACE_MSG(TRACE_APS3,
            "notify_resp status %hd, scanned_channels %x %x, total_transmissions %hd, "
            "transmission_failures %hd, scanned_channels_list_count %hd, buf len %hd",
            (FMT__H_D_D_H_H_H_H, notify_resp->status, (zb_uint16_t)notify_resp->scanned_channels,
             *((zb_uint16_t*)&notify_resp->scanned_channels + 1),
             notify_resp->total_transmissions, notify_resp->transmission_failures,
             notify_resp->scanned_channels_list_count, zb_buf_len(buf)));

  if (notify_resp->status == ZB_ZDP_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_APS3, "mgmt_nwk_update_notify received, Ok", (FMT__0));
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "mgmt_nwk_update_notify received, ERROR incorrect status %x",
              (FMT__D, notify_resp->status));
  }

  zb_buf_free(buf);
}
@endcode


 */
zb_uint8_t zb_zdo_mgmt_nwk_update_req(zb_uint8_t param, zb_callback_t cb);

/** @brief Performs Mgmt_NWK_Enhanced_Update_req request

   @param param - index of buffer with call parameters. Parameters must be
   put into buffer as parameters. \ref zb_zdo_mgmt_nwk_enhanced_update_req_param_s
   @param cb    - user's function to call when got response from the remote.
   \ref zb_zdo_mgmt_nwk_update_notify_hdr_s
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

*/
zb_uint8_t zb_zdo_mgmt_nwk_enh_update_req(zb_uint8_t param, zb_callback_t cb);

#ifdef ZB_DEPRECATED_API

/** @brief Performs Mgmt_NWK_Update_req request

    @deprecated This function is deprecated and will be removed in a future release.
                Use zb_zdo_mgmt_nwk_enh_update_req() instead.
*/
zb_uint8_t zb_zdo_mgmt_nwk_enhanced_update_req(zb_uint8_t param, zb_callback_t cb) ZB_DEPRECATED;

#endif /* ZB_DEPRECATED_API */


/** @brief Notification for Mgmt_NWK_Unsolicited_Enhanced_Update_Notify
  * @see ZB spec, subclause 2.4.4.4.12
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_unsol_enh_update_notify_s
{
  zb_uint8_t        status;
  zb_channel_page_t channel_in_use;
  zb_uint16_t       mac_tx_ucast_total;
  zb_uint16_t       mac_tx_ucast_failures;
  zb_uint16_t       mac_tx_ucast_retries;
  zb_uint8_t        period;
} ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_unsol_enh_update_notify_t;

/** @brief Parameters for Mgmt_NWK_Unsolicited_Enhanced_Update_Notify
  * @see ZB spec, subclause 2.4.4.4.12
  */
typedef struct zb_zdo_mgmt_nwk_unsol_enh_update_notify_param_s
{
  zb_zdo_mgmt_nwk_unsol_enh_update_notify_t notification;
  zb_uint16_t addr;
}
zb_zdo_mgmt_nwk_unsol_enh_update_notify_param_t;

/** @brief Performs Mgmt_NWK_Enhanced_Update_req request

   @param param - index of buffer with call parameters. Parameters must be
   put into buffer as parameters. \ref zb_zdo_mgmt_nwk_unsol_enh_update_notify_param_t
   @param cb    - user's function to call when the notification has been sent.
*/
void zb_zdo_mgmt_nwk_unsol_enh_update_notify(zb_uint8_t param, zb_callback_t cb);


/** @brief Sends  Mgmt_Lqi_req (see Zigbee spec 2.4.3.3.2)
   @param param - index of buffer with Lqi request parameters. \ref zb_zdo_mgmt_lqi_param_s
   @param cb    - user's function to call when got response from the remote.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)
   @ref zb_zdo_mgmt_lqi_resp_s, \ref zb_zdo_neighbor_table_record_s

   @b Example:
@snippet doxygen_snippets.dox zboss_api_zdo_h_2

*/
zb_uint8_t zb_zdo_mgmt_lqi_req(zb_uint8_t param, zb_callback_t cb);

/** @brief Parameters for Mgmt_Lqi_req.
  * @see ZB spec, subclause 2.4.3.3.2.
  */
typedef struct zb_zdo_mgmt_lqi_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Neighbor Table */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_lqi_param_t;

/** @brief Request for Mgmt_Lqi_req.
  * @see ZB spec, subclause 2.4.3.3.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_lqi_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Neighbor Table */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_lqi_req_t;

/** @brief Response for Mgmt_Lqi_rsp.
  * @see ZB spec, subclause 2.4.4.3.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_lqi_resp_s
{
  zb_uint8_t tsn;                        /*!< ZDP transaction sequence number */
  zb_uint8_t status;                     /*!< The status of the Mgmt_Lqi_req command.*/
  zb_uint8_t neighbor_table_entries;     /*!< Total number of Neighbor
                                          * Table entries within the Remote Device */
  zb_uint8_t start_index;                /*!< Starting index within the Neighbor
                                          * Table to begin reporting for the NeighborTableList.*/
  zb_uint8_t neighbor_table_list_count;  /*!< Number of Neighbor Table
                                          * entries included within NeighborTableList*/
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_lqi_resp_t;


/* bits 0 - 1, mask 0x3 */
/**
   Set device type of neighbor table record to type 'type': bits 0 - 1, mask 0x3;

   @param var - neighbor table record type_flags
   @param type - Zigbee device type value

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_DEVICE_TYPE(var, type) ( ( var ) &= ~3U, ( var ) |= ( type ) )
/**
   Get device type of neighbor table record.

   @param var - neighbor table record type_flags

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_DEVICE_TYPE(var) ( var & 3U )

/* bits 2 - 3, mask 0xC */
/**
   Set RxOnWhenIdle attribute of neighbor table record to type 'type':
   bits 2 - 3, mask 0xC;

   @param var - neighbor table record type_flags
   @param type - RxOnWhenIdle value

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_RX_ON_WHEN_IDLE(var, type) ( ( var ) &= ~0xCU, ( var ) |= (( type ) << 2U) )
/**
   Get RxOnWhenIdle of neighbor table record.

   @param var - neighbor table record type_flags

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_RX_ON_WHEN_IDLE(var) ( (var & 0xCU) >> 2U )

/* bits 4 - 6, mask 0x70 */
/**
   Set relationship attribute of neighbor table record to type 'type':
   bits 4 - 6, mask 0x70;

   @param var - neighbor table record type_flags
   @param type - Zigbee relationship value

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_RELATIONSHIP(var, type) ( ( var ) &= ~0x70U, ( var ) |= (( type ) << 4U) )
/**
   Get relationship of neighbor table record.

   @param var - neighbor table record type_flags

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_RELATIONSHIP(var) ( (var & 0x70) >> 4 )

/** @brief NeighborTableList Record Format for Mgmt_Lqi_resp */
typedef ZB_PACKED_PRE struct zb_zdo_neighbor_table_record_s
{
  zb_ext_pan_id_t ext_pan_id;   /*!< 64-bit extended Pan
                                 * ID of the neighboring device.*/
  zb_ieee_addr_t  ext_addr;     /*!< 64-bit IEEE address that is
                                 * unique to every device.*/
  zb_uint16_t     network_addr; /*!< The 16-bit network address of the
                                 * neighboring device */
  zb_uint8_t      type_flags;   /*!< device type, rx_on_when_idle,
                                 * relationship */
  zb_uint8_t      permit_join;  /*!< An indication of whether the
                                 * neighbor device is accepting join requests*/
  zb_uint8_t      depth;        /*!< The tree depth of the neighbor device. */
  zb_uint8_t      lqa;          /*!< The estimated link quality for RF
                                 * transmissions from this device */
}
ZB_PACKED_STRUCT
zb_zdo_neighbor_table_record_t;


#define ZB_ZDO_MGMT_RTG_RESP_RECORD_FLAGS_STATUS             0U
#define ZB_ZDO_MGMT_RTG_RESP_RECORD_FLAGS_MEM_CONST          3U
#define ZB_ZDO_MGMT_RTG_RESP_RECORD_FLAGS_MANY_TO_ONE        4U
#define ZB_ZDO_MGMT_RTG_RESP_RECORD_FLAGS_ROUTE_REC_REQUIRED 5U

zb_uint8_t zb_zdo_mgmt_rtg_req(zb_uint8_t param, zb_callback_t cb);

void zdo_mgmt_rtg_resp(zb_uint8_t param);

/** @brief Parameters for Mgmt_rtg_req.
  * @see ZB spec, subclause 2.4.3.3.4.
*/
typedef struct zb_zdo_mgmt_rtg_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Routing Table */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_rtg_param_t;

/** @brief Request for Mgmt_rtg_req.
  * @see ZB spec, subclause 2.4.3.3.4.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_rtg_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Routing Table */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_rtg_req_t;

/** @brief Response for Mgmt_Rtg_rsp.
  * @see ZB spec, subclause 2.4.4.3.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_rtg_resp_s
{
  zb_uint8_t tsn;                        /*!< ZDP transaction sequence number */
  zb_uint8_t status;                     /*!< The status of the Mgmt_Rtg_req command.*/
  zb_uint8_t routing_table_entries;     /*!< Total number of Routing
                                          * Table entries within the Remote Device */
  zb_uint8_t start_index;                /*!< Starting index within the Routing
                                          * Table to begin reporting for the RoutingTableList.*/
  zb_uint8_t routing_table_list_count;  /*!< Number of Routing Table
                                          * entries included within RoutingTableList*/
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_rtg_resp_t;

/** @brief RoutingTableList Record Format for mgmt_rtg_resp */
typedef ZB_PACKED_PRE struct zb_zdo_routing_table_record_s
{
  zb_uint16_t     dest_addr;     /*!< The 16-bit network address of the
                                  * destination device */
  zb_uint8_t      flags;         /*!< Routing flags */
  zb_uint16_t     next_hop_addr; /*!< The 16-bit network address of the
                                  * next-hop device */
}
ZB_PACKED_STRUCT
zb_zdo_routing_table_record_t;

/**
 * @brief Performs active scan
 *
 * The result will be provided via callback that is passed within
 * zb_nlme_network_discovery_request_t. The only
 * argument from this callback is the index of a buffer with
 * zb_nlme_network_discovery_confirm_t param, followed by a sequence of
 * zb_nlme_network_descriptor_t params (count is determined by 
 * zb_nlme_network_discovery_confirm_t - network_count)
 * 
 * Sample use of active scan request:
 * @snippet zdo_startup_nwk_scan/zdo_start_ze.c active_scan_complete_cb
 * @snippet zdo_startup_nwk_scan/zdo_start_ze.c zb_zdo_active_scan_request
 * 
 * @param param - index of buffer with zb_nlme_network_discovery_request_t param
 */
void zb_zdo_active_scan_request(zb_uint8_t param);

/** @} */ /* zdo_mgmt */
/** @addtogroup zdo_bind
    @{
  */

/**
 * @name Bind destination address mode
 * @brief The addressing mode for the destination address used in @ref
 * zb_zdo_binding_table_record_s, @ref zb_zdo_bind_req_param_s, @ref zb_zdo_bind_req_head_s command.
 * This field can take one of the non-reserved values from the list of defines below (@ref bind_dst_addr_mode).
 * Values 0x00, 0x02, 0x04-0xff are reserved.
 * @anchor bind_dst_addr_mode
  * @see ZB Spec, subclause 2.4.3.2.2.
  */
/** @{ */
/** 16-bit group address for DstAddress and DstEndp not present */
#define ZB_BIND_DST_ADDR_MODE_16_BIT_GROUP 0x01U
/** 64-bit extended address for DstAddress and DstEndp present */
#define ZB_BIND_DST_ADDR_MODE_64_BIT_EXTENDED 0x03U
/** @} */

/**
 * @brief Type for bind destination address mode.
 *
 * @deprecated holds one of @ref bind_dst_addr_mode. Kept only for backward compatibility as
 * @ref bind_dst_addr_mode were declared previously as enum. Can be removed in future releases.
 */
typedef zb_uint8_t zb_bind_dst_addr_mode_t;

/** @brief Parameters for Mgmt_Bind_req.
  * @see ZB spec, subclause 2.4.3.3.4.
  */
typedef struct zb_zdo_mgmt_bind_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Binding Table */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_bind_param_t;

/** @brief Request for Mgmt_Bind_req.
  * @see ZB spec, subclause 2.4.3.3.4.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_bind_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Binding Table */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_bind_req_t;

/** @brief Response for Mgmt_Bind_rsp.
  * @see ZB spec, subclause 2.4.4.3.4.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_bind_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the Mgmt_Rtg_req command.*/
  zb_uint8_t binding_table_entries; /*!< Total number of Binding Table
                                     * entries within the Remote Device*/
  zb_uint8_t start_index; /*!< Starting index within the Binding
                           * Table to begin reporting for the BindingTableList.*/
  zb_uint8_t binding_table_list_count; /*!< Number of Binding Table
                                         * entries included within BindingTableList*/
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_bind_resp_t;


/** @brief BindingTableList Record Format for mgmt_bind_resp. See ZB
 * spec, Table 2.131 */
typedef ZB_PACKED_PRE struct zb_zdo_binding_table_record_s
{
  zb_ieee_addr_t src_address; /*!< The source IEEE address for the binding entry. */
  zb_uint8_t src_endp;        /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;     /*!< The identifier of the cluster on the
                               *   source device that is bound to the
                               *   destination device. */
  zb_uint8_t dst_addr_mode;   /*!< Destination address mode @ref bind_dst_addr_mode */
  zb_addr_u dst_address;      /*!< The destination address for the
                               *   binding entry.16 or 64 bit. As specified by the
                               *   dst_addr_mode field.*/
  zb_uint8_t dst_endp;        /*!< This field shall be present only if the
                               *    DstAddrMode field has a value of
                               *    ZB_BIND_DST_ADDR_MODE_64_BIT_EXTENDED @ref bind_dst_addr_mode
                               *    and, if present, shall be the
                               *    destination endpoint for the binding
                               *    entry. */
}
ZB_PACKED_STRUCT
zb_zdo_binding_table_record_t;


/** @brief Sends Mgmt_Bind_req request.
  * @param param reference to the buffer to put request data to.
  * @param cb callback to be called on operation finish.
  * @return ZDP transaction sequence number
  * @return 0xFF if operation cannot be
  *         performed now (nor enough memory, resources, etc.)
  *
  */
zb_uint8_t zb_zdo_mgmt_bind_req(zb_uint8_t param, zb_callback_t cb);

/**
   @brief Sends 2.4.4.3.4 Mgmt_Bind_rsp
   @param param - index of buffer with Mgmt_Bind request
 */
void zdo_mgmt_bind_resp(zb_uint8_t param);

 /** @brief Parameters for zb_zdo_raw_req call
  */
typedef struct zb_zdo_raw_req_param_s
{
  zb_uint16_t cluster_id;  /*!< The identifier of the object for which this
                                frame is intended.  */
  zb_uint16_t dst_addr;    /*!< Destination address */
}
zb_zdo_raw_req_param_t;

 /** @brief Parameters of zb_zdo_raw_resp call
  */
typedef struct zb_zdo_raw_resp_param_s
{
  zb_uint16_t cluster_id;  /*!< The identifier of the object in the response.  */
  zb_uint16_t src_addr;    /*!< Source address of a device that sent a response */
}
zb_zdo_raw_resp_param_t;

/** @brief Response by Raw_req. */
typedef ZB_PACKED_PRE struct zb_zdo_raw_resp_s
{
  zb_uint8_t tsn;    /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
  zb_uint8_t payload[ZB_ZDO_MAX_PAYLOAD_SIZE - 1U]; /**< Raw response payload. */
}
ZB_PACKED_STRUCT
zb_zdo_raw_resp_t;

#ifdef ZB_NCP_ENABLE_ZDO_RAW_CMD
/** @brief Raw ZDO request.

   @param param - index of buffer with request. @ref zb_zdo_raw_req_param_s
   @param cb    - user's function to call when got response from the
   remote. @ref zb_zdo_raw_resp_param_s
   @return ZDP transaction sequence number
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)
 */
zb_uint8_t zb_zdo_raw_req(zb_uint8_t param, zb_callback_t cb);
#endif /* ZB_NCP_ENABLE_ZDO_RAW_CMD */

/** @brief Parameters for Bind_req API call
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef struct zb_zdo_bind_req_param_s
{
  zb_ieee_addr_t src_address;  /*!< The IEEE address for the source. */
  zb_uint8_t src_endp;         /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;      /*!< The identifier of the cluster on the
                                *   source device that is bound to the destination. */
  zb_uint8_t dst_addr_mode;    /*!< Destination address mode @ref bind_dst_addr_mode */
  zb_addr_u dst_address;       /*!< The destination address for the
                                *   binding entry. */
  zb_uint8_t dst_endp;         /*!< This field shall be present only if the
                                *   DstAddrMode field has a value of
                                *   @ref ZB_BIND_DST_ADDR_MODE_64_BIT_EXTENDED @ref bind_dst_addr_mode
                                *   and, if present, shall be the
                                *   destination endpoint for the binding
                                *   entry. */
  zb_uint16_t req_dst_addr;    /*!< Destination address of the request */
}
zb_zdo_bind_req_param_t;


/** @brief Bind_req request head send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_head_s
{
  zb_ieee_addr_t src_address; /*!< The IEEE address for the source. */
  zb_uint8_t src_endp;        /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;     /*!< The identifier of the cluster on the
                               *   source device that is bound to the destination. */
  zb_uint8_t dst_addr_mode;   /*!< Destination address mode @ref bind_dst_addr_mode */
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_head_t;

/** @brief Bind_req request tail 1st variant send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_tail_1_s
{
  zb_uint16_t dst_addr; /*!< The destination address for the
                         *   binding entry. */
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_tail_1_t;

/** @brief Bind_req request tail 2nd variant send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_tail_2_s
{
  zb_ieee_addr_t dst_addr; /*!< The destination address for the
                            * binding entry. */
  zb_uint8_t dst_endp;  /*!< The destination address for the
                         * binding entry. */
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_tail_2_t;

typedef ZB_PACKED_PRE struct zb_zdo_status_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT zb_zdo_status_resp_t;

/** @brief Response by Bind_req. */
typedef ZB_PACKED_PRE struct zb_zdo_bind_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT zb_zdo_bind_resp_t;

/** @brief Response by ZDO Set Configuration req. */
typedef ZB_PACKED_PRE struct zb_zdo_set_configuration_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT zb_zdo_set_configuration_resp_t;

/** @brief Bind_req request.

   @param param - index of buffer with request. @ref zb_zdo_bind_req_param_s
   @param cb    - user's function to call when got response from the
   remote. @ref zb_zdo_bind_resp_s
   @return ZDP transaction sequence number
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)

   @b Example:
@snippet simple_gw/simple_gw.c zb_zdo_bind_req_snippet

 */
zb_uint8_t zb_zdo_bind_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Unbind_req request.

   @param param - index of buffer with request. @ref zb_zdo_bind_req_param_s
   @param cb    - user's function to call when got response from the
   remote. @ref zb_zdo_bind_resp_s
   @return ZDP transaction sequence number
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)

   @b Example:
@code
{
  zb_bufid_t buf = param;
  zb_zdo_bind_req_param_t *bind_param;

  TRACE_MSG(TRACE_COMMON1, "unbind_device_1", (FMT__0));

  zb_buf_initial_alloc(buf, 0);
  bind_param = ZB_BUF_GET_PARAM(buf, zb_zdo_bind_req_param_t);
  ZB_MEMCPY(bind_param->src_address, g_ieee_addr_ed1, sizeof(zb_ieee_addr_t));
  bind_param->src_endp = TEST_ED1_EP;
  bind_param->cluster_id = TP_BUFFER_TEST_REQUEST_CLID;
  bind_param->dst_addr_mode = ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
  ZB_MEMCPY(bind_param->dst_address.addr_long, g_ieee_addr_ed2, sizeof(zb_ieee_addr_t));
  bind_param->dst_endp = TEST_ED2_EP;
  bind_param->req_dst_addr = zb_address_short_by_ieee(g_ieee_addr_ed1);
  TRACE_MSG(TRACE_COMMON1, "dst addr %d", (FMT__D, bind_param->req_dst_addr));

  zb_zdo_unbind_req(buf, unbind_device1_cb);
}


void unbind_device1_cb(zb_uint8_t param)
{
  zb_bufid_t buf = ZB_BUF_FROM_REF(param);
  zb_zdo_bind_resp_t *bind_resp = (zb_zdo_bind_resp_t*)zb_buf_begin(buf);

  TRACE_MSG(TRACE_COMMON1, "unbind_device1_cb resp status %hd", (FMT__H, bind_resp->status));
  if (bind_resp->status != ZB_ZDP_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_COMMON1, "Error bind device 1. Test status failed", (FMT__0));
  }
  zb_free_buf(buf);

}
@endcode

*/
zb_uint8_t zb_zdo_unbind_req(zb_uint8_t param, zb_callback_t cb);

/**
   Execute ZDO Clear All Bindings for specified devices

   @param param - buffer holding @ref zb_zdo_clear_all_bind_req_param_t in its params
   @param cb - callback to be called on complete
 */
zb_uint8_t zb_zdo_clear_all_bind_req(zb_uint8_t param, zb_callback_t cb);

/** @brief Perform unbind all entries locally. This custom function and it is not described
 * in Zigbee specification.
 * @param param - not used.
 */
void zb_zdo_unbind_all_local(zb_uint8_t param);

/**
 * @brief Checks if the binding with specified parameters exists
 *
 */
void zb_zdo_check_binding_request(zb_bufid_t param);
/** @} */

/** @addtogroup zdo_mgmt
    @{
*/

/** @brief Request for Mgmt_Leave_req
    @see ZB spec, subclause 2.4.3.3.5.

   Problem in the specification:
   in 2.4.3.3.5  Mgmt_Leave_req only one DeviceAddress exists.
   But, in such case it is impossible to satisfy 2.4.3.3.5.1:
   "The Mgmt_Leave_req is generated from a Local Device requesting that a Remote
   Device leave the network or to request that another device leave the network."
   Also, in the PRO TC document, 14.2TP/NWK/BV-04 ZR-ZDO-APL RX Join/Leave is
   following note:
   "gZC sends Mgmt_Leave.request with DevAddr=all zero, DstAddr=ZR"
 */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_param_s
{
  zb_ieee_addr_t device_address;   /*!< 64-bit IEEE address */
  zb_uint16_t    dst_addr;         /*!< Destination address. Not defined in
                                    *   the specification - let's it be short address */
  zb_bitfield_t reserved:6;        /*!< Reserve */
  zb_bitfield_t remove_children:1; /*!< Obsolete field */
  zb_bitfield_t rejoin:1;          /*!< Rejoin */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_param_t;

/** Request for Mgmt_Leave_req.
  * @see ZB spec, subclause 2.4.3.3.5.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_req_s
{
  zb_ieee_addr_t device_address;   /*!< 64-bit IEEE address */
  zb_bitfield_t reserved:6;        /*!< Reserve */
  zb_bitfield_t remove_children:1;   /*!< Obsolete field, should be always 0 according to CSA */
  zb_bitfield_t rejoin:1;          /*!< Rejoin */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_req_t;

/** @brief Response for Mgmt_Leave_rsp.
  * @see ZB spec, subclause 2.4.4.3.5.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_res_s
{
  zb_uint8_t tsn;    /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_res_t;

/** @brief Sends Mgmt_Leave_req
    @see ZB spec, subclause 2.4.3.3.2.

   @param param - index of buffer with Lqi request parameters. @ref zb_zdo_mgmt_leave_param_s
   @param cb    - user's function to call when got response from the remote.
   @return      - transaction sequence number of request or 0xFF if operation
                  was NOT executed (if there is no free slot for registering the callback)

   @snippet samples/se/energy_service_interface/se_esi_zc_ncp_leave_zdo.c mgmt_leave_req

   @b Example:
@code
{
  zb_bufid_t buf = param;
  zb_zdo_mgmt_leave_param_t *req = NULL;
  zb_ret_t ret = RET_OK;
  zb_uint8_t tsn;

  TRACE_MSG(TRACE_ERROR, "zb_leave_req", (FMT__0));

  req = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_leave_param_t);

  ZB_MEMSET(req->device_address, 0, sizeof(zb_ieee_addr_t));
  req->remove_children = ZB_FALSE;
  req->rejoin = ZB_FALSE;
  req->dst_addr = 1;
  tsn = zdo_mgmt_leave_req(param, leave_callback);
}

void leave_callback(zb_uint8_t param)
{
  zb_zdo_mgmt_leave_res_t *resp = (zb_zdo_mgmt_leave_res_t *)zb_buf_begin(param);

  TRACE_MSG(TRACE_ERROR, "LEAVE CALLBACK status %hd", (FMT__H, resp->status));
}
@endcode

*/
zb_uint8_t zdo_mgmt_leave_req(zb_uint8_t param, zb_callback_t cb);

/** @} */
/** @addtogroup zdo_bind
    @{
*/


/** @brief End_Device_Bind_req command head.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_req_head_s
{
  zb_uint16_t binding_target;   /*!< The address of the target for the
                                 * binding. This can be either the
                                 * primary binding cache device or the
                                 * short address of the local device. */
  zb_ieee_addr_t src_ieee_addr; /*!< IEEE address of the device generating the request */
  zb_uint8_t src_endp;          /*!< The endpoint on the device generating the request */
  zb_uint16_t profile_id;       /*!< ProfileID which is to be matched
                                 * between two End_Device_Bind_req
                                 * received at the Zigbee Coordinator */
  zb_uint8_t num_in_cluster;    /*!< The number of Input Clusters
                                 * provided for end device binding
                                 * within the InClusterList. */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_req_head_t;

/** @brief End_Device_Bind_req command head.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_req_tail_s
{
  zb_uint8_t num_out_cluster;   /*!< The number of Output Clusters
                                 * provided for matching within OutClusterList */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_req_tail_t;

/** @brief Parameters for End_Device_Bind_req.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_end_device_bind_req_param_s
{
  zb_uint16_t dst_addr;                         /*!< Destination address */
  zb_zdo_end_device_bind_req_head_t head_param; /*!< Parameters for command head */
  zb_zdo_end_device_bind_req_tail_t tail_param; /*!< Parameters for command tail */
  zb_uint16_t cluster_list[1];                  /*!< List of Input and Output
                                                 * ClusterIDs to be used for matching */
} ZB_PACKED_STRUCT
zb_end_device_bind_req_param_t;

/**
   sends 2.4.3.2.1 End_Device_Bind_req command
   @param param - index of buffer with request
   @param cb    - user's function to call when got response from the remote.
   @return ZDP transaction sequence number or
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)
*/
zb_uint8_t zb_end_device_bind_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Response from End_Device_Bind_req.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_resp_s
{
  zb_uint8_t tsn;    /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_resp_t;

/** @} addtogroup zdo_bind */
/** @addtogroup r23_secur
    @{
*/

/**
 * PostJoinKeyUpdateMethod enum (see Table 4-33)
 * @anchor zb_post_join_key_upd_method_t
 *
 * Note: These values were members of `enum zb_post_join_key_upd_method_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
typedef zb_uint8_t zb_post_join_key_upd_method_t;

#define ZB_POST_JOIN_KEY_UPD_METH_NOT_UPDATED                   0x00U
#define ZB_POST_JOIN_KEY_UPD_METH_KEY_REQ                       0x01U
#define ZB_POST_JOIN_KEY_UPD_METH_UNAUTH_KEY_NEGOTIATION        0x02U
#define ZB_POST_JOIN_KEY_UPD_METH_AUTH_KEY_NEGOTIATION          0x03U
#define ZB_POST_JOIN_KEY_UPD_METH_APP_DEFINED_CERT_BASED_MUTUAL 0x04U /* CBKE indeed */
#define ZB_POST_JOIN_KEY_UPD_METH_RESERVED1                     0x05U
#define ZB_POST_JOIN_KEY_UPD_METH_RESERVED2                     0x06U
#define ZB_POST_JOIN_KEY_UPD_METH_HASHED_TCLK                   0x07U /* Hashed TCLK is restored from backup at TC after TC swap */
/** @} */

/**
 * @name InitialJoinAuthentication enum (see Table 4-33)
 * @anchor zb_initial_join_auth_t
 *
 * Note: These values were members of `enum zb_initial_join_auth_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
typedef zb_uint8_t zb_initial_join_auth_t;

#define ZB_JOIN_NO_AUTHENTICATION 0x00U /*!< No auth token used */
#define ZB_JOIN_INSTALLCODE_KEY   0x01U /*!< Installcode used */
#define ZB_JOIN_ANON_KEY_NEG      0x02U /*!< Key Negotiation with Well-known key */
#define ZB_JOIN_AUTH_KEY_NEG      0x03U /*!< Key Negotiation with auth method */
/** @} */

/** @brief Parameters for get authentication level request
  */
typedef struct zb_zdo_secur_get_auth_level_req_s
{
  zb_ieee_addr_t target_ieee;
} zb_zdo_secur_get_auth_level_req_t;

typedef struct zb_zdo_secur_get_auth_level_rsp_s
{
  zb_uint8_t     tsn;
  zb_uint8_t     status;
  zb_ieee_addr_t target_ieee;
  zb_initial_join_auth_t        initial_join_auth;
  zb_post_join_key_upd_method_t key_upd_method;
} zb_zdo_secur_get_auth_level_rsp_t;

/** @brief Get authentication level for target device
    @param param - index of buffer @ref zb_zdo_secur_get_auth_level_req_s
    @param cb    - user's function to call when response is ready @ref zb_zdo_secur_get_auth_level_rsp_s
    @return        ZDP transaction sequence number if call was made by ZR/ZED
    @return        0x00 if call was made by ZC and transaction was successful
    @return        0xFF if operation cannot be performed now (nor enough memory, resources, etc.)

    @snippet r23_new_api/r23_zc.c sec_get_auth_lvl_snippet
 */
zb_uint8_t zb_zdo_get_auth_level_req(zb_uint8_t param, zb_callback_t cb);

/** @} r23_secur */

/** @addtogroup r23_allhubs
    @{
*/

#if defined ZB_PARENT_CLASSIFICATION || defined DOXYGEN
/**
   Overwrite the auto-detected TC connectivity flag.

   That flag is related to All Hubs feature and WWAH.
   Note: applicable for ZRs only.

   @param val - if ZB_TRUE, that ZR has TC connectivity
 */
void zb_set_tc_connectivity(zb_bool_t val);

/**
   Get current value of the TC connectivity flag.

   @return if ZB_TRUE, that ZR has TC connectivity
 */
zb_bool_t zb_get_tc_connectivity(void);
#endif /* ZB_PARENT_CLASSIFICATION && ZB_ROUTER_ROLE */

#if defined ZB_ROUTER_ROLE || defined ZB_COORDINATOR_ROLE || defined DOXYGEN
/**
 * Public API to set nwk_preferred_parent to val
 */
void zb_set_nwk_preferred_parent(zb_bool_t val);

#endif

/** @} addtogroup r23_allhubs */

/** @addtogroup zdo_mgmt
    @{
*/

/** @brief Parameters for Mgmt_Permit_Joining_req.
  * @see ZB spec, subclause 2.4.3.3.7.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_req_s
{
  zb_uint8_t permit_duration;   /**< The length of time in seconds.  0x00 and 0xff indicate that
                                  * permission is disabled or enabled
                                  */
  zb_uint8_t tc_significance;   /**< If this is set to 0x01 and the remote device is the Trust
                                  * Center, the command affects the
                                  * Trust Center authentication policy as described in the
                                  * sub-clauses below; According to
                                  * r21, should be always set to 0x01.
                                  */
} ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_req_t;

/** @brief Parameters for zb_zdo_mgmt_permit_joining_req.  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_req_param_s
{
  zb_uint16_t dest_addr;        /**< Destination address */
  zb_uint8_t permit_duration;   /**< The length of time in seconds.  0x00 and 0xff indicate that
                                  * permission is disabled or enabled
                                  */
  zb_uint8_t tc_significance;   /**< If this is set to 0x01 and the remote device is the Trust
                                  * Center, the command affects the Trust Center authentication
                                  * policy as described in the sub-clauses below; If this is set to
                                  * 0x00, there is no effect on the Trust
                                  * Center.
                                  * Ignored for r21.
                                  */
} ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_req_param_t;

/**
   @brief sends   Mgmt_Permit_Joining_req (See Zigbee spec 2.4.3.3.7)
   @param param - Index of buffer with request
   @param cb    - user's function to call when got response from the remote.
   @return        0xFF if the operation was NOT executed (not enough memory,
                  resources, etc.), a valid ZDP transaction sequence number
                  otherwise

   @note          If called with destination address equal to device's address,
                  calls nlme-permit_joining.req locally changing permit joining state on device.
                  If called with destination address != local address (can be unicast or broadcast),
                  forces nlme-permit_joining.req call on remote device (devices),
                  but not on a local device keeping local permit joining state unchanged.

   @snippet onoff_server/on_off_switch_zed.c zdo_mgmt_permit_joining_req

 */
zb_uint8_t zb_zdo_mgmt_permit_joining_req(zb_uint8_t param, zb_callback_t cb);

/** @} */
/** @addtogroup zdo_groups
    @{
*/


/** @brief Response from zb_zdo_mgmt_permit_joining_req.  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_resp_t;

/** @brief Not Supported Response */
typedef ZB_PACKED_PRE struct zb_zdo_not_supported_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_not_supported_resp_t;

/**
 * @brief Parameters for Mgmt_NWK_IEEE_Joining_List_req.
 * @see ZB r22 spec, subclause 2.4.3.3.11.
 */
typedef struct zb_zdo_mgmt_nwk_ieee_joining_list_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for nwkIeeeJoiningList to be returned */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_nwk_ieee_joining_list_param_t;

/**
 * @brief Request for Mgmt_NWK_IEEE_Joining_List_req.
 * @see ZB r22 spec, subclause 2.4.3.3.11.
 */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_ieee_joining_list_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for nwkIeeeJoiningList to be returned */
}
ZB_PACKED_STRUCT zb_zdo_mgmt_nwk_ieee_joining_list_req_t;


/**
 * @brief Response for Mgmt_NWK_IEEE_Joining_List_rsp.
 * @see ZB spec, subclause 2.4.4.4.11.
 */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_ieee_joining_list_rsp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */

  zb_uint8_t status; /*!< The status of the Mgmt_NWK_IEEE_Joining_List_req command. */

  zb_uint8_t ieee_joining_list_update_id;

  zb_uint8_t joining_policy; /*<! Joining policy enumeration. */

  zb_uint8_t ieee_joining_list_total;

  zb_uint8_t start_index; /*<! The starting index in the mibIeeeJoiningList. */

  zb_uint8_t ieee_joining_count; /*<! Number of ieee addresses following this structure. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_ieee_joining_list_rsp_t;

/** @brief ZDO interface for ADD-GROUP.request.

   @note zb_apsme_add_group_request does not call confirm callback.

   @param param - (in/out) buffer with parameters
      - in - @ref zb_apsme_add_group_req_s
      - out - @ref zb_apsme_add_group_conf_s

   @b Example
   @snippet scenes/scenes_zed.c zb_zdo_add_group_req_snippet

 */
void zb_zdo_add_group_req(zb_uint8_t param);


/** @brief ZDO interface for REMOVE-GROUP.request
  * @param param - (in/out) buffer with parameters
  * @snippet doxygen_snippets.dox tp_pro_bv-46_zed_certification_TP_PRO_BV-46_tp_pro_bv-46_zed_c
  *
  */
void zb_zdo_remove_group_req(zb_uint8_t param);

/** @brief ZDO interface for REMOVE-ALL-GROUPS.request
  * @param param - (in/out) buffer with parameters
  *
  */
void zb_zdo_remove_all_groups_req(zb_uint8_t param);

/** @brief ZDO interface for ZCL Get Group Membership Command
  * @param param - (in/out) buffer with parameters
  *
  */
void zb_zdo_get_group_membership_req(zb_uint8_t param);

/** @} */
/** @addtogroup zdo_mgmt
    @{ */

/**
   Parameters of Device_annce primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_device_annce_s
{
  zb_uint8_t       tsn;         /*!< TSN of command */
  zb_uint16_t      nwk_addr;    /*!< NWK address for the Local Device  */
  zb_ieee_addr_t   ieee_addr;   /*!< IEEE address for the Local Device  */
  zb_uint8_t       capability;  /*!< Capability of the local device */
} ZB_PACKED_STRUCT zb_zdo_device_annce_t;

/**
   Arguments of the NLME-LEAVE.indication routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_leave_indication_s
{
  zb_ieee_addr_t device_address; /**< 64-bit IEEE address of the device to remove, zero fill if device
                                  * itself */
  zb_uint8_t rejoin;            /**< Join after leave */
} ZB_PACKED_STRUCT
zb_nlme_leave_indication_t;

/**
   Arguments of asynchronous Get Long Poll Interval response.
*/
typedef ZB_PACKED_PRE struct zb_zdo_pim_get_long_poll_interval_resp_s
{
  zb_time_t interval; /**< long poll interval */
} ZB_PACKED_STRUCT
zb_zdo_pim_get_long_poll_interval_resp_t;

/*! @} */

#ifndef ZB_COORDINATOR_ONLY

/**
  *  @addtogroup zdo_poll_control
  *  @{
  */


/**
 * @brief Toggle Turbo Poll feature.
 * @param permit ZB_TRUE to enable, ZB_FALSE to disable Turbo Poll.
*/
void zb_zdo_pim_permit_turbo_poll(zb_bool_t permit);

/**
 * @brief Toggle Turbo Poll retry feature.
 * This feature enables device to retry polling attempt some times
 * in case if device haven't got any data after receiving ack with pending bit set.
 * This feature can be toggled after joining/rejoining any network.
 * It restores its default value upon leave.
 * Default value: disabled.
 * @param enable ZB_TRUE to enable, ZB_FALSE to disable retries.
*/
void zb_zdo_pim_toggle_turbo_poll_retry_feature(zb_bool_t enable);

#ifndef ZB_USE_INTERNAL_HEADERS

/**
 * @brief Start Turbo Poll adaptive algorithm for the number of packets.
 * Turbo Poll will be disabled after receiving the specified number of packets
 * or after the poll timeout will rise up to the Long Poll interval.
 * @param n_packets Number of packets.
 */
void zb_zdo_pim_start_turbo_poll_packets(zb_uint8_t n_packets);

/**
 * @brief Change the Long Poll interval.
 * @note
 *  - A preferable way to change the Long Poll interval if the Poll Control cluster is not defined.
 * Else the Poll Control cluster API should be used to set the Long Poll Interval attribute.
 *  - Long Poll interval configuration should be done after the join procedure because during join the Long Poll
 * interval will be set to the default value @ref ZB_PIM_DEFAULT_LONG_POLL_INTERVAL.
 * @param ms New Long Poll interval in milliseconds.
 */
void zb_zdo_pim_set_long_poll_interval(zb_time_t ms);

#endif /* ZB_USE_INTERNAL_HEADERS */

/**
 * @brief Start the Turbo Poll adaptive algorithm for the specified timeout.
 * @param turbo_poll_timeout_ms Timeout in milliseconds.
 */
void zb_zdo_pim_start_turbo_poll_continuous(zb_time_t turbo_poll_timeout_ms);

/**
 * @brief Stop the Turbo Poll adaptive algorithm initiated by @ref zb_zdo_pim_start_turbo_poll_continuous before the timeout expires.
 * Restores the Long Poll interval.
 * @param param Not used, added for the correct scheduling of this function. Usually specified as zero.
 */
void zb_zdo_pim_turbo_poll_continuous_leave(zb_uint8_t param);

/**
 * @brief Get the Long Poll Interval.
 *
 * @param param buffer that will be used to store response parameters.
 * @param cb
 *
 * @return Long Poll interval in milliseconds.
*/
void zb_zdo_pim_get_long_poll_interval_req(zb_uint8_t param, zb_callback_t cb);

#else
#define zb_zdo_pim_turbo_poll_continuous_leave(param)
#define zb_zdo_pim_permit_turbo_poll(permit)
#define zb_zdo_pim_start_turbo_poll_continuous(turbo_poll_timeout_ms)
#define zb_zdo_pim_start_turbo_poll_packets(n_packets)
#define zb_zdo_pim_set_long_poll_interval(ms)
#define zb_zdo_pim_get_long_poll_interval_req(param, cb)
#endif

/** @} */

#ifdef ZB_REJOIN_BACKOFF
/** @addtogroup zdo_rejoin_backoff
    @{ */

/** @brief Start rejoin backoff procedure.
 * If the method is to be directly called from application, it is expected that MAC layer and PIB cache
 * are properly initialized. For starting device call zb_zdo_dev_start_cont instead - if device is commissioned it will
 * reinit network settings and trigger rejoin from inside the stack.
 *
 * @param insecure_rejoin - specify if insecure rejoin is allowed
 * @return RET_OK on success
 * @return RET_ALREADY_EXISTS if rejoin backoff is running already
 */
zb_ret_t zb_zdo_rejoin_backoff_start(zb_bool_t insecure_rejoin);

/** @brief Run next rejoin backoff iteration */
void zb_zdo_rejoin_backoff_continue(zb_uint8_t param);

/** @brief Force rejoin backoff iteration start */
zb_bool_t zb_zdo_rejoin_backoff_force(void);

/** @brief Return rejoin backoff status: running (ZB_TRUE) or not running (ZB_FALSE) */
zb_bool_t zb_zdo_rejoin_backoff_is_running(void);

/** @brief Clear rejoin backoff context, cancel scheduled function */
void zb_zdo_rejoin_backoff_cancel(void);
/*! @} */
#endif /* ZB_REJOIN_BACKOFF */

#if defined ZB_ENABLE_ZLL
/** @cond touchlink */

/**
 *  @brief Set Touchlink Master key
 *  @param param [IN] - Touchlink Master key value
 */
void zb_zdo_touchlink_set_master_key(zb_uint8_t *key);

/**
 *  @brief Set Touchlink NWK channel
 *  @param param [IN] - Touchlink NWK channel value
 */
void zb_zdo_touchlink_set_nwk_channel(zb_uint8_t channel);

/**
 *  @brief Set Touchlink RSSI threshold (used for Touchlink commissioning procedure)
 *  @param param [IN] - Touchlink RSSI threshold value
 */
void zb_zdo_touchlink_set_rssi_threshold(zb_int8_t rssi_threshold);

/**
 *  @brief Get Touchlink RSSI threshold
 *  @return Current Touchlink RSSI threshold value
 */
zb_int8_t zb_zdo_touchlink_get_rssi_threshold(void);

/**
 *  @brief Set Touchlink RSSI correction
 *  @param param [IN] - Touchlink RSSI correction value
 */
void zb_zdo_touchlink_set_rssi_correction(zb_uint8_t rssi_correction);

/**
 *  @brief Get Touchlink RSSI correction
 *  @return Current Touchlink RSSI correction value
 */
zb_uint8_t zb_zdo_touchlink_get_rssi_correction(void);

/** @endcond */ /* touchlink */
#endif /* ZB_ENABLE_ZLL */

#define ZB_NWK_LPD_NOTIFICATION  0x01U
#define ZB_NWK_LPD_REQUEST       0x02U

#ifndef ZB_LITE_NO_CONFIGURABLE_POWER_DELTA
zb_ret_t zb_zdo_set_lpd_cmd_mode(zb_uint8_t mode);
void zb_zdo_set_lpd_cmd_timeout(zb_uint16_t timeout);
#else
#define zb_zdo_set_lpd_cmd_mode(mode)
#define zb_zdo_set_lpd_cmd_timeout(timeout)
#endif  /* ZB_LITE_NO_CONFIGURABLE_POWER_DELTA */

/**
   @brief Disable channel update (affected for coordinator role only)

   The current ZB channel can be changed inside the stack for internal reasons or
   as a result of processing Mgmt NWK Update Notify commands. Call this function to
   prevent such channel changes if it is not allowable for the application.

   @param disable - specify if channel update is disabled (enabled by default)
*/
void zb_zdo_disable_network_mgmt_channel_update(zb_bool_t disable);

/**
 *  @brief Setups insecure join flag
 */
void zb_zdo_set_aps_unsecure_join(zb_bool_t insecure_join);

/**
 * @addtogroup zdo_distributed_security
 * @{
 */
#if defined ZB_DISTRIBUTED_SECURITY_ON || defined DOXYGEN

/**
 *  @brief Set custom distributed key
 *
 *  @param key_ptr - pointer to the key
 */
void zb_zdo_set_tc_standard_distributed_key(zb_uint8_t *key_ptr);

/**
 *  @brief Allow to setup network as distributed when started
 */
void zb_zdo_setup_network_as_distributed(void);

/**
 *  Enable distributed security linkage
 * 
 *  Without that call ZR is not able to create a Distributed network.
 * 
 *  @deprecated This function will be removed in January 2024. 
 *  Use instead:
 *  - @ref zb_bdb_enable_distributed_network_formation
 */
void zb_enable_distributed(void);

/**
 *  @brief Disable distributed security network formation at runtime
 *  
 * 
 *  @deprecated This function will be removed in January 2024. 
 *  Use instead:
 *  - @ref zb_bdb_enable_distributed_network_formation
 */
void zb_disable_distributed(void);

/**
 *  @brief Enable distributed security network formation at runtime
 * 
 * After call the function device won't try
 * to join, but will form a distributed security network instead.
 */
void zb_bdb_enable_distributed_network_formation (void);


/**
 *  @brief Disable distributed security network formation at runtime
 * 
 * After call the function the device will not be able to form a distributed security
 * network, but can join another distributed network.
 */
void zb_bdb_disable_distributed_network_formation (void);


/**
 *  @brief Enable/disable distributed security network formation at runtime
 * 
 * After call the function with the enable param set to ZB_TRUE, device won't try
 * to join, but will form a distributed security network instead. If enable param
 * is set to ZB_FALSE, the device will not be able to form a distributed security
 * network, but can join another network.
 * 
 *  @param enable - ZB_TRUE to enable distributed formation, ZB_FALSE to disable. 
 *
 * @deprecated This function will be moved to the private header in April 2025. Use @ref
 * zb_bdb_enable_distributed_network_formation() or @ref zb_bdb_disable_distributed_network_formation() instead.
 */
void zb_bdb_enable_distributed_formation (zb_bool_t enable);

#if defined ZB_JOIN_CLIENT
/**
 *  @brief Enable joining to a distributed network
 **/
void zb_enable_joining_to_distributed_network (void);


/**
 *  @brief Disable joining to a distributed network
 */
void zb_disable_joining_to_distributed_network (void);


/**
 * @brief Enable joining to a distributed network
 * 
 *  @param enable - ZB_TRUE to enable, ZB_FALSE to disable
 *
 * @deprecated This function will be moved to the private header in April 2025. Use @ref
 * zb_enable_joining_to_distributed_network() or @ref zb_disable_joining_to_distributed_network() instead.
 */
void zb_enable_joining_to_distributed (zb_bool_t enable);

#endif /* ZB_JOIN_CLIENT */

#endif /* ZB_DISTRIBUTED_SECURITY_ON */

/**
 *  @brief Check if the current network is a distributed security network
 */
zb_bool_t zb_is_network_distributed(void);

/** @} */ /* zdo_distributed_security */

/*! @addtogroup af_api */
/*! @{ */

/**
 * @addtogroup af_management_service AF management service
 * @{
 */
/**
 *  @brief Set up application callback to be called for APS data packets.
 *
 *  That API call may be useful for tests which uses custom profile id or which needs to send raw
 *  data over APS.
 *  @attention Function is called on receive of APS data frame BEFORE internal processing. Please
 *  make sure that this callback does not handle packets on standard ZB profiles - this should be
 *  done by the stack.
 *
 *  @param cb - callback to call when AF got APS packet. @ref zb_apsde_data_indication_t
 *
 *  @b Example:
 *  @code
 *  void zboss_signal_handler(zb_uint8_t param)
 *  {
 *    zb_zdo_app_signal_hdr_t *sg_p = NULL;
 *    zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, &sg_p);
 *
 *    if (ZB_GET_APP_SIGNAL_STATUS(param) == 0)
 *    {
 *      switch(sig)
 *      {
 *        case ZB_ZDO_SIGNAL_DEFAULT_START:
 *          TRACE_MSG(TRACE_APS1, "Device STARTED OK", (FMT__0));
 *          zb_af_set_data_indication(data_indication);
 *          break;
 *        default:
 *          TRACE_MSG(TRACE_APP1, "Unknown signal", (FMT__0));
 *          break;
 *      }
 *      zb_buf_free(buf);
 *    }
 *  }
 *
 *  zb_uint8_t data_indication(zb_bufid_t param)
 *  {
 *    zb_ushort_t i;
 *    zb_uint8_t *ptr;
 *    zb_apsde_data_indication_t *ind = ZB_GET_BUF_PARAM(param, zb_apsde_data_indication_t);
 *
 *    if (ind->profileid == TEST_CUSTOM_PROFILE_ID)
 *    {
 *      ptr = ZB_APS_HDR_CUT(param);
 *
 *      TRACE_MSG(TRACE_APS3, "apsde_data_indication: packet %p len %hd status 0x%hx from %d",
 *                (FMT__P_D_D_D, param, zb_buf_len(param), zb_buf_get_status(param), ind->src_addr));
 *
 *      for (i = 0 ; i < zb_buf_len(param) ; ++i)
 *      {
 *        TRACE_MSG(TRACE_APS3, "%x %c", (FMT__D_C, (int)ptr[i], ptr[i]));
 *      }
 *      zb_buf_free(param);
 *      return ZB_TRUE;
 *    }
 *    return ZB_FALSE;
 *  }
 *  @endcode
 */
void zb_af_set_data_indication(zb_device_handler_t cb);

/**
 *  @brief Perform "Reset with a Local Action" procedure (as described in BDB spec, chapter 9.5).
 *  The device will perform the NLME leave and clean all Zigbee persistent data except the outgoing NWK
 *  frame counter and application datasets (if any).
 *  The reset can be performed at any time once the device is started (see @ref zboss_start).
 *  After the reset, the application will receive the @ref ZB_ZDO_SIGNAL_LEAVE signal.
 *
 *  @param param - buffer reference (if 0, buffer will be allocated automatically)
 */
void zb_bdb_reset_via_local_action(zb_uint8_t param);

#if defined ZB_BDB_MODE && !defined BDB_OLD
/**
 *  @brief Starts TC rejoin procedure
 *
 * If device doesn't have a TCLK and UnsecureTcRejoinEnabled policy
 * is set to ZB_FALSE (this is the default setting), TC rejoin won't
 * be performed and ZB_BDB_SIGNAL_TC_REJOIN_DONE signal with RET_ERROR
 * status will be raised.
 *
 *  @param param - buffer reference (if 0, buffer will be allocated automatically)
 */
void zb_bdb_initiate_tc_rejoin(zb_uint8_t param);
#endif /* ZB_BDB_MODE && !BDB_OLD */

/** @} */ /* af_management_service */
/*! @} */


/**
 * @addtogroup r23_allhubs
 * @{
 */

/**
   Parameter for number of r23 ZDO commands
 */
typedef ZB_PACKED_PRE struct zb_zdo_common_req_param_s
{
  zb_uint16_t   dst_addr; /*!< Destination address */
} ZB_PACKED_STRUCT
zb_zdo_common_req_param_t;

/**
 * @brief ZDO Get Configuration Request parameters
 */
typedef struct zb_zdo_get_configuration_req_param_s
{
  zb_uint16_t   dst_addr;                     /*!< Destination address */
  zb_bitfield_t get_supported_kn_methods:1;   /*!< Supported key negotiations methods */
  zb_bitfield_t get_pan_id_conflict_report:1; /*!< PAN ID conflict report */
  zb_bitfield_t get_next_pan_id:1;            /*!< Next PAN ID */
  zb_bitfield_t get_next_channel_change:1;    /*!< Next Channel change */
  zb_bitfield_t get_router_info:1;            /*!< Router information */
  zb_bitfield_t get_fragmentation_params:1;   /*!< Fragmentation information */
  zb_bitfield_t get_configuration_mode_params:1; /*!< Configuration parameters */
  zb_bitfield_t reserved:1;
}
zb_zdo_get_configuration_req_param_t;

/**
 * @brief ZDO Get Configuration Response parameters
 */
typedef struct zb_zdo_get_configuration_resp_param_s
{
  zb_uint8_t tsn;                                 /*!< tsn of ZDO request  */
  zb_uint8_t status;                              /*!< @ref zdp_status  */
  zb_bitfield_t configuration_mode_params_present:1; /*!< Configuration parameters */
  zb_bitfield_t supported_kn_methods_present:1;   /*!< Supported key negotiations methods */
  zb_bitfield_t pan_id_conflict_report_present:1; /*!< PAN ID conflict report */
  zb_bitfield_t next_pan_id_present:1;            /*!< Next PAN ID */
  zb_bitfield_t next_channel_change_present:1;    /*!< Next Channel change */
  zb_bitfield_t router_info_present:1;            /*!< Router information */
  zb_bitfield_t fragmentation_params_present:1;   /*!< Fragmentation information */
  /* if configuration_mode_params_present  */
  zb_bitfield_t restricted_mode_enabled:1;
  zb_bitfield_t require_link_key_encr_for_tk:1;
  zb_bitfield_t leave_req_allowed:1;
  /* if supported_kn_methods_present  */
  zb_uint8_t kn_protocols_bitmask;
  zb_uint8_t psk_secrets_bitmask;
  /* if pan_id_conflict_report_present  */
  zb_uint16_t pan_id_conflict_count;
  /* if next_pan_id_present  */
  zb_uint16_t next_pan_id;
  /* if next_channel_change_present  */
  zb_uint32_t next_channel_change;
  /* if router_info_present  */
  zb_uint16_t router_info;
  /* if fragmentation_params_present  */
  zb_uint8_t fragmentation_opt;
  zb_uint16_t max_incoming_transfer_unit;
} zb_zdo_get_configuration_resp_param_t;

/**
   @brief Configuration parameters
   Stack sends set_configuration_req to the device with set values
  */
typedef struct zb_zdo_configuration_parameters_s
{
  zb_uint16_t dst_addr;                         /*!< destination address */
  zb_bitfield_t restricted_mode_enabled:1;      /*!< apsZdoRestrictedMode, if 1 then certain ZDO commands will not be
                                                     accepted unless sent by Trust Center with APS encryption
                                                     if 0 then other nodes on the nwk may change configuration of the device
                                                     (e.g. bindings)*/
  zb_bitfield_t require_link_key_encr_for_tk:1; /*!< requireLinkKeyEncryptionForApsTransportKey, if 1 then device will
                                                     require that the APS Transport key command shall be APS encrypted with the
                                                     device's unique Trust Center Link Key*/
  zb_bitfield_t leave_req_allowed:1;            /*!< nwkLeaveRequestAllowed, this policy determines whether or not a remote NWK leave
                                                     request command frame received by the local device is accepted */
  zb_bitfield_t disable_channel_change:1;       /*!< if 1, then set current channel-page into device's nwkNextChannelChange.
                                                     It forbid channel change for device.
                                                     @see zb_zdo_send_next_channel_change for manage channel change */
  zb_bitfield_t disable_panid_change:1;         /*!< if 1, then set current PAN ID into device's nwkNextPanId.
                                                     It forbid pan id change for device.
                                                     @see zb_zdo_send_next_panid_change for manage PAN ID change */
  zb_bitfield_t reserved:3;
} zb_zdo_configuration_parameters_t;


/**
   @brief Next PAN ID change parameters
   Stack sends set_configuration_req to the device with set values
  */
typedef struct zb_zdo_next_panid_change_parameters_s
{
  zb_uint16_t dst_addr;                   /*!< destination address */
  zb_uint16_t next_panid_change;          /*!< nwkNextPanId, this field indicates the next PAN ID
                                               that will be used once a Network Update command is received
                                               to change PAN IDs. A value 0xFFFF allows any PAN ID to be accepted */
} zb_zdo_next_panid_change_parameters_t;


/**
   @brief Next channel change parameters
   Stack sends set_configuration_req to the device with set values
  */
typedef struct zb_zdo_next_channel_change_parameters_s
{
  zb_uint16_t dst_addr;                   /*!< destination address */
  zb_uint32_t next_channel_change;        /*!< nwkNextChannelChange, this field indicates the next channel
                                               that will be used once a start channel change command is received.
                                               Only 1 channel page and channel bit shall be set.
                                               A value 0 indicates any channel is valid as the next channel */
} zb_zdo_next_channel_change_parameters_t;


/**
 Max size of eui64_list. No more than 5 entry are placed in the ZB packet
*/
#define ZB_ZDO_CLEAR_ALL_BIND_EUI64_LIST_SIZE (5u)

/**
   @brief Parameters for zdo_clear_all_bind_req
  */
typedef struct zb_zdo_clear_all_bind_req_param_s
{
  zb_uint16_t dst_addr;                   /*!< destination address */
  zb_uint8_t eui64_count;                 /*!< eui64 count */
  zb_ieee_addr_t eui64_list[ZB_ZDO_CLEAR_ALL_BIND_EUI64_LIST_SIZE]; /*!< eui64 list, a value 0xFFFFFFFFFFFFFFFF is a wildcard */
} zb_zdo_clear_all_bind_req_param_t;

/**
Parameter for ZDO Secur_Decommission_req
 */
typedef zb_zdo_clear_all_bind_req_param_t zb_zdo_decommission_req_param_t;


/**
   @brief Sends secur_set_configuration_req command with configuration parameters

   That API can be called in centralized network only at TC; in
   distributed network - at any device.

   @param param - index of buffer with request @ref zb_zdo_configuration_parameters_s
   @param cb    - user's function to call when got response from the remote
   @return ZDP transaction sequence number or
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)

   @snippet r23_new_api/r23_zc.c r23_config_req_snippet
*/
zb_uint8_t zb_zdo_send_configuration_parameters(zb_uint8_t param, zb_callback_t cb);

/**
   @brief Sends secur_set_configuration_req command with next pan id change information
   @param param - index of buffer with request @ref zb_zdo_next_panid_change_parameters_s
   @param cb    - user's function to call when got response from the remote.
   @return ZDP transaction sequence number or
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)
*/
zb_uint8_t zb_zdo_send_next_panid_change(zb_uint8_t param, zb_callback_t cb);

/**
   @brief Sends secur_set_configuration_req command with next channel change information
   @param param - index of buffer with request @ref zb_zdo_next_channel_change_parameters_s
   @param cb    - user's function to call when got response from the remote.
   @return ZDP transaction sequence number or
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)
*/
zb_uint8_t zb_zdo_send_next_channel_change(zb_uint8_t param, zb_callback_t cb);


/**
   @brief Sends Secur_Get_Configuration_req command
   @param param - index of buffer with request
   @param cb    - user's function to call when got response from the remote.
   @return ZDP transaction sequence number or
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)
*/
zb_uint8_t zb_zdo_get_configuration_req(zb_uint8_t param, zb_callback_t cb);

/**
   @brief Start filling of body of ZDO Secur Get Configuration Req

   If ZDO Secur Get Configuration Req queries about just one tlv, call to that function is enough.
   If it queries about > 1 tlv, continue filling using returned ptr.

   @param param - output buffer
   @param dst - destination address
   @param tlv_count - count of tlvs in the request
   @param first_tlv_id - id of the first tlv id

   @return pointer to the rest of tlv ids (to be filled by the caller).
 */
zb_uint8_t *zdo_fill_get_configuration_req(zb_uint8_t param, zb_uint16_t dst, zb_uint8_t tlv_count, zb_uint8_t first_tlv_id);

#if defined ZB_COORDINATOR_ROLE || defined DOXYGEN
/**
   @brief Sends Secur_Decommission_req command
   @param param - index of buffer with request @ref zb_zdo_decommission_req_param_t
   @param cb    - user's function to call when got response from the remote.
   @return ZDP transaction sequence number or
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)

   @snippet r23_new_api/r23_zc.c decommis_snippet
*/
zb_uint8_t zb_zdo_decommission_req(zb_uint8_t param, zb_callback_t cb);
#endif /* ZB_COORDINATOR_ROLE */

/** @} r23_allhubs */

/**
@addtogroup r23_secur
@{
*/
#if defined ZB_COORDINATOR_ROLE || defined DOXYGEN
/**
   @brief Send Security_Key_Update_req for update EXISTING verified tclk
   @param param - index of buffer with request
   @param dev_ieee_addr - extended address of device, which tclk should be updated.
   @return RET_OK if the request was sent
 */
zb_ret_t zb_zdo_secur_update_device_tclk(zb_uint8_t param, zb_ieee_addr_t dev_ieee_addr);
#endif /* ZB_COORDINATOR_ROLE */

/** @} r23_secur */

/**
@addtogroup r23_allhubs
@{
*/

/**
   Max number of channels to process at a single beacon survey command
 */
#define ZB_SURVEY_BEACON_MAX_CHANNEL_PAGE_CNT 10U

/** @brief Parameters for beacon_survey_req command */
typedef struct zb_zdo_mgmt_beacon_survey_param_s
{
  zb_uint8_t  channel_page_cnt;
  zb_uint32_t channel_page_list[ZB_SURVEY_BEACON_MAX_CHANNEL_PAGE_CNT];
  zb_uint8_t  config_mask;
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_beacon_survey_param_t;

/**
   @brief Sends Mgmt beacon survey request

   @param param - buffer holding in parameters section @ref zb_zdo_mgmt_beacon_survey_param_t
   @param cb - callback to be called on ZDO command complete. Survey result is in data section of param of type @ref zb_zdo_beacon_survey_resp_params_t

   @snippet r23_new_api/r23_zc.c beacon_survey_snippet
 */
zb_uint8_t zdo_mgmt_beacon_survey_req(zb_uint8_t param, zb_callback_t cb);



/* moved there from zb_zcl_wwah.h, next from zdo_wwah_survey_beacons.h */
/**
   Beacon survey result for a single potential parent. Common deniminator for r23 and WWAH.
 */
typedef ZB_PACKED_PRE struct zb_zcl_wwah_beacon_survey_s
{
  /*<! The DeviceShort field contains the 16 bit short address of the beaconing device. */
  zb_uint16_t device_short;
  /*! RSSI of the beacon, expressed in dBm. Filled by WWAH onlt */
  zb_int8_t rssi;
  /*<! LQA of the beacon. Filled by r23 Beacon Survey only */
  zb_uint8_t lqi;
  /*<! parent classification mask. Filled only by WWAH, always 0 in r23 */
  zb_uint8_t classification_mask;
} ZB_PACKED_STRUCT zb_zcl_wwah_beacon_survey_t;

typedef zb_zcl_wwah_beacon_survey_t zb_zdo_beacon_survey_parent_info_t;

#if (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)

#define ZDO_WWAH_MAX_BEACON_SURVEY 10U

void zb_zcl_wwah_send_survey_beacons_response(zb_bufid_t buf, zb_uint16_t zcl_buf);

#else /* ZB_ZCL_SUPPORT_CLUSTER_WWAH */

/* see R23, I.3.5 Potential Parents TLV, 'Count of Potential Parents' field */
#define ZDO_WWAH_MAX_BEACON_SURVEY 5U
#endif /* (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER) */

#define ZDO_WWAH_MAX_BEACON_SURVEY_BYTES    \
  (sizeof(zb_zdo_beacon_survey_parent_info_t) \
   * (ZDO_WWAH_MAX_BEACON_SURVEY))          \

/**
 *  @brief R23, I.3.3 Beacon Survey Configuration TLV
 *
 *  This TLV is 5-bytes in length and contains information
 *  about the channels and scan configuration that is used
 *  when performing a beacon survey.
 */

/**
 *  @brief R23, I.3.4 Beacon Survey Results TLV
 *
 *  This TLV is 4-bytes in length and contains information about the
 *  channels, scan configuration, and counted devices or beacons.
 */
typedef ZB_PACKED_PRE struct zb_zdo_beacon_survey_results_t
{
  /*!< Total Beacons Surveyed */
  zb_uint8_t total_beacons_surveyed;

  /*!< Number of beacons with current PAN ID */
  zb_uint8_t num_cur_nwk_beacons;

  /*!< Number of potential parents from the current Zigbee Network */
  zb_uint8_t num_potential_parents_current_zbn;

  /*!< Number of other Zigbee beacons */
  zb_uint8_t num_other_nwk_beacons;
}
ZB_PACKED_STRUCT zb_zdo_beacon_survey_results_t;

/**
 *  @brief R23, I.3.5 Potential Parents TLV
 *
 *  This TLV is 3 to 13 bytes in length and indicates the number
 *  of available parents in radio range. A maximum number of 5
 *  parents is supported for this TLV. The list of potential
 *  parents SHALL be ordered as described in 3.6.1.3.2.
 */
typedef ZB_PACKED_PRE struct zb_zdo_beacon_survey_potential_parents_t
{
  /*!< r23 Short address of the parent. Filled by r23 Beacon Survey only */
  zb_uint16_t current_parent;
  zb_uint8_t  current_parent_lqa; /*!< r23 - LQA of the current parent. Filled by r23 Beacon Survey only */
  /*!< Count of potential parents. Valid range [0..5] for r23-only build, [0..10] for WWAH (Indeed, WWAH requires to put there as much info as can be put into the single frame) */
  zb_uint8_t count_potential_parents;
  /*!< Potential parents list */
  zb_zcl_wwah_beacon_survey_t parent_list[ZDO_WWAH_MAX_BEACON_SURVEY];
}
ZB_PACKED_STRUCT zb_zdo_beacon_survey_potential_parents_t;

/**
 *  @brief Structure to store information when 'Beacon Survey' procedure is executing
 *
 *  We reuse a buffer which contain 'Beacon Survey Request' command
 *  and store all necessary information into it. Format of the
 *  'Beacon Survey Response' command is different between ZCL WWAH
 *  and r23, so we need to use two functions to parse results and
 *  send response
 */
typedef ZB_PACKED_PRE struct zb_zdo_beacon_survey_resp_params_s
{
  zb_uint8_t tsn;               /*!< tsn of ZDO request  */
  zb_uint8_t status;            /*!< @ref zdp_status  */
  /*!< The array of potential parents - different in r23 Figure 2-79 and WWAH ('Survey Beacons Response' Command) */
  zb_zdo_beacon_survey_potential_parents_t parents;
  /*!< r23 2.4.4.3.13.1.1 Beacon Survey Results TLV - exists in r23 only*/
  zb_zdo_beacon_survey_results_t results;
  zb_uint16_t panid_conflict_count; /*<! panid conflict TLV - r23 only  */
}
ZB_PACKED_STRUCT zb_zdo_beacon_survey_resp_params_t;


/** @} r23_allhubs */


/**
@addtogroup r23_r22
@{
*/

/**
   Switch to the legacy r22 joining by association, key update by Transport Key

   @snippet r23_new_api/r22_zr.c zboss_use_r22_behavior_snippet
 */
void zboss_use_r22_behavior(void);

/**
   Enable using of r23 joining featires: NWK Commissioning then DLK.

   @snippet r23_new_api/r23_zc.c zboss_use_r23_behavior_snippet
 */
void zboss_use_r23_behavior(void);

/**
   Use r22 joining instead nwk commissioning request
   @param value - if true, then mac association or rejoin_req will be used
*/
void zb_nwk_use_r22_joining(zb_bool_t value);

/** @} r23_r22 */


/**
 * @addtogroup debug_keys
 * @{
 */

/**
   Broadcast TCLK or other APS key. For debugging purposes only.

   That function broadcasts APS key stored in keypair for that long_addr. Wireshark understands that broadcast, so it can decode the traffic.
   That function has pure debug purposes. It must be explicitly called by the application.
   The function works in debug ZBOSS builds only.
   The function does nothing in release ZBOSS builds.

   @param long_addr - address of the remote device
   @return RET_OK if broadcast done ok, RET_NOT_FOUND if no such keypair, RET_ERROR if that function is disabled in that ZBOSS build

   @snippet r23_new_api/r23_zc.c debug_broadcast_key_snippet
 */
zb_ret_t zb_debug_broadcast_aps_key(zb_ieee_addr_t long_addr);

/**
   Broadcast NWK key.

   FOR DEBUGGING PURPOSES ONLY. It zeroes your network security. Never use it in productuion!

   That function broadcasts the current NWK key. Wireshark understands that broadcast, so it can decode the traffic.
   That function has pure debug purposes. It must be explicitly called by the application.

   The function works in debug ZBOSS builds only.
   The function does nothing in release ZBOSS builds.

   @return RET_OK if broadcast done ok, RET_ERROR if that function is disabled in that ZBOSS build

 */
zb_ret_t zb_debug_broadcast_nwk_key(void);

/*! @} addtogroup debug_keys */

/**
 * @addtogroup r23_dev_iterview
 * @{
 */


#if defined ZB_COORDINATOR_ROLE || defined DOXYGEN

/**
  Checks whether Device Interview is enabled

  @return Device Interview status
*/
zb_bool_t zb_tc_is_device_interview_enabled(void);


/**
  Enables Device Interview stage.

  @param enable - flag that shows whether device interview should be enabled
  Default value - ZB_FALSE, the stage is disabled

  @see ZB_ZDO_SIGNAL_DEVICE_READY_FOR_INTERVIEW.

  @snippet r23_new_api/r23_zc.c device_interview1_snippet
  @snippet r23_new_api/r23_zc.c device_interview2_snippet
  @snippet r23_new_api/r23_zc.c device_interview3_snippet
*/
void zb_tc_enable_device_interview(zb_bool_t enable);


/**
  Authenticates device after Device Interview.

  @see ZB_ZDO_SIGNAL_DEVICE_INTERVIEW_FINISHED

  @param device_addr - address of the device to authenticate

  @snippet r23_new_api/r23_zc.c device_interview1_snippet
  @snippet r23_new_api/r23_zc.c device_interview2_snippet
  @snippet r23_new_api/r23_zc.c device_interview3_snippet
*/
zb_ret_t zb_tc_auth_device_after_interview(const zb_ieee_addr_t device_addr);


/**
  Rejects device after Device Interview and send Leave command.

  @see ZB_ZDO_SIGNAL_DEVICE_INTERVIEW_FINISHED

  @param device_addr - address of the device to reject
*/
void zb_tc_reject_device_after_interview(const zb_ieee_addr_t device_addr);


/**
  Checks whether Device Interview is active for a device.

  @param device_addr - address of the device
*/
zb_bool_t zb_tc_is_interview_active_for_device(zb_uint16_t device_addr);
#endif /* ZB_COORDINATOR_ROLE */

/*! @} addtogroup r23_dev_iterview */

#endif /*ZB_ZBOSS_API_ZDO_H*/
