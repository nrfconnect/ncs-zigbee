/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2025 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: features for diagnostic ZBOSS core and apps
*/

#ifndef ZB_DIAG_H
#define ZB_DIAG_H 1

#include "zboss_api.h"

/**
 * Empty init-function
 *
 * Should be called from @see zb_diag_init()
 */
void zb_diag_key_exposure_init(void);

/**
 * Broadcast a key using non-standard NWK report (command id 6)
 *
 * To be used for debug purposes only.
 *
 * @param peer_addr - address of remote device, or NULL
 * @param key - APS or NWK key
 */
void zb_diag_bcast_key(zb_uint8_t *peer_addr, zb_uint8_t key[ZB_CCM_KEY_SIZE]);

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
zb_ret_t zb_diag_broadcast_aps_key(zb_ieee_addr_t long_addr);

/**
   Broadcast NWK key.

   FOR DEBUGGING PURPOSES ONLY. It zeroes your network security. Never use it in productuion!

   That function broadcasts the current NWK key. Wireshark understands that broadcast, so it can decode the traffic.
   That function has pure debug purposes. It must be explicitly called by the application.

   The function works in debug ZBOSS builds only.
   The function does nothing in release ZBOSS builds.

   @return RET_OK if broadcast done ok, RET_ERROR if that function is disabled in that ZBOSS build

 */
zb_ret_t zb_diag_broadcast_nwk_key(void);

/**
 * Broadcast a key using non-standard NWK report (command id 6)
 *
 * The function was deprecated, now it's a wrapper for @see zb_diag_bcast_key
 *
 * @param peer_addr - address of remote device, or NULL
 * @param key - APS or NWK key
 */
#define zb_debug_bcast_key zb_diag_bcast_key

/**
 * @depreceted it's a wrapper for @see zb_diag_broadcast_aps_key()
 */
#define zb_debug_broadcast_aps_key zb_diag_broadcast_aps_key

/**
 * @depreceted it's a wrapper for @see zb_diag_broadcast_nwk_key()
 */
#define zb_debug_broadcast_nwk_key zb_diag_broadcast_nwk_key

/**
 * Initialize and run ZBOSS core watchdog
 *
 * Should be called from @see zb_diag_init()
 */
void zb_diag_core_watchdog_init(void);

/**
 * Periodically trace a message from ZBOSS core to inform external systems (for example, TDF)
 *
 * @param param - unused; it allows to use the ZBOSS scheduler
 *
 * You can redefine the message period in your vendor configuration file, @see ZB_DIAG_CORE_WATCHDOG_TMO_MS
 */
void zb_diag_core_watchdog(zb_uint8_t param);

#endif /* ZB_DIAG_H */
