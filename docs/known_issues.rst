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

..
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
