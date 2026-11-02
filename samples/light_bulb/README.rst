.. _zigbee_light_bulb_sample:

Zigbee: Light bulb
##################

.. contents::
   :local:
   :depth: 2

This sample demonstrates a simple light bulb whose brightness can be adjusted by another device.

You can use this sample with the :ref:`Zigbee Network coordinator <zigbee_network_coordinator_sample>` and :ref:`Zigbee Light switch <zigbee_light_switch_sample>` samples to set up a basic Zigbee network.

Requirements
************

The sample supports the following hardware platforms:

.. include:: /includes/device_table_single_multi.txt

..
  You can use one or more of the development kits listed above and mix different development kits.

To test this sample, you also need to program the following samples:

* The :ref:`Zigbee Network coordinator <zigbee_network_coordinator_sample>` sample on one separate device.
* The :ref:`Zigbee Light switch <zigbee_light_switch_sample>` sample on one or more separate devices.

Overview
********

The Zigbee light bulb sample takes the :ref:`Zigbee Router <zigbee_roles>` role and implements the Dimmable Light device specification, as defined in the Zigbee Home Automation public application profile.
This profile allows changing the brightness level of the light bulb's LED.


Configuration
*************

|config|

Source file setup
=================

This sample is split into the following source files:

* The :file:`main` file is the application entry point only.
* The :file:`app_task_zigbee` file manages the application task flow, user input handling, and Zigbee-specific startup and control logic.
  It also implements the dimmable light device behavior, including cluster handling and PWM-based brightness control.
  On boards without a devicetree PWM LED (for example the nRF54LC10 DK), the sample uses the DK LED library GPIO API instead of PWM.

..
  FEM support
  ===========

  .. include:: /includes/sample_fem_support.txt

.. _zigbee_light_bulb_activating_variants:

Configuration files for sample extensions
=========================================

The sample provides predefined configuration files for optional extensions.
You can find the configuration files in the :file:`samples/light_bulb` directory.

.. |sample activating variants ref| replace:: :ref:`zigbee_light_bulb_activating_variants`
.. |sample zigbee bt dfu testing ref| replace:: :ref:`zigbee_light_bulb_testing_zigbee_bt_dfu`
.. |sample dir| replace:: samples/light_bulb

.. include:: /includes/zigbee_bt_dfu_activation.txt

.. _zigbee_light_bulb_touchlink:

Touchlink variant
-----------------

Enable the ``CONFIG_ZIGBEE_TOUCHLINK_TARGET`` Kconfig option when building the light bulb sample.
The device acts as a Touchlink target: after network steering fails on a factory-new device, it automatically opens a Touchlink target window so a nearby initiator (the :ref:`zigbee_light_switch_sample` built with ``CONFIG_ZIGBEE_TOUCHLINK_INITIATOR=y``) can commission it without a Zigbee Coordinator.

.. note::
   Touchlink in the |addon| for the |NCS| is provided as an experimental feature with basic functionality.

.. note::
   Touchlink is not supported on the nRF54LM20 Dongle due to the limited number of buttons.
   The dongle has only one button, which is used for Identify mode and factory reset.

User interface
**************

