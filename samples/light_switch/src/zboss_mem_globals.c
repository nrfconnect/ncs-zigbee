/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file zboss_mem_globals.c
 *
 * Holds ZBOSS configurable RAM globals (via zb_mem_config_context.h) in a
 * dedicated translation unit. When linking LTO-built prebuilt libzboss with
 * CONFIG_LTO, zb_memconfig.c in the archive is built with -fno-lto (weak
 * defaults); defining the strong gc_* arrays in another LTO-compiled file
 * triggers ld "size of symbol ... changed" warnings. Compiling this file
 * with -fno-lto keeps weak + strong resolution in the classic ELF path.
 */

#include <zboss_api.h>
#include "zb_mem_config_custom.h"
