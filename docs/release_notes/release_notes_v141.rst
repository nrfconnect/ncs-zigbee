.. _release_notes_v141:

Release notes for |addon| v1.4.1
#################################

.. contents::
   :local:
   :depth: 2

This release has :ref:`supported <nrf:software_maturity>` maturity status.
It is the first release based on |NCS| v3.4.2 and ZBOSS v4.2.7.1, both of which are long-term support (LTS) releases.
See :ref:`nRF Connect SDK LTS releases <nrf:lts_releases>` for scope and maintenance.
This provides a stable base that will receive security and critical fixes in the future.

Changelog
*********

The following updates were introduced in this release.

* Added:

  * Support for the |NCS| v3.4.2.
  * The new ZBOSS R23 v4.2.7.1.
    See :ref:`zboss_stack_updates_4271` for the full list of stack fixes and enhancements.
  * The ZBOSS NCP Host package v3.8.0.

* Updated:

  * Improved ZBOSS NVRAM storage by batching flash writes into fewer operations.
    Cache size is controlled by ``CONFIG_ZIGBEE_NVRAM_WRITE_CACHE_SIZE``.
  * Default thread stack sizes for add-on applications: ``CONFIG_MAIN_STACK_SIZE`` 1280,
    ``CONFIG_ZBOSS_DEFAULT_THREAD_STACK_SIZE`` 3328, and ``CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE`` 1024.
    ``CONFIG_ZIGBEE_BT_DFU`` still uses a larger system workqueue for SMP.
  * Removed the stack memory requirements section from the :ref:`Memory requirements <zigbee_memory>` page.

* Removed:

  * Experimental ``CONFIG_ZIGBEE_GP_CB`` (Green Power Combo Basic) and ``libgpcb`` linking when building ZBOSS from sources.
    Zigbee Green Power on routers and coordinators continues to use ``CONFIG_ZIGBEE_GP_PB`` (``libgppb``), which is the default and matches shipped prebuilt libraries.

Migration notes
***************

On nRF52 and nRF53 DKs, the :ref:`zigbee_light_bulb_sample` uses updated LED assignments compared to |addon| v1.4.0:

* Default build — **LED 2** shows dimmable light level (was **LED 4**).
* FOTA build (``prj_fota.conf``) — **LED 4** indicates FOTA activity (was **LED 2**).
