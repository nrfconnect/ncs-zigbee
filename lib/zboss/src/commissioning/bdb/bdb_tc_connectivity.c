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
/*  PURPOSE: TC connectivity check for BDB commissioning.
*/

#define ZB_TRACE_FILE_ID 161

#include "zb_common.h"
#include "zb_bdb_internal.h"
#include "zcl/zb_zcl_commands.h"
#include "zb_aps.h"

/**
 * @brief Returns keep-alive interval to poll TC in BIs.
 *
 * @param base Base interval in seconds.
 * @param jitter Jitter max value in BIs.
 */
#define GET_KEEPALIVE_INTERVAL(base, jitter) (ZB_SECONDS_TO_BEACON_INTERVAL((zb_time_t)(base)) \
                                              + (zb_time_t) ((jitter) == 0 ? (jitter) : ZB_RANDOM_JTR((jitter))))

#if defined ZB_BDB_MODE && defined ZB_JOIN_CLIENT

/* TC Connectivity split into 3 stages:
    - BDB_TC_CONNECTIVITY_STATE_NOT_STARTED
      Idle state: No alarms are scheduled and no buffers are used by this module.
    - Discovery
      At this stage, device tries to find out which method supported on ZC side
        by issuing match descriptor request.
        If ZC inaccessible at discovery stage, device does
          three attempts with shorter period.
          In case if all attempts were failed, device goes to rejoin.

    - Polling
      At this stage, device sends commands to TC once per period.
        This period depends on selected method and has value equal to
          "keep_alive_base + RANDOM(keep_alive_jitter)" seconds.
*/
static zb_uint8_t bdb_tc_connectivity_select_method_and_start_discovery(zb_bufid_t param, zb_uint8_t first_method);

static void bdb_tc_connectivity_match_descr_resp_handler(zb_cb_param_t param);
static void bdb_tc_connectivity_discovery_attempt_failed(zb_cb_param_t param);
static void bdb_tc_connectivity_finish_discovery(zb_cb_param_t param);

static void bdb_tc_connectivity_do_poll(zb_cb_param_t unused);
static void bdb_tc_connectivity_poll_finished(zb_bool_t poll_successful);

static void bdb_tc_connectivity_init_rejoin(zb_cb_param_t param);

static void bdb_tc_connectivity_do_poll_keep_alive(zb_cb_param_t param);
static zb_bool_t bdb_tc_connectivity_handle_keepalive_read_attr(zb_bufid_t param);
static void bdb_tc_connectivity_poll_timeout(zb_cb_param_t unused);

static zb_ret_t bdb_tc_connectivity_discover_poll_ctrl_client(zb_bufid_t param);
static void bdb_tc_connectivity_poll_ctrl_check_binding_rsp(zb_cb_param_t param);
static void bdb_tc_connectivity_poll_ctrl_send_match_desc_req(zb_cb_param_t param);

static zb_time_t bdb_tc_connectivity_get_jitter_bi(void);

static void bdb_tc_connectivity_do_poll_node_desc_req(zb_cb_param_t param);

static void bdb_tc_connectivity_fill_match_desc_req_to_tc(zb_bufid_t param,
                                                    const zb_uint16_t profile_id,
                                                    const zb_uint16_t cluster_id,
                                                    const zb_bool_t is_in_cluster);


/* Wraps zb_bdb_tc_connectivity_start_checking,
   so if user disallows this check, it won't be started after BDB. */
void zb_bdb_tc_connectivity_autostart_checking_after_bdb(void)
{
  TRACE_MSG(TRACE_ZCL4, "zb_bdb_tc_connectivity_autostart_checking_after_bdb", (FMT__0));

  if (bdb_tc_connectivity_checks_enabled())
  {
    zb_bdb_tc_connectivity_start_checking();
  }
}

void zb_bdb_tc_connectivity_start_checking(void)
{
  TRACE_MSG(TRACE_ZCL3, "zb_bdb_tc_connectivity_start_checking state %hd curr_method %hd",
           (FMT__H_H, BDB_COMM_CTX().tc_connectivity_ctx.state, BDB_COMM_CTX().tc_connectivity_ctx.method));

  if (!ZB_IS_DEVICE_ZC()
      /* Do not restart if already started. */
      && !IS_DISTRIBUTED_SECURITY()
      && ZB_JOINED()
      && (BDB_COMM_CTX().tc_connectivity_ctx.state == BDB_TC_CONNECTIVITY_STATE_NOT_STARTED
          /* Only user (or WWAH) should call this func directly. */
          || BDB_COMM_CTX().tc_connectivity_ctx.state == BDB_TC_CONNECTIVITY_STATE_DISALLOWED))
  {
    BDB_COMM_CTX().tc_connectivity_ctx.state = BDB_TC_CONNECTIVITY_STATE_DISCOVERY;
    BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr = 0;

    BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_base = BDB_COMM_CTX().tc_connectivity_ctx.initial_backoff_time;

    zb_buf_get_out_delayed_ext(bdb_tc_connectivity_start_discovery,
                               BDB_TC_CONNECTIVITY_METHOD_NOT_SUPPORTED,
                               0);
  }
  /* User has called this function to start checks.
     It seems, that BDB hasn't passed yet: start in automatic mode.  */
  else if (BDB_COMM_CTX().tc_connectivity_ctx.state == BDB_TC_CONNECTIVITY_STATE_DISALLOWED)
  {
    BDB_COMM_CTX().tc_connectivity_ctx.state = BDB_TC_CONNECTIVITY_STATE_NOT_STARTED;
  }
  else
  {
    /* Hi to misra! */
  }
}


/**
 * @brief Stops periodic checking TC presence in network.
 *
 */
