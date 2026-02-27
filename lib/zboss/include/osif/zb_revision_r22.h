/*
 * Copyright (c) 2020 Nordic Semiconductor
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 *
 * This file implements mappings of Kconfig options enabling and configuring ZBOSS features.
 */

/**
 * Revision number of ZBOSS. It is defined as a sequential number of the commit,
 * starting from the latest tagged revision on the master branch.
 *
 * Note: This value is automatically updated by the release script.
 */
#define ZBOSS_SDK_REVISION 4U

/**
 * Major version number of the ZBOSS platform for NCS.
 */
 #define ZBOSS_PLATFORM_MAJOR 6U

 /**
  * Minor version number of the ZBOSS platform for NCS.
  */
 #define ZBOSS_PLATFORM_MINOR 0U

/**
 * Patch version number of the ZBOSS platform for NCS.
 *
 * Note: This value is automatically updated by the release script.
 */
#define ZBOSS_PLATFORM_PATCH 7U

/**
 * Patch version number of the ZBOSS core.
 *
 * Note: This value is automatically updated by the release script.
 */
#define ZBOSS_PATCH 2U

/**
 * Combined revision number for zboss_version_get() function.
 * This combines ZBOSS_PATCH and ZBOSS_SDK_REVISION into a 16-bit value:
 * Bits 15-8: ZBOSS_PATCH
 * Bits 7-0:  ZBOSS_SDK_REVISION
 * 
 * This gives the full 4-component version in zboss_version_get():
 * return (ZBOSS_MAJOR << 24 | ZBOSS_MINOR << 16 | ZBOSS_REVISION);
 * Result: MAJOR.MINOR.PATCH.SDK_REVISION
 */
#define ZBOSS_REVISION ((ZBOSS_PATCH << 8) | ZBOSS_SDK_REVISION)
