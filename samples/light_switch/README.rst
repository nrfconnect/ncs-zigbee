.. _zigbee_light_switch_sample:

Zigbee: Light switch
####################

.. contents::
   :local:
   :depth: 2

You can use the this Light switch sample to change the state of light sources on other devices within the same Zigbee network.

You can use it together with the :ref:`Zigbee Network coordinator <zigbee_network_coordinator_sample>` and the :ref:`Zigbee Light bulb <zigbee_light_bulb_sample>` samples to set up a basic Zigbee network.

This sample supports the optional `Sleepy End Device behavior`_ and :ref:`zigbee_light_switch_sample_nus`.
It also supports :ref:`lib_zigbee_fota` for nRF52840, nRF54L15 and nRF54L10 SoCs.
See :ref:`zigbee_light_switch_activating_variants` for details about how to enable these variants.

Requirements
************

The sample supports the following development kits:

.. include:: /includes/device_table_single_multi.txt

You can use one or more of the development kits listed above and mix different development kits.

To test this sample, you also need to program the following samples:

* The :ref:`Zigbee Network coordinator <zigbee_network_coordinator_sample>` sample on one separate device.
* The :ref:`zigbee_light_bulb_sample` sample on one or more separate devices.

Multiprotocol Bluetooth LE extension requirements
=================================================

If you enable the :ref:`zigbee_light_switch_sample_nus`, make sure you have a phone or a tablet with the `nRF Toolbox`_ application installed.

.. note::
    The `Testing`_ instructions refer to nRF Toolbox, but you can also use similar applications, for example `nRF Connect for Mobile`_.

Overview
********

The Light switch sample demonstrates the Zigbee End Device role and implements the Dimmer Switch device specification, as defined in the Zigbee Home Automation public application profile.

Once the light switch is successfully commissioned, it sends a broadcast message to find devices with the implemented Level Control and On/Off clusters.
The light switch remembers the device network address from the first response.
At this point, you can start using the buttons on the development kit to control the clusters on the newly found devices.

Additionally, the light switch sample powers down unused RAM sections to lower power consumption in the sleep state.

Sleepy End Device behavior
==========================

The light switch supports the :ref:`zigbee_ug_sed` that enables the sleepy behavior for the end device, for a significant conservation of energy.

.. tabs::

   .. group-tab:: nRF54L15 DK

      To enable the sleepy behavior, press **Button 2** while the light switch sample is booting.
   .. group-tab:: nRF52840 DK

      To enable the sleepy behavior, press **Button 3** while the light switch sample is booting.

This is required only when device is joining the network for the first time.
After restarting the device, it will boot with the sleepy behavior enabled.

.. _zigbee_light_switch_sample_nus:

Multiprotocol Bluetooth LE extension
====================================

This optional extension demonstrates dynamic concurrent switching between two protocols, Bluetooth® LE and Zigbee.
It uses the `Nordic UART Service (NUS)`_ library.

When this extension is enabled, you can use:

* Buttons on the light switch device to operate on the Zigbee network
* Nordic UART Service to operate on the Bluetooth LE network

Both networks are independent from each other.

To support both protocols at the same time, the Zigbee stack uses the `IEEE 802.15.4`_ radio during the inactive time of the Bluetooth LE radio (using the Timeslot API of the `Multiprotocol Service Layer`_).
Depending on the Bluetooth LE connection interval, the nRF52 development kits can spend up to 99% of the radio time on the Zigbee protocol.

Transmitting and receiving data when using this example does not break connection from any of the used radio protocols, either Bluetooth LE or Zigbee.

For more information about the multiprotocol feature, see `Multiprotocol support`_ in the |NCS| documentation.

.. _zigbee_light_switch_configuration:

Configuration
*************

|config|

Source file setup
=================

This sample is split into the following source files:

* The :file:`main` file to handle initialization and light switch basic behavior.
* An additional :file:`nus_cmd` file for handling NUS commands.

.. _zigbee_light_switch_activating_variants:

Configuration files for sample extensions
=========================================

The sample provides predefined configuration files for optional extensions.
You can find the configuration files in the :file:`samples/light_switch` directory.

Activating optional extensions
------------------------------

