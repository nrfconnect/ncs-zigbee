# Zigbee R23 Add-on for nRF Connect SDK (Experimental support)

> **Note**
> This is an initial and [experimental](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/releases_and_maturity/software_maturity.html) release of Zigbee R23 support for Nordic Semiconductor’s nRF54L15 SoC. You can expect updates and enhancements in the future, including potential optimizations related to overall performance.

The Zigbee R23 add-on for the nRF Connect SDK provides support for developing Zigbee applications based on the third-party precompiled ZBOSS stack. This stack is included as the ZBOSS Zigbee stack library. In combination with the nRF Connect SDK, the Zigbee R23 add-on allows for development of low-power connected solutions.

## Getting started
To get started with Zigbee R23 Add-on for nRF Connect SDK, follow [documentation](https://github.com/nrfconnect/ncs-zigbee).

## Documentation
Pre-build documentation is available [here](https://nrfconnect.github.io/ncs-zigbee).

A minimal documentation setup is provided for Sphinx. To build the
documentation first change to the ``docs`` folder:

```shell
cd docs
```
To install Sphinx, make sure you have a Python installation in place and run:

```shell
pip install -r requirements.txt
```

The Sphinx documentation (HTML) can be built using the following command:

```shell
make html
```

The output will be stored in the ``_build_sphinx`` folder. You may check for
other output formats other than HTML by running ``make help``.


##  License
* Source code included within this repository is licensed under the [LicenseRef-Nordic-5-Clause](https://github.com/nrfconnect/ncs-zigbee/blob/main/LICENSE)
* Pre-compiled ZBOSS libraries (`lib/zboss`) included within this repository are licensed under the [DSR Corporation License](https://github.com/nrfconnect/ncs-zigbee/blob/main/lib/zboss/license.txt)
