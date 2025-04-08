#
# Copyright (c) 2025 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

function(${SYSBUILD_CURRENT_MODULE_NAME}_post_cmake)
  sysbuild_get(CONFIG_ZIGBEE_ADD_ON IMAGE ${DEFAULT_IMAGE} VAR CONFIG_ZIGBEE_ADD_ON KCONFIG)
  sysbuild_get(CONFIG_ZIGBEE_FOTA IMAGE ${DEFAULT_IMAGE} VAR CONFIG_ZIGBEE_FOTA KCONFIG)

  if(CONFIG_ZIGBEE_ADD_ON AND CONFIG_ZIGBEE_FOTA)
    sysbuild_get(CONFIG_ZIGBEE_FOTA_GENERATE_LEGACY_IMAGE_TYPE IMAGE ${DEFAULT_IMAGE} VAR CONFIG_ZIGBEE_FOTA_GENERATE_LEGACY_IMAGE_TYPE KCONFIG)
    sysbuild_get(CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION IMAGE ${DEFAULT_IMAGE} VAR CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION KCONFIG)
    sysbuild_get(CONFIG_ZIGBEE_FOTA_MANUFACTURER_ID IMAGE ${DEFAULT_IMAGE} VAR CONFIG_ZIGBEE_FOTA_MANUFACTURER_ID KCONFIG)
    sysbuild_get(CONFIG_ZIGBEE_FOTA_IMAGE_TYPE IMAGE ${DEFAULT_IMAGE} VAR CONFIG_ZIGBEE_FOTA_IMAGE_TYPE KCONFIG)
    sysbuild_get(CONFIG_ZIGBEE_FOTA_COMMENT IMAGE ${DEFAULT_IMAGE} VAR CONFIG_ZIGBEE_FOTA_COMMENT KCONFIG)
    sysbuild_get(CONFIG_ZIGBEE_FOTA_MIN_HW_VERSION IMAGE ${DEFAULT_IMAGE} VAR CONFIG_ZIGBEE_FOTA_MIN_HW_VERSION KCONFIG)
    sysbuild_get(CONFIG_ZIGBEE_FOTA_MAX_HW_VERSION IMAGE ${DEFAULT_IMAGE} VAR CONFIG_ZIGBEE_FOTA_MAX_HW_VERSION KCONFIG)

    if(CONFIG_ZIGBEE_FOTA_GENERATE_LEGACY_IMAGE_TYPE)
      sysbuild_get(${DEFAULT_IMAGE}_image_dir IMAGE ${DEFAULT_IMAGE} VAR APPLICATION_BINARY_DIR CACHE)
      sysbuild_get(${DEFAULT_IMAGE}_kernel_name IMAGE ${DEFAULT_IMAGE} VAR CONFIG_KERNEL_BIN_NAME KCONFIG)

      set(firmware_binary "${${DEFAULT_IMAGE}_image_dir}/zephyr/${${DEFAULT_IMAGE}_kernel_name}.signed.bin")
      set(legacy_cmd "--legacy")
    elseif(SB_CONFIG_DFU_MULTI_IMAGE_PACKAGE_BUILD)
      set(firmware_binary "${CMAKE_BINARY_DIR}/dfu_multi_image.bin")
      set(legacy_cmd)
    else()
      message(WARNING "No Zigbee FOTA image format selected. Please enable either legacy or the multi-image format.")
    endif()

    if(CONFIG_ZIGBEE_FOTA_GENERATE_LEGACY_IMAGE_TYPE OR SB_CONFIG_DFU_MULTI_IMAGE_PACKAGE_BUILD)
      add_custom_target(zigbee_ota_image ALL
      COMMAND
      ${PYTHON_EXECUTABLE}
        ${ZEPHYR_NCS_ZIGBEE_MODULE_DIR}/resources/zb_add_ota_header.py 
        --application ${firmware_binary}
        --application-version-string ${CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION}
        --zigbee-manufacturer-id ${CONFIG_ZIGBEE_FOTA_MANUFACTURER_ID}
        --zigbee-image-type ${CONFIG_ZIGBEE_FOTA_IMAGE_TYPE}
        --zigbee-comment ${CONFIG_ZIGBEE_FOTA_COMMENT}
        --zigbee-ota-min-hw-version ${CONFIG_ZIGBEE_FOTA_MIN_HW_VERSION}
        --zigbee-ota-max-hw-version ${CONFIG_ZIGBEE_FOTA_MAX_HW_VERSION}
        --out-directory ${CMAKE_BINARY_DIR}
        ${legacy_cmd}

      DEPENDS
      ${firmware_binary}
      )
    endif()
  endif(CONFIG_ZIGBEE_ADD_ON AND CONFIG_ZIGBEE_FOTA)
endfunction()