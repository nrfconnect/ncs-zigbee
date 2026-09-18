/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *
 * @brief Simple Zigbee light bulb implementation.
 */

#include "app_task_zigbee.h"

#include <soc.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#ifdef CONFIG_ZIGBEE_SCENES
#include <zephyr/settings/settings.h>
#endif
#include <zephyr/types.h>

#if defined(CONFIG_ZIGBEE_FOTA) || defined(CONFIG_ZIGBEE_BT_DFU)
#include <zephyr/dfu/mcuboot.h>
#endif

#include <ram_pwrdn.h>

extern "C" {
#include <zboss_api.h>
#include <zb_ha_dimmable_light.h>
#include <dk_buttons_and_leds.h>
#include <zb_nrf_platform.h>
#include <zboss_api.h>
#include <zboss_api_addons.h>
#include <zigbee/zigbee_app_utils.h>
#include <zigbee/zigbee_error_handler.h>
#ifdef CONFIG_ZIGBEE_SCENES
#include <zigbee/zigbee_zcl_scenes.h>
#endif
#if CONFIG_ZIGBEE_FOTA
#include <zigbee/zigbee_fota.h>
#endif
#ifdef CONFIG_ZIGBEE_BT_DFU
#include <zigbee/zigbee_bt_dfu.h>
#endif
}

#if CONFIG_ZIGBEE_FOTA
#include <zephyr/sys/reboot.h>
#endif

#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000

/* Device endpoint, used to receive light controlling commands. */
#define DIMMABLE_LIGHT_ENDPOINT 10

/* Version of the application software (1 byte). */
#define BULB_INIT_BASIC_APP_VERSION 01

/* Version of the implementation of the Zigbee stack (1 byte). */
#define BULB_INIT_BASIC_STACK_VERSION 10

/* Version of the hardware of the device (1 byte). */
#define BULB_INIT_BASIC_HW_VERSION 11

/* Manufacturer name (32 bytes). */
#define BULB_INIT_BASIC_MANUF_NAME "Nordic"

/* Model number assigned by manufacturer (32-bytes long string). */
#define BULB_INIT_BASIC_MODEL_ID "Dimable_Light_v0.1"

/* First 8 bytes specify the date of manufacturer of the device
 * in ISO 8601 format (YYYYMMDD). The rest (8 bytes) are manufacturer specific.
 */
#define BULB_INIT_BASIC_DATE_CODE "20200329"

/* Type of power sources available for the device.
 * For possible values see section 3.2.2.2.8 of ZCL specification.
 */
#define BULB_INIT_BASIC_POWER_SOURCE ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE

/* Describes the physical location of the device (16 bytes).
 * May be modified during commissioning process.
 */
#define BULB_INIT_BASIC_LOCATION_DESC "Office desk"

/* Describes the type of physical environment.
 * For possible values see section 3.2.2.2.10 of ZCL specification.
 */
#define BULB_INIT_BASIC_PH_ENV ZB_ZCL_BASIC_ENV_UNSPECIFIED

/* LED indicating that light switch successfully joind Zigbee network. */
#define ZIGBEE_NETWORK_STATE_LED DK_LED3

/* LED immitaing dimmable light bulb - define for informational
 * purposes only.
 */
#define BULB_LED DK_LED4

#if CONFIG_ZIGBEE_FOTA
#define OTA_ACTIVITY_LED DK_LED2
#endif

/* Button used to enter the Bulb into the Identify mode. */
#define IDENTIFY_MODE_BUTTON DK_BTN4_MSK

/* Use onboard led4 to act as a light bulb.
 * The app.overlay file has this at node label "pwm_led3" in /pwmleds.
 */
#define PWM_DK_LED_NODE DT_NODELABEL(pwm_led3)

#if DT_NODE_HAS_STATUS(PWM_DK_LED_NODE, okay)
static const struct pwm_dt_spec led_pwm = PWM_DT_SPEC_GET(PWM_DK_LED_NODE);
#else
#error "Choose supported PWM driver"
#endif

