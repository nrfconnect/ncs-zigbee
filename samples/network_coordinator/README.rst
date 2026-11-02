.. _zigbee_network_coordinator_sample:

Zigbee: Network coordinator
###########################

.. contents::
   :local:
   :depth: 2

This Network coordinator sample establishes the Zigbee network and commissions Zigbee devices that want to join the network.

You can use this sample together with the :ref:`Zigbee Light bulb <zigbee_light_bulb_sample>` and the :ref:`Zigbee Light switch <zigbee_light_switch_sample>` to set up a basic Zigbee network.

Requirements
************

The sample supports the following development kits:

.. include:: /includes/device_table_single_multi.txt
   :end-before: * - `nRF54LM20 Dongle

..
  You can use one or more of the development kits listed above and mix different development kits.

Optionally, you can use this sample with one or both of the following samples:

* The :ref:`Zigbee Light bulb <zigbee_light_bulb_sample>` sample programmed on one or more separate devices.
* The :ref:`Zigbee Light switch <zigbee_light_switch_sample>` sample programmed on one or more separate devices.

You can mix different development kits.

Overview
********

This Zigbee Network coordinator sample demonstrates the Zigbee Coordinator role and implements two clusters (Basic and Identify) that used to be required by the Zigbee Home Automation profile.
It is a minimal implementation that supports only the network steering commissioning mechanism.
The Basic cluster provides attributes and commands for determining basic information about the node.
The Identify cluster allows to set the device into the identification mode, which provides a way to locate the device.

Configuration
*************

|config|

The Network coordinator acts as the Trust Center of the network.
By default, it uses the high security mode of the ZBOSS stack and accepts only devices whose install code was added to it before they join.
The sample has no user interface for adding install codes.
As a result, the :ref:`Zigbee Light bulb <zigbee_light_bulb_sample>` and :ref:`Zigbee Light switch <zigbee_light_switch_sample>` samples cannot join a Network coordinator built with the default configuration.

To commission devices securely with install codes, use the :ref:`Zigbee shell <zigbee_shell_sample>` sample as the Coordinator instead.
See :ref:`zigbee_install_code_commissioning`.
For details about the Trust Center policy, see :ref:`zigbee_commissioning_security`.

Snippets
========

Use the ``zigbee_demo_commissioning`` snippet, that enables the ``CONFIG_ZIGBEE_DEMO_COMMISSIONING`` Kconfig option.
It allows devices without an install code to join the network using the well-known link key.
The sample logs a warning at startup when this option is enabled.
Build only the Network coordinator with this snippet.
The joining samples do not need any changes.

To build the sample with this snippet, run the following command:

.. parsed-literal::
   :class: highlight

   west build samples/network_coordinator -b *board_target* -S zigbee_demo_commissioning

.. note::
   This configuration is not secure and does not comply with BDB 3.1.
   While the network is open, any device in radio range can join and obtain the network key, and any device that captures a join can decrypt the network key.
   Use it only for demonstrations and testing.

..
  FEM support
  ===========

  .. include:: /includes/sample_fem_support.txt

.. _zigbee_network_coordinator_user_interface:

User interface
**************

.. tabs::

   .. group-tab:: nRF54 DKs

      LED 0:
          Blinks to indicate that the main application thread is running.

      LED 2:
          Indicates whether the network is open or closed:

          * On - The network is open.
          * Off - The network is closed.

      LED 3:
          Blinks when the Network coordinator is in Identify mode.

      Button 0:
          Reopens the network for 180 seconds.

          .. note::
               The network is also opened after startup.

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
          Indicates whether the network is open or closed:

          * On - The network is open.
          * Off - The network is closed.

      LED 4:
          Blinks when the Network coordinator is in Identify mode.

      Button 1:
          Reopens the network for 180 seconds.

          .. note::
               The network is also opened after startup.

      Button 4:
          Depending on how long the button is pressed:

          * If pressed for less than five seconds, it starts or cancels the Identify mode.
          * If pressed for five seconds, it initiates the `factory reset of the device <Resetting to factory defaults_>`_.
            The length of the button press can be edited using the ``CONFIG_FACTORY_RESET_PRESS_TIME_SECONDS`` Kconfig option from :ref:`lib_zigbee_application_utilities`.
            Releasing the button within this time does not trigger the factory reset procedure.

Building and running
********************
.. |sample path| replace:: :file:`samples/network_coordinator`

|enable_zigbee_before_testing|

.. include:: /includes/build_and_run.txt

.. _zigbee_network_coordinator_sample_testing:

Testing
=======

The following steps require the sample to be built with the ``zigbee_demo_commissioning`` snippet, as described in `Snippets`_.
After programming the sample to your development kit, complete the following steps to test it:

.. tabs::

   .. group-tab:: nRF54 DKs

      1. Turn on the development kit that runs the coordinator sample.

         * When **LED 0** starts blinking, the main application thread has started.
         * When **LED 2** turns on, this development kit has become the Coordinator of the Zigbee network and the network is established.

      #. Turn on the other development kits that you programmed.

         * When **LED 2** turns on the development kit that runs the Light bulb sample, it has become a Router inside the network.
         * When **LED 2** turns on the development kit that runs the Light switch sample, it has become an End Device, connected directly to the Coordinator.

         .. note::
             If **LED 2** on the development kits does not turn on, press **Button 0** on the Coordinator to reopen the network.

      #. Optionally, if you are testing with both the Light bulb and the Light switch samples, complete the following additional steps:

         a. Wait until **LED 3** on the development kit that runs the Light switch sample turns on.

            This LED indicates that the switch found a light bulb to control.

         #. Use buttons on the development kit that runs the Light switch sample to control the light bulb, as described in the Light switch sample's user interface section.

            The result of using the buttons is reflected on the light bulb's **LED 1**.

   .. group-tab:: nRF52 and nRF53 DKs

      1. Turn on the development kit that runs the coordinator sample.

         * When **LED 1** starts blinking, the main application thread has started.
         * When **LED 3** turns on, this development kit has become the Coordinator of the Zigbee network and the network is established.

      #. Turn on the other development kits that you programmed.

         * When **LED 3** turns on the development kit that runs the Light bulb sample, it has become a Router inside the network.
         * When **LED 3** turns on the development kit that runs the Light switch sample, it has become an End Device, connected directly to the Coordinator.

         .. note::
             If **LED 3** on the development kits does not turn on, press **Button 1** on the Coordinator to reopen the network.

      #. Optionally, if you are testing with both the Light bulb and the Light switch samples, complete the following additional steps:

         a. Wait until **LED 4** on the development kit that runs the Light switch sample turns on.

            This LED indicates that the switch found a light bulb to control.

         #. Use buttons on the development kit that runs the Light switch sample to control the light bulb, as described in the Light switch sample's user interface section.

            The result of using the buttons is reflected on the light bulb's **LED 2**.

You can now use buttons on the light switch to control the light bulb, as described in the :ref:`zigbee_light_switch_user_interface` section of the Light switch sample page.

Dependencies
************

This sample uses the following |NCS| libraries:

* :ref:`lib_zigbee_error_handler`
* :ref:`lib_zigbee_application_utilities`
* Zigbee subsystem:

  * :file:`zb_nrf_platform.h`

* `DK Buttons and LEDs`_

This sample uses the ZBOSS stack:

* :ref:`zigbee_zboss` |zboss_version| (`API documentation`_)

In addition, it uses the following Zephyr libraries:

* :file:`include/zephyr.h`
* :file:`include/device.h`
* `Logging`_
