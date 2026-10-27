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
/* PURPOSE: ZBOSS BDB commissioning API header
*/

#ifndef ZBOSS_API_BDB_H
#define ZBOSS_API_BDB_H 1

#ifdef ZB_BDB_MODE
/*! \addtogroup zboss_bdb_api
@{
 @defgroup zboss_bdb_comm_params BDB commissioning parameters
 @defgroup zboss_bdb_comm_start BDB commissioning start & status
 @defgroup zboss_bdb_comm_fb BDB Finding and Binding
 @defgroup zboss_bdb_comm_tc_conn BDB TC connectivity checks
@}
*/

/**
   @addtogroup zboss_bdb_comm_params
   @{
*/

/** @cond internals_doc */
/**
  @brief BDB error codes
  */
enum zb_bdb_error_codes_e
{
  ZB_BDB_STATUS_SUCCESS = 0,                 /*!< The commissioning sub-procedure was successful.*/
  ZB_BDB_STATUS_IN_PROGRESS,                 /*!< One of the commissioning sub-procedures has started but is not yet complete.*/
  ZB_BDB_STATUS_NOT_AA_CAPABLE,              /*!< The initiator is not address assignment capable during touchlink. */
  ZB_BDB_STATUS_NO_NETWORK,                  /*!< A network has not been found during network steering or touchlink.*/
  ZB_BDB_STATUS_TARGET_FAILURE,              /*!< A node has not joined a network when requested during touchlink.*/
  ZB_BDB_STATUS_FORMATION_FAILURE,           /*!< A network could not be formed during network formation. */
  ZB_BDB_STATUS_NO_IDENTIFY_QUERY_RESPONSE,  /*!< No response to an identify query command has been received during finding and binding.*/
  ZB_BDB_STATUS_BINDING_TABLE_FULL,          /*!< A binding table entry could not be created due to insufficient space in the binding table during finding and binding. */
  ZB_BDB_STATUS_NO_SCAN_RESPONSE,            /*!< No response to a scan request inter-PAN command has been received during touchlink. */
  ZB_BDB_STATUS_NOT_PERMITTED,               /*!< A touchlink (steal) attempt was made when a node is already connected to a centralized security network.
                                                  A node was instructed to form a network when it did not have a logical type of either Zigbee coordinator or Zigbee router.*/
  ZB_BDB_STATUS_TCLK_EX_FAILURE,             /*!< The Trust Center link key exchange procedure has failed attempting to join a centralized security network.*/
  ZB_BDB_STATUS_NOT_ON_A_NETWORK,            /*!< A commissioning procedure was forbidden since the node was not currently on a network.*/
  ZB_BDB_STATUS_ON_A_NETWORK,                /*!< A commissioning procedure was forbidden since the node was currently on a network.*/
  ZB_BDB_STATUS_CANCELLED,                   /*!< The current operation (steering or formation) was cancelled by an app */
  ZB_BDB_STATUS_DEV_ANNCE_SEND_FAILURE,      /*!< A device announce sending has been failed (e.g. device announce haven't acked by parent router). */
  ZB_BDB_STATUS_PJOIN_FAILED,                /*!< Local Permit joining failed */
};
/** @endcond */ /* internals_doc */
/** @} */

/**
   @addtogroup zboss_bdb_comm_start
   @{
*/

/** @brief BDB commissioning mode
 * Parameter to the commissioning API
*/
typedef enum zb_bdb_commissioning_mode_e
{
  /** Call network steering procedure and network formation (if needed) */
  ZB_BDB_NETWORK_STEERING = 0,
  /** @cond touchlink */
  /** Touchlink: use Touchlink commissioning */
  ZB_BDB_TOUCHLINK_COMMISSIONING = 1,
  /** Touchlink: use Touchlink target */
  ZB_BDB_TOUCHLINK_TARGET = 2,
  /** @endcond */ /* touchlink */
  /** Call network steering only without formation procedure */
  ZB_BDB_NETWORK_STEERING_ONLY = 3,
  /** Call network formation only without steering procedure */
  ZB_BDB_NETWORK_FORMATION_ONLY = 4
} zb_bdb_commissioning_mode_t;


