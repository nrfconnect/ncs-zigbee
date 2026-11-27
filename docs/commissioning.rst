.. _ug_zigbee_commissioning:

Commissioning
#############

.. contents::
   :local:
   :depth: 2

Commissioning is a process that allows a new Zigbee device to join a Zigbee network.
The device is configured into the network, so that it can start communicating with other network nodes.
If there is no network to join, the commissioning procedure ensures that a new network is created.

You can configure Zigbee commissioning by setting parameters before the start of the :ref:`zigbee_zboss`.
For more information, see the `Support for Zigbee commissioning`_ section in the ZBOSS stack documentation.

.. _zigbee_commissioning_start:

Start of commissioning
**********************

There are several ways the commissioning process can be started, according to the `Zigbee Specification <CSA Specifications Download Request_>`_:

* Automatically after device initialization.
* With a user action, for example a button press or a command.
* Over the air, using a mechanism defined in the Identify cluster.

In the ZBOSS stack, the commissioning can be started using one of the following options:

* `zboss_start()`_ that initializes the stack and starts the BDB commissioning procedure with the default modes (Network Steering, Network Formation, Finding and Binding).
* `bdb_start_top_level_commissioning()`_ that should be started after the stack initialization (for example, from the ZBOSS signal handler) and requires providing the commissioning mode's `zb_bdb_commissioning_mode_mask_e`_ mask.

For more information, see the `Stack commissioning start sequence`_ section in the ZBOSS stack documentation.

With the |addon|, the application can use :ref:`lib_zigbee_signal_handler` to handle the ZBOSS signals and start the commissioning.

.. _zigbee_commissioning_modes:

Commissioning modes
*******************

Zigbee commissioning is organized into four different modes:

* Touchlink commissioning (optional; provided in the |addon| for the |NCS| as an experimental feature with basic functionality)
* Network Steering
* Network Formation
* Finding and Binding (optional)

These modes are triggered by the top-level commissioning procedure.
The procedure goes through one or more modes, depending on the bits specified in the ``bdbCommissioningMode`` mask (``0`` through ``3``).
A node can have one or more of these bits set.

The following figure shows a generic scenario where all modes are used, together with their corresponding ``bdbCommissioningMode`` mask bits:

.. figure:: images/zigbee_commissioning_overview.svg
   :alt: Simplified top-level Zigbee commissioning procedure

   Generic top-level Zigbee commissioning procedure

This top-level procedure defines the intended order of execution of Zigbee commissioning modes.
The commissioning procedure is common for all device roles, but some modes might not be executed for a given role.
For example, Network Formation is not executed for End Devices.

See the following sections for details about each mode.

.. _zigbee_commissioning_modes_touchlink:

Touchlink
=========

.. note::
   The Touchlink mode is optional in Zigbee.
   In the |addon| for the |NCS|, it is provided as an experimental feature with basic functionality through the ``CONFIG_ZIGBEE_TOUCHLINK_INITIATOR`` and ``CONFIG_ZIGBEE_TOUCHLINK_TARGET`` Kconfig options.

The Touchlink mode allows commissioning of devices in close proximity to each other, which means that the key criterium for this mode is the signal strength.
It involves an initiator device and a target device, where the initiator is a member of an existing Zigbee network or it can form a new network.
Once the network is created, the initiator looks for the target on four channels by sending InterPAN frames.
When the target is found, the initiator can create a local binding with a remote endpoint or request adding the remote endpoint to a group.
The target first identifies itself with the initiator and then receives requests from it and responds to them.

Touchlink only supports `Distributed security network`_ and it is provided as a ZCL cluster, which needs to be implemented on both the initiator and the target devices.

.. _zigbee_commissioning_modes_ns:

Network Steering
================

The Network Steering mode is used for finding open networks that a device can attempt to join or open a network for a specified period of time.

.. figure:: images/zigbee_commissioning_steering.svg
   :alt: Zigbee Network Steering procedure

   Zigbee Network Steering procedure

How the node acts in this mode depends on whether the node is in a Zigbee network or not:

* If the node is already a member of a Zigbee network, it opens the network and then closes it after the specified period of time (180 seconds by default).
  The opening of the network is triggered when any of the Router or Coordinator nodes broadcasts the permit join request.
* If the node is not a member of a Zigbee network, it scans channels for an open network to associate with.
  Once it finds a suitable network, it attempts to join it and get the credentials.
  If the node finds more than one available network, it will attempt to join one of them, one by one.

If the node does not find a network to join and it has either the Coordinator or the Router role, it can attempt to form a new network (see `Network Formation`_).
If the node has the End Device role, its commissioning procedure stops here.

If the node finds a network to join, it can continue to the `Finding and Binding`_ procedure if it is requested with the appropriate bit mask value.

The new device must be authenticated and authorized to become part of the network.
For this purpose, it needs credentials, which usually take form of a network key.
For `Centralized security network`_, the authentication and authorization also involves establishing a unique Trust Center Link Key (TCLK).

