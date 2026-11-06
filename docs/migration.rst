.. _migration:

Migration guides
################

.. contents::
   :local:
   :depth: 2

.. _migration_r22_addon:

From Zigbee R22 add-on to Zigbee R23 add-on
*******************************************

This section describes the changes required to successfully migrate a Zigbee application based on the latest |r22-addon| (v\ |r22_addon_version|) release to the |addon|.

If you already use |addon| v1.4.x with ZBOSS 4.2 and plan to move to ZBOSS 5, see :ref:`migration_zboss5` below.

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

.. _migration_zboss5:

From ZBOSS 4.2 to ZBOSS 5
*************************

This section describes changes required when you move an application from |addon| v1.4.x (ZBOSS stack v4.2.2.x) to a release based on **ZBOSS 5** (stack version |zboss_version|).

For migrating from the legacy |r22-addon| to the |addon|, see :ref:`migration_r22_addon`.

Overview
========

ZBOSS 5 is a major stack update on the Zigbee PRO R23 line (including R23.2 capabilities).
It keeps the same Zigbee Cluster Library and Base Device Behavior models, but changes several scheduler and platform integration APIs.
Many other symbols were added, replaced, or removed; this page covers only the changes most visible in |addon| applications.

Before you port application code, read the `ZBOSS 5 development guide`_ and the `ZBOSS 5 changelog`_.
Review the `ZBOSS 5 API reference`_ module list and the `ZBOSS 5 deprecated API list`_ for stack-wide API changes.
The `ZBOSS API documentation`_ for stack version |zboss_version| is the reference for types and functions mentioned below.

Application code changes
========================

Delayed callbacks and alarms
----------------------------

ZBOSS 5 uses :c:type:`zb_cb_param_t` as the unified 32-bit packed argument for delayed callbacks, application alarms, and related scheduler APIs.
The legacy two-argument callback type is removed.

When you schedule work from application code:

* Use :c:macro:`ZB_SCHEDULE_APP_CALLBACK` and :c:macro:`ZB_SCHEDULE_APP_ALARM` with a single :c:type:`zb_cb_param_t` argument.
* In the handler, unpack a buffer reference with :c:macro:`ZB_UNPACK_BUF_REF` and application data with :c:macro:`ZB_UNPACK_USER_PARAM`, as in the :ref:`zigbee_samples`.
* Replace any use of ``zb_callback2_t`` with a handler that takes :c:type:`zb_cb_param_t`.

If you call ZBOSS from other threads, use the OSIF wrappers documented in :ref:`zigbee_osif_thread_safe_scheduler`:

* :c:func:`zigbee_schedule_callback` forwards a full :c:type:`zb_cb_param_t`.
* :c:func:`zigbee_schedule_alarm` accepts only a buffer reference (:c:type:`zb_bufid_t`), matching :c:func:`zb_schedule_app_alarm`.
* :c:func:`zigbee_schedule_alarm_cancel` accepts :c:type:`zb_cb_param_t`, matching :c:func:`zb_schedule_alarm_cancel`.

Alarms that pack user data in the upper 16 bits of :c:type:`zb_cb_param_t` must be scheduled from the ZBOSS thread (for example with :c:macro:`ZB_SCHEDULE_APP_ALARM`) or use :c:func:`zigbee_schedule_callback`.

Base Device Behavior channel lists
----------------------------------

ZBOSS 5 expects primary and secondary channel masks to stay consistent when the channel plan is updated.
If your application or shell tooling sets the primary channel through BDB, make sure secondary channel lists are updated as well (see the :ref:`lib_zigbee_shell` ``bdb channel`` commands).

Trust Center security and commissioning
---------------------------------------

ZBOSS 5 applies a high-security Trust Center policy by default: joining devices must use an install code or passcode known to the Coordinator.
See :ref:`zigbee_commissioning_security`.

For lab and demonstration networks only, you can enable ``CONFIG_ZIGBEE_DEMO_COMMISSIONING`` or use the ``zigbee_demo_commissioning`` snippet.

Settings, PSA crypto, and NVRAM
-------------------------------

ZBOSS 5 builds use PSA-backed cryptography (``CONFIG_ZB_PSA_CRYPTO``).
The add-on initializes the Zephyr Settings subsystem before :c:func:`psa_crypto_init`, and uses ZMS-backed PSA storage on nRF54 RRAM SoCs when enabled.

.. note::
   The ZBOSS NVRAM page version increases from ``ZB_NVRAM_VER_15_0`` (|addon| v1.4.x / ZBOSS 4.2.2.x) to ``ZB_NVRAM_VER_16_0`` (ZBOSS 5).
   On the first ZBOSS startup after the upgrade, existing NVRAM is migrated to the new format when ``CONFIG_ZB_NVRAM_ENABLE_VER_MIGRATION`` is enabled (default in the add-on prebuilt libraries).
   Erased NVRAM or a factory reset before upgrading avoids migration.
   For dataset types and version definitions, see the `ZBOSS 5 NVRAM API`_ documentation.

After migration
===============

Compare :ref:`zigbee_memory` requirements after you move to the ZBOSS 5 prebuilt libraries; RAM and flash usage often increases