/* Led PWM period, calculated for 100 Hz signal - in microseconds. */
#define LED_PWM_PERIOD_US (USEC_PER_SEC / 100U)

#ifndef ZB_ROUTER_ROLE
#error Define ZB_ROUTER_ROLE to compile router source code.
#endif

/* Button to start Factory Reset */
#define FACTORY_RESET_BUTTON IDENTIFY_MODE_BUTTON

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

/* Main application customizable context.
 * Stores all settings and static values.
 */
typedef struct {
  zb_zcl_basic_attrs_ext_t basic_attr;
  zb_zcl_identify_attrs_t identify_attr;
  zb_zcl_scenes_attrs_t scenes_attr;
  zb_zcl_groups_attrs_t groups_attr;
  zb_zcl_on_off_attrs_t on_off_attr;
  zb_zcl_level_control_attrs_t level_control_attr;
} bulb_device_ctx_t;

/* Zigbee device application context storage. */
static bulb_device_ctx_t dev_ctx;

/* Last non-zero brightness used when turning on with CurrentLevel == 0. */
static zb_uint8_t s_last_on_level = ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE;

ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list,
                                    &dev_ctx.identify_attr.identify_time);

ZB_ZCL_DECLARE_GROUPS_ATTRIB_LIST(groups_attr_list,
                                  &dev_ctx.groups_attr.name_support);

ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST(scenes_attr_list,
                                  &dev_ctx.scenes_attr.scene_count,
                                  &dev_ctx.scenes_attr.current_scene,
                                  &dev_ctx.scenes_attr.current_group,
                                  &dev_ctx.scenes_attr.scene_valid,
                                  &dev_ctx.scenes_attr.name_support);

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(
    basic_attr_list, &dev_ctx.basic_attr.zcl_version,
    &dev_ctx.basic_attr.app_version, &dev_ctx.basic_attr.stack_version,
    &dev_ctx.basic_attr.hw_version, dev_ctx.basic_attr.mf_name,
    dev_ctx.basic_attr.model_id, dev_ctx.basic_attr.date_code,
    &dev_ctx.basic_attr.power_source, dev_ctx.basic_attr.location_id,
    &dev_ctx.basic_attr.ph_env, dev_ctx.basic_attr.sw_ver);

/* On/Off cluster attributes additions data */
ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST(on_off_attr_list,
                                  &dev_ctx.on_off_attr.on_off);

ZB_ZCL_DECLARE_LEVEL_CONTROL_ATTRIB_LIST(
    level_control_attr_list, &dev_ctx.level_control_attr.current_level,
    &dev_ctx.level_control_attr.remaining_time);

ZB_HA_DECLARE_DIMMABLE_LIGHT_CLUSTER_LIST(dimmable_light_clusters, basic_attr_list,
                                       identify_attr_list, groups_attr_list,
                                       scenes_attr_list, on_off_attr_list,
                                       level_control_attr_list);

ZB_HA_DECLARE_DIMMABLE_LIGHT_EP(dimmable_light_ep, DIMMABLE_LIGHT_ENDPOINT,
                             dimmable_light_clusters);

#ifndef CONFIG_ZIGBEE_FOTA
ZBOSS_DECLARE_DEVICE_CTX_1_EP(dimmable_light_ctx, dimmable_light_ep);
#else

#if DIMMABLE_LIGHT_ENDPOINT == CONFIG_ZIGBEE_FOTA_ENDPOINT
#error "Light bulb and Zigbee OTA endpoints should be different."
#endif

extern zb_af_endpoint_desc_t zigbee_fota_client_ep;
ZBOSS_DECLARE_DEVICE_CTX_2_EP(dimmable_light_ctx, zigbee_fota_client_ep,
                              dimmable_light_ep);
#endif /* CONFIG_ZIGBEE_FOTA */

