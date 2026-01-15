.. _migration:

Migration guide for R22 to R23 Add-on
#####################################

.. contents::
   :local:
   :depth: 3

This page describes the changes required to successfully migrate a Zigbee application based on the latest |r22-addon| (v\ |r22_addon_version|) release to |addon| v\ |addon_version|.

.. note::
   Updating |r22-addon|-based firmware to |addon|-based firmware through OTA is possible. 
   For testing purposes, use the :ref:`zigbee_light_switch_sample` with the :ref:`FOTA extension <zigbee_light_switch_activating_variants>`.

Complete the following steps:

1. Read the `ZBOSS Zigbee 2023 (R23) features`_ page about new R23 features.
#. Read the section about changes in PAN ID conflict resolution `PANID conflict resolution API`_.
#. If the application manually adjusts configurable memory settings, note the new :c:macro:`ZB_CONFIG_NWK_DISC_TABLE_SIZE` setting.
#. If the application manually adjusts configurable memory settings, note the new :c:macro:`ZB_CONFIG_ZDO_KEY_NEGOTIATIONS_NUM` setting.
#. If the application operates directly with ZBOSS neighbor table entries (the unusual case), note the changed format of :c:func:`zb_neighbor_tbl_ent_t` and the introduction of the :c:func:`zb_nwk_disc_tbl_ent_t` function.

.. warning::
   The NVRAM version has been changed. 
   Data stored in NVRAM on the device is automatically migrated to the new version on the first ZBOSS startup.
   For details on NVRAM dataset types and version definitions, see the `ZBOSS NVRAM API`_ documentation.

When migrating, note that support for new R23 features increases the required memory.
Refer to the :ref:`zigbee_memory` page for examples of memory requirements for different samples.