void zb_bdb_tc_connectivity_stop_checking(void)
{
  TRACE_MSG(TRACE_ZCL2, "zb_bdb_tc_connectivity_stop_checking", (FMT__0));

  ZB_SCHEDULE_ALARM_CANCEL(bdb_tc_connectivity_do_poll, ZB_ALARM_ALL_CB);
  ZB_SCHEDULE_ALARM_CANCEL(bdb_tc_connectivity_poll_timeout, ZB_ALARM_ALL_CB);

  BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr = 0;
  BDB_COMM_CTX().tc_connectivity_ctx.state = BDB_TC_CONNECTIVITY_STATE_NOT_STARTED;
  BDB_COMM_CTX().tc_connectivity_ctx.method = BDB_TC_CONNECTIVITY_METHOD_NOT_SUPPORTED;
  BDB_COMM_CTX().tc_connectivity_ctx.tsn = ZB_ZDO_INVALID_TSN;
  BDB_COMM_CTX().tc_connectivity_ctx.endpoint = 0;

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
  if (ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR())
  {
    zb_zcl_wwah_stop_periodic_checkin();
  }
#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */
}


zb_bool_t bdb_tc_connectivity_checks_enabled(void)
{
  zb_bool_t ret = BDB_COMM_CTX().tc_connectivity_ctx.state != BDB_TC_CONNECTIVITY_STATE_DISALLOWED
                    && !ZB_IS_DEVICE_ZC()
                    && !IS_DISTRIBUTED_SECURITY()
                    && ZB_JOINED();

  TRACE_MSG(TRACE_ZCL4, "bdb_tc_connectivity_checks_enabled, ret %hd", (FMT__H, ret));

  return ret;
}


/* Should be called only by user.
   It disables all auto starts of TC connectivity checks (including WWAH)*/
void bdb_tc_connectivity_disable_checking(void)
{
  zb_bdb_tc_connectivity_stop_checking();
  BDB_COMM_CTX().tc_connectivity_ctx.state = BDB_TC_CONNECTIVITY_STATE_DISALLOWED;
}

/**
 * @brief Starts discovery if it or TC polling haven't started before.
 *        Also, calls WWAH-specific discovery if WWAH server behavior enabled.
 *        Not that if `method` is not supported, than next method from
 *          BDB/WWAH-specific order will be chosen.
 *
 * @param param buf_id in that match descriptor req will be stored.
 * @param method First method that shall be used.
 */
void bdb_tc_connectivity_start_discovery(zb_cb_param_t cb_param)
{
  zb_bufid_t param = ZB_UNPACK_BUF_REF(cb_param);
  zb_uint8_t selected_method = (zb_uint8_t)ZB_UNPACK_USER_PARAM(cb_param);

  TRACE_MSG(TRACE_ZCL2, ">> bdb_tc_connectivity_start_discovery", (FMT__0));

  /* Lets assume that delayed buffer allocation can take a lot of time
      and user (or WWAH) decided to stop these checks. */
  if(BDB_COMM_CTX().tc_connectivity_ctx.state == BDB_TC_CONNECTIVITY_STATE_DISCOVERY)
  {
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
    if (ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR())
    {
      selected_method = zb_zcl_wwah_periodic_checkins_select_method_and_start_discovery(param, selected_method);
    }
    else
#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */
    {
      selected_method = bdb_tc_connectivity_select_method_and_start_discovery(param, selected_method);
    }

    ZB_ASSERT(selected_method <= BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC);

    /* Don't allow to access BDB ctx from WWAH, set method here. */
    BDB_COMM_CTX().tc_connectivity_ctx.method = selected_method;

    /* Change state to polling, WWAH will handle all other actions. */
    if (selected_method == BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC)
    {
      zb_buf_get_out_delayed(bdb_tc_connectivity_finish_discovery);
      /* WWAH should reuse the buffer to send match descriptor req
          to check that TC supports its method.
          Don't free the buffer. */
      param = ZB_BUF_INVALID;
    }
    /* It means that TC polling not supported currently.
       Possible for WWAH server.
       Only in case if periodic checkins are disabled. */
    else if (selected_method == BDB_TC_CONNECTIVITY_METHOD_NOT_SUPPORTED)
    {
      zb_bdb_tc_connectivity_stop_checking();
      /* No method selected, buffer hasn't been used, free it later. */
    }
    else
    {
      /* Some method has been selected, don't free the buffer. */
      param = ZB_BUF_INVALID;
    }
  }

  if (param != ZB_BUF_INVALID)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL2, "<< bdb_tc_connectivity_start_discovery method %hd", (FMT__H, selected_method));
}


/**
 * @brief Start BDB logic for method selection.
 *        Selects method starting from `first_method` value in the order,
 *          described in BDB Spec 7.3:
 *        - Keep-alive
 *        - Poll-control
 *        - Node descriptor req.
 *        Also starts discovery. (Sends match descriptor request)
 *
 * @param param         Buf id to ster match descriptor request
 * @param first_method  The highest priority method from that should be used for discovery.
 *                      BDB_TC_CONNECTIVITY_METHOD_NOT_SUPPORTED does the same as BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE.
 * @return zb_uint8_t   Method for that discovery has started.
 */
