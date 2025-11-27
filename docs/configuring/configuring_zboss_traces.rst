.. _ug_zigbee_configuring_zboss_traces:

Configuring ZBOSS traces
########################

.. contents::
   :local:
   :depth: 2

The :ref:`zigbee_zboss` (ZBOSS stack) comes included in the |NCS| in a set of precompiled libraries, which can complicate the debugging process.
To help with that, the ZBOSS stack can be configured to print trace logs that allow you to trace the stack behavior.
This page describes how to enable and configure ZBOSS trace logs.

Trace logs are printed in binary form and require access to stack source files to be decoded into log messages.
Enabling trace logs can help with the debugging process even if you cannot decode them (due to lack of access to stack source files).
This is because of the information they can provide about the stack behavior whenever an issue is found or reproduced.

If any issues are found with the ZBOSS stack, trace logs can be enabled to collect this information while reproducing the issue.
A ticket can then be created in `DevZone`_ with the log file attached for assistance with the debugging process, and for decoding trace log files.

.. note::
     ZBOSS trace logs are meant to be used for debugging ZBOSS based applications.
     These libraries are not certified and should not be used for production firmware or end products.


Requirements
************

To collect and view the log files, you will need the following:

* A terminal emulator, such as `nRF Connect Serial Terminal`_ (from the `nRF Connect for Desktop`_ application) or the nRF Terminal (part of the `nRF Connect for Visual Studio Code`_ extension).

* Correct COM port.

   * For traces using the UART, the J-Link COM port is used. The development kit is assigned to a COM port (Windows) or a ttyACM device (Linux), which is visible in the system's Device Manager.

.. rst-class:: numbered-step

Switch to ZBOSS libraries with compiled-in trace logs
*****************************************************

Set the Kconfig option ``CONFIG_ZIGBEE_ENABLE_TRACES`` to switch to ZBOSS libraries with compiled-in trace logs.

The ZBOSS stack comes in a precompiled form and trace logs are not compiled-in by default.
An additional set of ZBOSS libraries are available in the :file:`lib` folder, which does have trace logs compiled-in.


.. rst-class:: numbered-step

Select which ZBOSS trace logs to print
**************************************

Complete the following steps:

1. Select from which subsystems you would like to receive logs by configuring the ZBOSS trace mask with the Kconfig option ``CONFIG_ZBOSS_TRACE_MASK``.
   Trace masks can be created by adding up masks of subsystems to receive the trace logs from.
   For available subsystems, see :file:`lib/zboss/include/zb_trace.h`.

#. Select the level of logs you want to receive.
   Configure ZBOSS trace level by selecting one of the following levels with the corresponding Kconfig option:

   * Error trace logs level - Set ``CONFIG_ZBOSS_TRACE_LOG_LEVEL_ERR``
   * Warning trace logs level - Set ``CONFIG_ZBOSS_TRACE_LOG_LEVEL_WRN``
   * Info trace logs level - Set ``CONFIG_ZBOSS_TRACE_LOG_LEVEL_INF``
   * Debug trace logs level - Set ``CONFIG_ZBOSS_TRACE_LOG_LEVEL_DBG``

If you do not want to receive trace logs, turn them off by setting the Kconfig option ``CONFIG_ZBOSS_TRACE_LOG_LEVEL_OFF``.

Each of the following levels on the list also includes the previous one.
See :ref:`zigbee_ug_logging_stack_logs` to read more about trace logs.


.. rst-class:: numbered-step

Configure how to print ZBOSS trace logs
***************************************

The :ref:`zigbee_osif_zboss_osif_serial` offers a few backends to choose from for printing ZBOSS trace logs.
It is recommended to use the Zigbee serial logger, as it is the most efficient.
To enable it, set the Kconfig option ``CONFIG_ZBOSS_TRACE_BINARY_LOGGING``.

..
   Optional: Increasing the size of the ring buffer
      You can increase size of the ring buffer that temporarily stores the trace logs.
      To do this, use the ``CONFIG_ZBOSS_TRACE_LOGGER_BUFFER_SIZE`` Kconfig option to assign a value for size of the buffer.
      This can prevent losing some of the logs in demanding scenarios such as high network traffic, multiple devices being configured or joined, and so on.
      See :ref:`Zigbee serial logger <zigbee_osif_zigbee_async_serial>` for more information.

Trace logs using UART (default)
===============================

When the ``CONFIG_ZBOSS_TRACE_BINARY_LOGGING`` Kconfig option is selected, trace logs are printed using the UART by default.
To configure trace logs using the UART, complete the following steps:

1. Set the ``CONFIG_ZBOSS_TRACE_UART_LOGGING`` Kconfig option.

#. Provide the ZBOSS tracing serial device in Devicetree like this:

   .. code-block:: devicetree

      chosen {
          ncs,zboss-trace-uart = &uart1;
      };

