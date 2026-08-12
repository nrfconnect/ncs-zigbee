.. _ug_zigbee_configuring:

Configuring |addon|
###################

.. contents::
   :local:
   :depth: 2

This page describes what is needed to start working with Zigbee using the |addon| for the |NCS|.

.. _zigbee_ug_libs:

Required libraries and drivers
******************************

Zigbee requires the following modules:

* :ref:`zigbee_zboss`, with the :ref:`lib_zigbee_osif` subsystem acting as the linking layer between the ZBOSS stack and the |NCS|.
  The ZBOSS library is enabled by the ``CONFIG_ZIGBEE_ADD_ON`` Kconfig option.
  For more information about the ZBOSS stack, see also the `external ZBOSS development guide and API documentation`_.
* `IEEE 802.15.4`_ radio driver - This library is automatically enabled when working with Zigbee on Nordic Semiconductor's development kits.

.. _zigbee_ug_configuration:

Mandatory configuration
***********************

To use the Zigbee protocol, set the ``CONFIG_ZIGBEE_ADD_ON`` Kconfig option.
Setting this option enables all the peripherals required for the correct operation of the Zigbee protocol and allows you to use them.

After that, you have to define the Zigbee device role for the Zigbee application or sample by setting one of the following Kconfig options:

* Router role: ``CONFIG_ZIGBEE_ROLE_ROUTER``
* End Device role: ``CONFIG_ZIGBEE_ROLE_END_DEVICE``
* Coordinator role: ``CONFIG_ZIGBEE_ROLE_COORDINATOR``

Setting any of these options enables the respective ZBOSS role library.
This is needed because End Devices use different libraries than Routers and Coordinators.

For instructions about how to set Kconfig options, see `Configuring and building`_ in the |NCS| documentation.

.. _zigbee_ug_sed:

Sleepy End Device behavior
==========================

The Sleepy End Device (SED) behavior is a Zigbee stack feature that enables the sleepy behavior for the end device.

By default, the end device regularly polls its parent for data.
When the SED behavior is enabled and no frames are available for reception after the last poll, the SED disables its radio until the next scheduled poll.
The Zigbee stack's own scheduler informs the application about periods of time when nothing is scheduled for any of the device roles.
This allows the stack to enter the sleep state during these periods, which also powers off some peripherals for the SED.

When the Zigbee stack thread goes to sleep, the Zigbee thread can enter the suspend state for the same amount of time as the stack's sleep.
The thread will be automatically resumed after the sleep period is over or on an event.

For this feature to work, make sure to call the :c:func:`zb_set_rx_on_when_idle` ZBOSS API with the ``ZB_FALSE`` parameter before starting the ZBOSS stack.
This function specifies whether the device should have the radio disabled between the polls to parent.
This feature does not require enabling any additional options in Kconfig.

In the :ref:`Zigbee light switch sample <zigbee_light_switch_sample>` in the |NCS|, after you enable the SED behavior extension, the sleepy behavior can be triggered by pressing a predefined button when the device is booting.
This action results in calling the ZBOSS API that activates this feature.

Power saving during sleep
-------------------------

With the sleepy behavior enabled, the unused part of RAM memory is powered off, which allows to lower the power consumption even more.
The sleep current of MCU can be lowered by completing the following steps:

1. Turn off UART by setting the ``CONFIG_SERIAL`` Kconfig option to ``n``.
#. Enable the ``CONFIG_RAM_POWER_DOWN_LIBRARY`` Kconfig option.

Optional configuration
**********************

After enabling the Zigbee protocol and defining the Zigbee device role, you can enable additional options in Kconfig and modify `ZBOSS stack start options`_.

.. _zigbee_ug_r22_behavior:

Zigbee PRO core runtime behavior (R22 or R23 mode)
==================================================

The ZBOSS stack in the |addon| implements Zigbee PRO R23, but CSA requires Zigbee 3.0 products to run the core in R22 behavior mode during product certification.
See :ref:`zboss_certification_r22_mode` for the certification background.

By default, the add-on selects R22 mode (``CONFIG_ZIGBEE_PRO_CORE_BEHAVIOR_R22``).
When you call :c:func:`zigbee_enable`, the OSIF layer invokes :c:func:`zboss_use_r22_behavior` before the ZBOSS thread starts.
The stack then uses classic R22 joining (association and transport-key flow) instead of native R23 behavior (Network Commissioning followed by Dynamic Link Key negotiation).

To keep native R23 behavior, select R23 mode (``CONFIG_ZIGBEE_PRO_CORE_BEHAVIOR_R23``) in the Zigbee PRO core runtime behavior Kconfig choice.
Use R23 mode only for evaluation of R23-specific features; it is not suitable for Zigbee 3.0 product certification.

This Kconfig choice is not available when building with ``CONFIG_ZIGBEE_LIBRARY_NCP_DEV`` (NCP development libraries).

For the underlying ZBOSS API, see `Runtime switch of R23 ZBOSS into R22 mode`_ in the ZBOSS API documentation.

Device operational channel
==========================

You can enable one of the following alternative options to select the channel on which the Zigbee device can operate:

  * ``CONFIG_ZIGBEE_CHANNEL_SELECTION_MODE_SINGLE`` - Single mode is enabled by default.
    The default channel is set to 16.
    To set a different channel, edit the ``CONFIG_ZIGBEE_CHANNEL`` Kconfig option to the desired value.
  * ``CONFIG_ZIGBEE_CHANNEL_SELECTION_MODE_MULTI`` - In this mode, you get all the channels enabled by default.
    To configure a custom set of channels in the range from 11 to 26, edit the ``CONFIG_ZIGBEE_CHANNEL_MASK`` Kconfig option.
    For example, you can set channels 13, 16, and 21.
    You must have at least one channel enabled with this option.

.. _ug_zigbee_configuring_eui64:

IEEE 802.15.4 EUI-64 configuration
==================================

An IEEE EUI-64 address consists of two parts:

* Company ID, which is a 24-bit MAC Address Block Large (MA-L), formerly called Organizationally Unique Identifier (OUI).
* Extension identifier, which is a 40-bit device unique identifier.

You can configure the EUI-64 for a Zigbee device in the following ways:

.. tabs::

   .. tab:: Use the default

      By default, the company ID is set to Nordic Semiconductor's MA-L (``f4-ce-36``) through the ``CONFIG_ZIGBEE_VENDOR_OUI`` Kconfig option.
      The extension identifier is set to the DEVICEID from the factory information configuration registers (FICR).

   .. tab:: Replace the company ID

      Change the ``CONFIG_ZIGBEE_VENDOR_OUI`` Kconfig option to your company's MA-L value (24-bit, in hex).
      The extension identifier remains the DEVICEID from FICR.

   .. tab:: Replace the full EUI-64

      You can provide the full EUI-64 value by programming certain user information configuration registers (UICR).
      The nRF52 and nRF53 Series devices use the CUSTOMER registers block, while the nRF53 (application core) and nRF54L Series use the OTP registers block.

      To use the EUI-64 value from the UICR you must enable the ``CONFIG_ZIGBEE_UICR_EUI64_ENABLE`` Kconfig option. 
      Then, you need to the ``CONFIG_ZIGBEE_UICR_EUI64_REG`` to the base index of the two consecutive UICR registers that will contain your EUI-64 value.

      The following example shows how to replace the full EUI-64 on the nRF52840 device:

      1. Enable the ``CONFIG_ZIGBEE_UICR_EUI64_ENABLE`` Kconfig option.

      #. Set ``CONFIG_ZIGBEE_UICR_EUI64_REG`` to the UICR offset.
         For UICR->CUSTOMER[0] and UICR->CUSTOMER[1], use the default value ``0``.

      #. Build and program your application, erasing the whole memory.
         Replace *serial_number* with the serial number of your debugger:

         .. parsed-literal::
            :class: highlight

            west build -b nrf52840dk/nrf52840 -p always
            west flash --snr *serial_number* --erase

      #. Program the two consecutive UICR registers with your EUI-64 value (replace *serial_number* with the serial number of your debugger).
         For nRF52840 device with default ``CONFIG_ZIGBEE_UICR_EUI64_REG`` (0), the CUSTOMER base address is ``0x10001080``:

         .. parsed-literal::
            :class: highlight

            nrfutil device x-write --serial-number *serial_number* --address 0x10001080 --value 0x11223344
            nrfutil device x-write --serial-number *serial_number* --address 0x10001084 --value 0x55667788
            nrfutil device reset --reset-kind=RESET_PIN

         If you set ``CONFIG_ZIGBEE_UICR_EUI64_REG`` to a different value, use the corresponding register addresses for your SoC.