.. tabs::

   .. group-tab:: nRF54L15, nRF54L10, and nRF54LM20 DKs

      LED 0:
          Blinks to indicate that the main application thread is running.

      LED 1:
          Indicates the dimmable light option, that is changes to the light bulb brightness.
          It can be controlled by another Zigbee device in the network, for example a light switch.
          Blinks when the light bulb is in Identify mode.

      LED 2:
          Turns on when the light bulb joins the network.

      Button 3:
          Depending on how long the button is pressed:

          * If pressed for less than five seconds, it starts or cancels the Identify mode.
          * If pressed for five seconds, it initiates the `factory reset of the device <Resetting to factory defaults_>`_.
            The length of the button press can be edited using the ``CONFIG_FACTORY_RESET_PRESS_TIME_SECONDS`` Kconfig option from :ref:`lib_zigbee_application_utilities`.
            Releasing the button within this time does not trigger the factory reset procedure.

   .. group-tab:: nRF54LC10 DK

      The sample still implements a Zigbee Dimmable Light (Level Control cluster, brightness 0–255 over the air).
      This development kit has no devicetree PWM LED, so the firmware drives the indicator with GPIO: the LED is **on** when the current level is greater than 0 and **off** at level 0.
      A light switch can still send gradual level commands, but you will not see PWM fading on the DK (unlike nRF54L15, nRF54L10, and nRF54LM20 DK builds).

      LED 0:
          Blinks to indicate that the main application thread is running.

      LED 1:
          Indicates the simplified dimmable-light state described above (same position as on the **nRF54L15, nRF54L10, and nRF54LM20 DKs** tab).
          It can be controlled by another Zigbee device in the network, for example a light switch.
          Blinks when the light bulb is in Identify mode.

      LED 2:
          Turns on when the light bulb joins the network.

      Button 3:
          Depending on how long the button is pressed:

          * If pressed for less than five seconds, it starts or cancels the Identify mode.
          * If pressed for five seconds, it initiates the `factory reset of the device <Resetting to factory defaults_>`_.
            The length of the button press can be edited using the ``CONFIG_FACTORY_RESET_PRESS_TIME_SECONDS`` Kconfig option from :ref:`lib_zigbee_application_utilities`.
            Releasing the button within this time does not trigger the factory reset procedure.

   .. group-tab:: nRF54LM20 Dongle

      Red LED 1:
          Blinks to indicate that the main application thread is running.

      Blue LED 1:
          Turns on when the light bulb joins the network.

      Red LED 2:
          Indicates changes to the light bulb brightness.
          It can be controlled by another Zigbee device in the network, for example a light switch.
          Blinks when the light bulb is in Identify mode.

      Button 1:
          Depending on how long the button is pressed:

          * If pressed for less than five seconds, it starts or cancels the Identify mode.
          * If pressed for five seconds, it initiates the `factory reset of the device <Resetting to factory defaults_>`_.
            The length of the button press can be edited using the ``CONFIG_FACTORY_RESET_PRESS_TIME_SECONDS`` Kconfig option from :ref:`lib_zigbee_application_utilities`.
            Releasing the button within this time does not trigger the factory reset procedure.

   .. group-tab:: nRF52 and nRF53 DKs

      LED 1:
          Blinks to indicate that the main application thread is running.

      LED 3:
          Turns on when the light bulb joins the network.

      LED 2:
          Indicates the dimmable light option, that is changes to the light bulb brightness.
          It can be controlled by another Zigbee device in the network, for example a light switch.
          Blinks when the light bulb is in Identify mode.

      Button 4:
          Depending on how long the button is pressed:

          * If pressed for less than five seconds, it starts or cancels the Identify mode.
          * If pressed for five seconds, it initiates the `factory reset of the device <Resetting to factory defaults_>`_.
            The length of the button press can be edited using the ``CONFIG_FACTORY_RESET_PRESS_TIME_SECONDS`` Kconfig option from :ref:`lib_zigbee_application_utilities`.
            Releasing the button within this time does not trigger the factory reset procedure.

Building and running
********************
.. |sample path| replace:: :file:`samples/light_bulb`

|enable_zigbee_before_testing|

.. include:: /includes/build_and_run.txt

.. _zigbee_light_bulb_sample_testing:

Testing
=======

Build the :ref:`Network coordinator <zigbee_network_coordinator_sample>` sample with the ``zigbee_demo_commissioning`` snippet, so that it accepts devices without an install code (see :ref:`zigbee_commissioning_security`).
To join with an install code instead, see :ref:`zigbee_install_code_commissioning`.

After programming the samples to your devices, complete the following steps to test them:

