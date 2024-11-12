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

The |addon| v\ |addon_version| is compatible with |NCS| v\ |ncs_version| and uses the ZBOSS R23 stack version |zboss_version|.
For a full list of |addon|, |NCS| and ZBOSS R23 stack versions, view the following table:

.. toggle::

   +-------------------+------------------+-------------------------+---------------------+
   | |addon| version   | |NCS| version    | ZBOSS R23 stack version | NCP host version    |
   +===================+==================+=========================+=====================+
   | v0.1.0            | v2.8.0           | 4.1.4.2                 | v3.0.0              | 
   +-------------------+------------------+-------------------------+---------------------+

.. _zigbee_release:

Zigbee R23 add-on v0.1.0 - 14/11/2024
*************************************

Initial release.

* Added:

  * Experimental support for the ZBOSS R23 stack for the nRF54L15 DK.
    This release has not been certified.
  * Basic samples:

    * :ref:`Light bulb <zigbee_light_bulb_sample>`
    * :ref:`Light switch <zigbee_light_switch_sample>`
    * :ref:` Network coordinator <zigbee_network_coordinator_sample>`
    * :ref:`NCP <zigbee_ncp_sample>`
    * :ref:`Shell <zigbee_shell_sample>`
    * :ref:`Template <zigbee_template_sample>`
