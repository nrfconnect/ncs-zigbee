.. _zigbee_about:

About Zigbee protocol
#####################

Test1

The |addon| for the |NCS| uses the ZBOSS library, a third-party precompiled Zigbee stack.
It includes all mandatory features of the |zigbee_version| specification and provides an Application Programming Interface (API) to access different services.
The stack comes with the following features:

* Complete implementation of the Zigbee core specification and Zigbee Pro feature set, revision 21, 22, and 23.
* Support for all device roles: Coordinator, Router, and End Device.
* Zigbee Cluster Library.
* Base Device Behavior version 3.0.1.
* Zigbee Cluster Library, revision 8.
* Zigbee Green Power Proxy Basic.

See `Software maturity levels`_ in the |NCS| documentation for what experimental support means.
Experimental support also means that the feature is either not certified or no sample is provided for the given feature (or both).

See the :ref:`zigbee_zboss` page and the `external ZBOSS development guide and API documentation`_ for more information about the ZBOSS library.

For more information about Zigbee, download the `Zigbee Specification <CSA Specifications Download Request_>`_ from Connectivity Standards Alliance.

.. _zigbee_topologies:

Zigbee topologies
*****************

Depending on the available device types and their number, Zigbee supports the following network topologies:

Star topology
  The coordinator communicates directly with the end devices and no routers are actively present, meaning they are either not present or not routing packets within the network.

Tree topology
  The routers are located beyond their mutual radio range and are not able to communicate with each other.
  However, unlike in the star topology, the routers continue to move packets through the network.

Mesh topology
  This topology allows full peer-to-peer communication.

.. figure:: images/zigbee_topology_types.svg
   :alt: Zigbee topology types

   Zigbee topology types

.. _zigbee_roles:

In each topology, the nodes have the following responsibilities:

* *Zigbee Coordinator* - Initiates and maintains the devices on the network.
  It also chooses the key network parameters.
  The coordinator's radio is continuously listening for frames.
* *Zigbee Router* - Extends the range of the network.
  For this reason, the router has the radio enabled at all times.
  However, in the star topology it is not moving packets through the network.
* *Zigbee End Device* - Receives messages from the parent device.

In every Zigbee topology, each router and end device that joins the Zigbee network after its creation by the coordinator needs a parent device.
When a device wants to join the network, it sends a beacon request to scan for available devices.
The devices that can route the packets respond with beacons.
Based on different factors of responses, such as signal strength, the new device selects the parent.

For end devices, the parent device (a coordinator or a router) can store information meant for them.
This is required because the end devices do not receive packets directly from other devices.
Each packet meant for an end device needs to go through its parent, and the end devices need to regularly request and respond to packets from the parents.
For example, in the |addon| for the |NCS|, the Zigbee light switch device requests packets from the parent every three seconds.
The end device does not route packets.
It can also disable its radio to reduce the power consumption between the regular packet requests if the Sleepy End Device behavior is enabled.

Additional information
**********************

If you want to learn more about the Zigbee topics and terminology mentioned in this guide, read the following pages:

* :ref:`ug_zigbee_architectures` page to learn more about the Zigbee architecture.
* `Common ZCL terms and definitions`_ section in the ZBOSS user guide.
* Zigbee topologies in section 1.1.4 of the `Zigbee Specification <CSA Specifications Download Request_>`_.
* :ref:`zigbee_ug_sed` section on the :ref:`ug_zigbee_configuring` page.