static zb_uint8_t bdb_tc_connectivity_select_method_and_start_discovery(zb_bufid_t param, zb_uint8_t first_method)
{
  zb_uint8_t selected_method = first_method;
  zb_uint8_t method_idx;
  /* Support three error codes:
      - NOT FOUND (unsupported method on local node.)
      - RET_NO_MEMORY (Not enough memory to start discovery, but supported locally)
      - RET_OK (discovery started) */
  zb_ret_t err_code = RET_NOT_FOUND;

  TRACE_MSG(TRACE_ZCL1, ">> bdb_tc_connectivity_select_method_and_start_discovery param %d, method %hd",
           (FMT__D_H, param, first_method));

  ZB_ASSERT(ZB_IN_BDB() && !ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR());

  for (method_idx = first_method;
       method_idx <= BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ
        && err_code == RET_NOT_FOUND;
       method_idx++)
  {
    /* `method_idx` will be greater than `selected_method` by 1 after loop ends.
       It is needed to remove decrement of `method_idx` after loop ends in order to prevent MISRA warnings about underflow. */
    selected_method = method_idx;

    switch (selected_method)
    {
      case BDB_TC_CONNECTIVITY_METHOD_NOT_SUPPORTED:
        /* Skip. It means that device starts discovery first time after boot. */
        break;

      case BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE:
        /* If cluster unsupported on the local node, function should return RET_NOT_FOUND.
           Another return value, otherwise. */
        err_code = bdb_tc_connectivity_discover_keepalive_server(param);
        break;

      case BDB_TC_CONNECTIVITY_METHOD_POLL_CONTROL:
        /* ZCL 8 spec, 3.16.1: "The end device implements the server side of this cluster" */
        if (ZB_IS_DEVICE_ZED())
        {
          /* If cluster unsupported on the local node, function should return RET_NOT_FOUND.
          Another return value, otherwise. */
          err_code = bdb_tc_connectivity_discover_poll_ctrl_client(param);
        }
        break;

      /* Fallback method for BDB.
        It doesn't need discovery, do poll. */
      case BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ:
        /* Finish after callback, method will be assigned after return from this func. */
        ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_finish_discovery, param);
        /* Fallback method for BDB. Always supported. */
        err_code = RET_OK;
        break;

      default:
        ZB_ASSERT(0);
        break;
    }
  }

  /* Don't have enough memory to send match_desc_req or to put cb, retry discovery 3 times.
     3 retries seem more efficient than instant rejoin. */
  if (err_code == RET_NO_MEMORY)
  {
    ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_discovery_attempt_failed, param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< bdb_tc_connectivity_select_method_and_start_discovery ret %d, method %hd",
           (FMT__D_H, err_code, selected_method));

  return selected_method;
}


/**
 * @brief Handles match descriptor response in BDB mode.
 *
 * @param param Buf that contains match descriptor response.
 */
static void bdb_tc_connectivity_match_descr_resp_handler(zb_cb_param_t param)
{
  zb_zdo_match_desc_resp_t *resp = zb_buf_begin(param);
  zb_uint8_t next_method;

  TRACE_MSG(TRACE_ZCL1, ">> bdb_tc_connectivity_match_descr_resp_handler param %d", (FMT__D, param));

  /* It seems that TC polls were disabled externally, stop polling: free buffer. */
  if (BDB_COMM_CTX().tc_connectivity_ctx.state != BDB_TC_CONNECTIVITY_STATE_DISCOVERY)
  {
    TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity: discovery has stopped, just free buf", (FMT__0));
    zb_buf_free(param);
  }
  else if (resp->status == ZB_ZDP_STATUS_SUCCESS
           && resp->match_len > 0)
  {
    TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity: discovery matched method %hd",
             (FMT__H, BDB_COMM_CTX().tc_connectivity_ctx.method));

    switch (BDB_COMM_CTX().tc_connectivity_ctx.method)
    {
      case BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE:
        ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_handle_keep_alive_match_desc_rsp, param);
        break;

      case BDB_TC_CONNECTIVITY_METHOD_POLL_CONTROL:
        /* No need to do additional actions. */
        ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_finish_discovery, param);
        break;

      /* Should never happen.
         WWAH-specific logic should be handled on WWAH layer.
         Node descriptor req doesn't require match descriptor request. */
      case BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC:
      case BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ:
      /* FALLTHROUGH */
      default:
        ZB_ASSERT(0);
        zb_buf_free(param);
        break;
    }
  }
  /* Killed by ZDO cb killer. Retry sending. */
  else if (resp->status == ZB_ZDP_STATUS_TIMEOUT)
  {
    bdb_tc_connectivity_discovery_attempt_failed(param);
  }
  else
  {
    /* WWAH shares Keep-Alive method with BDB, call WWAH in WWAH server mode.
       Try next BDB method otherwise  */
    next_method = ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR()
                  ? BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC
                  : BDB_COMM_CTX().tc_connectivity_ctx.method + 1;

    TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity: discovery NOT matched, try next method %hd", (FMT__H, next_method));
    ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_start_discovery, ZB_PACK_2_U16_IN_U32(param, next_method));

    /* Node descriptor request will match for sure: it doesn't require discovery. */
    ZB_ASSERT(BDB_COMM_CTX().tc_connectivity_ctx.method != BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ);
  }
}


static void bdb_tc_connectivity_delay_discovery_retry(zb_cb_param_t param)
{
  if (param != ZB_BUF_INVALID)
  {
    ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_start_discovery, ZB_PACK_2_U16_IN_U32(param, BDB_COMM_CTX().tc_connectivity_ctx.method));
  }
  else
  {
    zb_buf_get_out_delayed_ext(bdb_tc_connectivity_start_discovery,
                               BDB_COMM_CTX().tc_connectivity_ctx.method,
                               0);
  }
}


/**
 * @brief Changes state to polling, schedules first poll.
 *        Should be called on successful discovery of EP on TC side.
 */
static void bdb_tc_connectivity_finish_discovery(zb_cb_param_t param)
{
  TRACE_MSG(TRACE_ZCL2, "bdb_tc_connectivity_finish_discovery method %hd",
           (FMT__H, BDB_COMM_CTX().tc_connectivity_ctx.method));

  BDB_COMM_CTX().tc_connectivity_ctx.state = BDB_TC_CONNECTIVITY_STATE_POLLING;
  BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr = 0;

  /* It seems impossible, just recheck that poll wasn't scheduled earlier. */
  ZB_SCHEDULE_ALARM_CANCEL(bdb_tc_connectivity_do_poll, ZB_ALARM_ALL_CB);

  if (BDB_COMM_CTX().tc_connectivity_ctx.method == BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE
      || BDB_COMM_CTX().tc_connectivity_ctx.method == BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ)
  {
    /* Do first poll as soon as possible. */
    ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_do_poll, ZB_BUF_INVALID);
  }

  ZB_ASSERT(COMM_SELECTOR().signal != NULL);
  COMM_SELECTOR().signal(ZB_COMM_SIGNAL_TC_CONNECTIVITY_METHOD_DISCOVERED, param);
}