To activate the :ref:`lib_zigbee_fota`, use the :file:`prj_fota.conf` configuration file.
For example, when building from the command line, use the following command:

.. parsed-literal::
   :class: highlight

   west build samples/light_switch -b  *board_target* -- -DFILE_SUFFIX=fota

Alternatively, you can :ref:`configure Zigbee FOTA manually <ug_zigbee_configuring_components_ota>`.

.. note::
   You can use the :file:`prj_fota.conf` file only with a development kit that contains the nRF52840, nRF54L15 or nRF54L10 SoC.

To activate the Multiprotocol Bluetooth LE extension, set :makevar:`EXTRA_CONF_FILE` to the :file:`overlay-multiprotocol_ble.conf`.
For example, when building from the command line, use the following command:

.. parsed-literal::
   :class: highlight

   west build samples/light_switch -b *board_target* -- -DEXTRA_CONF_FILE='overlay-multiprotocol_ble.conf'

For the board name to use instead of the ``board_target``, see `Programming board names`_.

See `Providing CMake options`_ in the |NCS| documentation for instructions on how to add flags to your build.
For more information about configuration files in the |NCS|, see `Build and configuration system`_ in the |NCS| documentation.

..
  FEM support
  ===========

  .. include:: /includes/sample_fem_support.txt

Configurable transmission power
===============================

To achieve a lower power consumption of the light switch, you can configure the transmission power using the ``CONFIG_LIGHT_SWITCH_CONFIGURE_TX_POWER`` Kconfig option.
You can select per-channel transmission power (in dBm) with the ``CONFIG_LIGHT_SWITCH_TX_POWER`` Kconfig option.
This affects to all frames sent by the device, even in the network scan phase.

.. note::
    The ``CONFIG_LIGHT_SWITCH_CONFIGURE_TX_POWER`` Kconfig option is enabled by default and this sample's transmission power is set to 0 dBm.

When the ``CONFIG_ZIGBEE_CHANNEL_SELECTION_MODE_MULTI`` Kconfig option is set to ``y``, the ``CONFIG_ZIGBEE_APP_CB_QUEUE_LENGTH`` Kconfig option must be increased depending on the channel mask.
For example, in case 16 channels are active, a proper value for ``CONFIG_ZIGBEE_APP_CB_QUEUE_LENGTH`` would be ``17``.
Similar consideration applies to the ``ZB_CONFIG_IOBUF_POOL_SIZE`` and ``ZB_CONFIG_SCHEDULER_Q_SIZE`` values configured in the :file:`include/zb_mem_config_custom.h` file.

.. _zigbee_light_switch_user_interface:

User interface
**************

