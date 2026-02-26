/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief Zigbee Network Co-processor sample
 */

#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zb_nrf_platform.h>

#if defined(CONFIG_ZIGBEE_NCP_USB)
#include <zephyr/device.h>
#include <zephyr/usb/usbd.h>
#endif
#include <zb_led_button.h>
#include <zb_osif_ext.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <dk_buttons_and_leds.h>
#include <ncp/ncp_dev_api.h>

#if CONFIG_BOOTLOADER_MCUBOOT
#include <zephyr/dfu/mcuboot.h>
#endif

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

#if defined(CONFIG_ZIGBEE_NCP_USB)
/* USB device setup - aligned with Zephyr sample_usbd_init.c / usbd_shell */
USBD_DEVICE_DEFINE(sample_usbd,
		  DEVICE_DT_GET(DT_NODELABEL(zephyr_udc0)),
		  CONFIG_ZIGBEE_NCP_USB_VID, CONFIG_ZIGBEE_NCP_USB_PID);

USBD_DESC_LANG_DEFINE(sample_lang);
USBD_DESC_MANUFACTURER_DEFINE(sample_mfr, CONFIG_ZIGBEE_NCP_USB_MANUFACTURER);
USBD_DESC_PRODUCT_DEFINE(sample_product, CONFIG_ZIGBEE_NCP_USB_PRODUCT);
IF_ENABLED(CONFIG_HWINFO, (USBD_DESC_SERIAL_NUMBER_DEFINE(sample_sn)));

USBD_DESC_CONFIG_DEFINE(fs_cfg_desc, "FS Configuration");
USBD_DESC_CONFIG_DEFINE(hs_cfg_desc, "HS Configuration");

static const uint8_t attributes = 0; /* Bus powered, no remote wakeup */

USBD_CONFIGURATION_DEFINE(sample_fs_config,
			 attributes,
			 CONFIG_ZIGBEE_NCP_USB_MAX_POWER, &fs_cfg_desc);
USBD_CONFIGURATION_DEFINE(sample_hs_config,
			 attributes,
			 CONFIG_ZIGBEE_NCP_USB_MAX_POWER, &hs_cfg_desc);

static void sample_fix_code_triple(struct usbd_context *uds_ctx,
                                  const enum usbd_speed speed)
{
	/* Always use class code information from Interface Descriptors */
	if (IS_ENABLED(CONFIG_USBD_CDC_ACM_CLASS) ||
	    IS_ENABLED(CONFIG_USBD_CDC_ECM_CLASS) ||
	    IS_ENABLED(CONFIG_USBD_CDC_NCM_CLASS) ||
	    IS_ENABLED(CONFIG_USBD_MIDI2_CLASS) ||
	    IS_ENABLED(CONFIG_USBD_AUDIO2_CLASS) ||
	    IS_ENABLED(CONFIG_USBD_VIDEO_CLASS)) {
		/*
		 * Class with multiple interfaces have an Interface
		 * Association Descriptor available, use an appropriate triple
		 * to indicate it.
		 */
		usbd_device_set_code_triple(uds_ctx, speed,
					    USB_BCC_MISCELLANEOUS, 0x02, 0x01);
	} else {
		usbd_device_set_code_triple(uds_ctx, speed, 0, 0, 0);
	}
}

static struct usbd_context *setup_usb_device(void)
{
	int err;

	err = usbd_add_descriptor(&sample_usbd, &sample_lang);
	if (err) {
		LOG_ERR("Failed to initialize language descriptor (%d)", err);
		return NULL;
	}

	err = usbd_add_descriptor(&sample_usbd, &sample_mfr);
	if (err) {
		LOG_ERR("Failed to initialize manufacturer descriptor (%d)", err);
		return NULL;
	}

	err = usbd_add_descriptor(&sample_usbd, &sample_product);
	if (err) {
		LOG_ERR("Failed to initialize product descriptor (%d)", err);
		return NULL;
	}

