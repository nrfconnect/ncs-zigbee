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
