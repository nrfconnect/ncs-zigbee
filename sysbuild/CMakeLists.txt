#
# Copyright (c) 2025 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Global SNIPPET/SB_SNIPPET is applied to every sysbuild image. Some addon
# snippets target the application only: they are removed from the other images,
# and some of them need a different MCUboot counterpart. This hook adjusts the
# snippets of those images so builds can pass a single -DSNIPPET=<app_snippet>.
# Extend the lists below as needed.
function(${SYSBUILD_CURRENT_MODULE_NAME}_pre_cmake)
  cmake_parse_arguments(PRE_CMAKE "" "" "IMAGES" ${ARGN})

  zephyr_get(SB_SNIPPET)
  if(SB_SNIPPET)
    set(global_snippets ${SB_SNIPPET})
  else()
    zephyr_get(SNIPPET)
    set(global_snippets ${SNIPPET})
  endif()

  set(app_only_snippets
    zigbee_demo_commissioning
  )

  # <app_snippet> <mcuboot_snippet> pairs.
  set(app_mcuboot_snippet_pairs
    zigbee_fota zigbee_fota_mcuboot
  )

  foreach(image ${PRE_CMAKE_IMAGES})
    if(image STREQUAL DEFAULT_IMAGE)
      continue()
    endif()

    set(image_snippets ${global_snippets})
    set(changed FALSE)

    foreach(app_snippet ${app_only_snippets})
      if(app_snippet IN_LIST image_snippets)
        set(changed TRUE)
        list(REMOVE_ITEM image_snippets ${app_snippet})
      endif()
    endforeach()

    if(image STREQUAL "mcuboot" AND SB_CONFIG_BOOTLOADER_MCUBOOT)
      list(LENGTH app_mcuboot_snippet_pairs pair_count)
      math(EXPR last_pair_idx "${pair_count} - 2")
      foreach(idx RANGE 0 ${last_pair_idx} 2)
        list(GET app_mcuboot_snippet_pairs ${idx} app_snippet)
        math(EXPR mcuboot_idx "${idx} + 1")
        list(GET app_mcuboot_snippet_pairs ${mcuboot_idx} mcuboot_snippet)

        if(app_snippet IN_LIST global_snippets)
          set(changed TRUE)
          list(REMOVE_ITEM image_snippets ${app_snippet})
          if(NOT mcuboot_snippet IN_LIST image_snippets)
            list(APPEND image_snippets ${mcuboot_snippet})
          endif()
        endif()
      endforeach()
    endif()

    if(changed)
      set(${image}_SNIPPET "${image_snippets}" CACHE STRING "" FORCE)
    endif()
  endforeach()
endfunction()

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