Network Steering in the |addon| for the |NCS|
   In the ZBOSS stack, Network Steering is started using the `ZB_BDB_NETWORK_STEERING`_ bit mask.

   In the |addon|, Network Steering for a node on a network can be started using the following additional options:

   * When running the :ref:`Zigbee Coordinator sample <zigbee_network_coordinator_sample>`, you can press the associated button.
   * If you are using the :ref:`lib_zigbee_shell` library in your application, you can use the ``bdb start`` command.
     This command always starts Network Steering for the Router role.
     For the Coordinator role, this command only starts Network Steering when the device is in the network; when not in the network, ``bdb start`` starts Network Formation.

.. _zigbee_commissioning_modes_nf:

Network Formation
=================

The Network Formation mode is available only to Coordinators and Routers, because End Devices are not capable of forming a Zigbee network.

.. figure:: images/zigbee_commissioning_formation.svg
   :alt: Zigbee Network Formation procedure

   Zigbee Network Formation procedure

In this mode, the node has not found a network to join during the Network Steering mode, and so it attempts to form a new network.
Depending on the node role:

* A Coordinator forms a centralized security network.
* If configured for forming a network, a Router forms a distributed security network.

If Network Formation is successful, you can add nodes to the new network using `Network Steering`_.
If unsuccessful, the commissioning procedure stops here.

Network Formation in the |addon| for the |NCS|
   In the ZBOSS stack, Network Formation is started using the `ZB_BDB_NETWORK_FORMATION`_ bit mask.

   In the |addon|, when using the :ref:`lib_zigbee_signal_handler` and the application implements the Zigbee Coordinator role, the Network Formation procedure is started right after the stack initialization.

Centralized security network
----------------------------

The centralized security network is started by the Zigbee Coordinator node, which assumes the Trust Center functionality.
As such, it is responsible for managing the security and authorization in the network.
In the centralized security network, only the Trust Center can provide the network key to the joining device.
This happens by using a link key specific to the joining device.
The link key is exchanged with the Trust Center, which allows for encrypting packets exchanged between the Trust Center and the new node.
This way, only the Trust Center and the new node can decrypt the packages exchanged between them.
Initially, the link key value can be defined in one of the following ways:

* Calculated based on the joining device's install code.
* Predefined in the specification: ``"ZigBeeAlliance09"``.

The link key value can be updated from the initial value by requesting a new key from the Trust Center.

Distributed security network
----------------------------

The distributed security network is formed by Routers and does not have a centralized node that manages security in the network.
In this network type, all Routers are equal and there is no root of trust between them.
Each Router can provide the network key to the joining device.
This happens by using a distributed security global link key.
The link key value is provided as a result of a successful end product certification.

.. _zigbee_commissioning_security:

Trust Center security policy
----------------------------

If a joining device knows only the well-known ``"ZigBeeAlliance09"`` link key, the Trust Center must encrypt the network key with that key.
Any device in radio range that captures this exchange can then decrypt the network key.

In a network with centralized security, the Coordinator acts as the Trust Center.
When a device running the |addon| acts as the Coordinator, the ZBOSS stack applies the default high security mode for the Trust Center.
This mode is defined by the Trust Center policies in BDB 3.1, sections 5.6.1 and 5.6.2:

* Every joining device must use an install code (``requireInstallCodesOrPresetPassphrase`` set to ``0x02``).
* Trust Center rejoins with the well-known key are not allowed (``allowRejoinsWithWellKnownKey`` set to ``FALSE``).
* Unsecured Trust Center rejoins are not allowed.
* Dynamic Link Key (DLK) negotiation uses only a link key derived from an install code or a unique passphrase, never the well-known key, as required by BDB 3.1, section 5.6.4.
  If the Trust Center has no key for a device that joins using DLK, it rejects the join, even if the install code policy is disabled.

As a result, a Coordinator accepts a new device only if the device install code was added on the Trust Center before the device joins.

BDB 3.1, section 5.6.2 lets a Trust Center provide a backwards compatibility mode to permit legacy devices without install codes to join.
Select this mode through a user interface.
The Trust Center then reverts to the high security mode when the commissioning window closes.
A Trust Center without the user interface must not change the high security policies.
The |addon| does not provide a backwards compatibility mode.

The :ref:`Zigbee samples <zigbee_samples>` use the secure default.
For demonstrations and testing, you can build the :ref:`Network coordinator <zigbee_network_coordinator_sample>` and :ref:`Zigbee shell <zigbee_shell_sample>` samples with the ``zigbee_demo_commissioning`` snippet (``-S zigbee_demo_commissioning``).
The snippet enables the ``CONFIG_ZIGBEE_DEMO_COMMISSIONING`` Kconfig option, which lets devices without an install code join using the well-known link key.

