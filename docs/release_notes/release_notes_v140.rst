.. _release_notes_v140:
.. _zigbee_release:

Release notes for |addon| v1.4.0
#################################

The following release is :ref:`supported <nrf:software_maturity>`.

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
