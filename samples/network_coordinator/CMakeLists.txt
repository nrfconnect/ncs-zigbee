#
# Copyright (c) 2024 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project("Zigbee coordinator")

# NORDIC SDK APP START
target_sources(app PRIVATE
  src/main.c
  src/zboss_mem_globals.c
)

target_include_directories(app PRIVATE include)
# NORDIC SDK APP END

# LTO link: zb_memconfig.o from prebuilts is plain ELF (weak gc_*); strong gc_*
# in zb_mem_config_context.h must not be LTO bytecode or ld warns about symbol size.
if(CONFIG_LTO)
	set_source_files_properties(
		src/zboss_mem_globals.c
		PROPERTIES COMPILE_OPTIONS -fno-lto
	)
endif()