/**
 * @brief Starts the specified device commissioning steps.
 *
 * @details This function performs steering and network formation if it is appropriate for the device type.
 * @details Finding and binding is not performed by this function.
 * @details When the selected commissioning procedure finishes, one of the following ZBOSS signals is generated:
 *          - @ref ZB_BDB_SIGNAL_STEERING
 *          - @ref ZB_BDB_SIGNAL_FORMATION
 *
 * @note
 * DO NOT call this function from a callback after local leave using @ref zdo_mgmt_leave_req(),
 * because internal contexts will not be cleared correctly in such case!
 * Wait for the @ref ZB_ZDO_SIGNAL_LEAVE signal to restart top level commissioning, if necessary.
 *
 * @param[in] mode - commissioning mode
 *
 * @retval ZB_TRUE - in case the device starts successfully
 * @retval ZB_FALSE - in case an error occurred (for example, the device has already been running)
 *
 * @par Example
 * Starting BDB top level commissioning on startup signals:
 * @snippet linky_sample/erl_gw/erl_gw.c bdb_start_top_level_commissioning_snippet
*/
zb_bool_t bdb_start_top_level_commissioning(zb_uint8_t mode);

/**
 * @brief Start device joining procedure.
 *
 * Performs BDB steering procedure.
 * Raises ZB_BDB_SIGNAL_STEERING when steering is finished.
 *
 * @return RET_OK - if the joining procedure has been started correctly
 * @return RET_ERROR - if the device has already been joined
 *
 */
zb_ret_t zb_start_join();

/**
 * @brief Broadcast permit join req and open a network.
 *
 * Performs BDB on network steering procedure.
 * Raises ZB_BDB_SIGNAL_STEERING when steering is finished.
 *
 * @return RET_OK - if the procedure has been started correctly
 * @return RET_ERROR - if some errors have been occurred
 *
 */
zb_ret_t zb_open_network();

/**
 * @brief Start network formation procedure.
 *
 * Performs BDB network formation procedure.
 * Raises ZB_BDB_SIGNAL_FORMATION when formation is finished.
 *
 * @return RET_OK - if the formation procedure has been started correctly
 * @return RET_ERROR - if the formation has already been done
 *
 */
zb_ret_t zb_start_formation();

/**
 * @brief Start touchlink commissioning.
 *
 * Performs BDB initiator touchlink procedure.
 * Raises ZB_BDB_SIGNAL_TOUCHLINK when procedure is finished.
 *
 * @return RET_OK - if the procedure has been started correctly
 * @return RET_ERROR - if the any error has been occurred
 *
 */
zb_ret_t zb_start_touchlink_commissioning();


/**
 * @brief Start target touchlink procedure.
 *
 * Performs BDB target touchlink procedure.
 * Raises ZB_BDB_SIGNAL_TOUCHLINK_TARGET when procedure is finished.
 *
 * @return RET_OK - if the procedure has been started correctly
 * @return RET_ERROR - if the any error has been occurred
 *
 */
zb_ret_t zb_start_touchlink_target();


/**
 * @brief Cancels Network Steering procedure for a node not on the network.
 *
 * @param[in] buf - ZBOSS buffer
 *
 * @note The ZBOSS @ref ZB_BDB_SIGNAL_STEERING_CANCELLED signal with the status of this operation will be
 *       raised.
 *       Possible statuses:
 *       - @b RET_ILLEGAL_REQUEST (device is a ZC)
 *       - @b RET_INVALID_STATE (steering for a node not on the network is not in progress)
 *       - @b RET_PENDING (it is too late to cancel a steering, it will be completed soon)
 *       - @b RET_IGNORE (cancellation was already requested)
 *       - @b RET_OK (steering is cancelled successfully)
 *
 * @note If the steering is cancelled, the @ref ZB_BDB_SIGNAL_STEERING signal with the
 *       @b ZB_BDB_STATUS_CANCELLED status will be raised as well.
*/
void bdb_cancel_joining(zb_bufid_t buf);


/**
 * @brief Cancels Network Formation procedure.
 *
 * @param buf - ZBOSS buffer
 *
 * @note The ZBOSS @b ZB_BDB_SIGNAL_FORMATION_CANCELLED signal with the status of this operation will be raised.
 *       Possible statuses:
 *       - @b RET_INVALID_STATE (formation is not in progress)
 *       - @b RET_PENDING (it is too late to cancel the formation, it will be completed soon)
 *       - @b RET_IGNORE (cancellation was already requested)
 *       - @b RET_OK (formation is cancelled successfully)
 *
 * @note If the formation is cancelled, the @ref ZB_BDB_SIGNAL_FORMATION signal with the
 *       @b ZB_BDB_STATUS_FORMATION status will be raised as well.
*/
void bdb_cancel_formation(zb_bufid_t buf);