.. tabs::

   .. group-tab:: nRF54L15, nRF54L10, and nRF54LM20 DKs

      1. Turn on the development kit that runs the Network coordinator sample.

         When **LED 2** turns on, this development kit has become the Coordinator of the Zigbee network and the network is established.

      #. Turn on the development kit that runs the Light bulb sample.

         When **LED 2** turns on, the light bulb has become a Router inside the network.

         .. note::
            If **LED 2** does not turn on, press **Button 0** on the Coordinator to reopen the network.

      #. Turn on the development kit that runs the Light switch sample.

         When **LED 2** turns on, the light switch has become an End Device, connected directly to the Coordinator.

      #. Wait until **LED 3** on the development kit that runs the Light switch sample turns on.

         This LED indicates that the switch found a light bulb to control.

      #. Use the buttons on the development kit that runs the :ref:`zigbee_light_switch_sample` sample to control the light bulb.

         The result of using the buttons is reflected on the light bulb's **LED 1**.

   .. group-tab:: nRF54LC10 DK

      Follow the steps in the **nRF54L15, nRF54L10, and nRF54LM20 DKs** tab, with these differences:

      * When checking whether the light bulb joined the network, use **LED 2** as in that tab.
      * When using the light switch to change brightness, watch **LED 1** on the light bulb DK.
        Short presses and on/off behave as on other DKs; long-press dimming still updates the Level Control attribute, but the LED switches between **off** (level 0) and **on** (any level above 0) instead of fading with PWM.

   .. group-tab:: nRF54LM20 Dongle

      1. Turn on the development kit that runs the Network coordinator sample.

         When the network status LED turns on, this development kit has become the Coordinator of the Zigbee network and the network is established.

      #. Connect the nRF54LM20 Dongle that runs the Light bulb sample.

         When **Blue LED 1** turns on, the light bulb has become a Router inside the network.

         .. note::
            If **Blue LED 1** does not turn on, use the Network coordinator controls to reopen the network.

      #. Turn on the development kit that runs the Light switch sample.

         When the network status LED turns on, the light switch has become an End Device, connected directly to the Coordinator.

      #. Wait until the bulb-found LED on the development kit that runs the Light switch sample turns on.

         This LED indicates that the switch found a light bulb to control.

      #. Use the buttons on the development kit that runs the :ref:`zigbee_light_switch_sample` sample to control the light bulb.

         The result of using the buttons is reflected on the light bulb's **Red LED 2**.

   .. group-tab:: nRF52 and nRF53 DKs

      1. Turn on the development kit that runs the Network coordinator sample.

         When **LED 3** turns on, this development kit has become the Coordinator of the Zigbee network and the network is established.

      #. Turn on the development kit that runs the Light bulb sample.

         When **LED 3** turns on, the light bulb has become a Router inside the network.

         .. note::
            If **LED 3** does not turn on, press **Button 1** on the Coordinator to reopen the network.

      #. Turn on the development kit that runs the Light switch sample.

         When **LED 3** turns on, the light switch has become an End Device, connected directly to the Coordinator.

      #. Wait until **LED 4** on the development kit that runs the Light switch sample turns on.

         This LED indicates that the switch found a light bulb to control.

      #. Use the buttons on the development kit that runs the :ref:`zigbee_light_switch_sample` sample to control the light bulb.

         The result of using the buttons is reflected on the light bulb's **LED 2**.

You can now use buttons on the light switch to control the light bulb, as described in the :ref:`zigbee_light_switch_user_interface` section of the Light switch sample page.

.. _zigbee_light_bulb_testing_touchlink:

Testing Touchlink commissioning
-------------------------------

.. include:: /includes/touchlink_testing.txt

.. _zigbee_light_bulb_testing_zigbee_bt_dfu:

Testing Zigbee FOTA DFU over Bluetooth SMP
------------------------------------------

.. |zigbee bt device name| replace:: Zigbee_Bulb
.. |zigbee mcumgr smp upload| replace:: mcumgr --conntype ble --hci 0 --connstring peer_name='Zigbee_Bulb' image upload build/light_bulb/zephyr/zephyr.signed.bin

.. include:: /includes/zigbee_bt_dfu_testing.txt

Dependencies
************

This sample uses the following |NCS| libraries:

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
* `Pulse Width Modulation (PWM)`_
