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

The sample supports the following development kit:

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

User interface
**************

.. tabs::

   .. group-tab:: nRF54 DKs

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

   .. group-tab:: nRF52 and nRF53 DKs

      LED 1:
          Blinks to indicate that the main application thread is running.

      LED 3:
          Turns on when the light bulb joins the network.

      LED 4:
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

After programming the sample to your development kits, complete the following steps to test it:

.. tabs::

   .. group-tab:: nRF54 DKs

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

         The result of using the buttons is reflected on the light bulb's **LED 4**.

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
