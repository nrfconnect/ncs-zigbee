.. _zigbee_setup:

Requirements and setup
######################

This page outlines the requirements that you need to meet before you start working with the experimental support release of Zigbee R23 protocol.

.. note::
   .. include:: /includes/experimental_note.txt

Hardware requirements
*********************

To work with the |addon| for the |NCS| and test the available samples, you need at least two `nRF54L15 development kits (DK) <Developing with nRF54L Series_>`_.

..
  * `nRF5340 DK <Developing with nRF53 Series_>`_
  * `nRF52840 Dongle <Developing with nRF52 Series_>`_
  * `nRF52840 DK <Developing with nRF52 Series_>`_
  * `nRF52833 DK <Developing with nRF52 Series_>`_
  * `nRF21540 DK <Developing with Front-End Modules_>`_

  Not all :ref:`zigbee samples` support all of the listed devices.
  Check the requirements for each sample to see which it supports.

For the :ref:`zigbee_quickstart`, you need three devices.

Software requirements
*********************

To work with the |addon|, you need to install the |NCS|, including all its prerequisites and the |NCS| toolchain.
Follow the `Installing the nRF Connect SDK`_ instructions, with the following exception:

.. tabs::

   .. group-tab:: |nRFVSC|

      In the **Get the nRF Connect SDK code** part of the install instructions:

      In Step 4, select :guilabel:`Browse nRF Connect SDK Add-on Index` instead of selecting the SDK version, then select :guilabel:`Zigbee R23 extension`, and select v\ |addon_version| of the |addon|, which also installs |NCS| v\ |ncs_version|.

   .. group-tab:: command line

      In the **Get the nRF Connect SDK code** part of the install instructions:

      In Step 4, use the following command to initialize west with |addon| v\ |addon_version|, which also initializes |NCS| v\ |ncs_version|:

      TODO: command should be updated to point to a release version branch, not `main`.

      .. code-block:: console

         west init -m "http://github.com/nrfconnect/ncs-zigbee" --mr main

To create an application, follow the `Creating an application`_ instructions, with the following differences:

.. tabs::

   .. group-tab:: |nRFVSC|

      In Step 4, select :guilabel:`Browse nRF Connect SDK Add-on Index`, then select :guilabel:`Zigbee R23 extension`, and select |addon_version| of the |addon|.

   .. group-tab:: command line

      TODO: This might require discrete steps.
      Maybe adapted from either:

      * https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/dev_model_and_contributions/adding_code.html#workflow_3_application_in_a_fork_of_sdk-nrf
      * https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/app_dev/create_application.html#creating_application_for_use_with_command_line
