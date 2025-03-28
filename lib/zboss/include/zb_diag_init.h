/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: common initialization functions for the diagnostic library */

#ifndef ZB_DIAG_INIT_H
#define ZB_DIAG_INIT_H

/**
 * Common function to initialize diagnostic functionality
 *
 * The diagnostic library is compiled in two versions:
 * a) empty library - contains stubs only
 * b) common library - contains all function implementations as is
 *
 * By default ZBOSS libs are compiled with the 1st type of the diagnostic lib,
 * so we have two ways to build an app:
 * 1) link ZBOSS libs only - diagnostic functionality will be DISABLED
 * 2) link the common diagnostic lib (type b) before ZBOSS libs - diagnostic functionality will be ENABLED
 *
 * To build an app with the diagnostic functionality enabled:
 * 1) need to link the common diagnostic library before ZBOSS libs;
 * 2) need to call zb_diag_init() function from an application. This function
 * should call an init-function for each diagnostic feature. A linker will have
 * to search for zb_diag_init() function and other init-functions in the diagnostic library first.
 *
 * If the diagnostic library with common function implementations is not linked,
 * the the linker will search for init-functions in ZBOSS libs, that contains stubs by default
 */
void zb_diag_init(void);

#endif /* ZB_DIAG_INIT_H */
