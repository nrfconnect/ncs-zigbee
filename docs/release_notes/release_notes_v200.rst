.. _release_notes_v200:
.. _zigbee_release:

Release notes for |addon| v2.0.0
#################################

.. contents::
   :local:
   :depth: 2

This is the first release to support Zigbee 4.0, integrating a Zigbee R23.2-compatible stack.
This release has :ref:`supported <nrf:software_maturity>` maturity status.
For Zigbee 3.0 designs, use the Zigbee R23 add-on v1.4.x in R22 compatibility mode, which is enabled by default.
The Zigbee R23 add-on v1.4.x integrates a Zigbee R23.1-compatible stack.

* Added:

  * Zigbee 4.0 compatible ZBOSS stack v5.0.5.2, replacing the ZBOSS 4.2 prebuilt libraries.
    See the `ZBOSS 5 changelog`_ and :ref:`migration_zboss5`.
  * PSA-backed cryptography integration in the OSIF layer, with Settings initialized before :c:func:`psa_crypto_init` and ZMS-backed PSA storage on nRF54 RRAM SoCs when enabled.
  * Support for the nRF54LM20 Dongle in the :ref:`zigbee_light_bulb_sample`, :ref:`zigbee_light_switch_sample`, and :ref:`zigbee_shell_sample` samples.
  * Experimental support for the nRF54LC10 DK (nRF54LC10A SoC) in the :ref:`zigbee_samples`.
  * The ``CONFIG_ZIGBEE_DEMO_COMMISSIONING`` Kconfig option, which lets a Coordinator accept devices without an install code (insecure, for demonstrations and testing only).
  * The ``zigbee_demo_commissioning`` snippet, which enables the ``CONFIG_ZIGBEE_DEMO_COMMISSIONING`` Kconfig option.
  * The :ref:`ug_zigbee_tools_production_config` script, which generates the ZBOSS production configuration of a device, including its install code or passcode from the product label or QR code.
  * The :ref:`zigbee_commissioning_security` section describing the Trust Center security policy.
  * The ``app_io`` shell command in the Zigbee application utilities, which simulates button presses and reads LED and PWM LED states.
    Enable it with the ``CONFIG_ZIGBEE_APP_IO_SHELL`` Kconfig option.

* Updated:

  * Support for the nRF Connect SDK v3.4.2.
  * The OSIF layer, application libraries, Zigbee Shell, and :ref:`zigbee_samples` for ZBOSS 5 APIs, including unified :c:type:`zb_cb_param_t` delayed callbacks and BDB channel list handling.
    Thread-safe scheduler wrappers are documented in :ref:`zigbee_osif_thread_safe_scheduler`.
  * The default Zigbee PRO core runtime behavior to R23 mode (``CONFIG_ZIGBEE_PRO_CORE_BEHAVIOR_R23``), which is required for Zigbee 4.0 product certification.
    R22 mode (``CONFIG_ZIGBEE_PRO_CORE_BEHAVIOR_R22``) remains available for Zigbee 3.0 product certification.
  * The :ref:`bdb_ic_add` and :ref:`bdb_ic_set` shell commands to accept 128-bit install codes without their CRC, and Zigbee R23 PAKE passcodes as setup codes or hexadecimal strings.
    The type is detected from the length of the value, and values of other lengths are rejected.
  * Improved ZBOSS NVRAM storage by batching flash writes into fewer operations.
    Cache size is controlled by ``CONFIG_ZIGBEE_NVRAM_WRITE_CACHE_SIZE``.
  * Renamed the FOTA snippets from ``zigbee_fota_ext_flash`` and ``zigbee_fota_mcuboot_ext_flash`` to ``zigbee_fota`` and ``zigbee_fota_mcuboot``.
    One application snippet still covers all supported FOTA boards; board overlays select ``dts/ext_flash/`` or ``dts/fota_internal/`` as needed.
  * Corrected LED assignments for the :ref:`zigbee_light_bulb_sample` and user interface references in the documentation.
  * Default thread stack sizes for add-on applications: ``CONFIG_MAIN_STACK_SIZE`` 1280,
    ``CONFIG_ZBOSS_DEFAULT_THREAD_STACK_SIZE`` 3328, and ``CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE`` 1024.
    ``CONFIG_ZIGBEE_BT_DFU`` still uses a larger system workqueue for SMP.
  * Removed the stack memory requirements section from the :ref:`Memory requirements <zigbee_memory>` page.

* Removed:

  * Precompiled NCP development libraries (``libncp-dev``) and a matching ZBOSS NCP Host package release.
    The :ref:`NCP sample <zigbee_ncp_sample>` cannot be built in this release; see :ref:`known_issues` (KRKNWK-22663).

* Migration notes:

  * Follow :ref:`migration_zboss5` when upgrading from |addon| v1.4.x (ZBOSS 4.2).
  * FOTA builds must pass ``SNIPPET=zigbee_fota`` instead of ``zigbee_fota_ext_flash``.
    Sysbuild still remaps the MCUboot image to ``zigbee_fota_mcuboot`` automatically.
  * Applications that target Zigbee 3.0 product certification must now explicitly set ``CONFIG_ZIGBEE_PRO_CORE_BEHAVIOR_R22`` to ``y``.
  * Coordinators now require install codes for all joining devices by default, following the ZBOSS 5 high-security Trust Center policy.
    Add the install code of each device on the Trust Center before it joins (see :ref:`zigbee_install_code_commissioning`), or build demonstration setups with the ``zigbee_demo_commissioning`` snippet (see :ref:`zigbee_commissioning_security`).
  * In the :ref:`zigbee_light_bulb_sample` on nRF52 and nRF53 DKs, use LED 2 instead of LED 4 for the dimmable light output.
    In FOTA builds of the :ref:`zigbee_light_bulb_sample`, use LED 4 instead of LED 2 for FOTA activity.