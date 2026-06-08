#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_library()
zephyr_library_sources(zigbee_bt_dfu.c)

zephyr_library_link_libraries(MCUBOOT_BOOTUTIL)
