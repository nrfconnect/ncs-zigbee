.. _release_notes:

Release notes
#############

.. contents::
   :local:
   :depth: 2

All the notable changes to the |addon| for the |NCS| are listed here.
See also the `Release notes for the nRF Connect SDK`_ and the `ZBOSS R23 changelog`_.

The |addon| v\ |addon_version| is compatible with |NCS| v\ |ncs_version| and uses the ZBOSS stack version |zboss_version|.
For a full list of |addon| releases, related |NCS| and ZBOSS stack and NCP host package versions, view the following table:

+-------------------+------------------+-----------------------+---------------------+
| |addon| version   | |NCS| version    | ZBOSS stack version   | NCP host version    |
+===================+==================+=======================+=====================+
| 2.0.0             | 3.4.1            | 5.0.5.1               | N/A                 |
+-------------------+------------------+-----------------------+---------------------+
| 1.4.0             | 3.4.0            | 4.2.2.5               | 3.7.0               |
+-------------------+------------------+-----------------------+---------------------+
| 1.3.0             | 2.9.2            | 4.2.2.4               | 3.6.0               |
+-------------------+                  +-----------------------+---------------------+
| 1.2.1             |                  | 4.2.2.3               | 3.5.0               |
+-------------------+------------------+-----------------------+---------------------+
| 1.2.0             | 2.9.0            | 4.2.2.0               | 3.4.0               |
+-------------------+                  +                       +---------------------+
| 1.1.0             |                  |                       | 3.3.0               |
+-------------------+                  +-----------------------+---------------------+
| 1.0.0             |                  | 4.2.0.0               | 3.2.0               |
+-------------------+                  +-----------------------+---------------------+
| 0.4.0             |                  | 4.1.14.0              | 3.1.0               |
+-------------------+                  +-----------------------+---------------------+
| 0.3.1             |                  | 4.1.4.2               | 3.0.1               | 
+-------------------+                  |                       +                     |
| 0.3.0             |                  |                       |                     | 
+-------------------+------------------+                       +---------------------+
| 0.2.0             | 2.8.0            |                       | 3.0.0               | 
+-------------------+                  |                       +---------------------+
| 0.1.0             |                  |                       | N/A                 | 
+-------------------+------------------+-----------------------+---------------------+

|addon| v2.0.0 - TBD
********************

The following release is `supported <Software maturity levels_>`_.

* Added:

  * Zigbee 4.0 compatible ZBOSS stack v5.0.5.1, replacing the ZBOSS 4.2 prebuilt libraries.
    See the `ZBOSS 5 changelog`_ and :ref:`migration_zboss5`.
  * PSA-backed cryptography integration in the OSIF layer, with Settings initialized before :c:func:`psa_crypto_init` and ZMS-backed PSA storage on nRF54 RRAM SoCs when enabled.
  * Support for the nRF54LM20 Dongle in the :ref:`zigbee_light_bulb_sample`, :ref:`zigbee_light_switch_sample`, and :ref:`zigbee_shell_sample` samples.
  * Experimental support for the nRF54LC10 DK (nRF54LC10A SoC) in the :ref:`zigbee_samples`.
  * The ``CONFIG_ZIGBEE_DEMO_COMMISSIONING`` Kconfig option, which lets a Coordinator accept devices without an install code (insecure, for demonstrations and testing only).
  * The ``zigbee_demo_commissioning`` snippet, which enables the ``CONFIG_ZIGBEE_DEMO_COMMISSIONING`` Kconfig option.
  * The :ref:`ug_zigbee_tools_production_config` script, which generates the ZBOSS production configuration of a device, including its install code or passcode from the product label or QR code.
  * The :ref:`zigbee_commissioning_security` section describing the Trust Center security policy.

