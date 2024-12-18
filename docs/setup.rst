.. _zigbee_setup:

Requirements and setup
######################

This page outlines the requirements that you need to meet before you start working with the experimental support release of |addon| and Zigbee R23 protocol.

.. note::
   .. include:: /includes/experimental_note.txt

Hardware requirements
*********************

The |addon| supports the nRF54L series: nRF54L05, nRF54L10 and nRF54L15 SoCs.

To work with the |addon| for the |NCS| and test the available samples, you need at least two `nRF54L15 development kits (DK) <Developing with nRF54L Series_>`_.

For the :ref:`zigbee_quickstart`, you need three devices.

Software requirements
*********************

To work with the |addon|, you need to install the |NCS|, including all its prerequisites and the |NCS| toolchain.
Follow the `Installing the nRF Connect SDK`_ instructions, with the following exception:

.. tabs::

   .. group-tab:: |nRFVSC|

      In the **Get the nRF Connect SDK code** part of the install instructions:

      In Step 4, select :guilabel:`Browse nRF Connect SDK Add-on Index` instead of selecting the SDK version, then select :guilabel:`Zigbee`, and select v\ |addon_version| of the |addon|, which also installs |NCS| v\ |ncs_version|.

   .. group-tab:: command line

      In the **Get the nRF Connect SDK code** part of the install instructions:

      In Step 4, use the following command to initialize west with |addon| v\ |addon_version|, which also initializes |NCS| v\ |ncs_version|:

      .. code-block:: console

         west init -m "http://github.com/nrfconnect/ncs-zigbee" --mr main


To create an application, use :ref:`zigbee_template_sample` sample as a starting point.