/* Can't start discovery */
static void bdb_tc_connectivity_discovery_attempt_failed(zb_cb_param_t param)
{
  TRACE_MSG(TRACE_ZCL1, "bdb_tc_discovery_failed param %d", (FMT__D, param));

  if (BDB_COMM_CTX().tc_connectivity_ctx.state == BDB_TC_CONNECTIVITY_STATE_DISCOVERY)
  {
    BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr++;

    if (BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr > BDB_TC_CONNECTIVITY_MAX_FAILURE_CNT)
    {
      ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_init_rejoin, param);
    }
    else
    {
      /* Retry with smaller period than keep-alive requires. */
      ZB_SCHEDULE_ALARM(bdb_tc_connectivity_delay_discovery_retry,
                        param,
                        BDB_TC_CONNECTIVITY_DISCOVERY_RETRY_TIMEOUT);
    }
  }
}


/**
 * @brief Does TC poll (ensures that TC connected) depending on currently selected method.
 *
 * @param unused
 */
static void bdb_tc_connectivity_do_poll(zb_cb_param_t unused)
{
  zb_ret_t ret = RET_OK;

  ZVUNUSED(unused);
  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_do_poll", (FMT__0));

  switch (BDB_COMM_CTX().tc_connectivity_ctx.method)
  {
    case BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE:
      ret = zb_buf_get_out_delayed(bdb_tc_connectivity_do_poll_keep_alive);
      break;

    case BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ:
      ret = zb_buf_get_out_delayed(bdb_tc_connectivity_do_poll_node_desc_req);
      break;

    /* These states are impossible:
        - BDB_TC_CONNECTIVITY_METHOD_NOT_SUPPORTED
          alarm must be closed on stop of TC connectivity check.
        - BDB_TC_CONNECTIVITY_METHOD_POLL_CONTROL
          implemented as independent cluster.
        - BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC
          Polls TC itself. */
    case BDB_TC_CONNECTIVITY_METHOD_NOT_SUPPORTED:
    case BDB_TC_CONNECTIVITY_METHOD_POLL_CONTROL:
    case BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC:
      ZB_ASSERT(ZB_FALSE);
      break;

    default:
      ZB_ASSERT(ZB_FALSE);
      break;
  }

  /* No memory. Consider poll as failed and retry later. */
  if (ret != RET_OK)
  {
    bdb_tc_connectivity_poll_finished(ZB_FALSE);
  }
}


/**
 * @brief Fills match descriptor request with:
 *          - TC destination address
 *          - Cluster ID
 *          - Application profile ID.
 *
 * @param param   buf into that match descriptor req will be written.
 * @param profile_id profile id that will be used in match descriptor req
 * @param cluster_id cluster id that will be written as input cluster into req.
 * @param is_in_cluster cluster type on local device input (ZB_TRUE) or output (ZB_FALSE).
 */
static void bdb_tc_connectivity_fill_match_desc_req_to_tc(zb_bufid_t param,
                                                    const zb_uint16_t profile_id,
                                                    const zb_uint16_t cluster_id,
                                                    const zb_bool_t is_in_cluster)
{
  zb_uint16_t tc_short_addr = zb_aib_get_trust_center_short_address();
  zb_zdo_match_desc_param_t *req;

  req = zb_buf_initial_alloc(param, sizeof(zb_zdo_match_desc_param_t));

  req->nwk_addr = tc_short_addr;
  req->addr_of_interest = tc_short_addr;

  req->num_in_clusters = is_in_cluster ? 1 : 0;
  req->num_out_clusters = is_in_cluster ? 0 : 1;

  req->cluster_list[0] = cluster_id;
  req->profile_id = profile_id;
}


/**
 * @brief Returns max jitter value in beacon intervals
 *        depending on currently selected polling method.
 *
 * @return zb_time_t max jitter value in beacon intervals.
 */
static zb_time_t bdb_tc_connectivity_get_jitter_bi(void)
{
  zb_uint16_t jitter_value_sec = BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_jitter;

  /* Spec requires to set jitter to 10 seconds for Node Descr Req method. */
  if (BDB_COMM_CTX().tc_connectivity_ctx.method == BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ)
  {
    jitter_value_sec = BDB_TC_CONNECTIVITY_NODE_DESCR_JITTER_SEC;
  }

  return ZB_SECONDS_TO_BEACON_INTERVAL(jitter_value_sec);
}


/**
 * @brief Checks that Keep-Alive cluster is present on local node
 *        and sends match descriptor request to TC.
 *
 * @param param Buffer to send match descriptor request.
 * @return zb_ret_t RET_NOT_FOUND if Keep-Alive is not present on local node.
 *                  RET_NO_MEMORY if match descriptor request can't be sent because ZDO CB queue is full.
 *                  RET_OK        match descriptor request has been successfully scheduled.
 */
zb_ret_t bdb_tc_connectivity_discover_keepalive_server(zb_bufid_t param)
{
  zb_ret_t ret = RET_NOT_FOUND;
  zb_af_endpoint_desc_t *ep_desc;
  zb_uint8_t zdo_tsn;

  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_discover_keepalive_server param %d",
           (FMT__D, param));

  ep_desc = get_endpoint_by_cluster_with_role(ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,
                                              ZB_ZCL_CLUSTER_CLIENT_ROLE);

  if (ep_desc != NULL)
  {
    bdb_tc_connectivity_fill_match_desc_req_to_tc(param,
                                                  ep_desc->simple_desc->app_profile_id,
                                                  ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,
                                                  ZB_TRUE);

    zdo_tsn = zb_zdo_match_desc_req(param, bdb_tc_connectivity_match_descr_resp_handler);

    ret = zdo_tsn != ZB_ZDO_INVALID_TSN ? RET_OK : RET_NO_MEMORY;
  }

  TRACE_MSG(TRACE_ZCL1, "<< bdb_tc_connectivity_discover_keepalive_server ret %d",
           (FMT__D, ret));

  return ret;
}


