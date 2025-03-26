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
/* PURPOSE: Routines to support WWAH moved there from ZCL, then All Hubs in r23
*/

#ifndef ZDO_HUBS_H
#define ZDO_HUBS_H

zb_bool_t zb_is_wwah_server(void);

zb_bool_t zb_wwah_check_nwk_upd_bcast_allowed(void);
zb_bool_t zb_wwah_check_require_lk_encryption(void);
void zb_wwah_set_require_lk_encryption(zb_bool_t require);

zb_bool_t zb_wwah_in_configuration_mode(void);
void zb_wwah_set_configuration_mode(zb_bool_t allowed);
zb_ret_t zb_wwah_check_zdo_command(zb_apsde_data_indication_t *di);
void zb_zcl_wwah_update_tc_connection(zb_uint16_t short_addr);


zb_bool_t zb_wwah_check_if_leave_without_rejoin_allowed(void);
void zb_wwah_set_leave_without_rejoin_allowed(zb_bool_t allowed);

zb_bool_t zb_wwah_check_if_interpan_supported(void);
void zb_wwah_set_interpan_supported(zb_bool_t enabled);

#ifdef ZB_CERTIFICATION_HACKS
void zb_wwah_overwrite_hub_connectivity(zb_bool_t overwrite, zb_bool_t val);
#endif

void zb_wwah_set_pending_channel(zb_uint32_t channel_mask);

void zb_wwah_set_pending_panid(zb_uint16_t next_pan_id);

void zdo_schedule_set_long_uptime(zb_uint8_t param);
void zdo_schedule_cancel_long_uptime(zb_uint8_t param);

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
zb_ret_t zb_zcl_wwah_set_pending_channel(zb_uint32_t channel_mask);
zb_ret_t zb_zcl_wwah_set_pending_panid(zb_uint16_t next_pan_id);
zb_ret_t zb_zcl_wwah_set_leave_without_rejoin(zb_bool_t allowed);
zb_ret_t zb_zcl_wwah_set_configuration_mode(zb_bool_t allowed);
zb_ret_t zb_zcl_wwah_set_require_lk_encryption(zb_bool_t require);
#endif

#endif  /* ZDO_HUBS_H */
