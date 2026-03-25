/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2026 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: internal definitions for ZigBee trace.
*/
#ifndef ZB_TRACE_INTERNAL_H
#define ZB_TRACE_INTERNAL_H 1

#include "zb_common.h"

#ifdef ZB_TRACE_LEVEL

#include "zb_ringbuffer.h"
#include "zb_mac_transport.h" /* for zb_mac_transport_hdr_t only */

#include <stdarg.h>

/* it is used to allocate buffer on stack to pass each trace batch bytes via trace handler */
#ifdef ZB_BINARY_TRACE
#define ZB_TRACE_BUFFER_SIZE 128
#else
#define ZB_TRACE_BUFFER_SIZE 4096
#endif

void zb_trace_inc_counter(void);

/**
 * Get current value if trace external handler
 * MUST be called under zb_trace_lock()/zb_trace_unlock() block
 *
 * @return trace handler or NULL in case it is not set
 */
zb_trace_handler_t zb_trace_handler_get(void);

#define TRACE_HDR_SIG0 0xdeU
#define TRACE_HDR_SIG1 0xadU

struct trace_hdr_s
{
  zb_uint8_t sig[2];
  zb_mac_transport_hdr_t h;
} ZB_PACKED_STRUCT;

#ifdef ZB_BINARY_TRACE

void zb_trace_put_buffer_set(zb_char_t * value);
void zb_trace_put_bytes(zb_uint8_t *buf, zb_short_t len);

void zb_trace_batch_start(zb_uint16_t batch_size);
void zb_trace_batch_commit(void);

#ifdef ZB_TRACE_FROM_CS
/* ring buffer for critical sections */
#ifndef ZB_TRACE_INTR_BUF_SIZE
#define ZB_TRACE_INTR_BUF_SIZE 1024
#endif
ZB_RING_BUFFER_DECLARE(cs_rb, zb_uint8_t, ZB_TRACE_INTR_BUF_SIZE);
cs_rb_t * zb_trace_cs_rb_get(void);
#endif  /* ZB_TRACE_FROM_CS */
#endif /* ZB_BINARY_TRACE */
#endif  /* ZB_TRACE_LEVEL */

#if defined(ZB_TRACE_LEVEL) && defined (ZB_TRACE_TO_FILE)
/**
 * Output text trace message.
 *
 * @param mask - trace mask
 * @param level - trace level
 * @param format - output format
 * @param file_name - source file's name
 * @param file_id - source file's trace id
 * @param line_number - source file's line number
 * @param arglist - variable arguments
 */
void zb_trace_msg_txt(zb_uint_t mask, zb_uint_t level,
                      const zb_char_t *format,
                      const zb_char_t *file_name,
                      zb_int_t line_number,
                      va_list arglist);

/**
 * Output text trace message.
 *
 * @param mask - trace mask
 * @param level - trace level
 * @param data - data that should be traced
 * @param length - length of data
 * @param title - title in the header of trace
 */
void zb_trace_hex_dump_txt(zb_uint_t mask, zb_uint_t level,
                           const zb_uint8_t *data, zb_uint16_t length,
                           const zb_char_t *title);

#ifdef ZB_TRACE_TO_SYSLOG
#ifdef __ANDROID__
const zb_char_t * zb_trace_tag_get(void);
#endif  /* __ANDROID__ */
#endif  /* ZB_TRACE_TO_SYSLOG */
#endif  /* defined(ZB_TRACE_LEVEL) && defined (ZB_TRACE_TO_FILE) */

#if defined(ZB_TRACE_LEVEL) && defined(ZB_BINARY_TRACE)
/**
 * Output binary trace message.
 *
 * @param mask - trace mask
 * @param level - trace level
 * @param file_id - source file's trace id
 * @param line_number - source file's line number
 * @param args_size - number of added parameters
 * @param arglist - variable arguments
 */
void zb_trace_msg_bin(zb_uint_t mask, zb_uint_t level,
                      zb_uint16_t file_id, zb_int_t line_number,
                      zb_int_t args_size, va_list arglist);
/**
 * Output binary trace message.
 *
 * @param mask - trace mask
 * @param level - trace level
 * @param data - data that should be traced
 * @param length - length of data
 */
void zb_trace_hex_dump_bin(zb_uint_t mask, zb_uint_t level,
                           const zb_uint8_t *data, zb_uint16_t length);
#endif  /* defined ZB_TRACE_LEVEL && defined ZB_BINARY_TRACE */

#endif /* ZB_TRACE_INTERNAL_H */