/**
 * @brief Handles keep-alive specific match desc response
 *        assuming that descriptor has matched.
 *        Schedules TC poll by itself.
 *        It means that all checks should be done BEFORE this func call.
 *
 * @param param Buf that contains match descriptor response
 *              having one endpoint with keep alive cluster on it.
 */
void bdb_tc_connectivity_handle_keep_alive_match_desc_rsp(zb_cb_param_t param)
{
  zb_zdo_match_desc_resp_t *resp = zb_buf_begin(param);
  zb_uint8_t *match_ep;

  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_handle_keep_alive_match_desc_rsp param %d status %hd match_len %hd",
           (FMT__D_H_H, param, resp->status, resp->match_len));

  ZB_ASSERT(resp->status == ZB_ZDP_STATUS_SUCCESS
            && resp->match_len > 0);

  /* Match the first endpoint with keep-alive cluster. */
  match_ep = (zb_uint8_t*)(resp + 1);
  BDB_COMM_CTX().tc_connectivity_ctx.endpoint = *match_ep;

  ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_finish_discovery, param);
}


/**
 * @brief Considers that TC poll has failed
 *        if read attributes response wasn't received in BDB_TC_CONNECTIVITY_POLL_TIMEOUT
 *        after request successfully sent.
 *        Used only for keep-alive method.
 *
 * @param unused
 */
static void bdb_tc_connectivity_poll_timeout(zb_cb_param_t unused)
{
  ZVUNUSED(unused);
  TRACE_MSG(TRACE_ZCL1, "Keep-alive poll timed out", (FMT__0));
  bdb_tc_connectivity_poll_finished(ZB_FALSE);
}


static void bdb_tc_connectivity_poll_keep_alive_cb(zb_cb_param_t param)
{
  zb_zcl_command_send_status_t *status = ZB_BUF_GET_PARAM(param, zb_zcl_command_send_status_t);

  /* Command was sent with APS ACK request bit set.
     As it hasn't received by TC, consider poll as failed. */
  if (status->status != ZB_ZCL_STATUS_SUCCESS)
  {
    bdb_tc_connectivity_poll_finished(ZB_FALSE);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "Keep-alive poll sent", (FMT__0));

    /* Schedule timeout just in case if TC doesn't sent response to us.
       Poll will be rescheduled after read attribute reception. */
    ZB_SCHEDULE_ALARM(bdb_tc_connectivity_poll_timeout, ZB_BUF_INVALID, BDB_TC_CONNECTIVITY_POLL_TIMEOUT);
  }

  zb_buf_free(param);
}


/**
 * @brief Starts poll using keep alive method.
 *
 * @param param buf_id to store ZCL pkt.
 */
static void bdb_tc_connectivity_do_poll_keep_alive(zb_cb_param_t param)
{
  zb_uint8_t *cmd_ptr;
  zb_uint16_t tc_short_addr = zb_aib_get_trust_center_short_address();
  zb_af_endpoint_desc_t *ep_desc;
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_do_poll_keep_alive %d", (FMT__D, param));

  ZB_ZCL_GENERAL_INIT_READ_ATTR_REQ(param, cmd_ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);

  ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID);
  ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID);

  ep_desc = get_endpoint_by_cluster_with_role(ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,
                                              ZB_ZCL_CLUSTER_CLIENT_ROLE);
  ZB_ASSERT(ep_desc);

  /* Call with cb set to not think about APS timings. */
  ret = zb_zcl_finish_and_send_packet(param,
                                      cmd_ptr,
                                      (zb_addr_u *)(&tc_short_addr),
                                      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                      BDB_COMM_CTX().tc_connectivity_ctx.endpoint,
                                      ep_desc->ep_id,
                                      get_profile_id_by_endpoint(ep_desc->ep_id),
                                      ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,
                                      bdb_tc_connectivity_poll_keep_alive_cb);

  if (ret == RET_OK)
  {
    BDB_COMM_CTX().tc_connectivity_ctx.tsn = ZCL_CTX().seq_number - 1;
  }
  else
  {
    /* ZCL doesn't allow to send pkt.
       Consider poll as failed. */
    bdb_tc_connectivity_poll_finished(ZB_FALSE);
    zb_buf_free(param);
  }
}


/**
 * @brief Handles read attributes response for keep-alive cluster.
 *
 * @param param
 * @return zb_bool_t
 */
