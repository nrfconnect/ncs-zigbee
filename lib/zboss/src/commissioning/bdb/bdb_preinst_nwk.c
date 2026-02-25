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
/*  PURPOSE: NOT standard feature: allow factory new devices join using pre-installed network parameters
*/

#define ZB_TRACE_FILE_ID 1403

#include "zb_common.h"
#include "zb_bdb_internal.h"
#include "zb_bdb_preinst_nwk.h"

#ifdef ZB_BDB_PREINST_NWK_JOINING

#ifdef ZB_JOIN_CLIENT

typedef struct zb_bdb_preinst_nwk_parameters_s
{
 zb_ext_pan_id_t extpanid;
 zb_ieee_addr_t tc_long_address;
 zb_uint8_t tclk[ZB_CCM_KEY_SIZE];
 zb_uint8_t nwkkey[ZB_CCM_KEY_SIZE];
 zb_uint32_t nwkkey_seq_num;
} zb_bdb_preinst_nwk_parameters_t;

typedef struct zb_bdb_preinst_nwk_ctx_s
{
  zb_bdb_preinst_nwk_parameters_t parameters;
  zb_bool_t is_applied_preinst_nwk_joining;
} zb_bdb_preinst_nwk_ctx_t;

static zb_bdb_preinst_nwk_ctx_t s_ctx;

static void zb_bdb_preinst_nwk_ensure_r22_behavior(void)
{
  /* Because @ref zboss_use_r22_behavior traces with INFO1,
   * it is better to call it once */
  if (!zb_nwk_need_use_r22_joining())
  {
    TRACE_MSG(TRACE_INFO1, "Enable R22 behavior because of pre-install network", (FMT__0));
    zboss_use_r22_behavior();
  }
}

void zb_bdb_preinst_nwk_set_extended_pan_id(const zb_ext_pan_id_t panid)
{
  ZB_ASSERT(panid);

  zb_bdb_preinst_nwk_ensure_r22_behavior();

  ZB_EXTPANID_COPY(s_ctx.parameters.extpanid, panid);
}

void zb_bdb_preinst_nwk_set_nwk_key(const zb_uint8_t nwkkey[ZB_CCM_KEY_SIZE],
                                    zb_uint32_t key_seq_num)
{
  ZB_ASSERT(nwkkey);

  zb_bdb_preinst_nwk_ensure_r22_behavior();

  s_ctx.parameters.nwkkey_seq_num = key_seq_num;
  ZB_MEMCPY(s_ctx.parameters.nwkkey, nwkkey, ZB_CCM_KEY_SIZE);
}

void zb_bdb_preinst_nwk_set_tc_long_address(const zb_ieee_addr_t tc_ieee)
{
  ZB_ASSERT(!ZB_IEEE_ADDR_IS_UNKNOWN(tc_ieee));

  zb_bdb_preinst_nwk_ensure_r22_behavior();

  ZB_IEEE_ADDR_COPY(s_ctx.parameters.tc_long_address, tc_ieee);
}

void zb_bdb_preinst_nwk_set_tclk(const zb_uint8_t tclk[ZB_CCM_KEY_SIZE])
{
  ZB_ASSERT(tclk);
  ZB_ASSERT(!ZB_CCM_KEY_IS_ZERO(tclk));

  zb_bdb_preinst_nwk_ensure_r22_behavior();

  ZB_MEMCPY(s_ctx.parameters.tclk, tclk, ZB_CCM_KEY_SIZE);
}

static void zb_bdb_preinst_nwk_setup_tclk(void)
{
  zb_ieee_addr_t tc_ieee;

  zb_aib_get_trust_center_address(tc_ieee);

  if (!ZB_IEEE_ADDR_IS_UNKNOWN(tc_ieee))
  {
    zb_secur_update_key_pair(tc_ieee,
                             s_ctx.parameters.tclk,
                             ZB_SECUR_UNIQUE_KEY,
                             ZB_SECUR_VERIFIED_KEY,
                             ZB_SECUR_KEY_SRC_UNKNOWN);
  }
}

static void zb_preinst_nwk_req_tc_long_address_cb(zb_uint8_t param)
{
  zb_zdo_ieee_addr_resp_t *resp = zb_buf_begin(param);

  zb_aib_set_trust_center_address(resp->ieee_addr_remote_dev);
  zb_bdb_preinst_nwk_setup_tclk();
}

static void zb_preinst_nwk_req_tc_long_address(zb_uint8_t param)
{
  zb_zdo_ieee_addr_req_param_t *req;

  req = zb_buf_alloc_tail(param, sizeof(*req));
  req->dst_addr = 0x0000;
  req->nwk_addr = 0x0000;
  req->request_type = ZB_ZDO_SINGLE_DEV_RESPONSE;

  (void) zb_zdo_ieee_addr_req(param, zb_preinst_nwk_req_tc_long_address_cb);
}