/**
 * @brief Sets scan duration for Energy Detection and Active scan.
 *
 * @param[in] duration - scan duration. Scan time is <tt>(@b aBaseSuperframeDuration * ((1<<@p duration) + 1))</tt>
 * @parblock
 *
 *
 *
 * Duration to seconds Table:
 * Duration | Time
 * :------: | :--:
 * 8 | ~4s
 * 5 | ~0.5s
 * 2 | ~0.08s
 * 1 | ~0.05s (0.046s)
 * @endparblock
 *
 * @cond DOCS_DEV_NOTES
 * In seconds - <tt>((@c 1l << @p duration) + 1) * 15360 / 1000000</tt>
 *
 * I am not sure about this formula ^^^
 * @endcond
 *
 * @see @e @b bdbScanDuration 5.3.9 (BDB 3.0.1)
 */
void bdb_set_scan_duration(zb_uint8_t duration);

/**
 * @brief Closes the network.
 *
 * @details This function implements BDB 3.0.1 - 8.1.1 "Local disabling of Network Steering."
 * @details It will broadcast a @b Mgmt_Permit_Joining_req with @b PermitDuration of 0.
 *
 * @details In case it is a router or a coordinator, the function will also issue @b NLME-PERMIT-JOINING.request primitive with @b PermitDuration of 0.
 * @details The ZBOSS signal @ref ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS will be raised with @ref zb_zdo_mgmt_permit_joining_req_param_t.permit_duration of 0.
 *
 * @param[in] buf - ZBOSS buffer; if zero is passed, a new buffer will be allocated.
 *
 * @retval RET_OK - broadcast was successful
 * @retval RET_NO_MEMORY - buffer allocation failed
 * @retval RET_ERROR - any error occurred
 *
 * @par Example
 * Application closing the network:
 * @snippet thermostat/thermostat_zr/thermostat_zr.c close_network_example
 */
zb_ret_t zb_bdb_close_network(zb_bufid_t buf);

/**
 * @brief Checks if the device is factory new.
 *
 * @retval ZB_TRUE - device is factory new
 * @retval ZB_FALSE - device is not factory new
 *
 * @par Example
 * Starting secure rejoin backoff if the device is not factory new:
 * @code
 *  if (!zb_bdb_is_factory_new())
 *  {
 *    zb_zdo_rejoin_backoff_start(ZB_FALSE);
 *  }
 * @endcode
 */
zb_bool_t zb_bdb_is_factory_new(void);

/** @} */

#if defined(ZB_BDB_ENABLE_FINDING_BINDING) || defined(DOXYGEN)

/**
   @addtogroup zboss_bdb_comm_fb
   @{
 */
/**
 * @brief Starts EZ-Mode finding and binding procedure on the target's endpoint.
 *
 * @details This function puts the device into the identifying mode.
 *          Default duration is 3 minutes.
 *
 * @param[in] endpoint - target endpoint
 *
 * @retval RET_OK - on success
 * @retval RET_INVALID_PARAMETER_1 - target endpoint is not registered
 * @retval RET_INVALID_STATE - finding and binding has already started or the device is not joined
 *
 * @note @p endpoint should be registered on the target.
 *
 * @par Example
 * Starts finding and binding target procedure upon @ref ZB_BDB_SIGNAL_STEERING
 * @snippet onoff_server/on_off_output_zc.c zb_bdb_finding_binding_target_usage
 *
 * @see <b>Finding & binding procedure for a target endpoint</b> 8.4 (BDB 3.0.1)
 * @see @ref zb_bdb_finding_binding_target_ext()
 */
zb_ret_t zb_bdb_finding_binding_target(zb_uint8_t endpoint);

