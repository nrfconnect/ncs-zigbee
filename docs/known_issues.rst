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
  :default: v1-2-1
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

.. rst-class:: v1-2-1 v1-2-0 v1-1-0 v1-0-0

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

.. rst-class:: v1-2-1 v1-2-0 v1-1-0 v1-0-0

KRKNWK-21014: Increased power consumption after leaving network
  After a SED leaves the network and starts scanning for networks again, it does not go back to sleep as expected, leading to increased power consumption.

  **Workaround:** In the application signal handler, at the end of :c:macro:`ZB_BDB_SIGNAL_STEERING`, :c:macro:`ZB_BDB_SIGNAL_TC_REJOIN_DONE`, and :c:macro:`ZB_BDB_SIGNAL_DEVICE_REBOOT` cases, add:

  .. code-block:: C

    ZB_TRANSCEIVER_SET_RX_ON_OFF(zb_get_rx_on_when_idle());
