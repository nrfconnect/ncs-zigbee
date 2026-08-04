.. _zigbee_memory:

Memory requirements
###################

.. contents::
   :local:
   :depth: 2

This page provides information about the amount of flash memory and RAM that is required by the :ref:`zigbee_samples`, as well as stack memory requirements for ``main`` and ``zboss`` threads.
Use it to check if your application has enough space for a given configuration.
Values are provided for :ref:`ZBOSS libraries <zigbee_zboss>`.

Samples were built using the available :ref:`zigbee_zboss`.
Unless stated otherwise, the default :file:`prj.conf` was used.

Memory layout in DTS
********************

The :ref:`zigbee_samples` define flash partitions in Devicetree (DTS).
The |addon| ships ready-made partition files under :file:`dts/` in the |addon| repository.
Use them as-is, or copy and adjust them for your board.

Each layout starts with a base :file:`.dtsi` file for your SoC.
To use MCUboot or FOTA together, include a matching :file:`.dtsi` file after the base file.
Some :file:`.dtsi` files apply to any board with a given SoC.
Others apply only to a specific development kit (DK).

Every base layout reserves the end of internal flash for:

* ``storage_partition`` - Zephyr settings storage
* ``zboss_nvram`` - ZBOSS non-volatile data
* ``zboss-product-config`` - ZBOSS product configuration

The nRF52840 Dongle layout reserves a further 128 KB at the top of flash for the factory-programmed Nordic Open USB DFU bootloader, which shrinks ``slot0_partition`` accordingly.
Keep that region free in derived layouts: writing to it erases the bootloader settings, after which the bootloader rejects the application and stays in DFU mode.

The following table lists the available files, which targets they cover, and when to use each variant:

.. list-table:: DTS partition layout variants
   :header-rows: 1

   * - Variant
     - Path
     - Targets
     - When to use

   * - Base
     - :file:`dts/<soc>_partitions.dtsi`
     - | nRF52833 (SoC)
       | nRF52840 (SoC)
       | nRF52840 Dongle (DK only)
       | nRF5340 application and network cores (SoC)
       | nRF54L05 (SoC)
       | nRF54L10 (SoC)
       | nRF54L15 (SoC)
       | nRF54LM20 (SoC)
     - Required starting point for any Zigbee application.

   * - MCUboot (USB transport)
     - :file:`dts/mcuboot_usb_transport/<soc>_partitions.dtsi`
     - | nRF52833 (SoC)
       | nRF52840 (SoC)
       | nRF5340 application core (SoC)
       | nRF54LM20 (SoC)
     - Adds ``boot_partition`` (MCUboot) and resizes ``slot0_partition`` to fit.
       Use with sysbuild when the bootloader is updated over USB.

   * - External flash (FOTA)
     - :file:`dts/ext_flash/<board>_partitions.dtsi`
     - | nRF52833 DK (DK only)
       | nRF52840 DK (DK only)
       | nRF5340 DK (DK only)
       | nRF54L10 DK (DK only)
       | nRF54L15 DK (DK only)
       | nRF54LM20 DK (DK only)
     - Keeps the primary image on internal flash and places the secondary upgrade slot (``slot1_partition``) on external flash.
       Adds ``boot_partition`` (MCUboot) and resizes ``slot0_partition`` to fit.
       On the nRF5340 DK, external flash can also hold ``image-3``.
       Reference layout for :ref:`lib_zigbee_fota` over external flash.

To add an overlay file to your application, add an ``#include`` line to :file:`app.overlay` or to a board overlay in :file:`boards/`.
Always include the base :file:`.dtsi` first, then any optional overlay.

See an example for the nRF54LM20 DK with external-flash FOTA in the :file:`boards/nrf54lm20dk_nrf54lm20a_cpuapp.overlay` file:

.. code-block:: dts

   #include <nrf54lm20_cpuapp_partitions.dtsi>
   #include <ext_flash/nrf54lm20dk_cpuapp_partitions.dtsi>

RAM and flash memory requirements
*********************************

RAM and flash memory requirement values differ depending on the programmed sample.

The following tables and bar charts list memory requirement values for Zigbee samples.

Memory layout is taken from the DTS files used by each sample variant, while memory usage is taken from the build output.

Values are provided in kilobytes (KB).
Unsupported sample variants are omitted from each board view.
``--`` indicates that a partition is not used by the sample configuration.

Table columns are grouped by internal NVM, external NVM (when used), and RAM.
Application, MCUboot, upgrade slot, and RAM cells show used and free space separated by ``/``.
Other NVM columns list the reserved partition size for that region.

