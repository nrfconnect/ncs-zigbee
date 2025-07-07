.. _zigbee_zboss:

ZBOSS Zigbee stack
##################

The |addon| for the |NCS| uses ZBOSS – a portable, high-performance Zigbee software protocol stack that allows for interoperability, customizing, testing, and optimizing of your Zigbee solution.

The |addon| repository contains a version of the latest stable ZBOSS libraries.
The library files are located in the :file:`lib/zboss/` directory.
The libraries conform to the certification, but they are not necessarily certified.

For information about additional configuration of these libraries and their certification status, see :ref:`zboss_configuration`.
For detailed documentation of the ZBOSS API for these versions and instructions on how to use it, check the ZBOSS API documentation using the following banner.

.. rst-class:: doc-link zboss-box
..

    .. rst-class:: doc-link-image

    .. image:: images/zoi-logo.png
       :target: `ZBOSS API documentation`_

    .. rst-class:: doc-link-text

    `ZBOSS API docs <ZBOSS API documentation_>`_

See also the :ref:`lib_zigbee_osif` library in the :ref:`lib_zigbee` section.


.. toctree::
   :maxdepth: 1
   :caption: Subpages:

   configuration
   certification