	IF_ENABLED(CONFIG_HWINFO, (
		err = usbd_add_descriptor(&sample_usbd, &sample_sn);
	))
	if (err) {
		LOG_ERR("Failed to initialize SN descriptor (%d)", err);
		return NULL;
	}

	if (USBD_SUPPORTS_HIGH_SPEED &&
	    usbd_caps_speed(&sample_usbd) == USBD_SPEED_HS) {
		err = usbd_add_configuration(&sample_usbd, USBD_SPEED_HS,
					    &sample_hs_config);
		if (err) {
			LOG_ERR("Failed to add High-Speed configuration");
			return NULL;
		}

		err = usbd_register_all_classes(&sample_usbd, USBD_SPEED_HS, 1, NULL);
		if (err) {
			LOG_ERR("Failed to add register classes");
			return NULL;
		}

		sample_fix_code_triple(&sample_usbd, USBD_SPEED_HS);
	}

	err = usbd_add_configuration(&sample_usbd, USBD_SPEED_FS,
				    &sample_fs_config);
	if (err) {
		LOG_ERR("Failed to add Full-Speed configuration");
		return NULL;
	}

	err = usbd_register_all_classes(&sample_usbd, USBD_SPEED_FS, 1, NULL);
	if (err) {
		LOG_ERR("Failed to add register classes");
		return NULL;
	}

	sample_fix_code_triple(&sample_usbd, USBD_SPEED_FS);
	usbd_self_powered(&sample_usbd, attributes & USB_SCD_SELF_POWERED);

	err = usbd_init(&sample_usbd);
	if (err) {
		LOG_ERR("Failed to initialize device support");
		return NULL;
	}

	err = usbd_enable(&sample_usbd);
	if (err) {
		LOG_ERR("Failed to enable device support");
		return NULL;
	}

	LOG_INF("USB device initialized and enabled");
	return &sample_usbd;
}
#endif /* CONFIG_ZIGBEE_NCP_USB */

#define VENDOR_SPECIFIC_LED DK_LED2

#define VENDOR_SPECIFIC_LED_ACTION_OFF (0U)
#define VENDOR_SPECIFIC_LED_ACTION_ON (1U)
#define VENDOR_SPECIFIC_LED_ACTION_TOGGLE (2U)

#define VENDOR_SPECIFIC_REQUEST_LEN (1U)
#define VENDOR_SPECIFIC_RESPONSE_LEN (1U)

#define VENDOR_SPECIFIC_IND_LEN (1U)
#define VENDOR_SPECIFIC_IND_DELAY (ZB_TIME_ONE_SECOND * 3)


/* The state of a led controlled by ncp custom commands */
static zb_uint8_t vendor_specific_led_state = VENDOR_SPECIFIC_LED_ACTION_OFF;


zb_ret_t zb_osif_bootloader_run_after_reboot(void)
{
#if DT_NODE_EXISTS(DT_ALIAS(rst0))
	int err = 0;
	const struct gpio_dt_spec rst0 = GPIO_DT_SPEC_GET(DT_ALIAS(rst0), gpios);

	if (!device_is_ready(rst0.port)) {
		return RET_ERROR;
	}

	err = gpio_pin_configure_dt(&rst0, GPIO_OUTPUT_ACTIVE);
	if (err) {
		return RET_ERROR;
	}
#endif
	return RET_OK;
}

void zb_osif_bootloader_report_successful_loading(void)
{
#if CONFIG_BOOTLOADER_MCUBOOT
	if (!boot_is_img_confirmed()) {
		int ret = boot_write_img_confirmed();

		if (ret) {
			LOG_ERR("Couldn't confirm image: %d", ret);
		} else {
			LOG_INF("Marked image as OK");
		}
	}
#endif
}

static void custom_indication(zb_uint8_t buf, zb_uint16_t led_idx)
{
	zb_uint8_t *ind_data = zb_buf_initial_alloc(buf, VENDOR_SPECIFIC_IND_LEN);

	*ind_data = (zb_uint8_t)led_idx;

	zb_ncp_custom_indication(buf);
}

static void perform_custom_indication(zb_uint8_t led_idx)
{
	zb_buf_get_out_delayed_ext(custom_indication, led_idx, 0);
}

#if (defined ZBOSS_PLATFORM_MAJOR) && (ZBOSS_PLATFORM_MAJOR < 5U)
static zb_ret_t ncp_vendor_specific_req_handler(zb_uint8_t buf)
#else /* (defined ZBOSS_PLATFORM_MAJOR) && (ZBOSS_PLATFORM_MAJOR < 5U) */
static zb_uint16_t ncp_vendor_specific_req_handler(zb_uint8_t buf)
#endif /* (defined ZBOSS_PLATFORM_MAJOR) && (ZBOSS_PLATFORM_MAJOR < 5U) */
{
	/* request tsn */
	zb_uint8_t tsn = *ZB_BUF_GET_PARAM(buf, zb_uint8_t);
	/* actual payload passed by the request */
	zb_uint8_t *led_action = (zb_uint8_t *)zb_buf_begin(buf);

	zb_uint8_t resp_buf = zb_buf_get(ZB_FALSE, VENDOR_SPECIFIC_RESPONSE_LEN);
	zb_uint8_t *resp_data;
	ncp_hl_custom_resp_t *resp_args;

	if (resp_buf == ZB_BUF_INVALID) {
		LOG_ERR("Couldn't get buf");
		return RET_NO_MEMORY;
	}

	resp_data = zb_buf_initial_alloc(resp_buf, VENDOR_SPECIFIC_RESPONSE_LEN);
	resp_args = ZB_BUF_GET_PARAM(resp_buf, ncp_hl_custom_resp_t);

	if (zb_buf_len(buf) == VENDOR_SPECIFIC_REQUEST_LEN) {
		switch (*led_action) {
		case VENDOR_SPECIFIC_LED_ACTION_OFF:
			zb_osif_led_off(VENDOR_SPECIFIC_LED);
			resp_args->status = RET_OK;

			vendor_specific_led_state = 0;
			break;
		case VENDOR_SPECIFIC_LED_ACTION_ON:
			zb_osif_led_on(VENDOR_SPECIFIC_LED);
			resp_args->status = RET_OK;

			vendor_specific_led_state = 1;
			break;
		case VENDOR_SPECIFIC_LED_ACTION_TOGGLE:
			vendor_specific_led_state ^= 0x01;
			if (vendor_specific_led_state) {
				zb_osif_led_on(VENDOR_SPECIFIC_LED);
			} else {
				zb_osif_led_off(VENDOR_SPECIFIC_LED);
			}
			resp_args->status = RET_OK;

			break;
		default:
			resp_args->status = RET_ERROR;
			break;
		}
	} else {
		resp_args->status = RET_ERROR;
	}

	resp_args->tsn = tsn;
	*resp_data = vendor_specific_led_state;
	ZVUNUSED(zb_ncp_custom_response(resp_buf));

	return NCP_RET_LATER;
}

static void ncp_vendor_specific_init(void)
{
	zb_osif_led_button_init();

	zb_ncp_custom_register_request_cb(ncp_vendor_specific_req_handler);

	ZB_SCHEDULE_APP_ALARM(perform_custom_indication, (zb_uint8_t)VENDOR_SPECIFIC_LED,
						  VENDOR_SPECIFIC_IND_DELAY);
}

int main(void)
{
	LOG_INF("Starting Zigbee R23 Network Co-processor sample");

#if defined(CONFIG_ZIGBEE_NCP_USB)
	/* Initialize USB device */
	if (setup_usb_device() == NULL) {
		LOG_ERR("Failed to setup USB device");
		return -1;
	}
#endif

	zb_osif_ncp_set_nvram_filter();

	/* Setup ncp custom command handling */
	ncp_vendor_specific_init();

	/* Start Zigbee default thread */
	zigbee_enable();

	LOG_INF("Zigbee R23 Network Co-processor sample started");

	return 0;
}
