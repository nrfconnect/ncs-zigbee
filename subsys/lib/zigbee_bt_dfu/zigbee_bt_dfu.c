/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zigbee/zigbee_bt_dfu.h>

#if !defined(CONFIG_MCUMGR_TRANSPORT_BT)
#error "DFU over SMP requires MCUmgr Bluetooth LE module config enabled"
#endif

#if (!defined(CONFIG_MCUMGR_GRP_IMG) || !defined(CONFIG_MCUMGR_GRP_OS))
#error "DFU over SMP requires MCUmgr IMG and OS groups"
#endif

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/dfu/mcuboot.h>
#include <zephyr/logging/log.h>
#include <zephyr/mgmt/mcumgr/grp/img_mgmt/img_mgmt.h>
#include <zephyr/mgmt/mcumgr/mgmt/callbacks.h>
#include <zephyr/mgmt/mcumgr/mgmt/mgmt_defines.h>

#define BT_ADV_INT_MIN 400
#define BT_ADV_INT_MAX 500

LOG_MODULE_REGISTER(zigbee_bt_dfu, CONFIG_ZIGBEE_BT_DFU_LOG_LEVEL);

static struct k_work advertise_work;

static enum mgmt_cb_return
upload_confirm_handler(uint32_t event, enum mgmt_cb_return prev_status,
                       int32_t *rc, uint16_t *group, bool *abort_more,
                       void *data, size_t data_size) {
  const struct img_mgmt_upload_check *img_data = data;

  if (data == NULL) {
    return MGMT_CB_OK;
  }

  LOG_INF("DFU over SMP progress: %d/%llu B of image %d", img_data->req->off,
          img_data->action->size, img_data->req->image);

  return MGMT_CB_OK;
}

static enum mgmt_cb_return command_handler(uint32_t event, enum mgmt_cb_return,
                                           int32_t *, uint16_t *, bool *,
                                           void *, size_t) {
  return MGMT_CB_OK;
}

static enum mgmt_cb_return dfu_stopped_handler(uint32_t, enum mgmt_cb_return,
                                               int32_t *, uint16_t *, bool *,
                                               void *, size_t) {
  return MGMT_CB_OK;
}

static struct mgmt_callback upload_confirm_callback = {
    .callback = upload_confirm_handler,
    .event_id = MGMT_EVT_OP_IMG_MGMT_DFU_CHUNK,
};

static struct mgmt_callback command_callback = {
    .callback = command_handler,
    .event_id = (MGMT_EVT_OP_CMD_RECV | MGMT_EVT_OP_CMD_DONE),
};

static struct mgmt_callback dfu_stopped_callback = {
    .callback = dfu_stopped_handler,
    .event_id =
        (MGMT_EVT_OP_IMG_MGMT_DFU_STOPPED | MGMT_EVT_OP_IMG_MGMT_DFU_PENDING),
};

void zigbee_bt_dfu_disconnected(struct bt_conn *conn, uint8_t reason) {
  LOG_INF("zigbee_bt_dfu_disconnected");

  ARG_UNUSED(conn);
  ARG_UNUSED(reason);

  k_work_submit(&advertise_work);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .disconnected = zigbee_bt_dfu_disconnected,
};

static const struct bt_data advertise_data[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
            sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

static const struct bt_data service_data[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
            sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

static void advertise(struct k_work *work) {
  struct bt_le_adv_param params = BT_LE_ADV_PARAM_INIT(
      BT_LE_ADV_OPT_CONN, BT_ADV_INT_MIN, BT_ADV_INT_MAX, NULL);
  params.id = 0;

  int result =
      bt_le_adv_start(&params, advertise_data, ARRAY_SIZE(advertise_data),
                      service_data, ARRAY_SIZE(service_data));

  if (result != 0) {
    LOG_ERR("Advertising failed to start (rc %d)", result);
    return;
  }

  LOG_INF("Bluetooth LE advertising successfully started");
}

void zigbee_bt_dfu_init(void) {

  mgmt_callback_register(&upload_confirm_callback);
  mgmt_callback_register(&command_callback);
  mgmt_callback_register(&dfu_stopped_callback);

  k_work_init(&advertise_work, advertise);
  int result = bt_enable(NULL);

  if (result != 0) {
    LOG_ERR("Bluetooth enable failed: %d", result);
    return;
  }

  k_work_submit(&advertise_work);
}