At the end of the configuration process, you can check the EUI-64 value using :ref:`lib_zigbee_shell`:

.. code-block:: console

   uart:~$ zdo eui64
   8877665544332211
   Done

.. note::
    Alternatively, you may use the Production Configuration feature to change the address.
    The Production Configuration takes precedence over the OSIF configuration.

ZBOSS stack start options
=========================

Zigbee is initialized after Zephyr's kernel start.
The ZBOSS stack can be started using one of the following options:

* Started and executed from the main thread, as `described in the ZBOSS development guide <Stack commissioning start sequence_>`_.
* Started from a dedicated Zephyr thread, which in turn can be created and started by calling :c:func:`zigbee_enable`.

The dedicated thread can be configured using the following Kconfig options:

* ``CONFIG_ZBOSS_DEFAULT_THREAD_PRIORITY`` - Defines thread priority; set to ``3`` by default.
* ``CONFIG_ZBOSS_DEFAULT_THREAD_STACK_SIZE`` - Defines the size of the thread stack; set to ``2048`` by default.

.. _zigbee_ug_logging:

Custom logging per module
=========================

Logging is handled with the ``CONFIG_LOG`` Kconfig option.
This option enables logging for both the stack and Zephyr's `Logging`_ API.

.. _zigbee_ug_logging_application_logs:

Default Zigbee application logging
----------------------------------

The Zigbee application uses the ``INF`` logging level by default.
This level can be changed only by modifying the sample code.

.. _zigbee_ug_logging_stack_logs:

Stack logs
----------

The stack logs are independent from Zephyr's `Logging`_ API.
To customize them, use the following Kconfig options:

* ``CONFIG_ZBOSS_ERROR_PRINT_TO_LOG`` - Allows the application to log ZBOSS error names; enabled by default.
* ``CONFIG_ZBOSS_TRACE_MASK`` - Sets the modules from which ZBOSS will log the debug messages with ``CONFIG_ZBOSS_TRACE_LOG_LEVEL``; no module is set by default.
* ``CONFIG_ZBOSS_TRAF_DUMP`` - Enables logging of the received 802.15.4 frames over ZBOSS trace log if ``CONFIG_ZBOSS_TRACE_LOG_LEVEL`` is set; disabled by default.

The stack logs are provided in a binary format.
You can configure how they are printed with the following Kconfig options:

* ``CONFIG_ZBOSS_TRACE_HEXDUMP_LOGGING`` - Stack logs are printed as hexdump using Zephyr's `Logging`_ API.
  This option is enabled by default.
* ``CONFIG_ZBOSS_TRACE_BINARY_LOGGING`` - Stack logs are printed in the binary format using one of the following independent serial backends of your choice:

  * ``CONFIG_ZBOSS_TRACE_UART_LOGGING`` - UART serial.
    This backend is enabled by default.

  To specify the serial device, you need to set the ``ncs,zboss-trace-uart`` choice in devicetree like this:

  .. code-block:: devicetree

     chosen {
         ncs,zboss-trace-uart = &uart1;
     };

..
  * ``CONFIG_ZBOSS_TRACE_BINARY_NCP_TRANSPORT_LOGGING`` - Stack logs are printed in the binary format using the NCP transport channel.

..
  Stack logs are stored in the internal buffer if they are printed using Zephyr's `Logging`_ API or the independent serial backend.
  You can customize the buffer size with the ``CONFIG_ZBOSS_TRACE_LOGGER_BUFFER_SIZE`` Kconfig option.
  The buffer size must be larger than ``256`` and smaller than ``2147483648``

