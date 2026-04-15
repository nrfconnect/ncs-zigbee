.. _known_issues:

Known issues
############

.. contents::
   :local:
   :depth: 3

Known issues listed on this page *and* tagged with the :ref:`latest release version <release_notes>` are valid for the current state of development.
Use the drop-down filter to see known issues for previous releases and check if they are still valid.

A known issue can list one or both of the following entries:

* **Affected platforms:**

  If a known issue does not have any specific platforms listed, it is valid for all hardware platforms.

* **Workaround:**

  Some known issues have a workaround.
  Sometimes, they are discovered later and added over time.

.. version-filter::
  :default: v1-3-0
  :container: dl/dt
  :tags: [("wontfix", "Won't fix")]

.. page-filter::
  :name: issues

  wontfix    Won't fix

List of known issues
********************

.. rst-class:: v1-2-1 v1-2-0 v1-1-0 v1-0-0

KRKNWK-21267: NCP crashes when migrating from the R22 to R23 firmware
  After migrating the ZBOSS firmware from R22 to R23, the Network Co-Processor (NCP) crashes during the migration of application datasets.
  This issue occurs only when the host application uses application‑specific datasets (:c:macro:`ZB_NVRAM_APP_DATA1` to :c:macro:`ZB_NVRAM_APP_DATA4` types).

.. rst-class:: v1-3-0 v1-2-1 v1-2-0 v1-1-0 v1-0-0

KRKNWK-21057: Poll Control cluster values overwritten to defaults on reboot
  After a sleepy end device (SED) reboots, the Poll Control cluster attributes are not restored to the values set before the reboot.
  Instead, they are reverted to their default values.

  **Workaround:** Restore the cluster attributes in the application signal handler, inside :c:macro:`ZB_BDB_SIGNAL_DEVICE_REBOOT` case:

  .. code-block:: C

    case ZB_BDB_SIGNAL_DEVICE_REBOOT:
      /* Apply Poll Control values loaded from NVRAM to the PIM (Poll Interval Manager) */
      {
        zb_uint8_t endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
        if (endpoint) {
          zb_zcl_attr_t *attr_desc;
          
          attr_desc = zb_zcl_get_attr_desc_a(endpoint,
            ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE,
            ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID);

          if (attr_desc) {
            zb_uint32_t long_poll = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
            zb_zdo_pim_set_long_poll_interval(ZB_QUARTERECONDS_TO_MSEC(long_poll));
          }
          
          attr_desc = zb_zcl_get_attr_desc_a(endpoint,
            ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE,
            ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID);

          if (attr_desc) {
            zb_uint16_t short_poll = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
            zb_zdo_pim_set_fast_poll_interval(ZB_QUARTERECONDS_TO_MSEC(short_poll));
          }

          attr_desc = zb_zcl_get_attr_desc_a(endpoint,
            ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE,
            ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_TIMEOUT_ID);

          if (attr_desc) {
            zb_uint16_t fast_poll_timeout = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
            zb_zdo_pim_set_fast_poll_timeout(ZB_QUARTERECONDS_TO_MSEC(fast_poll_timeout));
          }
        }
      }
    /* fall-through */

.. rst-class:: v1-2-1 v1-2-0 v1-1-0 v1-0-0

KRKNWK-20726: Too frequent keep-alive messages from SED to parent
  When using the Light Switch sample application with a sleepy end device (SED), the SED sends End Device Timeout Requests to its parent too often if the parent supports the End Device Timeout Request Keepalive method.
  This increases SED power consumption compared to using only the MAC Data Poll Keepalive method, because requests are longer and End Device Timeout Responses must be received.

  **Workaround:** Use :c:func:`zb_zdo_pim_set_long_poll_interval` rather than :c:func:`zb_set_keepalive_timeout` to set the long poll interval, or manually cherry-pick and apply commit with the fix from ``main`` (commit hash: ``aa9763a8b1296baab2f302a5816215df3e4523b7``).

.. rst-class:: v1-3-0 v1-2-1 v1-2-0 v1-1-0 v1-0-0

KRKNWK-21014: Increased power consumption after leaving network
  After a SED leaves the network and starts scanning for networks again, it does not go back to sleep as expected, leading to increased power consumption.

  **Workaround:** In the application signal handler, at the end of :c:macro:`ZB_BDB_SIGNAL_STEERING`, :c:macro:`ZB_BDB_SIGNAL_TC_REJOIN_DONE`, and :c:macro:`ZB_BDB_SIGNAL_DEVICE_REBOOT` cases, add:

  .. code-block:: C

    ZB_TRANSCEIVER_SET_RX_ON_OFF(zb_get_rx_on_when_idle());

