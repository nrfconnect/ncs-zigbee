
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

#ifndef ZB_LITE_H
#define ZB_LITE_H

#if defined(ZB_DIAGNOSTIC_TH_MODIFIERS_ENABLED)

#define ZB_DIAG_TH_BEHAVIOUR_INIT() zb_diag_th_behaviour_init()

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
#define ZB_DIAG_TH_KEY_NEG_RSP_NOT_AUTHO(resp) zb_diag_th_key_neg_rsp_not_autho(resp)
#define ZB_DIAG_TH_KEY_NEG_RSP_TRUNCATED_TLV(bufid, key_neg_ctx_ptr) zb_diag_th_key_neg_rsp_truncated_tlv((bufid), (key_neg_ctx_ptr))
#endif /* ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE */

#define ZB_DIAG_TH_KEY_NEG_REQ_TRUNCATED_TLV(bufid) zb_diag_th_key_neg_req_truncated_tlv(bufid)
#define ZB_DIAG_TH_KEY_NEG_RSP_ADD_EXTRA_TLV(bufid, allocation_bytes) zb_diag_th_key_neg_rsp_add_extra_tlv((bufid), (allocation_bytes))
#define ZB_DIAG_TH_KEY_NEG_REQ_ADD_EXTRA_TLV(bufid, allocation_bytes) zb_diag_th_key_neg_req_add_extra_tlv((bufid), (allocation_bytes))

#define ZB_DIAG_TH_IS_DISABLE_SUPPORT_KN_TLV_IN_BEACON() zb_diag_th_is_disable_support_kn_tlv_in_beacon()
#define ZB_DIAG_TH_IS_DISABLE_UPDATE_HUB_CONNECTIVITY() zb_diag_th_is_disable_update_hub_connectivity()
#define ZB_DIAG_TH_IS_KEEP_PROVISIONAL_KEY_FOR_16_3() zb_diag_th_is_keep_provisional_key_for_16_3()
#define ZB_DIAG_TH_IS_KEY_NEG_RSP_NO_TLV() zb_diag_th_is_key_neg_rsp_no_tlv()
#define ZB_DIAG_TH_IS_KEY_NEG_REQ_NO_TLV() zb_diag_th_is_key_neg_req_no_tlv()

#define ZB_DIAG_TH_IS_ALLOW_ENTRY_FOR_UNREGISTERED_EP() zb_diag_th_is_allow_entry_for_unregistered_ep()
#define ZB_DIAG_TH_IS_APS_DROP_ACK() zb_diag_th_is_aps_drop_ack()
#define ZB_DIAG_TH_APS_DUP_RELAYED_FRAMES(relay_tlv_hdr) zb_diag_th_aps_dup_relayed_frames(relay_tlv_hdr)
#define ZB_DIAG_TH_IS_DISABLE_APS_SEC_FOR_ALL_ZDO_CMD() zb_diag_th_is_disable_aps_sec_for_all_zdo_cmd()
#define ZB_DIAG_TH_IS_DISABLE_LINK_POWER_NEGOTIATION() zb_diag_th_is_disable_link_power_negotiation()
#define ZB_DIAG_TH_IS_DISABLE_REJOIN_RESP_TIMEOUT() zb_diag_th_is_disable_rejoin_resp_timeout()
#define ZB_DIAG_TH_IS_DISABLE_TP_APS_ENCRYPTION() zb_diag_th_is_disable_tp_aps_encryption()
#define ZB_DIAG_TH_IS_FORCE_SECURE_REJOIN() zb_diag_th_is_force_secure_rejoin()
#define ZB_DIAG_TH_RESET_BEACON_END_DEVICE_CAPACITY(beacon_payload_ptr) zb_diag_th_reset_beacon_end_device_capacity(beacon_payload_ptr)
#define ZB_DIAG_TH_IS_SEND_REJOIN_RSP_WO_SECUR() zb_diag_th_is_send_rejoin_rsp_wo_secur()
#define ZB_DIAG_TH_IS_TC_REJOIN_APS_DECRYPT_ERROR() zb_diag_th_is_tc_rejoin_aps_decrypt_error()
#define ZB_DIAG_TH_TC_REJOIN_MAC_CAP_WRONG_RX_ON_WHEN_IDLE(req) zb_diag_th_tc_rejoin_mac_cap_wrong_rx_on_when_idle(req)