/**
 * @brief Starts EZ-Mode finding and binding procedure on the target's endpoint with a given timeout.
 *
 * @param[in] endpoint - target endpoint
 * @param[in] commissioning_time_secs - time interval for the device to be in the identifying mode, in seconds. Can't be less than 3 minutes.
 *
 * @retval RET_OK - on success
 * @retval RET_INVALID_PARAMETER_1 - target endpoint is not registered
 * @retval RET_INVALID_PARAMETER_2 - @p commissioning_time_secs is less than @b ZB_BDBC_MIN_COMMISSIONING_TIME_S
 * @retval RET_INVALID_STATE - finding and binding procedure has already started or the device is not joined
 *
 * @see <b>Finding & binding procedure for a target endpoint</b> 8.4 (BDB 3.0.1)
 */
zb_ret_t zb_bdb_finding_binding_target_ext(zb_uint8_t endpoint, zb_uint16_t commissioning_time_secs);


/**
 * List of EZ-Mode binding callback states
 */
typedef enum zb_bdb_comm_binding_cb_state_e
{
  /** Previously user applied bind finished successfully */
  ZB_BDB_COMM_BIND_SUCCESS = 0,
  /** Previously user applied bind failed */
  ZB_BDB_COMM_BIND_FAIL = 1,
  /** Ask user whether to perform binding */
  ZB_BDB_COMM_BIND_ASK_USER = 2,
} zb_bdb_comm_binding_cb_state_t;

/**
 * @brief BDB finding and binding callback template.
 *
 * @details Callback is used both to interact with user application @n
 *           and get decision if new binding is needed or not, and to report the binding result
 *
 * @param[in] status - status of the binding (ask user, success or fail) @ref zb_bdb_comm_binding_cb_state_t
 * @param[in] addr - extended address of a device to bind
 * @param[in] ep - endpoint of a device to bind
 * @param[in] cluster - cluster ID to bind
 *
 * @retval ZB_TRUE - create a binding entry for the cluster
 * @retval ZB_FALSE - ignore the cluster and do not create a binding entry
 *
 * @par Example
 * Callback that prints its parameters and always allows binding
 * @snippet onoff_server/on_off_switch_zed.c zb_bdb_finding_binding_initiator_cb_example
 */
typedef zb_bool_t (ZB_CODE * zb_bdb_comm_binding_callback_t)(
  zb_int16_t status, zb_ieee_addr_t addr, zb_uint8_t ep, zb_uint16_t cluster);

/**
 * @brief Starts BDB finding and binding procedure on the initiator.
 *
 * @details This function calls the provided user callback to report the procedure status and to allow
 *          the application to skip binding of some clusters. @n
 *          It may be called several times with Success status and only once with Error status. @n
 *          If any error appears, finding and binding stops.
 *
 * @param[in] endpoint - initiator endpoint
 * @param[in] user_binding_cb - user callback, see @ref zb_bdb_comm_binding_callback_t()
 *
 * @retval RET_OK - on success
 * @retval RET_INVALID_PARAMETER_1 - @p endpoint is not registered
 * @retval RET_INVALID_STATE - device is not joined to the network
 * @retval RET_BUSY - commissioning is in progress
 *
 * @par Example
 * Define callback:
 * @snippet onoff_server/on_off_switch_zed.c zb_bdb_finding_binding_initiator_cb_example
 *
 * Start finding and binding as initiator:
 * @snippet onoff_server/on_off_switch_zed.c zb_bdb_finding_binding_initiator
 *
 * @see <b>Finding & binding procedure for an initiator endpoint</b> 8.5 (BDB 3.0.1)
 */
zb_ret_t zb_bdb_finding_binding_initiator(zb_uint8_t endpoint, zb_bdb_comm_binding_callback_t user_binding_cb);

/**
 * @brief Cancels previously started finding and binding procedure on all target endpoints.
 *
 * @see zb_bdb_finding_binding_target_cancel_ep()
 */
void zb_bdb_finding_binding_target_cancel(void);

/**
 * @brief  Cancels previously started finding and binding procedure on the particular target endpoint.
 *
 * @param[in] endpoint - target endpoint. The @ref ZB_ZCL_BROADCAST_ENDPOINT value is treated as cancel on all target endpoints.
 */
void zb_bdb_finding_binding_target_cancel_ep(zb_cb_param_t endpoint);

/**
 * @brief Cancel previously started finding and binding procedure on initiator
 *
 * @see zb_bdb_finding_binding_initiator
 */
