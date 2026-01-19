#
# Copyright (c) 2024Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

# PoC: Force linker to allow duplicate symbols (Zigbee vs OpenThread radio callbacks)
# This build is for memory estimation only - not functionally viable at runtime
add_link_options(-Wl,--allow-multiple-definition)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project("Zigbee + Matter Light Bulb with FOTA")

# Check if Matter is enabled
if(CONFIG_CHIP)
    # Enable GNU STD support for Matter
    include(${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/config/nrfconnect/app/enable-gnu-std.cmake)
    
    # Include Data Model utils (simplified for Zigbee coexistence)
    include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/data_model.cmake)
    
    include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/zap_helpers.cmake)
    ncs_get_zap_parent_dir(ZAP_PARENT_DIR)
    
    target_include_directories(app PRIVATE
        src
        include
        ${ZAP_PARENT_DIR}
    )
    
    target_compile_options(app PRIVATE -Wno-deprecated-declarations)
    
    # NORDIC SDK APP START
    target_sources(app PRIVATE
        src/main_matter.c
        src/matter_init.cpp
    )
    
    ncs_configure_data_model()
    # NORDIC SDK APP END
else()
    # Zigbee-only build
    target_sources(app PRIVATE
        src/main.c
    )
    
    target_include_directories(app PRIVATE include)
endif()