/**@brief Starts identifying the device.
 *
 * @param  bufid  Unused parameter, required by ZBOSS scheduler API.
 */
static void start_identifying(zb_cb_param_t param)
{
  ZVUNUSED(param);

  if (ZB_JOINED()) {
    /* Check if endpoint is in identifying mode,
     * if not, put desired endpoint in identifying mode.
     */
    if (dev_ctx.identify_attr.identify_time ==
        ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE) {

      zb_ret_t zb_err_code =
          zb_bdb_finding_binding_target(DIMMABLE_LIGHT_ENDPOINT);

      if (zb_err_code == RET_OK) {
        LOG_INF("Enter identify mode");
      } else if (zb_err_code == RET_INVALID_STATE) {
        LOG_WRN("RET_INVALID_STATE - Cannot enter identify mode");
      } else {
        ZB_ERROR_CHECK(zb_err_code);
      }
    } else {
      LOG_INF("Cancel identify mode");
      zb_bdb_finding_binding_target_cancel();
    }
  } else {
    LOG_WRN("Device not in a network - cannot enter identify mode");
  }
}

/**@brief Callback wrapper for button events. */
static void button_changed(uint32_t button_state, uint32_t has_changed) {
  if (IDENTIFY_MODE_BUTTON & has_changed) {
    if (IDENTIFY_MODE_BUTTON & button_state) {
      /* Button changed its state to pressed */
    } else {
      /* Button changed its state to released */
      if (was_factory_reset_done()) {
        /* The long press was for Factory Reset */
        LOG_DBG("After Factory Reset - ignore button release");
      } else {
        /* Button released before Factory Reset */

        /* Start identification mode */
        ZB_SCHEDULE_APP_CALLBACK(start_identifying, 0);
      }
    }
  }

  check_factory_reset_button(button_state, has_changed);
}

/**@brief Function for initializing additional PWM leds. */
static void pwm_led_init(void) {
  if (!device_is_ready(led_pwm.dev)) {
    LOG_ERR("Error: PWM device %s is not ready", led_pwm.dev->name);
  }
}

/**@brief Function for initializing LEDs and Buttons. */
static void configure_gpio(void) {
  int err;

  err = dk_buttons_init(button_changed);
  if (err) {
    LOG_ERR("Cannot init buttons (err: %d)", err);
  }

  err = dk_leds_init();
  if (err) {
    LOG_ERR("Cannot init LEDs (err: %d)", err);
  }

  pwm_led_init();
}

/**@brief Sets brightness of bulb luminous executive element
 *
 * @param[in] brightness_level Brightness level, allowed values 0 ... 255,
 *                             0 - turn off, 255 - full brightness.
 */
static void light_bulb_set_brightness(zb_uint8_t brightness_level) {
  uint32_t pulse = brightness_level * LED_PWM_PERIOD_US / 255U;

  if (pwm_set_dt(&led_pwm, PWM_USEC(LED_PWM_PERIOD_US), PWM_USEC(pulse))) {
    LOG_ERR("Pwm led 4 set fails:\n");
    return;
  }
}

/**@brief Return PWM brightness to apply when the bulb is turned on. */
static zb_uint8_t light_bulb_on_brightness(void) {
  zb_uint8_t level = dev_ctx.level_control_attr.current_level;

  if (level == 0U) {
    level = s_last_on_level;
  }

  return level;
}

/**@brief Function for setting the light bulb brightness.
 *
 * @param[in] new_level   Light bulb brightness value.
 */
static void level_control_set_value(zb_uint16_t new_level) {
  LOG_INF("Set level value: %i", new_level);

  if (new_level > 0U) {
    s_last_on_level = new_level;
  }

  ZB_ZCL_SET_ATTRIBUTE(DIMMABLE_LIGHT_ENDPOINT, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID,
                       (zb_uint8_t *)&new_level, ZB_FALSE);

  light_bulb_set_brightness(new_level);
}