void zb_bdb_finding_binding_initiator_cancel(void);


/** @} */

#endif /* ZB_BDB_ENABLE_FINDING_BINDING || DOXYGEN */

/**
   @addtogroup zboss_bdb_comm_params
   @{
*/
/**
 * @brief Sets primary channel set for the BDB energy scan.
 * @details Network scan will be performed on these channels.
 *
 * @param[in] channel_list - channel list
 *
 * @note This function is used in:
 *       @li Network Steering for a node not on the network;
 *       @li Network Formation.
 * @note Channel set is reset to zero after changing the network role of the device.
 *
 * @see @e @b bdbPrimaryChannelSet 5.3.8 (BDB 3.0.1)
 * @see @ref zb_get_bdb_primary_channel_list
 *
 * @cond DOCS_DEV_NOTES
 * link to zb_channel_page.h won't be generated.
 * Should links to files work?
 * @endcond
 *
*/
void zb_set_bdb_primary_channel_list(zb_channel_list_t channel_list);

/**
 * @brief Retrieves primary channel set for the BDB energy scan.
 *
 * @param[out] channel_list - buffer that receives the primary channel mask
 *
 * @cond DOCS_DEV_NOTES
 * Note is repeated.
 * In order to have single source, @copybrief @copydetails could be used
 * @copydetails copies not only @details (@param's as well)
 * @endcond
 *
 * @note Channel set is reset to zero after changing the network role of the device.
 *
 * @see @ref zb_set_bdb_primary_channel_list
*/
void zb_get_bdb_primary_channel_list(zb_channel_list_t channel_list);

/**
 * @brief Set the secondary channel set for the BDB energy scan.
 * @details Network scan will be performed on these channels if no network found after energy
 * scan on the primary channels (@ref zb_set_bdb_primary_channel_list).
 *
 * @param[in] channel_list - channel list
 *
 * @note This function is used in:
 *       @li Network Steering for a node not on the network;
 *       @li Network Formation.
 *
 * @see @e @b bdbSecondaryChannelSet 5.3.10 (BDB 3.0.1)
 * @see @ref zb_get_bdb_secondary_channel_list
*/
void zb_set_bdb_secondary_channel_list(zb_channel_list_t channel_list);

/**
   Retrieves secondary channel set for the BDB energy scan.
   @param channel_list - channel list.
*/
void zb_get_bdb_secondary_channel_list(zb_channel_list_t channel_list);

/**
 * @brief Enables Zigbee PRO complaint commissioning support.
 * @details This function turns off link key exchange thus supporting legacy devices (<ZB3.0).
 *
 * @param[in] state - controls requirement of trust center key exchange
 * @parblock
 * @arg @b 1 - to disable trust center requirement for the key exchange
 * @arg @b 0 - to enable trust center requirement for the key exchange
 * @endparblock
*/
void zb_bdb_set_legacy_device_support(zb_uint8_t state);

/** @} */

/**
   @addtogroup zboss_bdb_comm_start
   @{
*/
/**
  * @brief Sets BDB commissioning mode.
  * @details This function controls the commissioning procedures to be executed.
  *
  * @param[in] commissioning_mode - @b bdbCommissioningMode bitmask of @ref zb_bdb_commissioning_mode_t.
  *
  * @see @b bdbCommissioningMode 5.3.2 (BDB 1.0)
  * @see <b>Top level commissioning procedure</b> 5.3.2 (BDB 1.0)
 */
void zb_set_bdb_commissioning_mode(zb_uint8_t commissioning_mode);

/** @} */

/**
   @addtogroup zboss_bdb_comm_params
   @{
*/
/**
 * Maximum endpoints of the "respondent" that can be served
 */
#define ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN 4

/**
 * Identify query responses queue size
 */
#define BDB_MAX_IDENTIFY_QUERY_RESP_NUMBER 4

/**
 * List of BDB commissioning states
 */
typedef enum zb_bdb_comm_state_e
{
  ZB_BDB_COMM_IDLE                       = 0,   /*!< EZ-Mode isn't invoked */
  ZB_BDB_COMM_FINDING_AND_BINDING        = 4,   /*!< EZ-Mode finding and binding in progress (on initiator) */
  ZB_BDB_COMM_FINDING_AND_BINDING_TARGET = 5,   /*!< EZ-Mode finding and binding in progress (on target) */
}
zb_bdb_comm_state_t;

