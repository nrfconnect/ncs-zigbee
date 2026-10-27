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
/* PURPOSE: Internal BDB header file.
*/

#ifndef ZB_BDB_COMMISSIONING_H
#define ZB_BDB_COMMISSIONING_H 1

#ifdef ZB_BDB_MODE
/**
 *  \addtogroup bdb_mode_commissioning
 *  @{
 *    @details
 *
 */

/*! Maximum time  a node with a valid network key will wait for a response during a TCLK exchange */
#define ZB_BDBCF_SECURITY_TIMEOUT_PERIOD ZB_APS_SECURITY_TIME_OUT_PERIOD_IN_BEACON_INTERVAL()

typedef enum zb_bdb_commissioning_step_e
{
  ZB_BDB_STEP_INITIALIZATION = 0,
  ZB_BDB_STEP_TOUCHLINK_COMMISSIONING = 1,
  ZB_BDB_STEP_TOUCHLINK_TARGET = 2,
  ZB_BDB_STEP_NETWORK_STEERING = 3,
  ZB_BDB_STEP_NETWORK_FORMATION = 4,
  ZB_BDB_STEP_FINDING_N_BINDING = 5,
  ZB_BDB_STEP_WWAH_REJOIN = 6,
  ZB_BDB_STEP_COMMISSIONING_STOP = 7,
} zb_bdb_commissioning_step_t;

typedef enum bdb_join_machine_e
{
  ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_START = 0,
  ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN = 1,
  ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_DONE = 3,
  ZB_BDB_JOIN_MACHINE_DEVINFO_GATHER = 4,
  ZB_BDB_JOIN_MACHINE_ADDING_TO_NETWORK = 5,
} bdb_join_machine_t;


typedef enum bdb_rejoin_step_e
{
  ZB_BDB_REJOIN_STEP_CURRENT = 0,
  ZB_BDB_REJOIN_STEP_PRIMARY = 1,
  ZB_BDB_REJOIN_STEP_SECONDARY = 2,
  ZB_BDB_REJOIN_STEP_FINISH = 3,
} bdb_rejoin_step_t;


/** @cond internals_doc */
typedef ZB_PACKED_PRE struct zb_bdb_comm_respondent_info_s
{
  zb_address_ieee_ref_t addr_ref; /*!< Address reference in Translation Table for "respondent" */
  zb_uint8_t ep_list[ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN];  /*!< Respondent's active endpoints list */
#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  zb_uint8_t ep_cnt;   /*!< Count of endpoints received during finding and binding */
#endif

  zb_uint8_t eps_checked; /*!< Count of the currently checked endpoints */
  zb_bufid_t simple_desc_resp_buf;
  zb_bufid_t curr_bind_req_buf;
  zb_uindex_t curr_cluster_idx;
} ZB_PACKED_STRUCT zb_bdb_comm_respondent_info_t;

typedef enum bdb_commissioning_signal_e
{
  BDB_COMM_SIGNAL_BAD,          /* 0 */

  BDB_COMM_SIGNAL_INIT_START,   /* 1 */
  BDB_COMM_SIGNAL_INIT_FINISH,

  BDB_COMM_SIGNAL_TOUCHLINK_START, /* 3 */
  BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_DONE,
  BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_FAILED,
  BDB_COMM_SIGNAL_TOUCHLINK_NOTIFY_TASK_RESULT,
  BDB_COMM_SIGNAL_TOUCHLINK_FINISH,

  BDB_COMM_SIGNAL_NETWORK_STEERING_START, /* 8 */
  BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED,
  BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_EX_FAILURE,
  BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_DONE,
  /* TODO: BDB_COMM_SIGNAL_NETWORK_STEERING_LEAVE, */
  BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH,

  BDB_COMM_SIGNAL_NETWORK_FORMATION_START, /* 13 */
  BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH,

  BDB_COMM_SIGNAL_FINDING_N_BINDING_START, /* 15 */
  BDB_COMM_SIGNAL_FINDING_N_BINDING_FINISH,

  BDB_COMM_SIGNAL_REJOIN_START, /* 17 */
  BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL,
  BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL,
  BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_ALL_CHANNELS,
  BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS,
  BDB_COMM_SIGNAL_REJOIN_FINISH,

  BDB_COMM_SIGNAL_FINISH, /* 23 */

  BDB_COMM_SIGNAL_NWK_FORMATION_OK, /* 24 */
  BDB_COMM_SIGNAL_NWK_START_ROUTER_CONF, /* This signal is ZR-only.
                                             It raised after router start confirm has just received.
                                             During join/rejoin, it'll be called after device_annce sent. */
  BDB_COMM_SIGNAL_LEAVE_DONE,
  BDB_COMM_SIGNAL_NWK_JOIN_FAILED,
  BDB_COMM_SIGNAL_NWK_JOIN_DONE,         /* This signal sent in two cases:
                                              - For ZED and ZR roles - after key exchange scheduled (cbke or TCLK)
                                              - For ZED only - after device annce has been sent by ZED device
                                                               and TC key not needed. */
  BDB_COMM_SIGNAL_NWK_AUTH_FAILED,

  BDB_COMM_N_SIGNALS
} bdb_commissioning_signal_t;