.. tabs::

   .. group-tab:: Charts

      .. tabs::

         .. group-tab:: nRF52833

            Memory requirements for samples running on the `nRF52833 DK <Board support_>`_ (`nrf52833dk`_).

            .. memory-board::
               :board: nrf52833

         .. group-tab:: nRF52840

            Memory requirements for samples running on the `nRF52840 DK <Board support_>`_ (`nrf52840dk`_).

            .. memory-board::
               :board: nrf52840

         .. group-tab:: nRF52840 Dongle

            Memory requirements for samples running on the `nRF52840 Dongle DK <Board support_>`_  (`nrf52840dongle`_).

            .. memory-board::
               :board: nrf52840_dongle

         .. group-tab:: nRF5340

            Memory requirements for samples running on the `nRF5340 DK <Board support_>`_ (`nrf5340dk`_).

            .. memory-board::
               :board: nrf5340

         .. group-tab:: nRF54L05

            Memory requirements for samples running on the `nRF54L05 emulation on the nRF54L15 DK <Board support_>`_ (`nrf54l15dk`_).

            .. memory-board::
               :board: nrf54l05

         .. group-tab:: nRF54L10

            Memory requirements for samples running on the `nRF54L10 emulation on the nRF54L15 DK <Board support_>`_ (`nrf54l15dk`_).

            .. memory-board::
               :board: nrf54l10

         .. group-tab:: nRF54L15

            Memory requirements for samples running on the `nRF54L15 DK <Board support_>`_ (`nrf54l15dk`_).

            .. memory-board::
               :board: nrf54l15

         .. group-tab:: nRF54LM20

            Memory requirements for samples running on the `nRF54LM20 DK <Board support_>`_ (`nrf54lm20dk`_) with the ``nrf54lm20dk/nrf54lm20a/cpuapp`` or ``nrf54lm20dk/nrf54lm20b/cpuapp`` board targets.

            .. memory-board::
               :board: nrf54lm20

   .. group-tab:: Tables

      .. tabs::

         .. group-tab:: nRF52833

            The following table lists memory requirements for samples running on the `nRF52833 DK <Board support_>`_ (`nrf52833dk`_).

            .. memory-table::
               :board: nrf52833

         .. group-tab:: nRF52840

            The following table lists memory requirements for samples running on the `nRF52840 DK <Board support_>`_ (`nrf52840dk`_).

            .. memory-table::
               :board: nrf52840

         .. group-tab:: nRF52840 Dongle

            The following table lists memory requirements for samples running on the `nRF52840 Dongle DK <Board support_>`_  (`nrf52840dongle`_).

            .. memory-table::
               :board: nrf52840_dongle

         .. group-tab:: nRF5340

            The following table lists memory requirements for samples running on the `nRF5340 DK <Board support_>`_ (`nrf5340dk`_).

            .. memory-table::
               :board: nrf5340

         .. group-tab:: nRF54L05

            The following table lists memory requirements for samples running on the `nRF54L05 emulation on the nRF54L15 DK <Board support_>`_ (`nrf54l15dk`_).

            .. memory-table::
               :board: nrf54l05

         .. group-tab:: nRF54L10

            The following table lists memory requirements for samples running on the `nRF54L10 emulation on the nRF54L15 DK <Board support_>`_ (`nrf54l15dk`_).

            .. memory-table::
               :board: nrf54l10

         .. group-tab:: nRF54L15

            The following table lists memory requirements for samples running on the `nRF54L15 DK <Board support_>`_ (`nrf54l15dk`_).

            .. memory-table::
               :board: nrf54l15

         .. group-tab:: nRF54LM20

            The following table lists memory requirements for samples running on the `nRF54LM20 DK <Board support_>`_ (`nrf54lm20dk`_) with the ``nrf54lm20dk/nrf54lm20a/cpuapp`` or ``nrf54lm20dk/nrf54lm20b/cpuapp`` board targets.

            .. memory-table::
               :board: nrf54lm20


Stack memory requirements
*************************

The following tables and bar charts list stack memory requirement values for ``main`` and ``zboss`` threads.
These measurements were carried out with Zephyr's `Thread analyzer`_, using the following Kconfig options for configuration:

* For the stack memory size of the ``main`` thread - ``CONFIG_MAIN_STACK_SIZE``
* For the stack memory size of the ``zboss`` thread - ``CONFIG_ZBOSS_DEFAULT_THREAD_STACK_SIZE``

Values are provided in bytes (B).

.. tabs::

   .. group-tab:: Charts

      .. tabs::

         .. group-tab:: nRF52840

            Stack usage for samples running on the `nRF52840 DK <Board support_>`_ (`nrf52840dk`_).

            .. stack-board::
               :board: nrf52840

         .. group-tab:: nRF5340

            Stack usage for samples running on the `nRF5340 DK <Board support_>`_ (`nrf5340dk`_).

            .. stack-board::
               :board: nrf5340

         .. group-tab:: nRF54L15

            Stack usage for samples running on the `nRF54L15 DK <Board support_>`_ (`nrf54l15dk`_).

            .. stack-board::
               :board: nrf54l15

         .. group-tab:: nRF54LM20

            Stack usage for samples running on the `nRF54LM20 DK <Board support_>`_ (`nrf54lm20dk`_) with the ``nrf54lm20dk/nrf54lm20a/cpuapp`` or ``nrf54lm20dk/nrf54lm20b/cpuapp`` board targets.

            .. stack-board::
               :board: nrf54lm20

   .. group-tab:: Tables

      .. tabs::

         .. group-tab:: nRF52840

            The following table lists stack usage for samples running on the `nRF52840 DK <Board support_>`_ (`nrf52840dk`_).

            .. stack-table::
               :board: nrf52840

         .. group-tab:: nRF5340

            The following table lists stack usage for samples running on the `nRF5340 DK <Board support_>`_ (`nrf5340dk`_).

            .. stack-table::
               :board: nrf5340

         .. group-tab:: nRF54L15

            The following table lists stack usage for samples running on the `nRF54L15 DK <Board support_>`_ (`nrf54l15dk`_).

            .. stack-table::
               :board: nrf54l15

         .. group-tab:: nRF54LM20

            The following table lists stack usage for samples running on the `nRF54LM20 DK <Board support_>`_ (`nrf54lm20dk`_) with the ``nrf54lm20dk/nrf54lm20a/cpuapp`` or ``nrf54lm20dk/nrf54lm20b/cpuapp`` board targets.

            .. stack-table::
               :board: nrf54lm20
