.. _zigbee_memory:

Memory requirements
###################

.. contents::
   :local:
   :depth: 2

This page provides information about the amount of flash memory and RAM that is required by the :ref:`zigbee_samples`, as well as stack memory requirements for ``main`` and ``zboss`` threads.
Use it to check if your application has enough space for a given configuration.
Values are provided for :ref:`ZBOSS libraries <zigbee_zboss>`.

.. note::
   .. include:: /includes/experimental_note.txt

Samples were built using the available :ref:`zigbee_zboss`.
Unless stated otherwise, the default :file:`prj.conf` was used.

RAM and flash memory requirements
*********************************

RAM and flash memory requirement values differ depending on the programmed sample.

The following tables list memory requirement values for Zigbee samples.

Values are provided in kilobytes (KB).
``n/a`` indicates that the sample with the given variant is not supported on the DK.

.. tabs::

   .. tab:: nRF54L15

      The following table lists memory requirements for samples running on the `nRF54L15 DK <Developing with nRF54L Series_>`_ (`nrf54l15dk`_).

      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | Sample                                                                                                                 | ROM, ZBOSS stack + App   | ROM, MCUboot bootloader   | ROM, ZBOSS non-volatile memory   | ROM, ZBOSS product config   | Total ROM   | RAM, ZBOSS stack + App   | Total RAM   |
      +========================================================================================================================+==========================+===========================+==================================+=============================+=============+==========================+=============+
      | :ref:`Network coordinator <zigbee_network_coordinator_sample>`                                                         | 394                      | 0                         | 32                               | 4                           | 430         | 124                      | 124         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light bulb <zigbee_light_bulb_sample>`                                                                           | 418                      | 0                         | 32                               | 4                           | 454         | 69                       | 69          |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light switch <zigbee_light_switch_sample>`                                                                       | 319                      | 0                         | 32                               | 4                           | 355         | 55                       | 55          |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light switch <zigbee_light_switch_sample>` with :ref:`zigbee_light_switch_sample_nus`                            | 446                      | 0                         | 32                               | 4                           | 482         | 72                       | 72          |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`NCP <zigbee_ncp_sample>`                                                                                         | 392                      | 0                         | 32                               | 4                           | 428         | 119                      | 119         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Zigbee application template <zigbee_template_sample>`                                                            | 392                      | 0                         | 32                               | 4                           | 428         | 100                      | 100         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Zigbee shell <zigbee_shell_sample>`                                                                              | 472                      | 0                         | 32                               | 4                           | 508         | 117                      | 117         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+

Stack memory requirements
*************************

The following table lists stack memory requirement values for ``main`` and ``zboss`` threads.
These measurements were carried out with Zephyr's `Thread analyzer`_, using the following Kconfig options for configuration:

* For the stack memory size of the ``main`` thread - ``CONFIG_MAIN_STACK_SIZE``
* For the stack memory size of the ``zboss`` thread - ``CONFIG_ZBOSS_DEFAULT_THREAD_STACK_SIZE``

Values are provided in bytes (B).

.. tabs::

   .. tab:: nRF54L15

      The following table lists memory requirements for samples running on the `nRF54L15 DK <Developing with nRF54L Series_>`_ (`nrf54l15dk`_).

      +----------------------------------------------------------------+-------------------------------+------------------------------+--------------------------------+-------------------------------+
      | Sample                                                         | ``main`` thread stack usage   | ``main`` thread stack size   | ``zboss`` thread stack usage   | ``zboss`` thread stack size   |
      +================================================================+===============================+==============================+================================+===============================+
      | :ref:`Network coordinator <zigbee_network_coordinator_sample>` | 872                           | 2048                         | 2056                           | 5120                          |
      +----------------------------------------------------------------+-------------------------------+------------------------------+--------------------------------+-------------------------------+
      | :ref:`Light bulb <zigbee_light_bulb_sample>`                   | 872                           | 2048                         | 736                            | 5120                          |
      +----------------------------------------------------------------+-------------------------------+------------------------------+--------------------------------+-------------------------------+
      | :ref:`Light switch <zigbee_light_switch_sample>`               | 944                           | 2048                         | 736                            | 5120                          |
      +----------------------------------------------------------------+-------------------------------+------------------------------+--------------------------------+-------------------------------+
