/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
/* PURPOSE:  NCP platform API elements for nRF SoC.
*/

#ifndef ZB_NCP_NRF_PLATFORM_H__
#define ZB_NCP_NRF_PLATFORM_H__

/* Main ZBOSS stack routine in NCP mode.
 *
 * The implementation is in the ZBOSS library.
 * The name and signature are determined by the MAIN macro in zb_config_platform.h
 * and this is established at the library building stage.
 */
int zboss_app_main(void);

/* Callout function called in zboss_app_main() right after starting ZBOSS. */
void zb_ncp_app_fw_custom_post_start(void);

#endif /* ZB_NCP_NRF_PLATFORM_H__ */