static zb_bool_t bdb_tc_connectivity_handle_keepalive_read_attr(zb_bufid_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_zcl_read_attr_res_t *resp = NULL;
  zb_uint8_t read_attr_resp_cnt = 0;
  zb_uint16_t cluster_id = cmd_info->cluster_id;

  TRACE_MSG(TRACE_ZCL1, ">> bdb_tc_connectivity_handle_keepalive_read_attr param %d", (FMT__D, param));
  ZB_SCHEDULE_ALARM_CANCEL(bdb_tc_connectivity_poll_timeout, ZB_ALARM_ALL_CB);

  do
  {
    ZB_ZCL_GENERAL_GET_NEXT_READ_ATTR_RES(param, resp);

    if (resp && resp->status == ZB_ZCL_STATUS_SUCCESS)
    {
      if (cluster_id == ZB_ZCL_CLUSTER_ID_KEEP_ALIVE)
      {
        if (resp->attr_id == ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID)
        {
          BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_base = (zb_uint16_t) resp->attr_value[0] * 60U;
          TRACE_MSG(TRACE_ZCL1, "keep-alive: base %d seconds", (FMT__D, BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_base));
        }
        else if (resp->attr_id == ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID)
        {
          ZB_HTOLE16(&BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_jitter, resp->attr_value);
          TRACE_MSG(TRACE_ZCL1, "keep-alive: jitter %d seconds", (FMT__D, BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_jitter));
        }
        else
        {
          /* Don't need to handle another attributes.
             It seems impossible to have another attribute in response,
             but there is also no need to crash. */
        }
        ++read_attr_resp_cnt;
      }
    }
  }
  while (resp);


  if (!read_attr_resp_cnt
      || (BDB_COMM_CTX().tc_connectivity_ctx.method == BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE
      /* Keep alive should have 2 attributes in request (according to ZCL 8 spec, 3.18.4).
         It seems impossible to receive another amount of attributes in response. */
        && read_attr_resp_cnt != 2))
  {
    bdb_tc_connectivity_poll_finished(ZB_FALSE);
  }
  else
  {
    bdb_tc_connectivity_poll_finished(ZB_TRUE);
  }

  TRACE_MSG(TRACE_ZCL1, "<< bdb_tc_connectivity_handle_keepalive_read_attr", (FMT__0));

  return (zb_bool_t)(!param);
}


static void bdb_tc_connectivity_node_desc_req_poll_cb(zb_cb_param_t param)
{
  zb_zdo_node_desc_resp_t *rsp = zb_buf_begin(param);
  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_node_desc_req_poll_cb param %d", (FMT__D, param));

  bdb_tc_connectivity_poll_finished(rsp->hdr.status == ZB_ZDP_STATUS_SUCCESS);
  zb_buf_free(param);
}

static void bdb_tc_connectivity_do_poll_node_desc_req(zb_cb_param_t param)
{
  zb_uint16_t tc_short_address = zb_aib_get_trust_center_short_address();
  zb_zdo_node_desc_req_t *req = zb_buf_initial_alloc(param, sizeof(zb_zdo_node_desc_req_t));
  zb_uint8_t zdo_tsn;

  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_do_poll_node_desc_req param %d", (FMT__D, param));

  req->nwk_addr = tc_short_address;

  zdo_tsn = zb_zdo_node_desc_req(param, bdb_tc_connectivity_node_desc_req_poll_cb);

  /* It seems that there is no memory to store cb.
     Consider poll as failed, do retry after timeout. */
  if (zdo_tsn == ZB_ZDO_INVALID_TSN)
  {
    bdb_tc_connectivity_poll_finished(ZB_FALSE);
    zb_buf_free(param);
  }
}


/**
 * @brief Start discovery of poll control client on ZC.
 *        Issues zb_aps_check_binding_request in order to check that ZC has already bound to current device.
 *        Continues discovery in bdb_tc_connectivity_poll_ctrl_check_binding_rsp.
 *
 * @param param buffer_id to store check_binding_req in it.
 * @return zb_ret_t RET_NOT_FOUND if poll control server cluster is not supported on the current node.
 *                  RET_OK if check_binding_req has scheduled.
 */
static zb_ret_t bdb_tc_connectivity_discover_poll_ctrl_client(zb_bufid_t param)
{
  zb_ret_t ret = RET_NOT_FOUND;
  zb_aps_check_binding_req_t *req = ZB_BUF_GET_PARAM(param, zb_aps_check_binding_req_t);
  zb_af_endpoint_desc_t *ep_desc;

  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_discover_poll_ctrl_client param %hd",
           (FMT__H, param));

  ep_desc = get_endpoint_by_cluster_with_role(ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
                                              ZB_ZCL_CLUSTER_SERVER_ROLE);

  if (ep_desc != NULL)
  {
    req->cluster_id = ZB_ZCL_CLUSTER_ID_POLL_CONTROL;
    req->src_endpoint = ep_desc->ep_id;
    req->response_cb = bdb_tc_connectivity_poll_ctrl_check_binding_rsp;

    ZB_SCHEDULE_CALLBACK(zb_aps_check_binding_request, param);
    ret = RET_OK;
  }

  TRACE_MSG(TRACE_ZCL1, "<< bdb_tc_connectivity_discover_keepalive_server ret %d",
           (FMT__D, ret));

  return ret;
}


/**
 * @brief Callback that used to check bindings on APS.
 *        If binding exists, completes discovery.
 *        Otherwise, tries to send match descriptor request
 *          to TC in order to check that Poll Control client is supported on that side.
 *
 * @param param buffer_id that contains zb_aps_check_binding_resp_t.
 */
static void bdb_tc_connectivity_poll_ctrl_check_binding_rsp(zb_cb_param_t param)
{
  zb_aps_check_binding_resp_t *resp = ZB_BUF_GET_PARAM(param, zb_aps_check_binding_resp_t);

  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_poll_ctrl_check_binding_rsp exists %hd", (FMT__H, resp->exists));

  /* Binding exists.
     Poll control should've been started on ZCL
      inside zb_zcl_init_periodic_activities.
     Note that it is possible that user've called zb_zcl_poll_control_stop func.
     In such case, TC connectivity checks won't work. */
  if (resp->exists)
  {
    bdb_tc_connectivity_finish_discovery(param);
  }
  else
  {
    ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_poll_ctrl_send_match_desc_req, param);
  }
}


/**
 * @brief Schedules Match Descriptor Req sending to TC in order to discover
 *        if it supports Poll Control cluster client.
 *
 * @param param buffer_id to store Match Descriptor req.
 */