/** @} */

/**
   @addtogroup zboss_bdb_comm_tc_conn
   According to BDB v3.1, each device shall utilize mechanism to ensure that it is connected to the TC.

   ZBOSS automatically selects method to ensure that TC is accessible.

   The priority of the supported methods is the following:
    1. Using Keep-Alive cluster client.
       Device checks that it supports Keep-Alive cluster client,
       then it checks that TC supports Keep-Alive cluster server.
    2. Using Poll-Control cluster server.
       According to ZCL spec, this method may be used only for ZEDs.
       It is recommended for sleepy end-devices to use this method
        because this method combines Poll Control cluster logic with TC accessibility checks.
        Note that Poll Control cluster client is mandatory for ZC devices in centralized networks.
    3. By sending ZDO Node descriptor requests periodically to TC.

   The priorities described above are equal to description in the BDB specification.

   After BDB commissioning, ZBOSS automatically selects appropriate method depending on device supported clusters
    and periodically sends messages to TC.

   If there are three failed successive polls, device will automatically rejoin current network.
   @{
*/

/**
 * @brief Start checking connectivity with TC if not started already.
 *        Such checks start automatically right after commissioning ends
 *         if bdb_tc_connectivity_disable_checking hasn't been called before.
 *        If device can't start checking presence of TC in the centralized network
 *         (e.g. device isn't joined any network), checks will be started automatically after BDB commissioning ends.
 */
void zb_bdb_tc_connectivity_start_checking(void);

/**
 * @brief Disable checking connectivity with TC.
 *        Stops TC polling if it is in progress.
 *        May be resumed by calling zb_bdb_tc_connectivity_start_checking.
 *        Also stops WWAH periodic checkins for devices that have enabled WWAH server behavior.
 */
void bdb_tc_connectivity_disable_checking(void);

/**
 * @brief Sets initial backoff time (in seconds) to poll TC using Keep-Alive cluster or node descriptor request method.
 *        For Poll-control cluster cluster, polling period is computed in a cluster-specific way.
 *        TC Poll timeout is equal to (backoff time + random(jitter)) seconds.
 *
 *        Also, backoff time may be changed in runtime and such change will be applied after nearest poll of TC.
 *
 *        Backoff time will be modified by TC if it supports Keep-Alive cluster.
 *        Note that initial backoff time IS NOT saved in the NVRAM.
 *
 *        Described as bdbcfEnConnInitialBackoffTime configuration attribute in BDB3.1.
 *
 * @param backoff_time Initial backoff time in seconds.
 */
void zb_bdb_tc_connectivity_set_initial_backoff_time(zb_uint16_t backoff_time);


/**
 * @brief Get bdbcfEnConnInitialBackoffTime configuration attribute value.
 *        This attribute described in 5.4.1 of BDB3.1 specification.
 *
 * @return zb_uint16_t bdbcfEnConnInitialBackoffTime attribute value.
 */
zb_uint16_t zb_bdb_tc_connectivity_get_initial_backoff_time(void);


/**
 * @brief Sets maximal backoff time (in seconds) to poll TC using node descriptor request method.
 *
 *        Maximal backoff time may be changed in runtime and such change will be applied after nearest poll of TC.
 *
 *        Note that backoff time IS NOT saved in the NVRAM.
 *
 *        Described as bdbcfEnConnMaxBackoffTime configuration attribute in BDB3.1.
 *
 * @param backoff_time Max backoff time in seconds.
 */
void zb_bdb_tc_connectivity_set_max_backoff_time(zb_uint16_t backoff_time);


/**
 * @brief Get bdbcfEnConnMaxBackoffTime configuration attribute value.
 *        This attribute described in 5.4.2 of BDB3.1 specification.
 *
 * @return zb_uint16_t bdbcfEnConnMaxBackoffTime attribute value.
 */
zb_uint16_t zb_bdb_tc_connectivity_get_max_backoff_time(void);

/** @} */

#endif /* ZB_BDB_MODE*/

/**
 * @addtogroup zdo_distributed_security
 * @{
 */
#if defined ZB_DISTRIBUTED_SECURITY_ON || defined DOXYGEN