void zb_bdb_preinst_nwk_on_join_confirm(void)
{
  zb_ieee_addr_t tc_ieee;

  if (!s_ctx.is_applied_preinst_nwk_joining)
  {
    TRACE_MSG(TRACE_ZDO3, "It is not preinst network joining: skip TCLK setup", (FMT__0));
  }
  else if (ZB_CCM_KEY_IS_ZERO(s_ctx.parameters.tclk))
  {
    TRACE_MSG(TRACE_INFO1, "No TCLK provided for pre-installed network: skip TCLK setup", (FMT__0));
  }
  else
  {
    if (!zb_aib_trust_center_address_zero() &&
        !zb_aib_trust_center_address_unknown())
    {
      TRACE_MSG(TRACE_ZDO1, "TC address is known to apply pre-installed tclk", (FMT__0));
      zb_bdb_preinst_nwk_setup_tclk();
    }
    else if (zb_address_ieee_by_short(0x0000, tc_ieee) == RET_OK)
    {
      TRACE_MSG(TRACE_ZDO1, "ZC address is known to apply pre-installed tclk", (FMT__0));
      zb_aib_set_trust_center_address(tc_ieee);
      zb_bdb_preinst_nwk_setup_tclk();
    }
    else
    {
      TRACE_MSG(TRACE_ZDO1, "TC address is unknown to apply pre-installed tclk: request long address", (FMT__0));
      zb_buf_get_out_delayed(zb_preinst_nwk_req_tc_long_address);
    }
  }
}

static zb_bool_t zb_preinst_nwk_validate_parameters(void)
{
  zb_bool_t ret = ZB_TRUE;

  if (ZB_EXTPANID_IS_ZERO(s_ctx.parameters.extpanid))
  {
    TRACE_MSG(TRACE_ERROR, "BDB pre-installed network: no extended PAN ID", (FMT__0));
    ret = ZB_FALSE;
  }

  if (ZB_CCM_KEY_IS_ZERO(s_ctx.parameters.nwkkey))
  {
    TRACE_MSG(TRACE_ERROR, "BDB pre-installed network: no network key", (FMT__0));
    ret = ZB_FALSE;
  }

  return ret;
}

static void zb_preinst_nwk_apply_parameters(void)
{
  ZB_EXTPANID_COPY(ZB_NIB_EXT_PAN_ID(), s_ctx.parameters.extpanid);
  zb_secur_setup_nwk_key(s_ctx.parameters.nwkkey, s_ctx.parameters.nwkkey_seq_num);

  if (!ZB_IEEE_ADDR_IS_UNKNOWN(s_ctx.parameters.tc_long_address))
  {
    zb_aib_set_trust_center_address(s_ctx.parameters.tc_long_address);
  }

  if (!ZB_CCM_KEY_IS_ZERO(s_ctx.parameters.tclk))
  {
    /* ZC version is mostly used in commissioning, but
     * let it be R22, so that it is not considered as legacy <=R21 */
    ZB_AIB().coordinator_version = 22U;
  }
  else
  {
    /* No TCLK, assume Legacy <R21 here */
    ZB_AIB().coordinator_version = 20U;
  }

  ZB_SET_JOINED_STATUS(ZB_TRUE);
  ZG->aps.authenticated = ZB_TRUE;
}

zb_ret_t zb_bdb_preinst_nwk_on_factory_new(zb_uint8_t param)
{
  zb_ret_t ret = RET_IGNORE;

  if (zb_preinst_nwk_validate_parameters())
  {
    zb_aib_tcpol_set_update_trust_center_link_keys_required(ZB_FALSE);

    zb_preinst_nwk_apply_parameters();

    ZB_ASSERT(zb_zdo_joined());
    ZB_ASSERT(zb_zdo_authenticated());

    /* Restart BDB machine rejoining to the pre-installed network */
    ZB_BDB().bdb_commissioning_step = ZB_BDB_INITIALIZATION;
    bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_SECURE_REJOIN, param);

    /* Raise flag to know on join confirm, that it is pre-installed network joined */
    s_ctx.is_applied_preinst_nwk_joining = ZB_TRUE;
    ret = RET_OK;
  }

  return ret;
}

#endif /* ZB_JOIN_CLIENT */

#ifdef ZB_COORDINATOR_ROLE

void zb_bdb_preinst_nwk_set_joiner_tclk(const zb_ieee_addr_t joiner_address,
                                        const zb_uint8_t tclk[ZB_CCM_KEY_SIZE])
{
  zb_ieee_addr_t address;
  zb_uint8_t key[ZB_CCM_KEY_SIZE];

  ZB_ASSERT(joiner_address);
  ZB_ASSERT(tclk);
  ZB_ASSERT(!ZB_CCM_KEY_IS_ZERO(tclk));

  TRACE_MSG(TRACE_ZDO1, "Preinstall tclk for "TRACE_FORMAT_64, (FMT__A, TRACE_ARG_64(joiner_address)));

  ZB_IEEE_ADDR_COPY(address, joiner_address);
  ZB_MEMCPY(key, tclk, ZB_CCM_KEY_SIZE);

  zb_secur_update_key_pair(address,
                           key,
                           ZB_SECUR_UNIQUE_KEY,
                           ZB_SECUR_VERIFIED_KEY,
                           ZB_SECUR_KEY_SRC_UNKNOWN);
}

#endif /* ZB_COORDINATOR_ROLE */

#endif /* ZB_BDB_PREINST_NWK_JOINING */