Demo commissioning differs from the backward compatibility mode of BDB 3.1, section 5.6.2, in the following ways:

.. list-table::
   :header-rows: 1
   :widths: 20 40 40

   * - Aspect
     - Backward compatibility mode (BDB 3.1, section 5.6.2)
     - Demo commissioning
   * - How it is selected
     - By the user, through a user interface
     - At build time, using the ``CONFIG_ZIGBEE_DEMO_COMMISSIONING`` Kconfig option
   * - When it ends
     - When the commissioning window closes
     - It does not end. It applies to every commissioning window while the firmware runs.
   * - Trust Center rejoins with the well-known key
     - Allowed during the commissioning window
     - Not allowed
   * - Compliance with BDB 3.1
     - Compliant
     - Not compliant

.. note::
   Demo commissioning does not comply with BDB 3.1.
   Do not use it in products.
   While the network is open, any device in radio range can join and obtain the network key, and any device that captures a join can decrypt the network key.

.. _zigbee_install_code_commissioning:

Commissioning devices with install codes
----------------------------------------

The Trust Center accepts a new device only if you add the device's install code to the Trust Center before the device joins.
This section describes how to provide install codes to joining devices and to the Trust Center.

Install codes on joining devices
   Program a unique install code into each joining device as part of its :ref:`ug_zigbee_configuring_production_config`.
   The production configuration is stored outside of the application image, so all devices can run the same firmware.
   Do not hardcode install codes in the application, because every device programmed with the same image would then share the same install code.

Install codes on the Trust Center
   Provide the install code and the EUI-64 address of each device to the Trust Center out of band, for example through a label or QR code on the product.
   On a Trust Center implemented with the |addon|, add them with the :c:func:`zb_secur_ic_add` function before the device joins.
   On a Trust Center that runs the :ref:`Zigbee shell <zigbee_shell_sample>` sample, use the ``bdb ic add`` command.

Joining a device using development kits
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To join a device with an install code using development kits, complete the following steps:

1. Program the joining device with its application, for example the :ref:`Zigbee Light bulb <zigbee_light_bulb_sample>` sample.

#. Generate and program its production configuration with an install code using the :file:`scripts/zigbee_production_config.py` script, as described in :ref:`ug_zigbee_configuring_production_config`.
   For testing, you can use the ``83FED3407A939723A5C639B26916D505C3B5`` install code.
   Every product must have a unique install code.

#. On a Coordinator that runs the :ref:`Zigbee shell <zigbee_shell_sample>` sample, form the network using the following commands:

   .. code-block:: console

      bdb role zc
      bdb start

#. Get the EUI-64 address of the joining device:

   * If you set the ``--eui64`` parameter of the script, use that value.
   * Otherwise, reset the joining device and read the ``long`` address in the ``Device update received`` message that the Coordinator logs.
     The Coordinator rejects this first join attempt.

#. On the Coordinator, add the install code with its CRC, as printed by the script, and the EUI-64 address:

   .. code-block:: console

      bdb ic add <install_code> <eui64>

#. Reset the joining device.

.. _zigbee_commissioning_modes_fb:

Finding and Binding
===================

After the node joins a network, it can undergo the Finding and Binding procedure.
With this procedure, you can establish a connection between corresponding endpoints on two different nodes automatically.
As a result, new binding table entries are created and the nodes do not have to use the device addresses to communicate, but can rely on connections between the same endpoints in their respective cluster binding tables.

.. figure:: images/zigbee_commissioning_fb.svg
   :alt: Zigbee Finding and Binding procedure

   Zigbee Finding and Binding procedure

Finding and Binding can be started automatically (using the ``bdbCommissioningMode`` mask) or manually.
The process involves two device types: target and initiator.
The device type is defined by the endpoint cluster class implemented on the device.
Depending on the device type:

* The target node identifies itself and responds to queries from the initiator.
* The initiator starts Finding and Binding and periodically searches for endpoints on the target.
  When it receives a response, it identifies the endpoint on target with a ZDO simple descriptor request and creates connections with the clusters corresponding to the ones on the initiator.
  This connection can either be a binding (entry in the binding table) or a grouping (addition of the target node address to the address group).
  The procedure is performed using ZDO simple descriptor requests and responses for each endpoint pair, for each matching Zigbee cluster client and server pair for the endpoint.

Finding and Binding can be terminated if there are no corresponding clusters on the target node.

Finding and Binding in the |addon| for the |NCS|
   In the ZBOSS stack, Finding and Binding is started using the `ZB_BDB_FINDING_N_BINDING`_ bit mask.

   Apart from the BDB top-level procedure API, you can use the `zb_bdb_finding_binding_initiator()`_ function to manually control which binding table entries are to be created.
   For more information, see the `BDB Commissioning API`_ section in the ZBOSS stack documentation.

End of commissioning
********************

The top-level commissioning procedure ends when all the requested commissioning modes are done or the commissioning procedure stops.