#define ZB_DIAG_TH_CHALLENGE_REQ_ADD_EXTRA_TLV(param) zb_diag_th_challenge_req_add_extra_tlv(param)
#define ZB_DIAG_TH_CHALLENGE_RSP_WRONG_MIC(ptr) zb_diag_th_challenge_rsp_wrong_mic(ptr)
#define ZB_DIAG_TH_IS_DISABLE_TP_APS_ACK() zb_diag_th_is_disable_tp_aps_ack()
#define ZB_DIAG_TH_SECUR_APS_COUNTER_HACK_CB(out_sec_counter) zb_diag_th_secur_aps_counter_hack_cb(out_sec_counter)

#define ZB_DIAG_TH_IS_NWK_DISABLE_PASSIVE_ACKS() zb_diag_th_is_nwk_disable_passive_acks()

#else /* ZB_DIAGNOSTIC_TH_MODIFIERS_ENABLED */

#define ZB_DIAG_TH_BEHAVIOUR_INIT()
#define ZB_DIAG_TH_IS_DISABLE_SUPPORT_KN_TLV_IN_BEACON() zb_return_false_func()
#define ZB_DIAG_TH_IS_DISABLE_UPDATE_HUB_CONNECTIVITY() zb_return_false_func()
#define ZB_DIAG_TH_IS_KEEP_PROVISIONAL_KEY_FOR_16_3() zb_return_false_func()
#define ZB_DIAG_TH_KEY_NEG_RSP_NOT_AUTHO(resp)
#define ZB_DIAG_TH_KEY_NEG_RSP_TRUNCATED_TLV(bufid, key_neg_ctx_ptr)
#define ZB_DIAG_TH_IS_KEY_NEG_RSP_NO_TLV() zb_return_false_func()
#define ZB_DIAG_TH_KEY_NEG_RSP_ADD_EXTRA_TLV(bufid, allocation_bytes)
#define ZB_DIAG_TH_KEY_NEG_REQ_TRUNCATED_TLV(bufid)
#define ZB_DIAG_TH_IS_KEY_NEG_REQ_NO_TLV() zb_return_false_func()
#define ZB_DIAG_TH_KEY_NEG_REQ_ADD_EXTRA_TLV(bufid, allocation_bytes)
#define ZB_DIAG_TH_IS_ALLOW_ENTRY_FOR_UNREGISTERED_EP() zb_return_false_func()
#define ZB_DIAG_TH_IS_APS_DROP_ACK() zb_return_false_func()
#define ZB_DIAG_TH_APS_DUP_RELAYED_FRAMES(relay_tlv_hdr)
#define ZB_DIAG_TH_IS_DISABLE_APS_SEC_FOR_ALL_ZDO_CMD() zb_return_false_func()
#define ZB_DIAG_TH_IS_DISABLE_LINK_POWER_NEGOTIATION() zb_return_false_func()
#define ZB_DIAG_TH_IS_DISABLE_REJOIN_RESP_TIMEOUT() zb_return_false_func()
#define ZB_DIAG_TH_IS_DISABLE_TP_APS_ENCRYPTION() zb_return_false_func()
#define ZB_DIAG_TH_IS_FORCE_SECURE_REJOIN() zb_return_false_func()
#define ZB_DIAG_TH_RESET_BEACON_END_DEVICE_CAPACITY(beacon_payload_ptr)
#define ZB_DIAG_TH_IS_SEND_REJOIN_RSP_WO_SECUR() zb_return_false_func()
#define ZB_DIAG_TH_IS_TC_REJOIN_APS_DECRYPT_ERROR() zb_return_false_func()
#define ZB_DIAG_TH_TC_REJOIN_MAC_CAP_WRONG_RX_ON_WHEN_IDLE(req)
#define ZB_DIAG_TH_CHALLENGE_REQ_ADD_EXTRA_TLV(param)
#define ZB_DIAG_TH_CHALLENGE_RSP_WRONG_MIC(ptr)
#define ZB_DIAG_TH_IS_DISABLE_TP_APS_ACK()
#define ZB_DIAG_TH_SECUR_APS_COUNTER_HACK_CB(out_sec_counter)
#define ZB_DIAG_TH_IS_NWK_DISABLE_PASSIVE_ACKS() zb_return_false_func()

#endif /* ZB_DIAGNOSTIC_TH_MODIFIERS_ENABLED */
/* ---------------------------------------------------------------- */

#if defined(ZB_DIAGNOSTIC_DUT_MODIFIERS_ENABLED)

#define ZB_DIAG_DUT_BEHAVIOUR_INIT() zb_diag_dut_behaviour_init()

#else /* ZB_DIAGNOSTIC_DUT_MODIFIERS_ENABLED */

#define ZB_DIAG_DUT_BEHAVIOUR_INIT()

#endif /* ZB_DIAGNOSTIC_DUT_MODIFIERS_ENABLED */
/* ---------------------------------------------------------------- */

#endif /* ZB_LITE_H */