* Updated:

  * The OSIF layer, application libraries, Zigbee Shell, and :ref:`zigbee_samples` for ZBOSS 5 APIs, including unified :c:type:`zb_cb_param_t` delayed callbacks and BDB channel list handling.
    Thread-safe scheduler wrappers are documented in :ref:`zigbee_osif_thread_safe_scheduler`.
  * The default Zigbee PRO core runtime behavior to R23 mode (``CONFIG_ZIGBEE_PRO_CORE_BEHAVIOR_R23``), which is required for Zigbee 4.0 product certification.
    R22 mode (``CONFIG_ZIGBEE_PRO_CORE_BEHAVIOR_R22``) remains available for Zigbee 3.0 product certification.
  * The :ref:`bdb_ic_add` and :ref:`bdb_ic_set` shell commands to accept 128-bit install codes without their CRC, and Zigbee R23 PAKE passcodes as setup codes or hexadecimal strings.
    The type is detected from the length of the value, and values of other lengths are rejected.
  * Improved ZBOSS NVRAM storage by batching flash writes into fewer operations.
    Cache size is controlled by ``CONFIG_ZIGBEE_NVRAM_WRITE_CACHE_SIZE``.
  * Renamed the FOTA snippets from ``zigbee_fota_ext_flash`` and ``zigbee_fota_mcuboot_ext_flash`` to ``zigbee_fota`` and ``zigbee_fota_mcuboot``.
    One application snippet still covers all supported FOTA boards; board overlays select ``dts/ext_flash/`` or ``dts/fota_internal/`` as needed.
  * Corrected LED assignments for the :ref:`zigbee_light_bulb_sample` and user interface references in the documentation.
  * Default thread stack sizes for add-on applications: ``CONFIG_MAIN_STACK_SIZE`` 1280,
    ``CONFIG_ZBOSS_DEFAULT_THREAD_STACK_SIZE`` 3328, and ``CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE`` 1024.
    ``CONFIG_ZIGBEE_BT_DFU`` still uses a larger system workqueue for SMP.
  * Removed the stack memory requirements section from the :ref:`Memory requirements <zigbee_memory>` page.

* Removed:

  * Precompiled NCP development libraries (``libncp-dev``) and a matching ZBOSS NCP Host package release.
    The :ref:`NCP sample <zigbee_ncp_sample>` cannot be built in this release; see :ref:`known_issues` (KRKNWK-22663).

* Migration notes:

  * Follow :ref:`migration_zboss5` when upgrading from |addon| v1.4.x (ZBOSS 4.2).
  * FOTA builds must pass ``SNIPPET=zigbee_fota`` instead of ``zigbee_fota_ext_flash``.
    Sysbuild still remaps the MCUboot image to ``zigbee_fota_mcuboot`` automatically.
  * Applications that target Zigbee 3.0 product certification must now explicitly set ``CONFIG_ZIGBEE_PRO_CORE_BEHAVIOR_R22`` to ``y``.
  * Coordinators now require install codes for all joining devices by default, following the ZBOSS 5 high-security Trust Center policy.
    Add the install code of each device on the Trust Center before it joins (see :ref:`zigbee_install_code_commissioning`), or build demonstration setups with the ``zigbee_demo_commissioning`` snippet (see :ref:`zigbee_commissioning_security`).
  * In the :ref:`zigbee_light_bulb_sample` on nRF52 and nRF53 DKs, use LED 2 instead of LED 4 for the dimmable light output.
    In FOTA builds of the :ref:`zigbee_light_bulb_sample`, use LED 4 instead of LED 2 for FOTA activity.

.. _zigbee_release:

|addon| v1.4.0 - 04/08/2026
***************************

The following release is `supported <Software maturity levels_>`_.
 
* Added:
 
  * Support for the nRF Connect SDK v3.4.0.
  * The new ZBOSS R23 v4.2.2.5.
  * The ZBOSS NCP Host package v3.7.0.
  * Support for the nRF54LM20A and nRF54LM20B SoCs.
  * A feature to resume FOTA updates after an interrupted download.
  * A unified header file for Zigbee settings subsystem.
  * A snippet to support external flash usage for the samples.
  * Support for DFU over Bluetooth LE SMP.
    You can optionally enable it for the light switch and light bulb samples.
  * Kconfig choice to select Zigbee PRO core runtime behavior (R22 or R23 mode).
    R22 mode is enabled by default and is required for Zigbee 3.0 product certification on R23 platforms.
    The add-on calls :c:func:`zboss_use_r22_behavior` from :c:func:`zigbee_enable` when R22 mode is selected.
  * Experimental support for Touchlink initiator and target roles.

* Updated:

  * Regular ZBOSS libraries (non-trace) are now built with LTO enabled by default.
  * Memory partitioning for the :ref:`zigbee_samples` to use Devicetree (DTS) instead of Partition Manager, which is disabled by default.
    A ``storage_partition`` for Zephyr settings is included in every sample variant for consistency, even when most samples do not use it.
  * The light switch sample to use MCUboot image compression feature for FOTA updates.
    This implies that the device cannot revert to the previous image after the update is applied.
  * Improved network rejoin reliability in the Zigbee application utils.
  * The :ref:`Memory requirements <zigbee_memory>` page with a new memory usage visualization.
    Per-board bar charts and tables show reserved partition sizes from DTS and used versus free space from the build output.

* Migration notes:

  * When using the regular ZBOSS libraries (non-trace), you need to enable LTO in your application by setting the ``CONFIG_LTO`` Kconfig option to ``y``.
  * The settings key for Zigbee scenes extension has been shortened.
    To keep backward compatibility, you can set back the old key value in ``include/zigbee/zigbee_settings_subsys.h``.
  * Flash partition layouts have changed due to migration to DTS-based memory partitioning and, in the light switch sample, MCUboot image compression for FOTA updates.
    This breaks backward compatibility with previous sample and application builds that relied on the old Partition Manager layouts.
    To perform DFU between software versions, bring back the previous partition layout in your devicetree overlays so that partition addresses and sizes match the deployed firmware.