#. Configure the UART device that you want to use to be connected to the onboard J-Link instead of ``UART_0``, by extending the DTS overlay file for the selected board with the following:

   .. code-block:: devicetree

      &pinctrl {
         uart0_default_alt: uart0_default_alt {
            group1 {
               psels = <NRF_PSEL(UART_TX, 1, 2)>,
                       <NRF_PSEL(UART_RX, 1, 1)>;
            };
         };

         uart0_sleep_alt: uart0_sleep_alt {
            group1 {
               psels = <NRF_PSEL(UART_TX, 1, 2)>,
                       <NRF_PSEL(UART_RX, 1, 1)>;
               low-power-enable;
            };
         };

         uart1_default_alt: uart1_default_alt {
            group1 {
               psels = <NRF_PSEL(UART_TX, 0, 6)>,
                       <NRF_PSEL(UART_RX, 0, 8)>,
                       <NRF_PSEL(UART_RTS, 0, 5)>,
                       <NRF_PSEL(UART_CTS, 0, 7)>;
            };
         };

         uart1_sleep_alt: uart1_sleep_alt {
            group1 {
               psels = <NRF_PSEL(UART_TX, 0, 6)>,
                       <NRF_PSEL(UART_RX, 0, 8)>,
                       <NRF_PSEL(UART_RTS, 0, 5)>,
                       <NRF_PSEL(UART_CTS, 0, 7)>;
               low-power-enable;
            };
         };
      };

      &uart1 {
         pinctrl-0 = <&uart1_default_alt>;
         pinctrl-1 = <&uart1_sleep_alt>;
         pinctrl-names = "default", "sleep";
      };

      &uart0 {
         pinctrl-0 = <&uart0_default_alt>;
         pinctrl-1 = <&uart0_sleep_alt>;
         pinctrl-names = "default", "sleep";
      };

   .. note::
      By connecting the UART device to the on-board J-Link, trace logs can be read directly from the J-Link COM port.
      As a consequence, the UART device used by the logger is disconnected and application logs cannot be accessed from the J-Link COM port.


Optional: Increasing the UART throughput
   You can also increase the UART throughput by changing the baudrate.
   Some of the trace logs will be dropped if the throughput is too low.
   By default, the UART baudrate is set to ``115200``.
   To increase the baudrate to ``1000000``, add the ``current-speed = <1000000>;`` property to the ``uart1`` node in the DTS overlay file.
   This can be done like the following:

   .. code-block:: devicetree

      &pinctrl {
         uart1_default_alt: uart1_default_alt {
            group1 {
               psels = <NRF_PSEL(UART_TX, 0, 6)>,
                       <NRF_PSEL(UART_RX, 0, 8)>,
                       <NRF_PSEL(UART_RTS, 0, 5)>,
                       <NRF_PSEL(UART_CTS, 0, 7)>;
            };
         };

         uart1_sleep_alt: uart1_sleep_alt {
            group1 {
               psels = <NRF_PSEL(UART_TX, 0, 6)>,
                       <NRF_PSEL(UART_RX, 0, 8)>,
                       <NRF_PSEL(UART_RTS, 0, 5)>,
                       <NRF_PSEL(UART_CTS, 0, 7)>;
               low-power-enable;
            };
         };
      };

      &uart1 {
         current-speed = <1000000>;
         pinctrl-0 = <&uart1_default_alt>;
         pinctrl-1 = <&uart1_sleep_alt>;
         pinctrl-names = "default", "sleep";
      };

Trace logs using native USB
===========================

.. note::
  The nRF54L Series devices do not support the USB peripheral.

Trace logs can be configured to use a native USB connection. 
This allows trace logs to be printed through a separate virtual COM port, so that console logs remain accessible through the J-Link COM port.
For applications that depend on the UART connection through the J-Link COM port, such as the Network Co-Processor (NCP) sample, trace logs can only be sent over USB (using a USB-emulated COM port). 
For information on configuring USB trace logs in this scenario, see the :ref:`Zigbee NCP <zigbee_ncp_sample>` documentation page.

.. note::
   Before proceeding with the following steps, first check if your Zigbee application has USB enabled or is currently using USB.
   If your application is already using a virtual COM port through native USB, use a device name that is different from the default ``CDC_ACM_0`` when creating a new virtual COM port for printing trace logs.
   For example, if ``CDC_ACM_0`` is already in use, create a virtual COM port named ``CDC_ACM_1``.
   Additionally, you must set the ``CONFIG_USB_COMPOSITE_DEVICE`` Kconfig option when configuring multiple virtual COM ports.

   Refer to the :ref:`Zigbee NCP <zigbee_ncp_sample>` documentation page for an example in which one virtual COM port instance is already configured, and an additional instance must be created.

To configure trace logs using native USB, complete the following steps:

1. Set the Kconfig option ``CONFIG_ZBOSS_TRACE_USB_CDC_LOGGING``.
   This also enables the necessary USB Kconfig options.

#. Create a virtual COM port that will be used for printing ZBOSS trace logs by extending the DTS overlay file for the selected board by running the following command:

   .. code-block:: devicetree

      &zephyr_udc0 {
         cdc_acm_uart0: cdc_acm_uart0 {
            compatible = "zephyr,cdc-acm-uart";
            label = "CDC_ACM_0";
         };
      };

   .. note::
      For the ZBOSS trace logs to be printed correctly through the USB, it is recommended to avoid using the USB autosuspend.

#. Provide the ZBOSS tracing serial device in Devicetree as follows:

   .. code-block:: devicetree

      chosen {
          ncs,zboss-trace-uart = &cdc_acm_uart0;
      };