/**@brief Function for turning ON/OFF the light bulb.
 *
 * @param[in]   on   Boolean light bulb state.
 */
static void on_off_set_value(zb_bool_t on) {
  LOG_INF("Set ON/OFF value: %i", on);

  ZB_ZCL_SET_ATTRIBUTE(DIMMABLE_LIGHT_ENDPOINT, ZB_ZCL_CLUSTER_ID_ON_OFF,
                       ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                       (zb_uint8_t *)&on, ZB_FALSE);

  if (on) {
    level_control_set_value(light_bulb_on_brightness());
  } else {
    light_bulb_set_brightness(0U);
  }
}

/**@brief Function to toggle the identify LED - BULB_LED is used for this.
 *
 * @param  bufid  Unused parameter, required by ZBOSS scheduler API.
 */
static void toggle_identify_led(zb_cb_param_t param)
{
  static int blink_status;

  light_bulb_set_brightness(((++blink_status) % 2) ? (255U) : (0U));
  ZB_SCHEDULE_APP_ALARM(toggle_identify_led, param,
                        ZB_MILLISECONDS_TO_BEACON_INTERVAL(100));
}

/**@brief Function to handle identify notification events on the first endpoint.
 */
static void identify_cb(zb_cb_param_t param)
{
  zb_ret_t zb_err_code;

  if (param) {
    /* Schedule a self-scheduling function that will toggle the LED. */
    ZB_SCHEDULE_APP_CALLBACK(toggle_identify_led, param);
  } else {
    /* Cancel the toggling function alarm and restore current Zigbee LED state.
     */
    zb_err_code =
        ZB_SCHEDULE_APP_ALARM_CANCEL(toggle_identify_led, ZB_ALARM_ANY_PARAM);
    ZVUNUSED(zb_err_code);

    if (dev_ctx.on_off_attr.on_off) {
      level_control_set_value(light_bulb_on_brightness());
    } else {
      light_bulb_set_brightness(0U);
    }
  }
}

/**@brief Function for initializing all clusters attributes.
 */
static void bulb_clusters_attr_init(void) {
  /* Basic cluster attributes data */
  dev_ctx.basic_attr.zcl_version = ZB_ZCL_VERSION;
  dev_ctx.basic_attr.app_version = BULB_INIT_BASIC_APP_VERSION;
  dev_ctx.basic_attr.stack_version = BULB_INIT_BASIC_STACK_VERSION;
  dev_ctx.basic_attr.hw_version = BULB_INIT_BASIC_HW_VERSION;

  /* Use ZB_ZCL_SET_STRING_VAL to set strings, because the first byte
   * should contain string length without trailing zero.
   *
   * For example "test" string will be encoded as:
   *   [(0x4), 't', 'e', 's', 't']
   */
  ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.mf_name, BULB_INIT_BASIC_MANUF_NAME,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_MANUF_NAME));

  ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.model_id, BULB_INIT_BASIC_MODEL_ID,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_MODEL_ID));

  ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.date_code, BULB_INIT_BASIC_DATE_CODE,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_DATE_CODE));

  dev_ctx.basic_attr.power_source = BULB_INIT_BASIC_POWER_SOURCE;

  ZB_ZCL_SET_STRING_VAL(
      dev_ctx.basic_attr.location_id, BULB_INIT_BASIC_LOCATION_DESC,
      ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_LOCATION_DESC));

  dev_ctx.basic_attr.ph_env = BULB_INIT_BASIC_PH_ENV;

  /* Identify cluster attributes data. */
  dev_ctx.identify_attr.identify_time =
      ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;

  /* On/Off cluster attributes data. */
  dev_ctx.on_off_attr.on_off = (zb_bool_t)ZB_ZCL_ON_OFF_IS_OFF;

  dev_ctx.level_control_attr.current_level =
      ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE;
  dev_ctx.level_control_attr.remaining_time =
      ZB_ZCL_LEVEL_CONTROL_REMAINING_TIME_DEFAULT_VALUE;

  ZB_ZCL_SET_ATTRIBUTE(DIMMABLE_LIGHT_ENDPOINT, ZB_ZCL_CLUSTER_ID_ON_OFF,
                       ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                       (zb_uint8_t *)&dev_ctx.on_off_attr.on_off, ZB_FALSE);

  ZB_ZCL_SET_ATTRIBUTE(
      DIMMABLE_LIGHT_ENDPOINT, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
      ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID,
      (zb_uint8_t *)&dev_ctx.level_control_attr.current_level, ZB_FALSE);
}

#if defined(CONFIG_ZIGBEE_FOTA) || defined(CONFIG_ZIGBEE_BT_DFU)
static void confirm_image(void) {
  if (!boot_is_img_confirmed()) {
    int ret = boot_write_img_confirmed();

    if (ret) {
      LOG_ERR("Couldn't confirm image: %d", ret);
    } else {
      LOG_INF("Marked image as OK");
    }
  }
}
#endif

#ifdef CONFIG_ZIGBEE_FOTA
static void ota_evt_handler(const struct zigbee_fota_evt *evt) {
  switch (evt->id) {
  case ZIGBEE_FOTA_EVT_PROGRESS:
    dk_set_led(OTA_ACTIVITY_LED, evt->dl.progress % 2);
    break;

  case ZIGBEE_FOTA_EVT_FINISHED:
    LOG_INF("Reboot application.");
    if (IS_ENABLED(CONFIG_RAM_POWER_DOWN_LIBRARY)) {
      power_up_unused_ram();
    }

    sys_reboot(SYS_REBOOT_COLD);
    break;

  case ZIGBEE_FOTA_EVT_ERROR:
    LOG_ERR("OTA image transfer failed.");
    break;

  default:
    break;
  }
}
#endif /* CONFIG_ZIGBEE_FOTA */

/**@brief Callback function for handling ZCL commands.
 *
 * @param[in]   bufid   Reference to Zigbee stack buffer
 *                      used to pass received data.
 */
static void zcl_device_cb(zb_cb_param_t param)
{
  zb_bufid_t bufid = (zb_bufid_t)param;
  zb_uint8_t cluster_id;
  zb_uint8_t attr_id;
  zb_zcl_device_callback_param_t *device_cb_param =
      ZB_BUF_GET_PARAM(bufid, zb_zcl_device_callback_param_t);

  LOG_INF("%s id %hd", __func__, device_cb_param->device_cb_id);

  /* Set default response value. */
  device_cb_param->status = RET_OK;

  switch (device_cb_param->device_cb_id) {
  case ZB_ZCL_OTA_UPGRADE_VALUE_CB_ID:
#ifdef CONFIG_ZIGBEE_FOTA
    zigbee_fota_zcl_cb(bufid);
#else
    device_cb_param->status = RET_NOT_IMPLEMENTED;
#endif
    break;

  case ZB_ZCL_LEVEL_CONTROL_SET_VALUE_CB_ID:
    LOG_INF("Level control setting to %d",
            device_cb_param->cb_param.level_control_set_value_param.new_value);
    level_control_set_value(
        device_cb_param->cb_param.level_control_set_value_param.new_value);
    break;

  case ZB_ZCL_SET_ATTR_VALUE_CB_ID:
    cluster_id = device_cb_param->cb_param.set_attr_value_param.cluster_id;
    attr_id = device_cb_param->cb_param.set_attr_value_param.attr_id;

    if (cluster_id == ZB_ZCL_CLUSTER_ID_ON_OFF) {
      uint8_t value =
          device_cb_param->cb_param.set_attr_value_param.values.data8;

      LOG_INF("on/off attribute setting to %hd", value);
      if (attr_id == ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID) {
        on_off_set_value((zb_bool_t)value);
      }
    } else if (cluster_id == ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL) {
      uint16_t value =
          device_cb_param->cb_param.set_attr_value_param.values.data16;

      LOG_INF("level control attribute setting to %hd", value);
      if (attr_id == ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID) {
        level_control_set_value(value);
      }
    } else {
      /* Other clusters can be processed here */
      LOG_INF("Unhandled cluster attribute id: %d", cluster_id);
      device_cb_param->status = RET_NOT_IMPLEMENTED;
    }
    break;

  default:
#ifdef CONFIG_ZIGBEE_SCENES
    if (zcl_scenes_cb(bufid) == ZB_FALSE) {
      device_cb_param->status = RET_NOT_IMPLEMENTED;
    }
#else
    device_cb_param->status = RET_NOT_IMPLEMENTED;
#endif
    break;
  }

  LOG_INF("%s status: %hd", __func__, device_cb_param->status);
}