enum bdb_commissioning_rejoin_reason_e
{
  BDB_COMM_REJOIN_REASON_UNSPECIFIED = 0,
  BDB_COMM_REJOIN_REASON_POLL_CONTROL_CHECK_IN,
};

enum bdb_tc_connectivity_states_e
{
  BDB_TC_CONNECTIVITY_STATE_NOT_STARTED = 0U,
  BDB_TC_CONNECTIVITY_STATE_DISCOVERY = 1U,
  BDB_TC_CONNECTIVITY_STATE_POLLING = 2U,
  BDB_TC_CONNECTIVITY_STATE_DISALLOWED = 0xFFU /*!< Special state that disallows using of these checks.
                                                    Possible only if user called bdb_tc_connectivity_disable_checking. */
};

enum bdb_tc_connectivity_methods_e
{
  /* TC connectivity check is disabled or not supported by current device. */
  BDB_TC_CONNECTIVITY_METHOD_NOT_SUPPORTED = 0U,
  /* TC connectivity check uses Keep-alive cluster to ensure that TC is on network. */
  BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE = 1U,
  /* TC connectivity check uses Poll-control cluster to ensure that TC is on network. */
  BDB_TC_CONNECTIVITY_METHOD_POLL_CONTROL = 2U,
  /* TC connectivity check uses Node descriptor request to ensure that TC is on network. */
  BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ = 3U,
  /* TC connectivity check uses WWAH specific method to ensure that TC is on network.
   This method is implemented in WWAH cluster. */
  BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC = 4U
};

/*!
* @ brief BDB Commissioning rejoin context
*/
typedef ZB_PACKED_PRE struct bdb_commissioning_rejoin_ctx_t
{
  zb_bitfield_t rejoin_by_checkin_failure:1;
  zb_bitfield_t rr_have_unique_tclk:1;
  zb_bitfield_t waiting:1;
  zb_bitfield_t reserved:5;

  zb_uint8_t rr_sv_device_type; /* zb_nwk_device_type_t */
  zb_uint16_t rr_sv_parent_short;
  zb_ieee_addr_t rr_sv_parent_long;
  zb_neighbor_tbl_ent_t rr_sv_parent_nent;
  zb_uint8_t rr_sv_authenticated;
  zb_uint8_t rr_retries;
  zb_uint8_t rr_ignore_start_router_conf;
  zb_uint16_t rr_global_retries;
  zb_uint8_t rr_skip_savepoint;

  bdb_commissioning_signal_t last_rejoin_signal;
  bdb_commissioning_signal_t next_rejoin_signal;
}
ZB_PACKED_STRUCT bdb_commissioning_rejoin_ctx_t;


typedef ZB_PACKED_PRE struct bdb_tc_connectivity_ctx_s
{
  zb_uint8_t  state;              /*!< Current state. @see bdb_tc_connectivity_states_e */
  zb_uint8_t  method;             /*!< Current method. @see bdb_tc_connectivity_methods_e */
  zb_uint8_t  failure_ctr;        /*!< Amount of failed polling attempts in succession.
                                       Also counts failed discovery attempts. */
  zb_uint8_t  tsn;                /*!< ZDO TSN of sent pkt.
                                      Shared between keepalive and node descr_req methods. */
  zb_uint8_t  endpoint;           /*!< Endpoint that should be used for Keep-Alive cluster. */
  zb_uint16_t keep_alive_base;   /*!< In seconds. Shared between keepalive and node descr_req.
                                      Referred as backoff time for node_descr method in bdb3.1 spec (see 7.3.3.1). */
  zb_uint16_t keep_alive_jitter;  /*!< In seconds. Shared between keepalive and node descr_req. */
  zb_uint16_t initial_backoff_time; /*!< In seconds. bdbcfEnConnInitialBackoffTime. */
  zb_uint16_t max_backoff_time;     /*!< In seconds. bdbcfEnConnMaxBackoffTime. */
  zb_bool_t   tc_rejoin_initiated; /*!< Indicates that rejoin initiated by tc connectivity  */
} ZB_PACKED_STRUCT bdb_tc_connectivity_ctx_t;