.. tabs::

   .. group-tab:: nRF54L15 DK

      LED 2:
          Lit and solid when the device is connected to a Zigbee network.

      LED 3:
          Lit and solid when the light switch has found a light bulb to control.

      Button 0:
          Turn on the light bulb connected to the network (light bulb's **LED 1**).
          This option is available after the successful commissioning (light switch's **LED 2** turned on).

          Pressing this button for a longer period of time increases the brightness of the **LED 1** of the connected light bulb.

      Button 1:
          Turn off the light bulb connected to the network (light bulb's **LED 1**).
          This option is available after the successful commissioning (light switch's **LED 2** turned on).

          Pressing this button for a longer period of time decreases the brightness of the **LED 1** of the connected light bulb.

      Button 3:
          When pressed for five seconds, it initiates the `factory reset of the device <Resetting to factory defaults_>`_.
          The length of the button press can be edited using the ``CONFIG_FACTORY_RESET_PRESS_TIME_SECONDS`` Kconfig option from :ref:`lib_zigbee_application_utilities`.
          Releasing the button within this time does not trigger the factory reset procedure.

   .. group-tab:: nRF52840 DK

      LED 3:
          Lit and solid when the device is connected to a Zigbee network.

      LED 4:
          Lit and solid when the light switch has found a light bulb to control.

      Button 1:
          Turn on the light bulb connected to the network (light bulb's **LED 4**).
          This option is available after the successful commissioning (light switch's **LED 3** turned on).

          Pressing this button for a longer period of time increases the brightness of the **LED 4** of the connected light bulb.

      Button 2:
          Turn off the light bulb connected to the network (light bulb's **LED 4**).
          This option is available after the successful commissioning (light switch's **LED 3** turned on).

          Pressing this button for a longer period of time decreases the brightness of the **LED 4** of the connected light bulb.

      Button 4:
          When pressed for five seconds, it initiates the `factory reset of the device <Resetting to factory defaults_>`_.
          The length of the button press can be edited using the ``CONFIG_FACTORY_RESET_PRESS_TIME_SECONDS`` Kconfig option from :ref:`lib_zigbee_application_utilities`.
          Releasing the button within this time does not trigger the factory reset procedure.

.. note::
    If the brightness level is at the minimum level, you may not notice the effect of turning on the light bulb.

FOTA behavior assignments
=========================

.. tabs::

   .. group-tab:: nRF54L15 DK

      LED 1:
         Indicates the OTA activity.
         Used only if the FOTA support is enabled.

   .. group-tab:: nRF52840 DK

      LED 2:
         Indicates the OTA activity.
         Used only if the FOTA support is enabled.

Sleepy End Device behavior assignments
======================================

.. tabs::

   .. group-tab:: nRF54L15 DK

      Button 2:
          When pressed while resetting the kit, enables the :ref:`zigbee_ug_sed`.

   .. group-tab:: nRF52840 DK

      Button 3:
          When pressed while resetting the kit, enables the :ref:`zigbee_ug_sed`.

Multiprotocol Bluetooth LE extension assignments
================================================

.. tabs::

   .. group-tab:: nRF54L15 DK

      LED 0:
          Lit and solid when a Bluetooth LE Central is connected to the NUS service.
          Available when using `Nordic UART Service (NUS)`_ in the multiprotocol configuration.

   .. group-tab:: nRF52840 DK

      LED 1:
          Lit and solid when a Bluetooth LE Central is connected to the NUS service.
          Available when using `Nordic UART Service (NUS)`_ in the multiprotocol configuration.

UART command assignments:
    The following command assignments are configured and used in nRF Toolbox when :ref:`zigbee_light_switch_testing_ble`:

    * ``n`` - Turn on the Zigbee Light bulb.
    * ``f`` - Turn off the Zigbee Light bulb.
    * ``t`` - Toggle the Zigbee Light bulb on or off.
    * ``i`` - Increase the brightness level of the Zigbee Light bulb.
    * ``d`` - Decrease the brightness level of the Zigbee Light bulb.

    If more than one light bulb is available in the network, these commands apply to all light bulbs in the network.
    See :ref:`zigbee_light_switch_testing_ble` for details.

Building and running
********************

.. |sample path| replace:: :file:`samples/light_switch`

|enable_zigbee_before_testing|

.. include:: /includes/build_and_run.txt

.. _zigbee_light_switch_testing:

Testing
=======

After programming the sample to your development kits, complete the following steps to test it:

.. tabs::

   .. group-tab:: nRF54L15 DK

      1. Turn on the development kit that runs the Network coordinator sample.

         When **LED 2** turns on, this development kit has become the Coordinator of the Zigbee network.

      #. Turn on the development kit that runs the Light bulb sample.

         When **LED 2** turns on, the light bulb has become a Router inside the network.

         .. note::
              If **LED 2** does not turn on, press **Button 0** on the Coordinator to reopen the network.

      #. Turn on the development kit that runs the Light switch sample.

         When **LED 2** turns on, the light switch has become an End Device, connected directly to the Coordinator.

      #. Wait until **LED 3** on the light switch node turns on.

         This LED indicates that the light switch found a light bulb to control.

   .. group-tab:: nRF52840 DK

      1. Turn on the development kit that runs the Network coordinator sample.

         When **LED 3** turns on, this development kit has become the Coordinator of the Zigbee network.

      #. Turn on the development kit that runs the Light bulb sample.

         When **LED 3** turns on, the light bulb has become a Router inside the network.

         .. note::
              If **LED 3** does not turn on, press **Button 1** on the Coordinator to reopen the network.

      #. Turn on the development kit that runs the Light switch sample.

         When **LED 3** turns on, the light switch has become an End Device, connected directly to the Coordinator.

      #. Wait until **LED 4** on the light switch node turns on.

         This LED indicates that the light switch found a light bulb to control.

You can now use buttons on the development kit to control the light bulb, as described in :ref:`zigbee_light_switch_user_interface`.

.. _zigbee_light_switch_testing_ble:

Testing multiprotocol Bluetooth LE extension
--------------------------------------------

To test the multiprotocol Bluetooth LE extension, you need to complete the standard `Testing`_ procedure, set up nRF Toolbox, and then perform the tests using nRF Toolbox.

Set up nRF Toolbox by completing the following steps:

1. Start UART.

   Tap :guilabel:`UART` to open the UART application in nRF Toolbox.

   .. figure:: /images/nrftoolbox_dynamic_zigbee_uart_1.png
      :alt: UART application in nRF Toolbox

      UART application in nRF Toolbox

#. Configure commands.

   Configure the UART commands by completing the following steps:

   a. Tap :guilabel:`EDIT` in the top right corner of the application.
      The button configuration window appears.
   #. Create the active application buttons by completing the following steps:

      i. Bind the ``n`` command to one of the buttons, with EOL set to LF and an icon of your choice.
      #. Bind the ``f`` command to one of the buttons, with EOL set to LF and an icon of your choice.
      #. Bind the ``t`` command to one of the buttons, with EOL set to LF and an icon of your choice.
      #. Bind the ``d`` command to one of the buttons, with EOL set to LF and an icon of your choice.
      #. Bind the ``i`` command to one of the buttons, with EOL set to LF and an icon of your choice.

      .. figure:: /images/nrftoolbox_dynamic_zigbee_uart_2.png
         :alt: Configuring buttons in nRF Toolbox - UART application

         Configuring buttons in the UART application of nRF Toolbox

   #. Tap :guilabel:`DONE` in the top right corner of the application.

#. Connect to the device.

   Tap :guilabel:`CONNECT` and select the **Zigbee_Switch** device from the list of devices.

   .. figure:: /images/nrftoolbox_dynamic_zigbee_uart_3.png
      :alt: nRF Toolbox - UART application view after establishing connection

      The UART application of nRF Toolbox after establishing the connection


   .. tabs::
      .. group-tab:: nRF54L15 DK
	  
         Observe that **LED 0** on the light switch node is solid.

      .. group-tab:: nRF52840 DK
	  
         Observe that **LED 1** on the light switch node is solid.   

   This indicates that the Bluetooth LE connection is established.

In nRF Toolbox, tap the buttons you assigned to perform the test:

1. Tap the :guilabel:`n` and :guilabel:`f` command buttons to turn the LED on the Zigbee Light bulb node on and off, respectively.
#. Tap the :guilabel:`t` command button two times to toggle the LED on the Zigbee Light bulb node on and off.
#. Tap the :guilabel:`i` and :guilabel:`d` command buttons to make adjustments to the brightness level.

You can now control the devices either with the buttons on the development kits or with the NUS UART command buttons in the nRF Toolbox application.

Sample output
-------------

You can observe the sample logging output through a serial port after connecting with a terminal emulator (for example, nRF Connect Serial Terminal).
See `Testing and optimization`_ in the |NCS| documentation for the required settings and steps.

Dependencies
************

This sample uses the following |NCS| libraries:

* `RAM power-down`_
* :ref:`lib_zigbee_error_handler`
* :ref:`lib_zigbee_application_utilities`
* Zigbee subsystem:

  * :file:`zb_nrf_platform.h`

* `DK Buttons and LEDs`_

It uses the ZBOSS stack:

* :ref:`zigbee_zboss` |zboss_version| (`API documentation`_)

In addition, it uses the following Zephyr libraries:

* :file:`include/zephyr.h`
* :file:`include/device.h`
* `Logging`_

The following dependencies are added by the multiprotocol Bluetooth LE extension:

* `SoftDevice Controller`_
* `Nordic UART Service (NUS)`_
* Zephyr's `Bluetooth API`_:

  * ``include/bluetooth/bluetooth.h``
  * ``include/bluetooth/gatt.h``
  * ``include/bluetooth/hci.h``
  * ``include/bluetooth/uuid.h``
  * ``include/bluetooth/services/nus.h``