/**@brief Zigbee stack event handler.
 *
 * @param[in]   bufid   Reference to the Zigbee stack buffer
 *                      used to pass signal.
 */
void zboss_signal_handler(zb_bufid_t bufid) {
#ifdef CONFIG_ZIGBEE_FOTA
  zigbee_fota_signal_handler(bufid);
#endif

  /* Update network status LED. */
  zigbee_led_status_update(bufid, ZIGBEE_NETWORK_STATE_LED);

#if defined(CONFIG_ZIGBEE_TOUCHLINK_TARGET)
  zigbee_touchlink_target_signal_handler(bufid);
#endif

  ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));

  /* All callbacks should either reuse or free passed buffers.
   * If bufid == 0, the buffer is invalid (not passed).
   */
  if (bufid) {
    zb_buf_free(bufid);
  }
}

extern "C" int ZigbeeStart(void) {
  int blink_status = 0;
#ifdef CONFIG_ZIGBEE_SCENES
  int err;
#endif

  LOG_INF("Starting Zigbee R23 Light Bulb application");

  configure_gpio();
#ifdef CONFIG_ZIGBEE_SCENES
  err = settings_subsys_init();
  if (err) {
    LOG_ERR("settings initialization failed");
  }
#endif
  register_factory_reset_button(FACTORY_RESET_BUTTON);

  /* Register callback for handling ZCL commands. */
  ZB_ZCL_REGISTER_DEVICE_CB(zcl_device_cb);

  /* Register dimmer switch device context (endpoints). */
  ZB_AF_REGISTER_DEVICE_CTX(&dimmable_light_ctx);

  bulb_clusters_attr_init();

  if (dev_ctx.on_off_attr.on_off) {
    level_control_set_value(dev_ctx.level_control_attr.current_level);
  } else {
    /* Keep CurrentLevel at its default; only drive PWM off. */
    light_bulb_set_brightness(0U);
  }

#if defined(CONFIG_ZIGBEE_FOTA) || defined(CONFIG_ZIGBEE_BT_DFU)
  confirm_image();
#endif

#ifdef CONFIG_ZIGBEE_FOTA
  zigbee_fota_init(ota_evt_handler);
#endif

#ifdef CONFIG_ZIGBEE_BT_DFU
  zigbee_bt_dfu_init();
#endif

  /* Register handler to identify notifications. */
  ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(DIMMABLE_LIGHT_ENDPOINT, identify_cb);
#ifdef CONFIG_ZIGBEE_FOTA
  ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(CONFIG_ZIGBEE_FOTA_ENDPOINT, identify_cb);
#endif

#ifdef CONFIG_ZIGBEE_SCENES
  /* Initialize ZCL scene table */
  zcl_scenes_init();

  /* Settings should be loaded after zcl_scenes_init */
  err = settings_load();
  if (err) {
    LOG_ERR("settings loading failed");
  }
#endif

  /* Start Zigbee default thread */
  zigbee_enable();

  LOG_INF("Zigbee R23 Light Bulb application started");

  while (1) {
    dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
    k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
  }

  return 0;
}
