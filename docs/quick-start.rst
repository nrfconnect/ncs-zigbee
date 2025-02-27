.. _zigbee_quickstart:
.. _ug_zigee_qsg:

Quick start guide
#################

.. contents::
   :local:
   :depth: 2

.. note::
   .. include:: /includes/experimental_note.txt

This guide demonstrates some of the basic concepts of the Zigbee network using the |addon| for the |NCS| and the |nRFVSC|.
It guides you through programming and testing of the required samples.

Overview
********

As part of this guide, you are going to program three Zigbee samples onto three different development kits in order to set up a basic Zigbee network composed of three devices, or nodes.
A node is "a testable implementation of a Zigbee application on a single stack with a single network address, and on a single network".

.. figure:: images/zigbee_qsg_topology.svg
   :alt: Zigbee star topology setup for this guide

   Zigbee star topology setup for this guide

The figure shows the Zigbee network star topology you are going to set up.
See :ref:`zigbee_topologies` for more information.

Each of the samples you will use for forming the network is based on the single-chip, single-protocol architecture, uses the default configuration (that is, without any additional sample extensions enabled) and has a clearly defined role:

* :ref:`Zigbee network coordinator <zigbee_network_coordinator_sample>` demonstrates the Zigbee Coordinator role and supports only the network steering commissioning mechanism.
* :ref:`Zigbee light bulb <zigbee_light_bulb_sample>` demonstrates the Zigbee Router role and implements the Dimmable Light device specification, as defined in the Zigbee Home Automation public application profile.
* :ref:`Zigbee light switch <zigbee_light_switch_sample>` demonstrates the Zigbee End Device role and implements the Dimmer Switch device specification, as defined in the Zigbee Home Automation public application profile.
  Once commissioned into the network, it looks for devices with the implemented Level Control and On/Off clusters and can control the one that answers first.

All these samples also implement the Basic and Identify clusters, which are the basic Zigbee clusters.

Program the samples
*******************

To program the samples, complete the following steps:

.. tabs::

   .. group-tab:: nRF54L Series DK

      .. include:: includes/vsc_build_and_run.txt

      3. Program the Zigbee network coordinator sample to one of the boards:

         a. Connect your development kit using the serial port.
         #. In the :guilabel:`Connected Devices View`, click :guilabel:`Refresh Connected Devices`.
            Your development kit appears on the list.
         #. In the :guilabel:`Actions View`, click :guilabel:`Flash`.

            .. figure:: images/zigbee_qsg_vsc_flash.png
               :alt: nRF Connect for Visual Studio Code - Refresh Connected Devices and Flash buttons

         When the programming is done, the **LED 1** on the development kit turns on to indicate that the Zigbee network is open.
         After some time, it turns off and the Zigbee network needs to be reopened (see below).
      #. Add the Zigbee light switch sample.

         Repeat Steps 2 and 3 for the sample.
      #. Add the Zigbee light bulb sample.

         Repeat Steps 2 and 3 for the sample.
         The **LED 2** turns on when the application is started.
      #. Press **Button 0** on the development kit programmed with the network coordinator sample to reopen the Zigbee network.
         After some time, the devices join the Zigbee network.

         * On the development kit programmed with the light bulb sample, **LED 1** turns on when the light bulb joins the network.
         * On the development kit programmed with the light switch sample, **LED 1** turns on when the device joins the network and **LED 2** turns on when the light switch finds a light bulb to control.

      Reopening the network
         If you find that you are having issues with your devices joining the Zigbee network, it may be because the network needs to be reopened manually.
         By default, the network remains open for 180 seconds at startup and after **Button 0** is pressed.
         After 180 seconds have passed, the network will close and needs to be re-opened.

         When the network is open, **LED 1** on the development kit programmed with the network coordinator sample will be on.
         If **LED 1** is not on, then the network is closed and needs to be reopened again.

         To reopen the network, press **Button 0** on the development kit that has been programmed as the network coordinator.

         For more information, see the :ref:`User interface <zigbee_network_coordinator_user_interface>` section for the :ref:`Zigbee Network coordinator <zigbee_network_coordinator_sample>` sample.

      .. rst-class:: numbered-step

   .. group-tab:: nRF52840 DK

      .. include:: includes/vsc_build_and_run.txt

      3. Program the Zigbee network coordinator sample to one of the boards:

         a. Connect your development kit using the serial port.
         #. In the :guilabel:`Connected Devices View`, click :guilabel:`Refresh Connected Devices`.
            Your development kit appears on the list.
         #. In the :guilabel:`Actions View`, click :guilabel:`Flash`.

            .. figure:: images/zigbee_qsg_vsc_flash.png
               :alt: nRF Connect for Visual Studio Code - Refresh Connected Devices and Flash buttons

         When the programming is done, the **LED 3** on the development kit turns on to indicate that the Zigbee network is open.
         After some time, it turns off and the Zigbee network needs to be reopened (see below).
      #. Add the Zigbee light switch sample.

         Repeat Steps 2 and 3 for the sample.
      #. Add the Zigbee light bulb sample.

         Repeat Steps 2 and 3 for the sample.
         The **LED 4** turns on when the application is started.
      #. Press **Button 1** on the development kit programmed with the network coordinator sample to reopen the Zigbee network.
         After some time, the devices join the Zigbee network.

         * On the development kit programmed with the light bulb sample, **LED 3** turns on when the light bulb joins the network.
         * On the development kit programmed with the light switch sample, **LED 3** turns on when the device joins the network and **LED 4** turns on when the light switch finds a light bulb to control.

      Reopening the network
         If you find that you are having issues with your devices joining the Zigbee network, it may be because the network needs to be reopened manually.
         By default, the network remains open for 180 seconds at startup and after **Button 1** is pressed.
         After 180 seconds have passed, the network will close and needs to be re-opened.

         When the network is open, **LED 3** on the development kit programmed with the network coordinator sample will be on.
         If **LED 3** is not on, then the network is closed and needs to be reopened again.

         To reopen the network, press **Button 1** on the development kit that has been programmed as the network coordinator.

         For more information, see the :ref:`User interface <zigbee_network_coordinator_user_interface>` section for the :ref:`Zigbee Network coordinator <zigbee_network_coordinator_sample>` sample.

      .. rst-class:: numbered-step

Test the network
****************

After forming the Zigbee network, test the interaction between the devices. 

.. tabs::

   .. group-tab:: nRF54L Series DK

      1. Press **Button 1** once on the development kit programmed with the light switch sample to turn off the light bulb's **LED 1**.
      #. Press **Button 0** once on the development kit programmed with the light switch sample to turn on the light bulb's **LED 1**.
      #. Press and hold **Button 0** on the development kit programmed with the light switch sample to decrease the light bulb's **LED 1** brightness.
      #. Press and hold **Button 1** on the development kit programmed with the light switch sample to increase the light bulb's **LED 1** brightness.

   .. group-tab:: nRF52840 DK
   
      1. Press **Button 2** once on the development kit programmed with the light switch sample to turn off the light bulb's **LED 4**.
      #. Press **Button 1** once on the development kit programmed with the light switch sample to turn on the light bulb's **LED 4**.
      #. Press and hold **Button 2** on the development kit programmed with the light switch sample to decrease the light bulb's **LED 4** brightness.
      #. Press and hold **Button 1** on the development kit programmed with the light switch sample to increase the light bulb's **LED 4** brightness.

Next steps
**********

After you complete this quick start guide, we recommend that you get familiar with the following topics:

* If you want to start configuring samples:

  * :ref:`ug_zigbee_configuring`.
  * :ref:`ug_zigbee_configuring_libraries`.
  * :ref:`ug_zigbee_other_ecosystems`.
  * Zigbee sample variants - see the :ref:`Zigbee samples <zigbee_samples>` pages.

* If you want to test a multiprotocol solution, see the :ref:`zigbee_light_switch_sample_nus` section of the light switch sample.

* If you want to start developing for co-processor designs:

  * :ref:`ug_zigbee_platform_design_ncp`
  * `NCP Host documentation`_

* If you want to learn more about ZBOSS:

  * ZBOSS user guide - see `API documentation`_
  * :ref:`ug_zigee_adding_clusters`
