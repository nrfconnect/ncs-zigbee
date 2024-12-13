.. _release_notes:

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

+-------------------+------------------+-----------------------+---------------------+
| |addon| version   | |NCS| version    | ZBOSS stack version   | NCP host version    |
+===================+==================+=======================+=====================+
| 0.2.0             | 2.8.0            | 4.1.4.2               | 3.0.0               |
+-------------------+                  +                       +---------------------+
| 0.1.0             |                  |                       | N/A                 |
+-------------------+------------------+-----------------------+---------------------+

.. _zigbee_release:

|addon| v0.2.0 - 28/11/2024
***************************

This is an experimental release.

* Added:

  * The :ref:`NCP <zigbee_ncp_sample>` sample.
  * The `ZBOSS NCP Host`_ package v\ |zigbee_ncp_package_version|.

* Updated the documentation with small improvements.

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
