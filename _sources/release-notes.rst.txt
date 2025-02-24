.. _example_release_notes:

Release notes
#############

.. contents::
   :local:
   :depth: 2

All the notable changes to the |addon| for the |NCS| are listed here.
See also the `Release notes for the nRF Connect SDK`_ and the :ref:`zboss_changelog`.

.. note::
   .. include:: /includes/experimental_note.txt

The |addon| v\ |addon_version| is compatible with |NCS| v\ |ncs_version| and uses the ZBOSS stack version |zboss_version|.
For a full list of |addon| releases, related |NCS| and ZBOSS stack and NCP host package versions, view the following table:

+-------------------+------------------+-----------------------+---------------------+
| |addon| version   | |NCS| version    | ZBOSS stack version   | NCP host version    |
+===================+==================+=======================+=====================+
| 0.3.1             | 2.9.0            | 4.1.4.2               | 3.0.1               | 
+-------------------+                  |                       +                     |
| 0.3.0             |                  |                       |                     | 
+-------------------+------------------+                       +---------------------+
| 0.2.0             | 2.8.0            |                       | 3.0.0               | 
+-------------------+                  |                       +---------------------+
| 0.1.0             |                  |                       | N/A                 | 
+-------------------+------------------+-----------------------+---------------------+

.. _zigbee_release:

|addon| v0.3.1 - 14/01/2025
***************************

This is an `experimental <Software maturity levels_>`_ release.
 
* Added:
 
  * Fix ZCL common function.

|addon| v0.3.0 - 10/01/2025
***************************

This is an `experimental <Software maturity levels_>`_ release.
 
* Added:
 
  * Experimental support for the ZBOSS R23 stack for the nRF54L10 and nRF54L05 SoCs. Released Zigbee libraries are not certified.
  * The ZBOSS NCP Host package v3.0.1.
  * Flash and RAM memory usage optimization.

* Updated the documentation with nR54L10 and RF54L05 support.

|addon| v0.2.0 - 28/11/2024
***************************

This is an `experimental <Software maturity levels_>`_ release.
 
* Added:
 
  * The :ref:`NCP <zigbee_ncp_sample>` sample.
  * The ZBOSS NCP Host package v3.0.0.

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
