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
/* PURPOSE: ZGP Proxy public header
*/

#ifndef ZBOSS_ZGP_PROXY_H
#define ZBOSS_ZGP_PROXY_H 1

#include "zboss_api.h"

#ifdef ZB_ENABLE_ZGP_PROXY

/* ZGP spec, A.3.4.2.7 */
typedef enum zgp_gpp_functionality_e {
  ZGP_GPP_GP_FEATURE                               = (1 <<  0),
  ZGP_GPP_DIRECT_COMMUNICATION                     = (1 <<  1),
  ZGP_GPP_DERIVED_GROUPCAST_COMMUNICATION          = (1 <<  2),
  ZGP_GPP_PRECOMMISSIONED_GROUPCAST_COMMUNICATION  = (1 <<  3),
  ZGP_GPP_FULL_UNICAST_COMMUNICATION               = (1 <<  4),
  ZGP_GPP_LIGHTWEIGHT_UNICAST_COMMUNICATION        = (1 <<  5),
  ZGP_GPP_RESERVED_B6                              = (1 <<  6),
  ZGP_GPP_BIDIRECTIONAL_OPERATION                  = (1 <<  7),
  ZGP_GPP_PROXY_TABLE_MAINTENANCE                  = (1 <<  8),
  ZGP_GPP_RESERVED_B9                              = (1 <<  9),
  ZGP_GPP_GP_COMMISSIONING                         = (1 << 10),
  ZGP_GPP_CT_BASED_COMMISSIONING                   = (1 << 11),
  ZGP_GPP_MAINTENANCE_OF_GPD                       = (1 << 12),
  ZGP_GPP_SEC_LEVEL_NO_SECURITY                    = (1 << 13),
  ZGP_GPP_SEC_LEVEL_REDUCED                        = (1 << 14),
  ZGP_GPP_SEC_LEVEL_FULL_NO_ENC                    = (1 << 15),
  ZGP_GPP_SEC_LEVEL_FULL_WITH_ENC                  = (1 << 16),
  ZGP_GPP_RESERVED_B17                             = (1 << 17),
  ZGP_GPP_RESERVED_B18                             = (1 << 18),
  ZGP_GPP_GPD_IEEE_ADDRESS                         = (1 << 19)
} zgp_gpp_functionality_t;

/**
 * @brief Check that proxy support requested functionality
 *
 * @param rfb [in]  Requested functionality
 *
 * @return ZB_TRUE if requested functionality is supported, ZB_FALSE otherwise
 *
 * @see ZGP spec, A.3.4.2.7
 */
zb_bool_t zgp_proxy_is_support_functionality(zgp_gpp_functionality_t gpp_f);
#define ZB_ZGP_PROXY_IS_SUPPORT_FUNCTIONALITY(f) zgp_proxy_is_support_functionality(f)

#endif /* ZB_ENABLE_ZGP_PROXY */
#endif /* ZBOSS_ZGP_PROXY_H */