.. rst-class:: v1-3-0 v1-2-1 v1-2-0 v1-1-0 v1-0-0

NCSIDB-1336: Zigbee Router device cannot rejoin after missing Network Key update or rotation
  If a Zigbee Router device does not receive Network Key update or rotation messages (for example, while resetting or powered off), it may fall out of sync with the Trust Center and continue using old keys.

  Section 4.3.6.1 of the Zigbee Core Specification states that a device which has missed a network key update may use procedures to obtain the latest key. 
  That optional wording means the stack is not required to recover automatically.
  In addition, BDB 3.0 does not describe a procedure a Zigbee Router can use to verify that its security keys are still valid.

  **Workaround:** Extend the Router application to handle :c:macro:`ZB_NLME_STATUS_INDICATION` when the status is ``ZB_NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER``.
  Do not initiate a rejoin immediately after the first such status, because that could help an attacker force a rejoin without knowing the network key.
  After several indications (for example, five), check whether the device is still connected to the network.
  Do this by using the :c:func:`zb_zdo_simple_desc_req` function.
  After several indications (for example, five), check whether the device is still connected to the network, for example using :c:func:`zb_zdo_simple_desc_req`.
  If the returned message status is not ``ZB_ZDP_STATUS_SUCCESS``, start rejoin by calling :c:func:`zb_bdb_initiate_tc_rejoin`.
  The device can then obtain the current key and rejoin whether the network is open or closed.

  Place the logic in ``zboss_signal_handler()`` (inside the ``switch`` over the signal type), for example:

  .. code-block:: C
     :caption: Excerpt inside ``zboss_signal_handler()``

     case ZB_NLME_STATUS_INDICATION: {
       zb_zdo_signal_nlme_status_indication_params_t *nlme_status_ind =
         ZB_ZDO_SIGNAL_GET_PARAMS(sig_hdr, zb_zdo_signal_nlme_status_indication_params_t);

       if (nlme_status_ind->nlme_status.status ==
           ZB_NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER) {
         /* Count indications; after a threshold, verify connectivity, then
          * zb_bdb_initiate_tc_rejoin if needed. */
       }
       break;
     }

.. rst-class:: v1-3-0 v1-2-1 v1-2-0 v1-1-0 v1-0-0

KRKNWK-12115: Simultaneous commissioning of many devices can cause the Coordinator device to assert
  The Zigbee Coordinator can assert when multiple devices are being commissioned at the same time.
  In some cases, the device can end up in a low memory state.

  The :ref:`Zigbee network coordinator <zigbee_network_coordinator_sample>` sample includes :file:`zb_mem_config_max.h` by default, which already selects the coordinator-oriented configurable-memory profile, but the derived scheduler queue and buffer pool sizes can still be too small for very bursty join traffic.

  **Workaround:** Where possible, reduce parallel commissioning. 
  If you still hit asserts or allocation failures, increase the coordinator's scheduler queue and buffer pool:

  #. Add a custom memory configuration header to your coordinator application (for layout, follow the :ref:`Light switch <zigbee_light_switch_sample>` sample and its :file:`samples/light_switch/include/zb_mem_config_custom.h` file).
  #. Populate that header starting from the contents of :file:`zb_mem_config_max.h`, then override the limits as needed (see :file:`zb_mem_config_common.h` for how the base profile maps to numeric sizes).
  #. In the custom header, after the block that includes :file:`zb_mem_config_common.h`, add overrides such as:

     .. code-block:: c

        /* Increase scheduler queue depth (allowed range 48U–256U). */
        #undef ZB_CONFIG_SCHEDULER_Q_SIZE
        #define ZB_CONFIG_SCHEDULER_Q_SIZE 96U
        /* Increase IOBUF pool (allowed range 48U–127U). */
        #undef ZB_CONFIG_IOBUF_POOL_SIZE
        #define ZB_CONFIG_IOBUF_POOL_SIZE 64U

  #. In the custom header, after the block that includes :file:`zb_mem_config_common.h`, add overrides such as:
  #. In the coordinator :file:`main.c`, replace ``#include <zb_mem_config_max.h>`` with ``#include "zb_mem_config_custom.h"`` (or your header name).

  Rebuild and re-check static RAM usage (see :ref:`zigbee_memory`). 
  Higher pools increase footprint.

.. rst-class:: v1-3-0 v1-2-1 v1-2-0 v1-1-0 v1-0-0

KRKNWK-7723: OTA upgrade process restarting after client reset
  If the device resets while the ZCL OTA Upgrade Client is downloading an image, the transfer typically starts again from the beginning instead of continuing at the previous offset.

  Download progress depends on volatile stack client state as well as OTA attributes. 
  That state is not preserved across a chip reset in the default configuration.