|addon| v1.3.0 - 23/02/2026
***************************

The following release is `supported <Software maturity levels_>`_.
 
* Added:
 
  * Support for the nRF52833 SoC.
  * The new ZBOSS R23 v4.2.2.4.
  * The ZBOSS NCP Host package v3.6.0.

* Updated:

  * Several documentation improvements and clarifications, including a known issues page and a migration guide from R22 to R23.
  * Adjusted keepalive mechanism in :ref:`zigbee_light_switch_sample` sample.
  * The content of the ``File Version`` field format from OTA Header.
  * Fixed the TC rejoin mechanism after a failed attempt.

|addon| v1.2.1 - 10/10/2025
***************************

The following release is `supported <Software maturity levels_>`_.
 
* Added:

  * Support for the nRF Connect SDK v2.9.2.
  * The new ZBOSS R23 v4.2.2.3.
  * The ZBOSS NCP Host package v3.5.0 with improved NCP host application.
  
* Updated:

   * The CMSA-CA parameters to improve joining performance in dense networks.
   * The documentation to reflect the changes.

|addon| v1.2.0 - 21/08/2025
***************************

The following release is `supported <Software maturity levels_>`_.
 
* Added:

  * Support for the nRF5340 SoC.
  * The ZBOSS NCP Host package v3.4.0.

* Updated the documentation to reflect the changes.

|addon| v1.1.0 - 08/08/2025
***************************

The following release is `supported <Software maturity levels_>`_.
 
* Added:
 
  * A fix for the nRF54L10 memory layout.
  * CCA ED threshold for nRF54L and nRF52840 SoCs adjusted to correspond to -75dBm.
  * The new ZBOSS R23 v4.2.2.0.
  * The ZBOSS NCP Host package v3.3.0.

* Updated the documentation to reflect the changes.

|addon| v1.0.0 - 07/07/2025
***************************

The following release is `supported <Software maturity levels_>`_.
 
* Added:
 
  * Production-ready support for the nRF52840, nRF54L15, nRF54L10, and nRF54L05 SoCs.
    However, the released Zigbee libraries are not certified.
  * FOTA support for the nRF52840, nRF54L15, and nRF54L10 SoCs, with an example of usage provided in the :ref:`Light switch <zigbee_light_switch_sample>` sample.
  * The new ZBOSS R23 v4.2.0.0.
  * The ZBOSS NCP Host package v3.2.0.

* Updated the documentation to reflect the changes.

|addon| v0.4.0 - 31/03/2025
***************************

This is an `experimental <Software maturity levels_>`_ release.
 
* Added:
 
  * Experimental support for the ZBOSS R23 stack for the nRF52840 SoC.
    Released Zigbee libraries are not certified.
  * The new ZBOSS R23 v4.1.14.0.
  * The ZBOSS NCP Host package v3.1.0.

* Updated the documentation with information regarding the nRF52840 SoC support.

|addon| v0.3.1 - 14/01/2025
***************************

This is an `experimental <Software maturity levels_>`_ release.
 
* Added:
 
  * Fix ZCL common function.

|addon| v0.3.0 - 10/01/2025
***************************

This is an `experimental <Software maturity levels_>`_ release.
 
* Added:
 
  * Experimental support for the ZBOSS R23 stack for the nRF54L10 and nRF54L05 SoCs.
    Released Zigbee libraries are not certified.
  * The ZBOSS NCP Host package v3.0.1.
  * Flash and RAM memory usage optimization.

* Updated the documentation with nRF54L10 and RF54L05 support.

|addon| v0.2.0 - 28/11/2024
***************************

This is an `experimental <Software maturity levels_>`_ release.
 
* Added:

  * The :ref:`NCP <zigbee_ncp_sample>` sample.
  * The ZBOSS NCP Host package v3.0.0.

* Updated the documentation with small improvements.

|addon| v0.1.0 - 15/11/2024
***************************

Initial release.

* Added:

  * Experimental support for the ZBOSS R23 stack for the nRF54L15 SoC.
    Released Zigbee library is not certified.
  * Basic samples:

    * :ref:`Light bulb <zigbee_light_bulb_sample>`
    * :ref:`Light switch <zigbee_light_switch_sample>`
    * :ref:`Network coordinator <zigbee_network_coordinator_sample>`
    * :ref:`Shell <zigbee_shell_sample>`
    * :ref:`Template <zigbee_template_sample>`
