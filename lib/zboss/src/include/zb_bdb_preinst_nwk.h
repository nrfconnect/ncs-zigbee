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

#ifndef ZB_BDB_PREINST_NWK_COMMISSIONING_H
#define ZB_BDB_PREINST_NWK_COMMISSIONING_H 1

#ifdef ZB_BDB_PREINST_NWK_JOINING

#ifdef ZB_JOIN_CLIENT
/**
 * @brief Set pre-installed extended PAN ID. Mandatory for pre-installed network.
 *
 * @param panid - extended PAN ID to be set for joining pre-installed network.
 */
void zb_bdb_preinst_nwk_set_extended_pan_id(const zb_ext_pan_id_t panid);

/**
 * @brief Set pre-installed network key. Mandatory for pre-installed network.
 *
 * @param nwk_key - network key to be set after joining pre-installed network.
 * @param key_seq_num - sequence number of network key.
 */
void zb_bdb_preinst_nwk_set_nwk_key(const zb_uint8_t nwk_key[ZB_CCM_KEY_SIZE], zb_uint32_t key_seq_num);

/**
 * @brief Set pre-installed TC long address. Optional for pre-installed network.
 *
 * Allows to skip requesting long address of ZC on join.
 *
 * @param nwk_key - network key to be set after joining pre-installed network.
 * @param key_seq_num - sequence number of network key.
 */
void zb_bdb_preinst_nwk_set_tc_long_address(const zb_ieee_addr_t tc_ieee);

/**
 * @brief Set pre-installed TCLK. Optional for pre-installed network.
 *
 * The very same key must be pre-installed to ZC providing IEEE address of this joiner
 * @see zb_bdb_preinst_nwk_set_joiner_tclk
 *
 * @param tclk - TCLK to be set after joining pre-installed network.
 */
void zb_bdb_preinst_nwk_set_tclk(const zb_uint8_t tclk[ZB_CCM_KEY_SIZE]);
#endif /* ZB_JOIN_CLIENT */

#ifdef ZB_COORDINATOR_ROLE

/**
 * @brief Set pre-installed joiner's TCLK. For testing purposes only.
 *
 * @param joiner_address - long address of a joiner device.
 * @param tclk - TCLK of the joiner.
 */
void zb_bdb_preinst_nwk_set_joiner_tclk(const zb_ieee_addr_t joiner_address, const zb_uint8_t tclk[ZB_CCM_KEY_SIZE]);

#endif /* ZB_COORDINATOR_ROLE */

/*****************************************************************************/
#endif /* ZB_BDB_PREINST_NWK_JOINING */
#endif /* ZB_BDB_PREINST_NWK_COMMISSIONING_H */