/**
 * BDB commissioning context
 */
typedef struct zb_bdb_comm_ctx_s
{
  /** State of commissioning */
  zb_bdb_comm_state_t state;
  zb_ret_t status;
  /** Callback function invoked when factory fresh or network steering operations finish */
  zb_callback_t user_cb;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Callback function invoked when binding operation start */
  zb_bdb_comm_binding_callback_t finding_binding_progress_cb;
/* [AV] After having separated binding user callbacks from f&b complete callback
    the last one can be stored in the user_cb field. */
#endif

  /** Duration of PermitJoining and IdentifyTime */
  zb_uint16_t duration;
/*Data array to store info from Identity_Query_Resp */
  zb_bdb_comm_respondent_info_t respondent[BDB_MAX_IDENTIFY_QUERY_RESP_NUMBER];
  zb_uint8_t respondent_number;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Endpoint which participate in finding and binding*/
  zb_uint8_t ep;
#endif

  /** Endpoint descriptor*/
  zb_af_endpoint_desc_t *ep_desc;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Count of endpoints received during finding and binding */
  zb_uint8_t ep_cnt;

  /** Signals that at least one endpoint was bound during finding and binding;
    * it is used to invoke user callback if no endpoint was bound
    */
  zb_bool_t was_bound;
#endif
  /*EZ mode in progress flag. Sets for EZScanTimeout*/
  zb_bool_t ez_scan_inprogress;
  /** Reference to the buffer holding simple descriptor response */
  zb_bufid_t simple_desc_buf_ref;

  /* ------------- */
  bdb_commissioning_signal_t signal;
  bdb_commissioning_rejoin_ctx_t rejoin;
#ifdef ZB_JOIN_CLIENT
  zb_time_t tclku_timeout;
  bdb_tc_connectivity_ctx_t tc_connectivity_ctx;
#endif

  /* Moved here from BDB_CTX */
#define FIRST_GENERAL_BDB_FIELD bdb_commissioning_group_id
  /* BDB attributes */
  zb_uint16_t bdb_commissioning_group_id; /*!< specifies the identifier of the group on which the initiator applies finding & binding.  */
  zb_uint8_t bdb_commissioning_mode;      /*!< @see zb_bdb_commissioning_mode_t */
  zb_uint8_t  bdb_commissioning_status; /*!< see zb_bdb_error_codes_e  */

  zb_channel_list_t bdb_primary_channel_list;
  zb_channel_list_t bdb_secondary_channel_list;
  zb_channel_list_t v_scan_channels_list;
  zb_uint32_t   bdb_commissioning_time;
  zb_uint8_t    bdb_scan_duration;
  zb_uint8_t    bdb_commissioning_step;
  zb_bitfield_t v_do_primary_scan:3; /*!< a bit more than vDoPromaryScan in
                                      * BDB: really scan & join machine
                                      * state. @see enum bdb_join_machine_e */

  zb_bitfield_t bdb_ext_channel_scan:1; /*!< Touchlink performs ext scan if 1 */
  zb_bitfield_t ignore_aps_channel_mask:1; /*!< Non standard, but useful: if 1,
                                            * use hard-coded channels set. if 0,
                                            * mask channels sets by
                                            * aps_channel_mask. To be used to
                                            * debug at single channel, or 2
                                            * channels etc */
  zb_bitfield_t bdb_first_start:1;
  zb_bitfield_t bdb_start_after_reboot:1;

  zb_uint8_t    bdb_application_signal;  /* Application signal code to be passed into
                                          * zb_zdo_startup_complete */
#ifdef ZB_BDB_TOUCHLINK
  zb_uint8_t    tl_first_channel_rpt;
  zb_uint8_t    tl_channel_i;
#endif  /* ZB_BDB_TOUCHLINK */
  zb_bitfield_t bdb_force_rejoin:1;             /* Force rejoin for the router/ZED */
  zb_bitfield_t bdb_tc_rejoin_after_reboot:1;   /* Is TC rejoin started when reboot signal is scheduled */
  zb_bitfield_t bdb_tc_rejoin_active:1;         /* Is TC rejoin active */
  zb_bitfield_t bdb_op_cancelled:1;       /* if the BDB operation (steering or formation) is cancelled */
  zb_bitfield_t bdb_leave_initiated:1;   /* if leave was initiated, prevent comm signals */
  zb_bitfield_t bdb_next_rejoin_step:2;        /* Rejoin step @see bdb_rejoin_step_e*/
  zb_bitfield_t disable_silent_rejoin:1;     /*!<   Disable silent rejoin.
                                                     It has ZB_FALSE value by default for ZR and TRUE for ZED. (Similar to R22 BDB 3.0)
                                                     Can be overridden by bdb_force_rejoin flag.
                                                     Not that there is no "silent rejoin" term in R23 core nor BDB3.1 specs.
                                                     There is such term only in R23 test spec, but it isn't described.
                                                     This term can be described as "device resumes normal operation after restart".
                                                     It means that device doesn't perform (or just skips) rejoin procedure
                                                     if network is still present. */
  zb_bitfield_t off_nwk_steering_jitter_enabled:1;
} zb_bdb_comm_ctx_t;
/** @endcond */ /* internals_doc */

