#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_library()
zephyr_library_sources(coexistence.cpp)

# Propagate include paths and compile definitions from `chip`,
# `zigbee`, `zboss` and the Nordic Matter sample commons. Avoid
# zephyr_library_link_libraries(chip): it would reorder libCHIP.a after
# the OpenThread archives in the final link, breaking Matter -> OT
# symbol resolution when OT is built from sources.
zephyr_library_include_directories(
	$<TARGET_PROPERTY:chip,INTERFACE_INCLUDE_DIRECTORIES>
	$<TARGET_PROPERTY:zigbee,INTERFACE_INCLUDE_DIRECTORIES>
	$<TARGET_PROPERTY:zboss,INTERFACE_INCLUDE_DIRECTORIES>
	${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/src
)

target_compile_definitions(${ZEPHYR_CURRENT_LIBRARY} PRIVATE
	$<TARGET_PROPERTY:chip,INTERFACE_COMPILE_DEFINITIONS>
)

# Build-order edge only: wait for chip-gn to emit generated headers (e.g.
# <platform/CHIPDeviceBuildConfig.h>) without pulling libCHIP.a into this
# archive's link closure.
add_dependencies(${ZEPHYR_CURRENT_LIBRARY} chip)

# Matter headers require C++17. The GNU std flag is only linked to the `app`
# target by matter/config/nrfconnect/app/enable-gnu-std.cmake, so apply it
# explicitly to this library as well.
zephyr_library_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=gnu++17>)

# Matter public headers use deprecated symbols in some CHIP releases; the
# sample's own target_compile_options does not propagate here.
zephyr_library_compile_options(-Wno-deprecated-declarations)