..
  If NCP transport channel is used, stack logs are stored in the buffer used for NCP transport.
  This buffer's size can be configured with the ``CONFIG_ZIGBEE_UART_TX_BUF_LEN`` Kconfig option.

.. _zigbee_ug_logging_logger_options:

Zephyr's logger options
-----------------------

You can configure custom logger options for each Zigbee and ZBOSS module.
To do this, configure the related Kconfig option for one or more modules:

* ``CONFIG_ZBOSS_TRACE_LOG_LEVEL``
* ``CONFIG_ZBOSS_OSIF_LOG_LEVEL``
* ``CONFIG_ZIGBEE_SHELL_LOG_LEVEL``
* ``CONFIG_ZIGBEE_APP_UTILS_LOG_LEVEL_CHOICE``
* ``CONFIG_ZIGBEE_LOGGER_EP_LOG_LEVEL_CHOICE``
* ``CONFIG_ZIGBEE_SCENES_LOG_LEVEL``

For each of the modules, you can set the following logging options:

* ``LOG_LEVEL_OFF`` - Turns off logging for this module.
* ``LOG_LEVEL_ERR`` - Enables logging only for errors.
* ``LOG_LEVEL_WRN`` - Enables logging for errors and warnings.
* ``LOG_LEVEL_INF`` - Enables logging for informational messages, errors, and warnings.
* ``LOG_LEVEL_DBG`` - Enables logging for debug messages, informational messages, errors, and warnings.

For example, setting ``CONFIG_ZBOSS_TRACE_LOG_LEVEL_INF`` will enable logging of informational messages, errors, and warnings for the ZBOSS Trace module.

Reduced power consumption
=========================

You can reduce the amount of power used by your device by enabling the `RAM power-down`_ library.
This library is also used for `Power saving during sleep`_.


.. _zigbee_ug_static_partition:

Upgrading Zigbee application
****************************

When upgrading the Zigbee application, use Zephyr's devicetree-based flash partitioning to ensure that ZBOSS' NVRAM is placed in the same area of flash.
This is because enabling additional features (for example, Zephyr's `Non-Volatile Storage (NVS)`_) can change the placement of the partition in the flash and the ZBOSS settings can be lost, as the application is not able to find the partition.

The Partition Manager is deprecated in the |NCS|.
New Zigbee designs and migrated applications should define fixed flash partitions in devicetree instead of using :file:`pm_static*.yml` files.
See `Migrating partition configuration from Partition Manager to devicetree (DTS)`_ in the |NCS| documentation for the general migration workflow.

When Partition Manager is disabled (``SB_CONFIG_PARTITION_MANAGER=n``), the Zigbee add-on requires the following devicetree partition nodes:

* ``zboss_nvram`` — Stores ZBOSS non-volatile settings.
* ``zboss_product_config`` — Stores ZBOSS production configuration (if production config is enabled).

The add-on provides base partition :file:`.dtsi` files in the :file:`dts/` directory of the |addon| repository (for example, :file:`nrf5340_cpuapp_partitions.dtsi` and :file:`nrf54lm20_cpuapp_partitions.dtsi`).
Include the appropriate file from a board-specific devicetree overlay (for example, :file:`boards/<board_target>.overlay`) and adjust partition sizes and addresses as needed.
Samples that use MCUboot also provide matching overlays under :file:`sysbuild/mcuboot/boards/`.

If you are migrating from an existing Partition Manager configuration, use the :file:`scripts/pm_to_dts.py` helper script from the |NCS| to generate devicetree overlays from a configured build directory.
After migration, remove the corresponding :file:`pm_static*.yml` files from your project.

.. note::
   If devices are already deployed in the field, the partition addresses and sizes in the new devicetree overlays must match the layout previously defined in the Partition Manager static configuration.
   Mismatched partition addresses break backwards compatibility and can prevent DFU from working between old and new firmware images.

The fixed partition layout is required regardless of the application version and the upgrading method (:ref:`lib_zigbee_fota` or `Secure bootloader chain`).