extern zb_bdb_comm_ctx_t g_bdb_ctx;

#define BDB_COMM_CTX() g_bdb_ctx

void bdb_commissioning_machine(zb_cb_param_t param);
void bdb_commissioning_signal(bdb_commissioning_signal_t sig, zb_bufid_t param);

#if defined ZB_BDB_ENABLE_FINDING_BINDING
void zb_bdb_finding_binding_init_ctx(void);

void zb_bdb_process_identify_query_res(zb_cb_param_t param);
#endif /* ZB_BDB_ENABLE_FINDING_BINDING */

/** @cond touchlink */

#if defined ZB_BDB_TOUCHLINK && !defined ZB_COORDINATOR_ONLY && defined ZB_DISTRIBUTED_SECURITY_ON
void bdb_touchlink_target_start(zb_cb_param_t param);
void bdb_touchlink_initiator(zb_cb_param_t param);
#endif /* ZB_BDB_TOUCHLINK && !ZB_COORDINATOR_ONLY && ZB_DISTRIBUTED_SECURITY_ON */

/** @endcond */ /* touchlink */


#if defined(ZB_ROUTER_ROLE) || defined(ZB_COORDINATOR_ROLE)
void bdb_remove_joiner(zb_cb_param_t param);
#endif /* ZB_ROUTER_ROLE || ZB_COORDINATOR_ONLY */

#ifdef ZB_BDB_TOUCHLINK
void bdb_check_fn(void);
#endif /* ZB_BDB_TOUCHLINK */

void bdb_start_rejoin_recovery(zb_cb_param_t cb_param);

zb_bool_t bdb_joined(void);

void bdb_force_link(void);

zb_uint8_t bdb_get_scan_duration(void);

#ifdef ZB_JOIN_CLIENT
void zb_bdb_tc_connectivity_autostart_checking_after_bdb(void);
zb_bool_t zb_bdb_tc_connectivity_block_zcl_cmd(zb_zcl_parsed_hdr_t *cmd_info);
zb_bool_t zb_bdb_tc_connectivity_handle_read_attr_resp(zb_bufid_t param);
void bdb_tc_connectivity_restart_timer(void);
void zb_bdb_tc_connectivity_stop_checking(void);
void bdb_tc_connectivity_poll_control_checkin_handler(zb_zcl_status_t checkin_status);

void bdb_tc_connectivity_start_discovery(zb_cb_param_t cb_param);
zb_ret_t bdb_tc_connectivity_discover_keepalive_server(zb_bufid_t param);
void bdb_tc_connectivity_handle_keep_alive_match_desc_rsp(zb_cb_param_t param);
zb_uint8_t bdb_tc_connectivity_get_current_method(void);
void bdb_tc_connectivity_set_jitter(zb_uint16_t jitter);
zb_uint16_t zb_bdb_tc_connectivity_get_jitter(void);
zb_bool_t bdb_tc_connectivity_checks_enabled(void);
#endif /* ZB_JOIN_CLIENT */

#ifndef ZB_COORDINATOR_ONLY
void bdb_partner_lk_verification_timeout(zb_cb_param_t ref);
void zb_bdb_partner_link_key_received(zb_bufid_t ref, zb_bool_t is_initiator);
void zb_bdb_partner_auth_level_rsp_handler(zb_cb_param_t param);
#endif /* !ZB_COORDINATOR_ONLY */

#ifdef ZB_BDB_PREINST_NWK_JOINING
zb_ret_t zb_bdb_preinst_nwk_on_factory_new(zb_bufid_t param);
void zb_bdb_preinst_nwk_on_join_confirm(void);
#endif /* ZB_BDB_PREINST_NWK_JOINING */


/** @}  */ /* bdb_mode_commissioning */

#endif /* ZB_BDB_MODE */

#endif /* ZB_BDB_COMMISSIONING_H */