static void bdb_tc_connectivity_poll_ctrl_send_match_desc_req(zb_cb_param_t param)
{
  zb_uint8_t zdo_tsn;
  zb_af_endpoint_desc_t *ep_desc;

  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_poll_ctrl_send_match_desc_req param %d", (FMT__D, param));

  ep_desc = get_endpoint_by_cluster_with_role(ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
                                              ZB_ZCL_CLUSTER_SERVER_ROLE);

  /* Has been checked in bdb_tc_connectivity_discover_poll_ctrl_client */
  ZB_ASSERT(ep_desc != NULL);

  bdb_tc_connectivity_fill_match_desc_req_to_tc(param,
                                                ep_desc->simple_desc->app_profile_id,
                                                ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
                                                ZB_FALSE);

  zdo_tsn = zb_zdo_match_desc_req(param, bdb_tc_connectivity_match_descr_resp_handler);

  if (zdo_tsn == ZB_ZDO_INVALID_TSN)
  {
    ZB_SCHEDULE_CALLBACK(bdb_tc_connectivity_discovery_attempt_failed, param);
  }
}


/**
 * @brief Handles checkin status upon sending and links it to BDB TC Connectivity.
 *
 * @param checkin_status ZB_ZCL_STATUS_SUCCESS - if checkin has sent successfully,
 *                       Any other value - if checkin hasn't delivered to TC or TC hasn't sent response.
 */
void bdb_tc_connectivity_poll_control_checkin_handler(zb_zcl_status_t checkin_status)
{
  TRACE_MSG(TRACE_ZCL1, "Poll control checking handler state %hd, method %hd",
           (FMT__H_H, BDB_COMM_CTX().tc_connectivity_ctx.state, BDB_COMM_CTX().tc_connectivity_ctx.method));
  if (BDB_COMM_CTX().tc_connectivity_ctx.state == BDB_TC_CONNECTIVITY_STATE_POLLING
      && BDB_COMM_CTX().tc_connectivity_ctx.method == BDB_TC_CONNECTIVITY_METHOD_POLL_CONTROL)
  {
    bdb_tc_connectivity_poll_finished(checkin_status == ZB_ZCL_STATUS_SUCCESS);
  }
}

static void bdb_tc_connectivity_schedule_poll(void)
{
  zb_time_t jitter = bdb_tc_connectivity_get_jitter_bi();
  ZB_SCHEDULE_ALARM(bdb_tc_connectivity_do_poll, ZB_BUF_INVALID,
                    GET_KEEPALIVE_INTERVAL(BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_base,
                                            jitter));
}

/**
 * @brief Does common actions upon finished poll depending on its status.
 *        If there are 3 failed polls in succession, forces device to rejoin.
 *        Otherwise, schedules poll.
 *
 * @param poll_successful ZB_TRUE indicates that poll has successfully finished
 *                  and response from TC has received.
 *                ZB_FALSE indicates that poll has unsuccessfully finished.
 *                  It may mean that
 *
 */
static void bdb_tc_connectivity_poll_finished(zb_bool_t poll_successful)
{
  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_poll_finished poll_successful %hd failure_ctr %hd",
           (FMT__H_H, poll_successful, BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr));

  BDB_COMM_CTX().tc_connectivity_ctx.tsn = ZB_ZDO_INVALID_TSN;

  if (poll_successful)
  {
    BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr = 0;

    /* Don't set hub connectivity bit here.
       It'll be set on APS layer right after pkt decryption
        if pkt has been encrypted by TCLK. */
  }
  else
  {
    BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr++;

    /* Hub connectivity bit also cleared on APS layer, but clear it here too.
        This case may occur if TC ACK'ed our pkt on APS, but hasn't sent a response using TCLK. */
#if defined ZB_ROUTER_ROLE
    if (ZB_IS_DEVICE_ZR())
    {
      nwk_set_tc_connectivity(ZB_FALSE);
    }
#endif
  }

  if (BDB_COMM_CTX().tc_connectivity_ctx.failure_ctr >= BDB_TC_CONNECTIVITY_MAX_FAILURE_CNT)
  {
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
    if (ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR())
    {
      zb_bdb_tc_connectivity_stop_checking();
      zb_buf_get_out_delayed_ext(bdb_start_rejoin_recovery, BDB_COMM_REJOIN_REASON_UNSPECIFIED, 0);
    }
    else
#endif /* ZB_ZCL_WWAH_BEHAVIOR_SERVER */
    {
      zb_buf_get_out_delayed(bdb_tc_connectivity_init_rejoin);
    }
  }
  else if (BDB_COMM_CTX().tc_connectivity_ctx.method == BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE)
  {
    /* Interval already updated in bdb_tc_connectivity_handle_keepalive_read_attr func. */
    bdb_tc_connectivity_schedule_poll();
  }
  else if (BDB_COMM_CTX().tc_connectivity_ctx.method == BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ)
  {
    if (poll_successful)
    {
      BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_base = BDB_COMM_CTX().tc_connectivity_ctx.initial_backoff_time;
    }
    else
    {
      /* 32-bit variable needed to prevent possible overflow. */
      zb_uint32_t new_backoff_time = BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_base * 2U;
      new_backoff_time = new_backoff_time < BDB_COMM_CTX().tc_connectivity_ctx.max_backoff_time
                         ? new_backoff_time : BDB_COMM_CTX().tc_connectivity_ctx.max_backoff_time;

      /* Impossible to get uint32 value here, cast to uint16. */
      BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_base = (zb_uint16_t) new_backoff_time;
    }

    bdb_tc_connectivity_schedule_poll();
  }
  else
  {
    /* Poll control method, do nothing. */
  }
}


