.. _zigbee_setup:

Requirements and setup
######################

This page outlines the requirements that you need to meet before you start working with the experimental support release of |addon| and Zigbee R23 protocol.

Hardware requirements
*********************

The Zigbee R23 add-on supports the nRF52840, nRF52833, nRF5340, nRF54L15, nRF54L10, and nRF54L05 SoCs.

For the :ref:`zigbee_quickstart`, you need three development kits of the following types:

.. include:: /includes/device_table_all.txt

You can mix different development kits.

.. note::
   Make sure that you have three development kits before proceeding to the next steps.

.. note::
   The nRF52833 SoC is supported only for NCP sample.
   
Software requirements
*********************

For libraries and code for |addon|, see the `ncs-zigbee repository <Zigbee R23 add-on_>`_.

To work with the |addon|, you need to install the |NCS|, including all its prerequisites and the |NCS| toolchain.
Follow the `Installing the nRF Connect SDK`_ instructions, with the following exception:

.. tabs::

   .. group-tab:: |nRFVSC|

      In the **Get the nRF Connect SDK code** part of the install instructions:

      In Step 2, click on :guilabel:`Create a new application`, select :guilabel:`Browse nRF Connect SDK Add-on Index`, then select :guilabel:`Zigbee R23`, and select v\ |addon_version| of the |addon|, which also installs |NCS| v\ |ncs_version|.

   .. group-tab:: command line

      In the **Get the nRF Connect SDK code** part of the install instructions:

      In Step 4, use the following command to initialize west with |addon| v\ |addon_version|, which also initializes |NCS| v\ |ncs_version|:

      .. code-block:: console

         west init -m "http://github.com/nrfconnect/ncs-zigbee" --mr v1.2.1


To create an application, use :ref:`zigbee_template_sample` sample as a starting point.