/**
 *  @brief Enable distributed security network formation at runtime
 *
 * After call the function device won't try
 * to join, but will form a distributed security network instead.
 */
void zb_bdb_enable_distributed_network_formation(void);

/**
 *  @brief Disable distributed security network formation at runtime
 *
 * After call the function the device will not be able to form a distributed security
 * network, but can join another distributed network.
 */
void zb_bdb_disable_distributed_network_formation(void);

#endif /* ZB_DISTRIBUTED_SECURITY_ON */

/** @} */ /* zdo_distributed_security */


/*! @addtogroup af_api */
/*! @{ */

/**
 * @addtogroup af_management_service AF management service
 * @{
 */

/**
 *  @brief Perform "Reset with a Local Action" procedure (as described in BDB spec, chapter 9.5).
 *  The device will perform the NLME leave and clean all Zigbee persistent data except the outgoing NWK
 *  frame counter and application datasets (if any).
 *  The reset can be performed at any time once the device is started (see @ref zboss_start).
 *  After the reset, the application will receive the @ref ZB_ZDO_SIGNAL_LEAVE signal.
 *
 *  @param param - buffer reference (if 0, buffer will be allocated automatically)
 */
void zb_bdb_reset_via_local_action(zb_cb_param_t param);

#if defined ZB_BDB_MODE && defined ZB_JOIN_CLIENT
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
void zb_bdb_initiate_tc_rejoin(zb_cb_param_t param);

/**
 * @brief Set TCLK update period.
*  The device will send node desc request and tries to establish a TCLK in that period,
*  if a TCLK isn't established after commissioning procedure by any reason.
 * @param value - number of seconds. The value equals zero disables TCLK update procedure. Default value is 0x15180 (24h)
*/
void zb_bdb_set_tclku_period(zb_time_t value);

/**
 * @brief Get TCLK update period.
 * @return value in seconds.
*/
zb_time_t zb_bdb_get_tclku_period(void);

#endif /* ZB_BDB_MODE && ZB_JOIN_CLIENT */

/** @} */ /* af_management_service */
/*! @} */ /* af_api */


#if defined ZB_JOIN_CLIENT && defined ZB_BDB_MODE

/**
 * @brief Enables (or disables) skipping of rejoin procedure after router restart.
 *        If this feature is enabled and device had been on network before restart,
 *          it doesn't perform scan and rejoin procedures.
 *        It considers that it is still on network and it continues normal operation on the same network if it still exists.
 *        In order to ensure network existence device performs the following action:
 *          - ZR sends APS-encrypted message to the TC.
 *            If there is no response, it performs TC rejoin.
 *            Such action performed by ZR in BDB mode if TC connectivity is enabled.
 *          - ZED sends End Device Timeout request to its parent.
 *            In case if there is no response,
 *            ZED considers that parent has been lost and it performs rejoin operation.
 *        If the described behavior is disabled, device tries to perform secure rejoin after restart.
 *
 * @param val Enable the described behavior or not.
 *
 * @note By default, such behavior enabled for all ZRs and disabled for all ZEDs.
 * (ZR doesn't perform scan and rejoin operation, ZED performs scan rejoin and rejoin after restart)
 * @note In some cases, the described behavior may be referred using "silent rejoin" term.
 */
void zb_bdb_skip_rejoin_procedure_after_restart(zb_bool_t val);

/**
 * @brief Getter for the value that is set by zb_bdb_skip_rejoin_procedure_after_restart func.
 *
 * @return zb_bool_t Will device skip rejoin procedure after restart or not.
 */
zb_bool_t zb_bdb_get_rejoin_skipping_after_restart_enabled(void);

/**
 * Enable/disable a randomized period between 0 and 5 seconds before attempting join procedure
 */
void zb_bdb_enable_jitter_before_join(zb_bool_t val);

#endif /* ZB_JOIN_CLIENT && ZB_BDB_MODE */

#if defined ZB_BDB_MODE
/**
   Set primary and secondary channel lists to default values
*/
void zb_bdb_set_default_channel_settings(void);

/**
   Set bdb channel settings
   @param page_num - channel page number
*/
void zb_bdb_set_default_channel_settings_for_page(zb_uint8_t page_num);
#endif /* ZB_BDB_MODE */

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

#endif /* ZBOSS_API_BDB_H */