/* May be used only for keep-alive method and WWAH */
zb_bool_t zb_bdb_tc_connectivity_block_zcl_cmd(zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t ret = ZB_FALSE;
  zb_uint16_t tc_short_addr = zb_aib_get_trust_center_short_address();

  TRACE_MSG(TRACE_ZCL1, ">> zb_bdb_tc_connectivity_block_zcl_cmd cluster %d, endpoint %hd",
           (FMT__D_H, cmd_info->cluster_id, ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint));

  if (BDB_COMM_CTX().tc_connectivity_ctx.state == BDB_TC_CONNECTIVITY_STATE_POLLING)
  {
    switch (BDB_COMM_CTX().tc_connectivity_ctx.method)
    {
      case BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE:
        ret = BDB_COMM_CTX().tc_connectivity_ctx.method == BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE
            && !IS_DISTRIBUTED_SECURITY()
            && cmd_info->cluster_id == ZB_ZCL_CLUSTER_ID_KEEP_ALIVE
            && tc_short_addr != ZB_UNKNOWN_SHORT_ADDR
            && cmd_info->addr_data.common_data.source.addr_type == ZB_ZCL_ADDR_TYPE_SHORT
            && cmd_info->addr_data.common_data.source.u.short_addr == tc_short_addr
            && BDB_COMM_CTX().tc_connectivity_ctx.endpoint == ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint
            && BDB_COMM_CTX().tc_connectivity_ctx.tsn == cmd_info->seq_number;
        break;

      case BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC:
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
        ret = zb_zcl_wwah_periodic_checkin_block_zcl_cmd(cmd_info);
#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */
        break;

      default:
        /* Don't block any command. */
        break;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_bdb_tc_connectivity_block_zcl_cmd ret %hd",
           (FMT__H, ret));

  return ret;
}


zb_bool_t zb_bdb_tc_connectivity_handle_read_attr_resp(zb_bufid_t param)
{
  zb_bool_t processed = ZB_FALSE;
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  TRACE_MSG(TRACE_ZCL1, ">> zb_bdb_tc_connectivity_handle_read_attr_resp param %hd",
           (FMT__H, param));

  if (zb_bdb_tc_connectivity_block_zcl_cmd(cmd_info))
  {
    switch(BDB_COMM_CTX().tc_connectivity_ctx.method)
    {
      case BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE:
        processed = bdb_tc_connectivity_handle_keepalive_read_attr(param);
        break;

      case BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC:
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
        processed = zb_zcl_wwah_periodic_checkin_read_attr_handle(param);
#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */
        break;

      /* All other cases should be handled independently. */
      default:
        ZB_ASSERT(0);
        break;
    }
  }

  return processed;
}

/**
 * @brief Restarts timer for TC poll.
 *        Should be called if device has received any msg from the TC.
 *        Consider this case as successful poll.
 */
void bdb_tc_connectivity_restart_timer(void)
{
  zb_bool_t restart_timer = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "bdb_tc_connectivity_restart_timer", (FMT__0));

  /* Don't restart timer during discovery. */
  if (BDB_COMM_CTX().tc_connectivity_ctx.state == BDB_TC_CONNECTIVITY_STATE_POLLING)
  {
    switch(BDB_COMM_CTX().tc_connectivity_ctx.method)
    {
      case BDB_TC_CONNECTIVITY_METHOD_KEEPALIVE:
        /* It means that read attributes req hasn't sent.
          Otherwise, it seems that poll in progress: ignore timer restart. */
        restart_timer = BDB_COMM_CTX().tc_connectivity_ctx.tsn == ZB_ZDO_INVALID_TSN;
        break;

      case BDB_TC_CONNECTIVITY_METHOD_NODE_DESCR_REQ:
        /* This method doesn't require discovery,
          just check that node descriptor wasn't sent earlier. */
        restart_timer = BDB_COMM_CTX().tc_connectivity_ctx.tsn == ZB_ZDO_INVALID_TSN;
        break;
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
      case BDB_TC_CONNECTIVITY_METHOD_WWAH_SPECIFIC:
        /* Don't restart timer, just notify WWAH. */
        zb_zcl_wwah_recounter_checkin();
        break;
#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */

      /* Can't update timer for poll control cluster.*/
      default:
        break;
    }
  }

  if (restart_timer)
  {
    ZB_SCHEDULE_ALARM_CANCEL(bdb_tc_connectivity_do_poll, ZB_ALARM_ALL_CB);
    bdb_tc_connectivity_poll_finished(ZB_TRUE);
  }
}

void zb_bdb_tc_connectivity_set_initial_backoff_time(zb_uint16_t backoff_time)
{
  BDB_COMM_CTX().tc_connectivity_ctx.initial_backoff_time = backoff_time;
}


void zb_bdb_tc_connectivity_set_max_backoff_time(zb_uint16_t backoff_time)
{
  BDB_COMM_CTX().tc_connectivity_ctx.max_backoff_time = backoff_time;
}


zb_uint16_t zb_bdb_tc_connectivity_get_initial_backoff_time(void)
{
  return BDB_COMM_CTX().tc_connectivity_ctx.initial_backoff_time;
}


zb_uint16_t zb_bdb_tc_connectivity_get_max_backoff_time(void)
{
  return BDB_COMM_CTX().tc_connectivity_ctx.max_backoff_time;
}

void bdb_tc_connectivity_set_jitter(zb_uint16_t jitter)
{
  BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_jitter = jitter;
}


zb_uint16_t zb_bdb_tc_connectivity_get_jitter(void)
{
  return BDB_COMM_CTX().tc_connectivity_ctx.keep_alive_jitter;
}

/**
 * @brief Initiates leave with rejoin, stops any action.
 *
 * @param param Buf id to store leave req.
 */
static void bdb_tc_connectivity_init_rejoin(zb_cb_param_t param)
{
  TRACE_MSG(TRACE_ZCL4, "bdb_tc_connectivity_init_rejoin %d", (FMT__D, param));

  zb_bdb_tc_connectivity_stop_checking();
  BDB_COMM_CTX().tc_connectivity_ctx.tc_rejoin_initiated = ZB_TRUE;

  zdo_commissioning_initiate_rejoin(param);
}

zb_uint8_t bdb_tc_connectivity_get_current_method(void)
{
  return BDB_COMM_CTX().tc_connectivity_ctx.method;
}

#endif /* ZB_JOIN_CLIENT */
