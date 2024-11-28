.. _example_release_notes:

Release notes
#############

.. contents::
   :local:
   :depth: 2

All the notable changes to the |addon| for the |NCS| are listed here.
See also the `Release notes for the nRF Connect SDK`_ and the :ref:`zboss_changelog`

.. note::
   .. include:: /includes/experimental_note.txt

The |addon| v\ |addon_version| is compatible with |NCS| v\ |ncs_version| and uses the ZBOSS stack version |zboss_version|.
For a full list of |addon|, related |NCS| and ZBOSS stack and NCP host package versions, view the following table:

.. toggle::

   +-------------------+------------------+-----------------------+---------------------+
   | |addon| version   | |NCS| version    | ZBOSS stack version   | NCP host version    |
   +===================+==================+=======================+=====================+
   | 0.1.0             | 2.8.0            | 4.1.4.2               | N/A                 | 
   +-------------------+------------------+-----------------------+---------------------+

.. _zigbee_release:

|addon| v0.1.0 - 15/11/2024
***************************

Initial release.

* Added:

  * Experimental support for the ZBOSS R23 stack for the nRF54L15 SoC.
    Released Zigbee library is not certified.
  * Basic samples:

    * :ref:`Light bulb <zigbee_light_bulb_sample>`
    * :ref:`Light switch <zigbee_light_switch_sample>`
    * :ref:`Network coordinator <zigbee_network_coordinator_sample>`
    * :ref:`Shell <zigbee_shell_sample>`
    * :ref:`Template <zigbee_template_sample>`
