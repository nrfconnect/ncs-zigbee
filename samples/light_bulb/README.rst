.. _zigbee_light_bulb_sample:

Zigbee: Light bulb
##################

.. contents::
   :local:
   :depth: 2

This sample demonstrates a simple light bulb whose brightness can be adjusted by another device.

You can use this sample with the :ref:`Zigbee Network coordinator <zigbee_network_coordinator_sample>` and :ref:`Zigbee Light switch <zigbee_light_switch_sample>` samples to set up a basic Zigbee network.

As a proof of concept, the sample also supports the optional :ref:`zigbee_light_bulb_sample_matter`, which lets the same firmware start as a Zigbee Router and migrate to a Matter Dimmable Light after Matter commissioning.
This combined Matter build also enables :ref:`Touchlink <zigbee_commissioning_modes_touchlink>` target support, so it can be commissioned by a nearby Touchlink initiator without a Zigbee Coordinator on the network.

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

.. _zigbee_light_bulb_sample_matter:

Matter extension
================

This optional extension is a **proof of concept** of a combined Matter + Zigbee build on a single SoC, sharing the 802.15.4 radio between the ZBOSS stack and OpenThread (used by Matter).
For Thread networking in Matter mode, the light bulb acts as an OpenThread Full Thread Device (FTD).
It is supported only on the ``nrf54lm20dk/nrf54lm20a/cpuapp`` board target.

The sample-specific behavior is:

* On first boot, the device is a standard :ref:`Zigbee Router <zigbee_roles>` exposing the Dimmable Light device, while the Matter stack advertises for commissioning over Bluetooth LE (CHIPoBLE).
* After successful Matter commissioning, the device is converted to a Matter Dimmable Light endpoint that serves the On/Off and Level Control clusters and drives the same LED through PWM.
  It can then be controlled by any device on the Matter fabric bound to it (for example, the :ref:`zigbee_light_switch_sample` built with the :ref:`zigbee_light_switch_sample_matter`).
* On subsequent boots, the device starts directly as a Matter Dimmable Light; a Matter factory reset reverts it to a fresh Zigbee Router.

The radio hand-over, persistent protocol state, factory-reset behavior and onboarding-data generation are common to both combined samples and are described in detail in the :ref:`zigbee_light_switch_sample_matter` section of the Light switch sample.

.. _zigbee_light_bulb_sample_touchlink:

Touchlink commissioning
-----------------------

The combined Matter build enables the light bulb as a Touchlink target (``CONFIG_ZIGBEE_TOUCHLINK_TARGET``).
This lets a nearby Touchlink initiator (for example, the :ref:`zigbee_light_switch_sample` built with the :ref:`zigbee_light_switch_sample_matter`) commission the device directly and form a distributed-security Zigbee network without a Zigbee Coordinator.
No dedicated button action is required on the light bulb to enable this role; the device responds to Touchlink scans whenever it is powered and not yet joined to a network.

.. note::
   Touchlink in the |addon| for the |NCS| is provided as an experimental feature with basic functionality.
   See :ref:`zigbee_commissioning_modes_touchlink` for details.

.. _zigbee_light_bulb_matter_limitations:

Matter extension limitations
----------------------------

.. include:: /includes/matter_extension_limitations.txt

Configuration
*************

|config|

..
  FEM support
  ===========

  .. include:: /includes/sample_fem_support.txt

.. _zigbee_light_bulb_activating_variants:

Configuration files for sample extensions
=========================================

.. |sample matter ref| replace:: :ref:`zigbee_light_bulb_sample_matter`
.. |sample matter limitations ref| replace:: :ref:`zigbee_light_bulb_matter_limitations`
.. |sample dir| replace:: samples/light_bulb

.. include:: /includes/matter_extension_activation.txt

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

.. _zigbee_light_bulb_testing_matter:

Testing the Matter extension
----------------------------

See :ref:`zigbee_light_bulb_sample_matter` for the runtime behavior driving the steps below.

To test the extension, you need:

* A light bulb built with the Matter extension (see :ref:`zigbee_light_bulb_activating_variants`).
* A Zigbee test setup to verify Zigbee operation before Matter commissioning.
  You can use either the standard setup (a Network coordinator and a Zigbee light switch) or, alternatively, only a Touchlink-capable light switch (for example, the :ref:`zigbee_light_switch_sample` built with the Matter extension), in which case the Zigbee Coordinator is not needed.
* A Matter controller that can commission a Thread device over Bluetooth LE, for example `CHIP Tool`_ or an ecosystem app (Apple Home, Google Home, Amazon Alexa).
* A Thread Border Router reachable by the Matter fabric.
* Optionally, a Matter switch on the same Thread fabric to be bound to the light bulb (for example, the :ref:`zigbee_light_switch_sample` built with the :ref:`zigbee_light_switch_sample_matter`).

Complete the following steps to exercise the full Zigbee-to-Matter flow:

1. Verify Zigbee operation in one of the following ways:

   * Follow the standard `Testing`_ procedure with a Zigbee Network coordinator and a Zigbee light switch.
   * Or, skip the Zigbee Coordinator and pair the light bulb directly with a Touchlink-capable light switch:

     a. Power the light bulb (Touchlink target).
     #. Power the light switch and press its Touchlink button (see :ref:`zigbee_light_switch_sample_touchlink`).
        The two devices form a distributed-security Zigbee network without a Zigbee Coordinator, and the light switch finds and controls the light bulb.

   While the device is still a Zigbee Router, it also advertises for Matter commissioning over Bluetooth LE.
#. Commission the device using the onboarding payload produced by the Matter factory data build (QR code or manual pairing code).
   After the Matter CASE session is established, the light bulb hands the radio over to Thread and stops participating in the Zigbee network.
#. Drive the light bulb from a Matter peer:

   * From the controller directly, with ``chip-tool onoff toggle …`` or ``chip-tool levelcontrol move-to-level …``.
   * Or by binding a Matter switch to the light bulb and using the switch's dimmer button.

#. To return the device to Zigbee operation, trigger a Matter factory reset from the controller (for example, ``chip-tool pairing unpair …``).
   The device reboots as a fresh Zigbee Router with Matter Bluetooth LE advertising active again.

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

The following dependencies are added by the :ref:`zigbee_light_bulb_sample_matter`:

* The Matter stack (``CONFIG_CHIP``) shipped with the |NCS|, including the On/Off, Level Control and Identify clusters and the Matter factory data module.
* OpenThread (used by Matter on 802.15.4) and the `SoftDevice Controller`_ (used for CHIPoBLE commissioning).
* The ``zigbee_matter_coexistence`` and ``zigbee_matter_protocol_state`` libraries, which orchestrate the 802.15.4 radio hand-over and persist the selected protocol.
* The ``nrf_802154_callbacks_dispatcher`` (``CONFIG_NRF_802154_CALLBACKS_DISPATCHER``) with runtime re-init (``CONFIG_NRF_802154_DRV_REINIT_ENABLED``).
