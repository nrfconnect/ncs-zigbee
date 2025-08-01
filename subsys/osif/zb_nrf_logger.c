/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zboss_api.h>

#if defined ZB_NRF_TRACE

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zboss_osif, CONFIG_ZBOSS_OSIF_LOG_LEVEL);

void zb_osif_logger_put_bytes(const zb_uint8_t *buf, zb_short_t len)
{
	if (IS_ENABLED(CONFIG_ZBOSS_TRACE_LOG_LEVEL_OFF)) {
		return;
	}

	/* Log data directly using Zephyr logging system */
	LOG_HEXDUMP_DBG(buf, len, "");
}

#if defined(CONFIG_ZB_NRF_TRACE_RX_ENABLE)
/* Function set UART RX callback function */
void zb_osif_logger_set_uart_byte_received_cb(zb_callback_t cb)
{
	LOG_ERR("Command reception is not available through logger");
}
#endif /* CONFIG_ZB_NRF_TRACE_RX_ENABLE */

#endif /* defined ZB_NRF_TRACE */
