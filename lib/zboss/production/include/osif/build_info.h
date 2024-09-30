/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#ifndef ZBOSS_BUILD_INFO_H__
#define ZBOSS_BUILD_INFO_H__

/**
 * This is an autogenerated file.
 * Please use release scripts to update it's contents.
 *
 * Repositories:
 * platform_ncs       bab62cf3718c0b5fb6dff37b63c719926a03d1b0 https://projecttools.nordicsemi.no/bitbucket/scm/zoi/platform_ncs.git
 * zboss-stack-src    38c026b4db545fd2540f12ec537322e4a1981c0d https://projecttools.nordicsemi.no/bitbucket/scm/zoi/zboss_r23
 * zboss-stack-doc    2cad66c0a1943dda14683ce6d2981cf6180e259b https://projecttools.nordicsemi.no/bitbucket/scm/zoi/zboss_doc
 * nrf                042baa9f9a4596a632accb5b015c4ccebcb523fc https://github.com/edmont/sdk-nrf
 * zephyr             74bb2cf0a96ad8be4d98c390040b91fbb95e91b7 https://github.com/nrfconnect/sdk-zephyr
 * wfa-qt-control-app af011c8a8d338ba529f17aed2cc2ef4c1c591a58 https://github.com/nrfconnect/sdk-wi-fiquicktrack-controlappc
 * mcuboot            b1cbeef4d76d5f96885a518025634b197d56dd8f https://github.com/nrfconnect/sdk-mcuboot
 * qcbor              751d36583a9ce1a640900c57e13c9b6b8f3a2ba2 https://github.com/laurencelundblade/QCBOR
 * mbedtls            98603a8c91660beac00e0ee1d76198fb7c4ed29b https://github.com/nrfconnect/sdk-mbedtls
 * oberon-psa-crypto  b41e899e7302462eb952b0b6a7c6903e368fb395 https://github.com/nrfconnect/sdk-oberon-psa-crypto
 * nrfxlib            5a5d9298b0a3f073462911e0c381eb8289315129 https://github.com/edmont/sdk-nrfxlib
 * trusted-firmware-m eab5057595c6ec3b9b3349ae3f3a8aec8a8ac02d https://github.com/nrfconnect/sdk-trusted-firmware-m
 * psa-arch-tests     3da9313e64806d352c519e3205e81cf959067588 https://github.com/nrfconnect/sdk-psa-arch-tests
 * matter             4b61fdbc17066a15fbb78c4423c0d86b9d7d8e52 https://github.com/nrfconnect/sdk-connectedhomeip
 * soc-hwmv1          bd39fcf7832b4eb682a9c5ef5ad0e3b2e34cd502 https://github.com/nrfconnect/sdk-soc-hwmv1
 * cjson              c6af068b7f05207b28d68880740e4b9ec1e4b50a https://github.com/nrfconnect/sdk-cjson
 * azure-sdk-for-c    308c171cb4b5eed266649012a68406487ec81fb2 https://github.com/nrfconnect/azure-sdk-for-c
 * cirrus             3873a08377d93a479105a75ac390d3bbcd31d690 https://github.com/nrfconnect/sdk-mcu-drivers
 * openthread         ee86dc26d71c9ef021fad8a28898829df772011a https://github.com/nrfconnect/sdk-openthread
 * suit-generator     a9b80a109a5da13101b69120f678add48d68cc57 https://github.com/nrfconnect/suit-generator
 * suit-processor     7d94614168540359651391e64aa4de6515e43fca https://github.com/nrfconnect/suit-processor
 * cmock              f65066f15d8248e6dcb778efb8739904a4512087 https://github.com/ThrowTheSwitch/cmock
 * memfault-firmware-sdk 47d69c572c44c19d23936193629e19372e22c7e2 https://github.com/memfault/memfault-firmware-sdk
 * bsim               9ee22c707970f6621adba0375841c0a609e24628 https://github.com/BabbleSim/bsim_west
 * coremark           d5fad6bd094899101a4e5fd53af7298160ced6ab https://github.com/eembc/coremark
 * canopennode        dec12fa3f0d790cafa8414a4c2930ea71ab72ffd https://github.com/zephyrproject-rtos/canopennode
 * chre               3b32c76efee705af146124fb4190f71be5a4e36e https://github.com/zephyrproject-rtos/chre
 * lz4                11b8a1e22fa651b524494e55d22b69d3d9cebcfd https://github.com/zephyrproject-rtos/lz4
 * nanopb             4474bd35bd39de067f0532a1b19ce3aed9ed9807 https://github.com/zephyrproject-rtos/nanopb
 * tf-m-tests         d552e4f18b92032bd335d5e3aa312f6acd82a83b https://github.com/zephyrproject-rtos/tf-m-tests
 * zscilib            ee1b287d9dd07208d2cc52284240ac25bb66eae3 https://github.com/zephyrproject-rtos/zscilib
 * cmsis              4b96cbb174678dcd3ca86e11e1f24bc5f8726da0 https://github.com/zephyrproject-rtos/cmsis
 * cmsis-dsp          6489e771e9c405f1763b52d64a3f17a1ec488ace https://github.com/zephyrproject-rtos/cmsis-dsp
 * cmsis-nn           ea987c1ca661be723de83bd159aed815d6cbd430 https://github.com/zephyrproject-rtos/cmsis-nn
 * edtt               b9ca3c7030518f07b7937dacf970d37a47865a76 https://github.com/zephyrproject-rtos/edtt
 * fatfs              427159bf95ea49b7680facffaa29ad506b42709b https://github.com/zephyrproject-rtos/fatfs
 * hal_nordic         5c8d109371ebb740fbef1f440a3b59e488a36717 https://github.com/zephyrproject-rtos/hal_nordic
 * hal_st             b2f548fe672f24122c7f92027b2c9eeea8a0483a https://github.com/zephyrproject-rtos/hal_st
 * hal_wurthelektronik e5bcb2eac1bb9639ce13b4dafc78eb254e014342 https://github.com/zephyrproject-rtos/hal_wurthelektronik
 * hostap             44285310338f423021cc7df2c1056256882a00cc https://github.com/zephyrproject-rtos/hostap
 * libmetal           a6851ba6dba8c9e87d00c42f171a822f7a29639b https://github.com/zephyrproject-rtos/libmetal
 * liblc3             1a5938ebaca4f13fe79ce074f5dee079783aa29f https://github.com/zephyrproject-rtos/liblc3
 * littlefs           009bcff0ed4853a53df8256039fa815bda6854dd https://github.com/zephyrproject-rtos/littlefs
 * loramac-node       fb00b383072518c918e2258b0916c996f2d4eebe https://github.com/zephyrproject-rtos/loramac-node
 * lvgl               2b498e6f36d6b82ae1da12c8b7742e318624ecf5 https://github.com/zephyrproject-rtos/lvgl
 * mipi-sys-t         71ace1f5caa03e56c8740a09863e685efb4b2360 https://github.com/zephyrproject-rtos/mipi-sys-t
 * net-tools          93acc8bac4661e74e695eb1aea94c7c5262db2e2 https://github.com/zephyrproject-rtos/net-tools
 * nrf_hw_models      eeed2591d38e5e9bf89658df67555f2777249fc0 https://github.com/zephyrproject-rtos/nrf_hw_models
 * open-amp           b735edbc739ad59156eb55bb8ce2583d74537719 https://github.com/zephyrproject-rtos/open-amp
 * picolibc           06bde1fd7531b1f788f6e42b6f7b358c0fe4f814 https://github.com/zephyrproject-rtos/picolibc
 * segger             b011c45b585e097d95d9cf93edf4f2e01588d3cd https://github.com/zephyrproject-rtos/segger
 * tinycrypt          1012a3ebee18c15ede5efc8332ee2fc37817670f https://github.com/zephyrproject-rtos/tinycrypt
 * uoscore-uedhoc     84ef879a46d7bfd9a423fbfb502b04289861f9ea https://github.com/zephyrproject-rtos/uoscore-uedhoc
 * zcbor              47f34dd7f5284e8750b5a715dee7f77c6c5bdc3f https://github.com/zephyrproject-rtos/zcbor
 */


/** ZBOSS build tag */
#define ZBOSS_BUILD_TAG "ncs_release-4.1.0.0"

/** ZBOSS platform build tag */
#define ZBOSS_PLATFORM_BUILD_TAG "v4.1.0.2+v5.1.6"

/** ZBOSS build date (UTC) */
#define ZBOSS_BUILD_DATE 20241108

/** ZBOSS build time (UTC) */
#define ZBOSS_BUILD_TIME 153039


#endif /* ZBOSS_BUILD_INFO_H__ */