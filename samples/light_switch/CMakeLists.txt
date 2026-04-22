#
# Copyright (c) 2024-2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

if(CONFIG_CHIP)
	project("Zigbee + Matter Light Switch")
else()
	project("Zigbee Light Switch")
endif()

if(CONFIG_CHIP)
	include(${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/config/nrfconnect/app/enable-gnu-std.cmake)
	include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/source_common.cmake)
	include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/data_model.cmake)
	include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/zap_helpers.cmake)
	ncs_get_zap_parent_dir(ZAP_PARENT_DIR)

	target_include_directories(app PRIVATE
		src
		include
		${ZAP_PARENT_DIR}
	)

	target_compile_options(app PRIVATE -Wno-deprecated-declarations)

	target_sources(app PRIVATE
		src/main.cpp
		src/app_task.cpp
		src/app_task_zigbee.c
		src/light_switch.cpp
		src/shell_commands.cpp
		${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/src/binding/binding_handler.cpp
	)

	ncs_configure_data_model()
else()
	target_sources(app PRIVATE
		src/main.cpp
		src/app_task_zigbee.c
	)

	target_include_directories(app PRIVATE include)
endif()

target_sources_ifdef(CONFIG_BT_NUS app PRIVATE
	src/nus_cmd.c
)
