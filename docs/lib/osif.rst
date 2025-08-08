.. _lib_zigbee_osif:

Zigbee ZBOSS OSIF
#################

.. contents::
   :local:
   :depth: 2

The Zigbee ZBOSS OSIF layer subsystem acts as the linking layer between the :ref:`zigbee_zboss` and the |addon| for the |NCS|.

.. _zigbee_osif_configuration:

Configuration
*************

The ZBOSS OSIF layer implements a series of functions used by ZBOSS and is included in the Zigbee subsystem.

The ZBOSS OSIF layer is automatically enabled when you enable the ZBOSS library with the ``CONFIG_ZIGBEE_ADD_ON`` Kconfig option.

You can also configure the following OSIF-related Kconfig options:

* ``CONFIG_ZIGBEE_APP_CB_QUEUE_LENGTH`` - Defines the length of the application callback and alarm queue.
  This queue is used to pass application callbacks and alarms from other threads or interrupts to the ZBOSS main loop context.
* ``CONFIG_ZIGBEE_DEBUG_FUNCTIONS`` - Includes functions to suspend and resume the ZBOSS thread.

  .. note::
      These functions are useful for debugging, but they can cause instability of the device.

* ``CONFIG_ZBOSS_RESET_ON_ASSERT`` - Configures the ZBOSS OSIF layer to reset the device when a ZBOSS assert occurs.
  This option is enabled by default.
  Use it for production-ready applications.
* ``CONFIG_ZBOSS_HALT_ON_ASSERT`` - Configures the ZBOSS OSIF layer to halt the device when a ZBOSS assert occurs.
  Use this option only for testing and debugging your application.
* ``CONFIG_ZIGBEE_HAVE_SERIAL`` - Enables the UART serial abstract for the ZBOSS OSIF layer and allows to configure the serial glue layer.
  For more information, see the :ref:`zigbee_osif_zboss_osif_serial` section.

  .. note::
      Serial abstract must be enabled when using the precompiled ZBOSS libraries since they have dependencies on it.

* ``CONFIG_ZIGBEE_USE_BUTTONS`` - Enables the buttons abstract for the ZBOSS OSIF layer.
  You can use this option if you want to test ZBOSS examples directly in the |addon| for the |NCS|.
* ``CONFIG_ZIGBEE_USE_DIMMABLE_LED`` - Dimmable LED (PWM) abstract for the ZBOSS OSIF layer.
  You can use this option if you want to test ZBOSS examples directly in the |addon| for the |NCS|.
* ``CONFIG_ZIGBEE_USE_LEDS`` - LEDs abstract for the ZBOSS OSIF layer.
  You can use this option if you want to test ZBOSS examples directly in the |addon| for the |NCS|.
* ``CONFIG_ZIGBEE_USE_SOFTWARE_AES`` - Configures the ZBOSS OSIF layer to use the software encryption.
* ``CONFIG_ZIGBEE_NVRAM_PAGE_COUNT`` - Configures the number of ZBOSS NVRAM logical pages.
* ``CONFIG_ZIGBEE_NVRAM_PAGE_SIZE`` - Configures the size of the RAM-based ZBOSS NVRAM.
  This option is used only if the device does not have NVRAM storage.
* ``CONFIG_ZIGBEE_TIME_COUNTER`` - Configures the ZBOSS OSIF layer to use a dedicated timer-based counter as the Zigbee time source.
* ``CONFIG_ZIGBEE_TIME_KTIMER`` - Configures the ZBOSS OSIF layer to use Zephyr's system time as the Zigbee time source.

Additionally, the following Kconfig option is available when setting :ref:`zigbee_ug_logging_logger_options`:

* ``CONFIG_ZBOSS_OSIF_LOG_LEVEL`` - Configures the custom logger options for the ZBOSS OSIF layer.

.. _zigbee_osif_zboss_osif_serial:

ZBOSS OSIF serial abstract
**************************

Setting the ``CONFIG_ZIGBEE_HAVE_SERIAL`` Kconfig option enables the serial abstract for the ZBOSS OSIF layer.

The ZBOSS OSIF serial implements sets of backend functions that are used by the ZBOSS stack for serial communication:

* Zigbee async serial
* Zigbee serial logger
* Zigbee logger

These backend functions serve logging ZBOSS traces that are useful for debugging and are provided in binary format.


These backend functions serve one or both of the following purposes:

* Logging ZBOSS traces - Used for handling stack logs that are useful for debugging and are provided in binary format.
* Handling NCP communication with the host device - Used only for the :ref:`NCP architecture <ug_zigbee_platform_design_ncp>`.

The following table shows which sets of functions serve which purpose.

.. _osif_table:

+----------------------------+---------------+---------------+----------+
|                            | Async serial  | Serial logger | Logger   |
+============================+===============+===============+==========+
| Logging ZBOSS traces       | -             | -             | -        |
+----------------------------+---------------+---------------+----------+
| Handling NCP communication | -             |               |          |
+----------------------------+---------------+---------------+----------+

For more information about configuring ZBOSS stack logs, see :ref:`zigbee_ug_logging_stack_logs`.

.. _zigbee_osif_zigbee_async_serial:

Zigbee async serial
===================

..
  The Zigbee async serial is the only backend that the ZBOSS OSIF serial supports for handling the NCP communication.
  This set of functions uses `UART API`_ and can be configured to use UART peripheral.
  The data received are internally buffered.

  You can also use the Zigbee async serial for logging ZBOSS traces.
  When enabled, it logs ZBOSS traces in the binary format.
  In such case, the transmission data is also buffered.

The Zigbee async serial supports logging ZBOSS traces. When enabled, it logs ZBOSS traces in the binary format.
The data received are internally buffered.


Zigbee async serial configuration options
-----------------------------------------

To configure this set of functions, use the following Kconfig options:

* ``CONFIG_ZIGBEE_HAVE_ASYNC_SERIAL`` - This option enables Zigbee async serial.
* ``CONFIG_ZIGBEE_UART_SUPPORTS_FLOW_CONTROL`` - This option should be set if serial device supports flow control.
* ``CONFIG_ZIGBEE_UART_RX_BUF_LEN`` - This option enables and configures the size of internal RX and TX buffer.
* ``CONFIG_ZBOSS_TRACE_BINARY_NCP_TRANSPORT_LOGGING`` - This option enables logging ZBOSS traces in binary format with Zigbee async serial.

The Zigbee ZBOSS OSIF layer serial device needs to be provided in devicetree as follows:

.. code-block:: devicetree

   chosen {
       ncs,zigbee-uart = &uart20;
   };

Zigbee serial logger
====================

This set of functions uses Zephyr's `UART API`_ and can be configured to use either the UART peripheral.
Data is buffered internally in ring buffer and printed in the binary format.
This ring buffer has a size of 4096 bytes by default.

Zigbee serial logger configuration options
------------------------------------------

Use the following Kconfig options to configure the Zigbee serial logger:

* ``CONFIG_ZBOSS_TRACE_BINARY_LOGGING`` - This option enables logging ZBOSS traces with Zigbee serial logger.
* ``CONFIG_ZBOSS_TRACE_UART_LOGGING`` - This option selects the UART serial backend.

..
  * ``CONFIG_ZBOSS_TRACE_LOGGER_BUFFER_SIZE`` - This option specifies the size of the internal ring buffer.

The ZBOSS tracing serial device needs to be provided in Devicetree like this:

.. code-block:: devicetree

   chosen {
       ncs,zboss-trace-uart = &uart21;
   };

Zigbee logger
=============

This set of functions uses Zephyr's `Logging`_ API for logging hexdumps of received binary data.
Data is buffered internally in ring buffer.

Zigbee logger configuration options
-----------------------------------

Use the following Kconfig options to configure the Zigbee logger:

* ``CONFIG_ZBOSS_TRACE_HEXDUMP_LOGGING`` - This option enables Logging ZBOSS Traces with Zigbee logger.

..
  * ``CONFIG_ZBOSS_TRACE_LOGGER_BUFFER_SIZE`` - This option specifies size of internal ring buffer.

API documentation
*****************

| Header files: :file:`subsys/osif/zb_nrf_platform.h`
| Source files: :file:`subsys/osif/`

.. doxygengroup:: zigbee_zboss_osif
   :members:
