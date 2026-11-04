#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_library()
zephyr_library_sources(zigbee_app_utils.c)
zephyr_library_sources_ifdef(CONFIG_ZIGBEE_APP_IO_SHELL zigbee_app_io_shell.c)

zephyr_library_link_libraries(zboss)
zephyr_library_link_libraries(zigbee)

if(CONFIG_ZIGBEE_DEMO_COMMISSIONING)
  message(WARNING "CONFIG_ZIGBEE_DEMO_COMMISSIONING is enabled: the Trust Center accepts "
                  "devices without install codes. Any device in radio range can join the "
                  "open network and obtain the network key. Use it only for demonstrations "
                  "and testing, never in products.")
endif()
