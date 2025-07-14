/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2025 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ZigBee trace. Application should include it.
*/
#ifndef ZB_LOGGER_H
#define ZB_LOGGER_H 1

/** @cond DOXYGEN_DEBUG_SECTION */
/**
 * @addtogroup ZB_TRACE Debug trace
 * @{
 */
/** @cond DSR_TRACE */
/**
 * @addtogroup ZB_TRACE_CONFIG Trace configuration
 *
 * Note: if ZB_TRACE_LEVEL is defined, at least TRACE_ERROR messages are issued every time
 *       regardless ZB_TRACE_LEVEL value (precompiled or configured during runtime).
 *       Along with that, ZB_TRACE_ALWAYS_ENABLED_INFO_LEVEL may be declared having range 1 .. 4,
 *       it will enable INFO1 .. INFO4 messages to be issued every time as well.
 *
 *---------------------------------------------------------------------------------------------------------------------
 * Text trace configuration:
 * Declare ZB_TRACE_TO_FILE
 *
 *   Along with it the following defines MAY be declared:
 *
 *    - ZB_TRACE_TO_STDOUT in order to override trace output to STDOUT
 *    - ZB_TRACE_TO_SYSLOG in order to override trace output to SYSLOG
 *
 *   Along with it the following defines MAY be declared:
 *
 *    - ZB_BINARY_AND_TEXT_TRACE_MODE in order to have both TEXT and BINARY modes
 *      TEXT or BINARY mode may be selected during initialization (by default it is TEXT mode)
 *      The mode may be selected only before calling TRACE_INIT() function otherwise the selection call will be ignored
 *
 *    - ZB_BINARY_TRACE in order to have only BINARY mode
 *
 *    - ZB_TRAFFIC_DUMP_ON in order to have ZB communication traffic be dumped
 *      It is dumped into the same trace file in case BINARY mode is used,
 *      or into separated dump file in case TEXT mode is used
 *
 *---------------------------------------------------------------------------------------------------------------------
 * Binary trace configuration:
 * Declare ZB_TRACE_TO_PORT
 *
 *   Along with it the following defines MUST be declared:
 *
 *    - ZB_TRACE_OVER_USART in order to have trace be transferred over USART interface
 *    - ZB_TRACE_OVER_JTAG in order to have trace be transferred over JTAG interface
 *    - ZB_TRACE_OVER_MACSPLIT in order to have trace be transferred over MACSPLIT interface
 *    - custom interfaces like ZB_TI13XX_ITM_TRACE, etc.
 *
 *   Along with it the following defines MAY be declared:
 *
 *    - ZB_TRAFFIC_DUMP_ON in order to have ZB communication traffic be dumped
 *      It is dumped into the same interface
 *
 *    - ZB_MEMTRACE in order to duplicate trace messages into the special memory area
 *      that contains ring buffer to keep latest trace messages that may be used to
 *      retrieve information and form a crash dump in case it is needed
 *
 *    - ZB_TRACE_FROM_CS in order to have trace messages be generated under critical section
 *      but be issued with a delay along with next trace message once this next trace
 *      message is generated outside of critical section
 *
 * @{
 */
/** @endcond */ /* DSR_TRACE */

#if defined(ZB_TRACE_LEVEL)

void zb_trace_set_level(zb_uint8_t level);

/**
   Set trace level at runtime

   That macro can switch trace level on and off. Trace level must be included
   into build at compile time by setting ZB_TRACE_LEVEL compiler time define.

   @param l - new trace level.

   @par Example
   @snippet thermostat/thermostat_zr/thermostat_zr.c set_trace
   @par
 */
#define ZB_SET_TRACE_LEVEL(l) zb_trace_set_level(l)

void zb_trace_set_off(void);

/**
   Switch off all trace at runtime
 */
#define ZB_SET_TRACE_OFF() zb_trace_set_off()

void zb_trace_set_on(void);

/**
   Switch on trace at runtime

   That macro enables trace which was active before call to ZB_SET_TRACE_OFF().

   @snippet light_sample/dimmable_light/bulb.c switch_trace_on
 */
#define ZB_SET_TRACE_ON() zb_trace_set_on()

void zb_trace_set_mask(zb_uint32_t mask);

/**
   Set trace mask at runtime

   That macro can switch trace mask on and off. Trace mask must be included
   into build at compile time by setting ZB_TRACE_MASK compiler time define.

   @param m - new trace mask.

   @par Example
   @snippet thermostat/thermostat_zr/thermostat_zr.c set_trace
   @par
 */
#define ZB_SET_TRACE_MASK(m) zb_trace_set_mask(m)

#ifdef ZB_BINARY_AND_TEXT_TRACE_MODE

zb_ret_t zb_trace_set_binary_mode(void);

/**
   Set trace binary mode at runtime

   That macro can switch trace mode.
   It should be used before TRACE_INIT() macro.

   @return RET_OK if success, RET_IGNORE if trace subsystem was already initialized
 */
#define ZB_SET_TRACE_BINARY_MODE() zb_trace_set_binary_mode()

zb_ret_t zb_trace_set_text_mode(void);

/**
   Set trace text mode at runtime

   That macro can switch trace mode.
   It should be used before TRACE_INIT() macro.

   @return RET_OK if success, RET_IGNORE if trace subsystem was already initialized
 */
#define ZB_SET_TRACE_TEXT_MODE() zb_trace_set_text_mode()
#endif  /* ZB_BINARY_AND_TEXT_TRACE_MODE */

/**
 *  @brief A universal way to print raw string as trace message (usually used to print a string data in case of BINARY mode).
 */
void zb_trace_str(const zb_char_t *str);

/**
 * @brief Put a hex representation of a byte into a buffer at specified position
 *
 * @param byte [in]     Value to be formatted as 2 hex digits
 * @param buf  [in]     Pointer to char buffer
 * @param pos  [in,out] Position in buf at which to put the hex data (incremented by 2)
 */
void zb_print2x(zb_uint8_t byte, zb_char_t *buf, zb_uint16_t *pos);

#else

#define ZB_SET_TRACE_OFF()
#define ZB_SET_TRACE_LEVEL(l)
#define ZB_SET_TRACE_ON()
#define ZB_SET_TRACE_MASK(m)

#endif /* defined(ZB_TRACE_LEVEL) */

/** @cond DSR_TRACE */
/** @} */ /* ZB_TRACE_CONFIG */
/** @endcond */ /* DSR_TRACE */

/** @cond DSR_TRACE */
/**
 *  @addtogroup TRACE_SUBSYSTEMS
 *  @{
 */
#define TRACE_SUBSYSTEM_COMMON    0x0001U  /**< Common subsystem. */
#define TRACE_SUBSYSTEM_MEM       0x0002U  /**< MEM subsystem (buffers pool). */
#define TRACE_SUBSYSTEM_MAC       0x0004U  /**< MAC subsystem. */
#define TRACE_SUBSYSTEM_NWK       0x0008U  /**< NWK subsystem. */

#define TRACE_SUBSYSTEM_APS       0x0010U  /**< APS subsystem. */
#define TRACE_SUBSYSTEM_ZSE       0x0020U  /**< ZSE subsystem. */
#define TRACE_SUBSYSTEM_ZDO       0x0040U  /**< ZDO subsystem. */
#define TRACE_SUBSYSTEM_SECUR     0x0080U  /**< Security subsystem. */

#define TRACE_SUBSYSTEM_ZCL       0x0100U  /**< ZCL subsystem. */
/** @cond DOXYGEN_TOUCHLINK_FEATURE */
#define TRACE_SUBSYSTEM_ZLL       0x0200U  /**< ZLL/Touchlink subsystem. */
/** @endcond */ /* DOXYGEN_TOUCHLINK_FEATURE */
/** @cond DOXYGEN_INTERNAL_DOC */
#define TRACE_SUBSYSTEM_SSL       0x0400U  /**< SSL subsystem - not really used */
#define TRACE_SUBSYSTEM_NCP_TRANSPORT  TRACE_SUBSYSTEM_SSL
#define TRACE_SUBSYSTEM_MACSPLIT  TRACE_SUBSYSTEM_NCP_TRANSPORT
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */
/** @endcond */ /* DSR_TRACE */
#define TRACE_SUBSYSTEM_APP       0x0800U  /**< User Application */

/** @cond DOXYGEN_INTERNAL_DOC */
#define TRACE_SUBSYSTEM_LWIP      0x1000U  /* LWIP is used, else free */
#define TRACE_SUBSYSTEM_ALIEN     0x2000U  /* Some special debug */
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */
#define TRACE_SUBSYSTEM_ZGP       0x4000U  /**< ZGP subsystem */
/** @cond DOXYGEN_INTERNAL_DOC */
#define TRACE_SUBSYSTEM_MAC_API   0x8000U    /**< MAC API subsystem */
#define TRACE_SUBSYSTEM_MACLL     0x10000U   /**< MAC LL subsystem */
#define TRACE_SUBSYSTEM_SPECIAL1  0x20000U   /**< Special subsystem */
#define TRACE_SUBSYSTEM_BATTERY   0x40000U   /**< Battery subsystem */
#define TRACE_SUBSYSTEM_OTA       0x80000U   /**< OTA subsystem */
#define TRACE_SUBSYSTEM_TRANSPORT 0x100000U  /**< Transport subsystem */
#define TRACE_SUBSYSTEM_USB       0x200000U  /**< USB subsystem */
#define TRACE_SUBSYSTEM_SPI       0x400000U  /**< SPI subsystem */
#define TRACE_SUBSYSTEM_UART      0x800000U  /**< UART subsystem */
#define TRACE_SUBSYSTEM_PTA       0x1000000U /**< PTA subsystem */
#define TRACE_SUBSYSTEM_JSON      0x1000000U /**< JSON subsystem */
#define TRACE_SUBSYSTEM_HTTP      0x2000000U /**< HTTP subsystem */
#define TRACE_SUBSYSTEM_CLOUD     0x4000000U /**< Interface to the Cloud */
#define TRACE_SUBSYSTEM_ZBDIRECT  0x8000000U /**< Zigbee Direct subsystem */
#define TRACE_SUBSYSTEM_DIAGNOSTIC  0x10000000U /**< Diagnostic subsystem */
#define TRACE_SUBSYSTEM_NS          0x20000000U /**< Network simulator subsystem */
#define TRACE_SUBSYSTEM_TEST        0x40000000U /**< Subsystem for tests and CI */
#define TRACE_SUBSYSTEM_ADDR        0x80000000U /**< to trace address lock / unlock operations */

/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

#define TRACE_SUBSYSTEM_INFO      (~0U)  /**< Common subsystem */

/* to be continued... */

/** @cond DSR_TRACE */
/** @} */ /* TRACE_SUBSYSTEMS */
/** @endcond */ /* DSR_TRACE */

#if defined ZB_TRACE_LEVEL || defined DOXYGEN
/**
 * @addtogroup ZB_TRACE_CONFIG Trace configuration
 * @{
 */

/**
 *  @brief Check that trace is enabled for provided level and mask.
 *  @param level - trace level.
 *  @param mask - trace mask.
 *  @return ZB_TRUE if enabled, ZB_FALSE if disabled.
 */
zb_bool_t zb_trace_level_mask_enabled(zb_uint8_t level, zb_uint32_t mask);

#ifndef DOXYGEN
#define TRACE_ENABLED_(mask,lev) ((lev) <= ZB_TRACE_LEVEL && ((mask) & ZB_TRACE_MASK))
#endif /* DOXYGEN */

/**
 *  @brief Check that trace is enabled for provided level.
 *  To be used in constructions like:
 *  @code
 *  if (TRACE_ENABLED(TRACE_APS3))
 *  {
 *    call_some_complex_trace();
 *  }
 *  @endcode
 *  @param m - trace level macro.
 *  @return 1 if enabled, 0 if disabled.
 */
#define TRACE_ENABLED(m) TRACE_ENABLED_(m)

#ifdef DOXYGEN
/**
   Trace file ID used by win_com_dump to identify source file.

   Must be first define in the .c file, just before first include directive.
  @par Example
  @snippet light_sample/dimmable_light/bulb.c trace_file_id
  @par
*/
#define ZB_TRACE_FILE_ID 12345U
#endif

/** @} */ /* ZB_TRACE_CONFIG */
#endif /* defined ZB_TRACE_LEVEL || defined DOXYGEN */

zb_uint32_t zb_trace_get_counter(void);

#if defined ZB_TRACE_LEVEL || defined DOXYGEN
#if defined ZB_TRACE_TO_FILE || defined ZB_TRACE_TO_PORT || defined DOXYGEN
/** @cond DOXYGEN_INTERNAL_DOC */
zb_ret_t zb_trace_init(const zb_char_t * name);
void zb_trace_deinit(void);
void zb_trace_disable_deinit(void);
zb_bool_t zb_trace_is_disable_deinit(void);
void zb_trace_commit(void);
void zb_trace_flush(void);
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */
/**

 Initialize trace subsystem

 @param name - trace file name component
 @return 0 if success, otherwise an error code (depends on platform).
*/
#define TRACE_INIT(name) (void)zb_trace_init(name)

/**
   Deinitialize trace subsystem
*/
#define TRACE_DEINIT() zb_trace_deinit()

/**
   Prevent Trace subsystem be de-initialized
   MAY be used when STACK restart logic is used
*/
#define TRACE_DISABLE_DEINIT() zb_trace_disable_deinit()

/**
   Check that trace subsystem deinitialization logic is disabled

   @return ZB_TRUE if it is disabled, otherwise ZB_FALSE.
*/
#define TRACE_IS_DISABLE_DEINIT() zb_trace_is_disable_deinit()

/**
   Commit forces the file system to flush its buffers to disk.
*/
#define TRACE_COMMIT() zb_trace_commit()

/**
   Flush forces trace buffered data to be written to the file system or port
*/
#define TRACE_FLUSH() zb_trace_flush()

/**
   Lock the trace object. Completely platform depended function

   It could be used in cases implementation is used trace object,
   for example, by using, zb_trace_file_object_get() in multitheraded
   applications.
 */
void zb_trace_lock(void);

/**
   Unlock the trace object. Completely platform depended function
 */
void zb_trace_unlock(void);

typedef struct zb_trace_handler_info_s
{
  zb_uint_t level;
  zb_uint_t mask;
} zb_trace_handler_info_t;

/**
 * Prototype of trace callback that external code may setup
 */
typedef zb_ret_t (*zb_trace_handler_t)(const zb_trace_handler_info_t * info,
                                       const zb_char_t               * trace_msg,
                                       zb_uint16_t                     trace_len);

/**
 * Setup trace callback or clear it using NULL as argument value
 * Two possible cases
 * - redirecting trace messages
 * - filtering trace messages
 *
 * Note: the setup itself is performed under zb_trace_lock()/zb_trace_unlock() block
 */
void zb_trace_handler_setup(zb_trace_handler_t handler);
/** @cond DOXYGEN_INTERNAL_DOC */
void zb_trace_enter_cs(void);
void zb_trace_leave_cs(void);
zb_bool_t zb_trace_inside_cs(void);
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/**
 * Enter to critical section
 *
 * Macro that can be used to prevent trace to be issued
 * in case there some critical section of code.
 * Instead, the trace itself will be generated but put
 * into the special internal ring buffer,
 * the trace issue itself will be delayed and finally issued
 * once next trace message will be generated outside of critical section
 */
#define TRACE_ENTER_CS() zb_trace_enter_cs()

/**
 * Leave from critical section
 */
#define TRACE_LEAVE_CS() zb_trace_leave_cs()

/**
 * Check that critical section is placed
 */
#define TRACE_INSIDE_CS() zb_trace_inside_cs()
/** @cond DOXYGEN_INTERNAL_DOC */
/**
 * Output trace message.
 *
 * @param mask - requested trace mask
 * @param level - requested trace level
 * @param format - printf-like format string
 * @param file_name - source file name
 * @param file_id - source file id
 * @param line_number - source file line
 * @param args_size - number of added parameters
 */
void zb_trace_msg(zb_uint_t        mask,
                  zb_uint_t        level,
                  const zb_char_t *format,
                  const zb_char_t *file_name,
                  zb_uint16_t      file_id,
                  zb_int_t         line_number,
                  zb_int_t         args_size, ...);

#define ZB_T0_TRACE(...) __VA_ARGS__

#if defined ZB_BINARY_AND_TEXT_TRACE_MODE || !defined ZB_BINARY_TRACE
/* text and binary mode or only text mode */
#define ZB_T1_TRACE(s, l, fmts, args) if ((zb_int_t)ZB_TRACE_LEVEL>=(zb_int_t)l && ((s) & ZB_TRACE_MASK)) zb_trace_msg(s, l, fmts, ZB_T0_TRACE args)
#else
/* only binary mode */
#define ZB_T1_TRACE(s, l, fmts, args) if ((zb_int_t)ZB_TRACE_LEVEL>=(zb_int_t)l && ((s) & ZB_TRACE_MASK)) zb_trace_msg(s, l, NULL, NULL, ZB_T0_TRACE args)
#endif

/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/**
 *  @brief Put trace output.
 *  @param lm - trace subsystem and level marker.
 *  @param fmts - printf()-like format string.
 *  @param args - format string substitution parameters with a size marker.
 *
 *  @par Example
 *  @snippet light_sample/dimmable_light/bulb.c trace_msg
 *  @par
 */
#define TRACE_MSG(lm, fmts, args) \
  do { \
    ZB_T1_TRACE(lm, fmts, args); \
  } while (0)

/** @cond DOXYGEN_INTERNAL_DOC */
void zb_trace_hex_dump(zb_uint_t         mask,
                       zb_uint_t         level,
                       const zb_uint8_t *data,
                       zb_uint16_t       length,
                       const zb_char_t  *title);
#define ZB_T1_TRACE_HEX_DUMP(s, l, data, length, title) if ((zb_int_t)ZB_TRACE_LEVEL>=(zb_int_t)l && ((s) & ZB_TRACE_MASK)) zb_trace_hex_dump(s, l, data, length, title)
/** @cond DOXYGEN_INTERNAL_DOC */

/**
 *  @brief Formatted hex dump to text output
 *  @param lm - trace subsystem and level marker
 *  @param data - data that should be traced
 *  @param length - length of data
 *  @param title - title in the header of trace
 *
 *  @return RET_OK if success, an error otherwise
 *
 *  Example:
 *  [MAC1] zb_extmac.c:624: process_simple_frame stack_id 0 cmd01 0x4284 data 0xf10bfb9d len 16
 *  [MAC1] zb_trace.c:36:  =========================[cmd01=0x428 len=016]==========================
 *  [MAC1] zb_trace.c:101: | 00 07 63 9B 5C 00 23 00 | 00 00 00 00 0D 0C 01 00 | ..c.\.#......... |
 *  [MAC1] zb_trace.c:101: |                         |                         |                  |
 *  [MAC1] zb_trace.c:108: ------------------------------------------------------------------------
 */
#define TRACE_HEX_DUMP(lm, data, length, title) \
  do { \
    ZB_T1_TRACE_HEX_DUMP(lm, data, length, title); \
  } while (0)

/** @cond DOXYGEN_INTERNAL_DOC */

#ifndef ZB_TRACE_FILE_ID
#ifndef ZB_DONT_NEED_TRACE_FILE_ID
#warning ZB_TRACE_FILE_ID undefined! Insert #define ZB_TRACE_FILE_ID NNN before 1-st #include
#endif
#define ZB_TRACE_FILE_ID 0
#else
ZB_ASSERT_COMPILE_DECL(ZB_TRACE_FILE_ID < ZB_UINT16_MAX);
#endif

#else

#error Must define either ZB_TRACE_TO_FILE or ZB_TRACE_TO_PORT

#endif  /* trace type */

#ifndef ZB_TRACE_MASK
#define ZB_TRACE_MASK ((zb_uint_t)-1)
#endif  /* if not defined trace_mask */

#ifdef ZB_TRACE_TO_FILE
zb_osif_file_t * zb_trace_file_object_get(void);
zb_osif_file_t * zb_dump_file_object_get(void);
#endif  /* ZB_TRACE_TO_FILE */

#else  /* if trace off */

#define TRACE_INIT(name)
#define TRACE_DEINIT()
#define TRACE_DISABLE_DEINIT()
#define TRACE_IS_DISABLE_DEINIT()

#define TRACE_COMMIT()
#define TRACE_FLUSH()

#define TRACE_ENABLED(m) ZB_FALSE

#define TRACE_ENTER_CS()
#define TRACE_LEAVE_CS()
#define TRACE_INSIDE_CS() ZB_FALSE

#define TRACE_MSG(...) ((void)0)
#define TRACE_HEX_DUMP(...) ((void)0)

#endif  /* trace on/off */
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/* Backward compatibility */
#define TRACE_ENTER_INT TRACE_ENTER_CS
#define TRACE_LEAVE_INT TRACE_LEAVE_CS

#if defined ZB_TRAFFIC_DUMP_ON || defined DOXYGEN
/**
   @addtogroup DUMP_ON_OFF
   @{
*/

void zb_trace_traffic_dump_disable(void);
void zb_trace_traffic_dump_enable(void);
zb_uint8_t zb_trace_traffic_dump_state_get(void);

/**
 * Switch Zigbee traffic dump OFF
 */
#define ZB_SET_TRAF_DUMP_OFF() zb_trace_traffic_dump_disable()
/**
 * Switch Zigbee traffic dump ON
 *
 * That function works only if traffic dump is enabled at compile time.
 */
#define ZB_SET_TRAF_DUMP_ON() zb_trace_traffic_dump_enable()
#define ZB_GET_TRAF_DUMP_STATE() zb_trace_traffic_dump_state_get()
/** @} */ /* DUMP_ON_OFF */
#else
#define ZB_SET_TRAF_DUMP_OFF()
#define ZB_SET_TRAF_DUMP_ON()
#define ZB_GET_TRAF_DUMP_STATE()  0U
#endif

/** @cond DSR_TRACE */
/**
 *  @addtogroup TRACE_DATA_FORMAT_ADDITIONAL Trace data format for keys
 *  @{
 */
#if defined ZB_TRACE_TO_FILE || defined DOXYGEN
/**
   Trace format for 64-bit address.

   @snippet light_sample/dimmable_light/bulb.c trace_64_example_variable
   @snippet light_sample/dimmable_light/bulb.c trace_64_example
*/
#define TRACE_FORMAT_64 "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx"

/**
 Trace format for 128-bit number (key etc).

 @snippet ic_sample/ic_zc.c TRACE_128
*/
#define TRACE_FORMAT_128 "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx"

/**
   Trace format arguments for 64-bit address.

   @snippet light_sample/dimmable_light/bulb.c trace_64_example_variable
   @snippet light_sample/dimmable_light/bulb.c trace_64_example
*/
#define TRACE_ARG_64(a) (zb_uint8_t)((a)[7]),(zb_uint8_t)((a)[6]),(zb_uint8_t)((a)[5]),(zb_uint8_t)((a)[4]),(zb_uint8_t)((a)[3]),(zb_uint8_t)((a)[2]),(zb_uint8_t)((a)[1]),(zb_uint8_t)((a)[0])

/**
   Trace format arguments for 128-bit number.

   @snippet ic_sample/ic_zc.c TRACE_128
*/
#define TRACE_ARG_128(a) (zb_uint8_t)((a)[0]),(zb_uint8_t)((a)[1]),(zb_uint8_t)((a)[2]),(zb_uint8_t)((a)[3]),(zb_uint8_t)((a)[4]),(zb_uint8_t)((a)[5]),(zb_uint8_t)((a)[6]),(zb_uint8_t)((a)[7]),(zb_uint8_t)((a)[8]),(zb_uint8_t)((a)[9]),(zb_uint8_t)((a)[10]),(zb_uint8_t)((a)[11]),(zb_uint8_t)((a)[12]),(zb_uint8_t)((a)[13]),(zb_uint8_t)((a)[14]),(zb_uint8_t)((a)[15])


#else

/** @brief Trace format for 64-bit address - single argument for 8051. */
#define TRACE_FORMAT_64 "%A"
#define TRACE_FORMAT_128 "%A:%A"

/** @endcond */ /* DSR_TRACE */
/** @cond internals_doc */
typedef struct zb_addr64_struct_s
{
  zb_64bit_addr_t addr;
} ZB_PACKED_STRUCT zb_addr64_struct_t;

typedef struct zb_byte128_struct_s
{
  zb_uint8_t d[16];
} ZB_PACKED_STRUCT zb_byte128_struct_t;

/* Pass 8-bytes address as structure by value */
#define TRACE_ARG_64(a) *((zb_addr64_struct_t *)a)
/* Pass 16-bytes key as structure by value */
#define TRACE_ARG_128(a) *((zb_byte128_struct_t *)a)
/** @endcond */ /* internals_doc */
/** @cond DSR_TRACE */

#endif  /* ti file / !to file */
/** @} */ /* TRACE_DATA_FORMAT_ADDITIONAL */
/** @endcond */ /* DSR_TRACE */


/** @cond DSR_TRACE */
/**
 *  @addtogroup TRACE_GENERAL_MESSAGES
 *  @{
 */
/** @brief Error message. */
#define TRACE_ERROR TRACE_SUBSYSTEM_INFO, 0U
/** @brief Information message level 1. */
#define TRACE_INFO1 TRACE_SUBSYSTEM_INFO, 1U
/** @brief Information message level 2. */
#define TRACE_INFO2 TRACE_SUBSYSTEM_INFO, 2U
/** @brief Information message level 3. */
#define TRACE_INFO3 TRACE_SUBSYSTEM_INFO, 3U

/** @} */ /* TRACE_GENERAL_MESSAGES */

/**
 *  @addtogroup TRACE_DATA_FORMAT
 *  @{
 */

/*
  Trace format constants for serial trace
*/

/* Keil and sdcc put byte values to the stack as is, but IAR/8051 casts it to 16-bit
 * integers, so constant lengths differs */

/**
 *  @brief Calculates trace argument size.
 *  @param n_h - number of Hs/Cs.
 *  @param n_d - number of Ds.
 *  @param n_l - number of Ls.
 *  @param n_p - number of Ps.
 *  @param n_a - number of As.
 *  @hideinitializer
 */
#ifdef ZB_PLATFORM_XAP5

#ifndef __XAP5_NEAR__
/* XAP5 passes bytes as shorts */
#ifndef ZB_BINARY_TRACE
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) __FILE__,ZB_TRACE_FILE_ID,__LINE__, (n_h*2 + n_d*2 + n_l*4 + n_p*4 + n_a*8)
#else
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) ZB_TRACE_FILE_ID,__LINE__, (n_h*2 + n_d*2 + n_l*4 + n_p*4 + n_a*8)
#endif
#else
#ifndef ZB_BINARY_TRACE
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) __FILE__,ZB_TRACE_FILE_ID,__LINE__, (n_h*2 + n_d*2 + n_l*4 + n_p*2 + n_a*8)
#else
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) ZB_TRACE_FILE_ID,__LINE__, (n_h*2 + n_d*2 + n_l*4 + n_p*2 + n_a*8)
#endif
#endif

#else  /* IAR & GCC - 32-bit */
/* IAR for Cortex passes 1-byte and 2-bytes arguments as 4-bytes to vararg functions.
 * Pointers are 4-bytes. */

#if defined ZB_BINARY_AND_TEXT_TRACE_MODE || !defined ZB_BINARY_TRACE
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) __FILE__,ZB_TRACE_FILE_ID,__LINE__, (n_h*4 + n_d*4 + n_l*4 + n_p*4 + n_a*8)
#else
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) ZB_TRACE_FILE_ID,__LINE__, (n_h*4 + n_d*4 + n_l*4 + n_p*4 + n_a*8)
#endif

#endif

#define FMT__0                                          TRACE_ARG_SIZE(0,0,0,0,0)
#define FMT__A                                          TRACE_ARG_SIZE(0,0,0,0,1)
#define FMT__A_A                                        TRACE_ARG_SIZE(0,0,0,0,2)
#define FMT__A_A_A                                      TRACE_ARG_SIZE(0,0,0,0,3)
#define FMT__A_A_A_A                                    TRACE_ARG_SIZE(0,0,0,0,4)
#define FMT__A_A_A_A_A_A                                TRACE_ARG_SIZE(0,0,0,0,6)
#define FMT__A_D_A_P                                    TRACE_ARG_SIZE(0,1,0,1,2)
#define FMT__A_D_D                                      TRACE_ARG_SIZE(0,2,0,0,1)
#define FMT__A_D_D_D                                    TRACE_ARG_SIZE(0,3,0,0,1)
#define FMT__A_D_D_P_H                                  TRACE_ARG_SIZE(1,2,0,1,1)
#define FMT__A_D_H                                      TRACE_ARG_SIZE(1,1,0,0,1)
#define FMT__A_D_H_H                                    TRACE_ARG_SIZE(2,1,0,0,1)
#define FMT__A_D_P_H_H_H                                TRACE_ARG_SIZE(3,1,0,1,1)
#define FMT__A_H                                        TRACE_ARG_SIZE(1,0,0,0,1)
#define FMT__A_H_H                                      TRACE_ARG_SIZE(2,0,0,0,1)
#define FMT__A_H_H_H                                    TRACE_ARG_SIZE(3,0,0,0,1)
#define FMT__A_H_H_H_H                                  TRACE_ARG_SIZE(4,0,0,0,1)
#define FMT__A_A_H_H_H                                  TRACE_ARG_SIZE(3,0,0,0,2)
#define FMT__A_A_H_H_H_L                                TRACE_ARG_SIZE(3,0,1,0,2)
#define FMT__A_D_H_H_H                                  TRACE_ARG_SIZE(3,1,0,0,1)
#define FMT__A_P                                        TRACE_ARG_SIZE(0,0,0,1,1)
#define FMT__AA                                         TRACE_ARG_SIZE(0,0,0,0,2)
#define FMT__C                                          TRACE_ARG_SIZE(1,0,0,0,0)
#define FMT__C_C                                        TRACE_ARG_SIZE(2,0,0,0,0)
#define FMT__C_C_C                                      TRACE_ARG_SIZE(3,0,0,0,0)
#define FMT__C_C_C_C                                    TRACE_ARG_SIZE(4,0,0,0,0)
#define FMT__C_C_C_C_C                                  TRACE_ARG_SIZE(5,0,0,0,0)
#define FMT__C_C_H_H                                    TRACE_ARG_SIZE(4,0,0,0,0)
#define FMT__C_H                                        TRACE_ARG_SIZE(2,0,0,0,0)
#define FMT__D                                          TRACE_ARG_SIZE(0,1,0,0,0)
#define FMT__D_A                                        TRACE_ARG_SIZE(0,1,0,0,1)
#define FMT__D_A_A_H                                    TRACE_ARG_SIZE(1,1,0,0,2)
#define FMT__D_A_D_D_D_D_D_D_D_D                        TRACE_ARG_SIZE(0,9,0,0,1)
#define FMT__D_A_D_P_H_H_H                              TRACE_ARG_SIZE(3,2,0,1,1)
#define FMT__D_A_H_H                                    TRACE_ARG_SIZE(2,1,0,0,1)
#define FMT__D_A_H_H_H_H                                TRACE_ARG_SIZE(4,1,0,0,1)
#define FMT__D_A_P                                      TRACE_ARG_SIZE(0,1,0,1,1)
#define FMT__D_C                                        TRACE_ARG_SIZE(1,1,0,0,0)
#define FMT__D_D                                        TRACE_ARG_SIZE(0,2,0,0,0)
#define FMT__D_D_A                                      TRACE_ARG_SIZE(0,2,0,0,1)
#define FMT__D_D_A_A                                    TRACE_ARG_SIZE(0,2,0,0,2)
#define FMT__D_D_A_D                                    TRACE_ARG_SIZE(0,3,0,0,1)
#define FMT__D_D_A_D_D_D_D                              TRACE_ARG_SIZE(0,6,0,0,1)
#define FMT__D_D_D                                      TRACE_ARG_SIZE(0,3,0,0,0)
#define FMT__D_D_D_C                                    TRACE_ARG_SIZE(1,3,0,0,0)
#define FMT__D_D_D_D                                    TRACE_ARG_SIZE(0,4,0,0,0)
#define FMT__D_D_D_D_D                                  TRACE_ARG_SIZE(0,5,0,0,0)
#define FMT__D_D_D_D_D_D                                TRACE_ARG_SIZE(0,6,0,0,0)
#define FMT__D_D_D_D_D_D_H                              TRACE_ARG_SIZE(1,6,0,0,0)
#define FMT__D_D_D_D_D_D_D                              TRACE_ARG_SIZE(0,7,0,0,0)
#define FMT__D_D_D_D_D_D_D_D                            TRACE_ARG_SIZE(0,8,0,0,0)
#define FMT__D_D_D_D_D_D_D_D_D                          TRACE_ARG_SIZE(0,9,0,0,0)
#define FMT__D_D_D_D_D_D_D_D_D_D_D                      TRACE_ARG_SIZE(0,11,0,0,0)
#define FMT__D_D_D_D_D_D_D_D_D_D_D_D_D_D                TRACE_ARG_SIZE(0,14,0,0,0)
#define FMT__D_D_D_D_D_D_D_D_D_D_D_D_D_D_D              TRACE_ARG_SIZE(0,15,0,0,0)
#define FMT__D_D_D_D_D_D_D_D_D_D_D_D_D_D_D_D_D          TRACE_ARG_SIZE(0,17,0,0,0)
#define FMT__D_D_D_D_D_D_P                              TRACE_ARG_SIZE(0,6,0,1,0)
#define FMT__D_D_D_D_H                                  TRACE_ARG_SIZE(1,4,0,0,0)
#define FMT__D_D_D_H                                    TRACE_ARG_SIZE(1,3,0,0,0)
#define FMT__D_D_D_H_H                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__D_D_D_H_H_H_H                              TRACE_ARG_SIZE(3,4,0,0,0)
#define FMT__D_D_D_L                                    TRACE_ARG_SIZE(0,3,1,0,0)
#define FMT__D_D_D_L_L                                  TRACE_ARG_SIZE(0,3,2,0,0)
#define FMT__D_D_D_P                                    TRACE_ARG_SIZE(0,3,0,1,0)
#define FMT__D_D_H                                      TRACE_ARG_SIZE(1,2,0,0,0)
#define FMT__D_D_H_D_H                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__D_D_H_H                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__D_D_H_P_D                                  TRACE_ARG_SIZE(1,3,0,1,0)
#define FMT__D_D_L                                      TRACE_ARG_SIZE(0,2,1,0,0)
#define FMT__D_D_L_P                                    TRACE_ARG_SIZE(0,2,1,1,0)
#define FMT__D_D_P                                      TRACE_ARG_SIZE(0,2,0,1,0)
#define FMT__D_D_P_D                                    TRACE_ARG_SIZE(0,3,0,1,0)
#define FMT__D_P_P_D                                    TRACE_ARG_SIZE(0,2,0,2,0)
#define FMT__D_P_P_P                                    TRACE_ARG_SIZE(0,1,0,3,0)
#define FMT__D_D_P_D_D                                  TRACE_ARG_SIZE(0,4,0,1,0)
#define FMT__D_D_P_D_L                                  TRACE_ARG_SIZE(0,3,1,1,0)
#define FMT__D_D_P_H                                    TRACE_ARG_SIZE(1,2,0,1,0)
#define FMT__D_D_P_H_H_H                                TRACE_ARG_SIZE(3,2,0,1,0)
#define FMT__D_D_P_P_P                                  TRACE_ARG_SIZE(0,2,0,3,0)
#define FMT__D_H                                        TRACE_ARG_SIZE(1,1,0,0,0)
#define FMT__D_H_A_D_D_D_H                              TRACE_ARG_SIZE(2,4,0,0,1)
#define FMT__D_H_D                                      TRACE_ARG_SIZE(1,2,0,0,0)
#define FMT__D_H_D_D                                    TRACE_ARG_SIZE(1,3,0,0,0)
#define FMT__D_H_D_D_H                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__D_H_D_D_H_D                                TRACE_ARG_SIZE(2,4,0,0,0)
#define FMT__D_H_D_H                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__D_H_D_H_H                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__D_H_D_P                                    TRACE_ARG_SIZE(1,2,0,1,0)
#define FMT__D_H_D_P_D                                  TRACE_ARG_SIZE(1,3,0,1,0)
#define FMT__D_H_H                                      TRACE_ARG_SIZE(2,1,0,0,0)
#define FMT__D_H_H_D                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__D_H_H_D_H                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__D_H_H_D_H_D                                TRACE_ARG_SIZE(3,3,0,0,0)
#define FMT__D_H_H_D_H_D_H                              TRACE_ARG_SIZE(4,3,0,0,0)
#define FMT__D_H_H_D_D_H_H_D_H                          TRACE_ARG_SIZE(5,4,0,0,0)
#define FMT__D_H_H_D_D_D_D                              TRACE_ARG_SIZE(2,5,0,0,0)
#define FMT__D_H_H_H                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__D_H_H_H_H                                  TRACE_ARG_SIZE(4,1,0,0,0)
#define FMT__D_H_H_H_H_H                                TRACE_ARG_SIZE(5,1,0,0,0)
#define FMT__D_H_H_H_H_H_H_D_D_D_D                      TRACE_ARG_SIZE(6,5,0,0,0)
#define FMT__D_H_L                                      TRACE_ARG_SIZE(1,1,1,0,0)
#define FMT__D_H_P                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__D_L                                        TRACE_ARG_SIZE(0,1,1,0,0)
#define FMT__D_L_L                                      TRACE_ARG_SIZE(0,1,2,0,0)
#define FMT__D_L_L_D                                    TRACE_ARG_SIZE(0,2,2,0,0)
#define FMT__D_L_L_H                                    TRACE_ARG_SIZE(1,1,2,0,0)
#define FMT__D_L_L_L                                    TRACE_ARG_SIZE(0,1,3,0,0)
#define FMT__D_P                                        TRACE_ARG_SIZE(0,1,0,1,0)
#define FMT__D_P_D                                      TRACE_ARG_SIZE(0,2,0,1,0)
#define FMT__D_P_D_D                                    TRACE_ARG_SIZE(0,3,0,1,0)
#define FMT__D_P_H_H_D_D                                TRACE_ARG_SIZE(2,3,0,1,0)
#define FMT__D_P_H_H_D_H_H                              TRACE_ARG_SIZE(4,2,0,1,0)
#define FMT__D_P_H_H_H_H                                TRACE_ARG_SIZE(4,1,0,1,0)
#define FMT__D_P_H_H_H_H_H                              TRACE_ARG_SIZE(5,1,0,1,0)
#define FMT__D_P_H_H_H_H_H_H                            TRACE_ARG_SIZE(6,1,0,1,0)
#define FMT__D_P_H_P                                    TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__D_P_P                                      TRACE_ARG_SIZE(0,1,0,2,0)
#define FMT__D_P_P_D_D                                  TRACE_ARG_SIZE(0,3,0,2,0)
#define FMT__D_P_P_D_D_P                                TRACE_ARG_SIZE(0,3,0,3,0)
#define FMT__D_P_P_D_D_H_H                              TRACE_ARG_SIZE(2,3,0,2,0)
#define FMT__D_P_P_H                                    TRACE_ARG_SIZE(1,1,0,2,0)
#define FMT__H                                          TRACE_ARG_SIZE(1,0,0,0,0)
#define FMT__H_A                                        TRACE_ARG_SIZE(1,0,0,0,1)
#define FMT__H_B                                        TRACE_ARG_SIZE(1,0,0,0,2)
#define FMT__H_A_A                                      TRACE_ARG_SIZE(1,0,0,0,2)
#define FMT__H_A_A_H_H                                  TRACE_ARG_SIZE(3,0,0,0,2)
#define FMT__H_A_H                                      TRACE_ARG_SIZE(2,0,0,0,1)
#define FMT__H_A_H_H_H                                  TRACE_ARG_SIZE(4,0,0,0,1)
#define FMT__H_A_H_H_H_H_H_H_H_H                        TRACE_ARG_SIZE(9,0,0,0,1)
#define FMT__H_C_D_C                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__H_C_H_C                                    TRACE_ARG_SIZE(4,0,0,0,0)
#define FMT__H_D                                        TRACE_ARG_SIZE(1,1,0,0,0)
#define FMT__H_D_L                                      TRACE_ARG_SIZE(1,1,1,0,0)
#define FMT__H_D_A                                      TRACE_ARG_SIZE(1,1,0,0,1)
#define FMT__H_D_A_H_D                                  TRACE_ARG_SIZE(2,2,0,0,1)
#define FMT__H_D_A_H_D_H                                TRACE_ARG_SIZE(3,2,0,0,1)
#define FMT__H_D_A_H_H                                  TRACE_ARG_SIZE(3,1,0,0,1)
#define FMT__H_D_A_H_H_H_H                              TRACE_ARG_SIZE(5,1,0,0,1)
#define FMT__H_D_D                                      TRACE_ARG_SIZE(1,2,0,0,0)
#define FMT__H_D_D_D                                    TRACE_ARG_SIZE(1,3,0,0,0)
#define FMT__H_D_D_D_D                                  TRACE_ARG_SIZE(1,4,0,0,0)
#define FMT__H_D_D_D_D_H                                TRACE_ARG_SIZE(2,4,0,0,0)
#define FMT__H_D_D_D_H_D                                TRACE_ARG_SIZE(2,4,0,0,0)
#define FMT__H_D_D_D_H_H_D                              TRACE_ARG_SIZE(3,4,0,0,0)
#define FMT__H_D_D_H                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__H_D_D_H_D                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__H_D_D_H_D_H                                TRACE_ARG_SIZE(3,3,0,0,0)
#define FMT__H_D_D_H_H                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__H_D_D_H_H_H                                TRACE_ARG_SIZE(4,2,0,0,0)
#define FMT__H_D_D_H_H_H_H                              TRACE_ARG_SIZE(5,2,0,0,0)
#define FMT__H_D_D_H_P                                  TRACE_ARG_SIZE(2,2,0,1,0)
#define FMT__H_D_D_P                                    TRACE_ARG_SIZE(1,2,0,1,0)
#define FMT__H_D_H                                      TRACE_ARG_SIZE(2,1,0,0,0)
#define FMT__H_D_H_D                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__H_D_H_D_D                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__H_D_H_D_H                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__H_D_H_D_H_H                                TRACE_ARG_SIZE(4,2,0,0,0)
#define FMT__H_D_H_A                                    TRACE_ARG_SIZE(2,1,0,0,1)
#define FMT__H_D_H_H                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__H_D_H_P_D                                  TRACE_ARG_SIZE(2,2,0,1,0)
#define FMT__H_D_H_H_H                                  TRACE_ARG_SIZE(4,1,0,0,0)
#define FMT__H_D_H_H_H_H                                TRACE_ARG_SIZE(5,1,0,0,0)
#define FMT__H_D_H_H_H_H_H_H                            TRACE_ARG_SIZE(7,1,0,0,0)
#define FMT__H_D_P_H_H_H_H_H                            TRACE_ARG_SIZE(6,1,0,1,0)
#define FMT__H_D_P_H_H_H_H_H_H_H                        TRACE_ARG_SIZE(8,1,0,1,0)
#define FMT__H_D_P_P_P                                  TRACE_ARG_SIZE(1,1,0,3,0)
#define FMT__H_H                                        TRACE_ARG_SIZE(2,0,0,0,0)
#define FMT__H_H_A                                      TRACE_ARG_SIZE(2,0,0,0,1)
#define FMT__H_H_H_A                                    TRACE_ARG_SIZE(3,0,0,0,1)
#define FMT__H_H_A_A                                    TRACE_ARG_SIZE(2,0,0,0,2)
#define FMT__H_H_D                                      TRACE_ARG_SIZE(2,1,0,0,0)
#define FMT__H_H_D_A                                    TRACE_ARG_SIZE(2,1,0,0,1)
#define FMT__H_H_D_D                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__H_H_D_D_D                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__H_H_D_H                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__H_H_D_H_H_H_H_H_H_H_H                      TRACE_ARG_SIZE(10,1,0,0,0)
#define FMT__D_H_H_H_H_H_H_H_H                          TRACE_ARG_SIZE(8,1,0,0,0)
#define FMT__H_H_D_H_P                                  TRACE_ARG_SIZE(3,1,0,1,0)
#define FMT__H_D_P                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__H_H_D_P                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__H_H_H                                      TRACE_ARG_SIZE(3,0,0,0,0)
#define FMT__H_H_H_D                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__H_H_H_D_D                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__H_H_H_D_D_H_A_H_A                          TRACE_ARG_SIZE(5,2,0,0,2)
#define FMT__H_D_H_H_H_D_D_H_A_H_A                      TRACE_ARG_SIZE(6,3,0,0,2)
#define FMT__D_H_H_H_D_D_H_A_H_A                        TRACE_ARG_SIZE(5,3,0,0,2)
#define FMT__H_H_H_D_D_D                                TRACE_ARG_SIZE(3,3,0,0,0)
#define FMT__H_H_H_D_H                                  TRACE_ARG_SIZE(4,1,0,0,0)
#define FMT__H_H_H_D_H_P                                TRACE_ARG_SIZE(4,1,0,1,0)
#define FMT__H_H_H_H                                    TRACE_ARG_SIZE(4,0,0,0,0)
#define FMT__H_H_H_H_D                                  TRACE_ARG_SIZE(4,1,0,0,0)
#define FMT__H_H_H_H_D_H                                TRACE_ARG_SIZE(5,1,0,0,0)
#define FMT__H_H_H_H_H                                  TRACE_ARG_SIZE(5,0,0,0,0)
#define FMT__H_H_H_H_H_H                                TRACE_ARG_SIZE(6,0,0,0,0)
#define FMT__H_H_H_H_H_H_H                              TRACE_ARG_SIZE(7,0,0,0,0)
#define FMT__H_H_H_H_H_H_H_H                            TRACE_ARG_SIZE(8,0,0,0,0)
#define FMT__H_H_H_H_H_H_H_H_H                          TRACE_ARG_SIZE(9,0,0,0,0)
#define FMT__H_H_H_H_H_L                                TRACE_ARG_SIZE(5,0,1,0,0)
#define FMT__H_H_H_H_H_H_H_H_P_P                        TRACE_ARG_SIZE(8,0,0,2,0)
#define FMT__H_H_H_H_H_H_P_P                            TRACE_ARG_SIZE(6,0,0,2,0)
#define FMT__H_H_H_H_H_H_P_P_P_P                        TRACE_ARG_SIZE(6,2,0,4,0)
#define FMT__H_H_H_H_P_P_P                              TRACE_ARG_SIZE(4,0,0,3,0)
#define FMT__H_H_H_P                                    TRACE_ARG_SIZE(3,0,0,1,0)
#define FMT__H_D_H_P                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__H_H_L                                      TRACE_ARG_SIZE(2,0,1,0,0)
#define FMT__H_H_L_H                                    TRACE_ARG_SIZE(3,0,1,0,0)
#define FMT__H_H_H_L                                    TRACE_ARG_SIZE(3,0,1,0,0)
#define FMT__H_H_H_H_L                                  TRACE_ARG_SIZE(4,0,1,0,0)
#define FMT__H_H_L_H_P                                  TRACE_ARG_SIZE(3,0,1,4,0)
#define FMT__H_H_P                                      TRACE_ARG_SIZE(2,0,0,1,0)
#define FMT__H_H_P_D                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__H_H_P_P_P                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__H_L                                        TRACE_ARG_SIZE(1,0,1,0,0)
#define FMT__H_L_A                                      TRACE_ARG_SIZE(1,0,1,0,1)
#define FMT__H_L_D                                      TRACE_ARG_SIZE(1,1,1,0,0)
#define FMT__H_L_L                                      TRACE_ARG_SIZE(1,0,2,0,0)
#define FMT__H_L_H                                      TRACE_ARG_SIZE(2,0,1,0,0)
#define FMT__H_L_D_D                                    TRACE_ARG_SIZE(1,2,1,0,0)
#define FMT__H_L_D_D_D                                  TRACE_ARG_SIZE(1,3,1,0,0)
#define FMT__H_L_D_P                                    TRACE_ARG_SIZE(1,1,1,1,0)
#define FMT__H_L_H_D                                    TRACE_ARG_SIZE(2,1,1,0,0)
#define FMT__H_L_H_D_D                                  TRACE_ARG_SIZE(2,2,1,0,0)
#define FMT__H_L_H_H_H_H_H_H_H_H_H_H_H_H                TRACE_ARG_SIZE(13,0,1,0,0)
#define FMT__H_D_H_H_H_H_H_H_H_H_H_H_H_H                TRACE_ARG_SIZE(13,1,0,0,0)
#define FMT__H_L_H_H_P                                  TRACE_ARG_SIZE(3,0,1,4,0)
#define FMT__H_L_L_H_A                                  TRACE_ARG_SIZE(2,0,2,0,1)
#define FMT__H_L_D_D_H                                  TRACE_ARG_SIZE(2,2,1,0,0)
#define FMT__H_P_D_H_H                                  TRACE_ARG_SIZE(3,1,0,1,0)
#define FMT__H_P_D_P                                    TRACE_ARG_SIZE(1,1,0,2,0)
#define FMT__H_P_P_D_P                                  TRACE_ARG_SIZE(1,1,0,3,0)
#define FMT__H_P_D_D_H                                  TRACE_ARG_SIZE(2,2,0,1,0)
#define FMT__H_P                                        TRACE_ARG_SIZE(1,0,0,1,0)
#define FMT__H_P_D                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__H_P_D_D_H_P                                TRACE_ARG_SIZE(2,2,0,2,0)
#define FMT__H_P_D_D_P                                  TRACE_ARG_SIZE(1,2,0,2,0)
#define FMT__H_P_H                                      TRACE_ARG_SIZE(2,0,0,1,0)
#define FMT__H_P_H_D                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__H_P_H_D_H                                  TRACE_ARG_SIZE(3,1,0,1,0)
#define FMT__H_P_H_H                                    TRACE_ARG_SIZE(3,0,0,1,0)
#define FMT__H_P_H_H_H                                  TRACE_ARG_SIZE(4,0,0,1,0)
#define FMT__H_P_H_H_D                                  TRACE_ARG_SIZE(3,1,0,1,0)
#define FMT__H_P_H_H_H_H                                TRACE_ARG_SIZE(5,0,0,1,0)
#define FMT__H_P_H_P                                    TRACE_ARG_SIZE(2,0,0,2,0)
#define FMT__H_P_H_P_H                                  TRACE_ARG_SIZE(3,0,0,2,0)
#define FMT__H_P_H_P_H_H                                TRACE_ARG_SIZE(4,0,0,2,0)
#define FMT__H_P_H_P_H_H_P                              TRACE_ARG_SIZE(4,0,0,3,0)
#define FMT__H_P_L_H                                    TRACE_ARG_SIZE(2,0,1,1,0)
#define FMT__H_P_P_P                                    TRACE_ARG_SIZE(1,0,0,3,0)
#define FMT__H_P_P_P_D_P_P                              TRACE_ARG_SIZE(1,1,0,5,0)
#define FMT__H_P_P_P_P                                  TRACE_ARG_SIZE(1,0,0,4,0)
#define FMT__H_P_P_P_P_P                                TRACE_ARG_SIZE(1,0,0,5,0)
#define FMT__L                                          TRACE_ARG_SIZE(0,0,1,0,0)
#define FMT__L_A                                        TRACE_ARG_SIZE(0,0,1,0,1)
#define FMT__L_D                                        TRACE_ARG_SIZE(0,1,1,0,0)
#define FMT__L_D_D                                      TRACE_ARG_SIZE(0,2,1,0,0)
#define FMT__L_D_D_D                                    TRACE_ARG_SIZE(0,3,1,0,0)
#define FMT__L_H                                        TRACE_ARG_SIZE(1,0,1,0,0)
#define FMT__L_H_H                                      TRACE_ARG_SIZE(2,0,1,0,0)
#define FMT__L_H_H_H                                    TRACE_ARG_SIZE(3,0,1,0,0)
#define FMT__L_L                                        TRACE_ARG_SIZE(0,0,2,0,0)
#define FMT__L_L_L                                      TRACE_ARG_SIZE(0,0,3,0,0)
#define FMT__L_L_L_H                                    TRACE_ARG_SIZE(1,0,3,0,0)
#define FMT__L_L_A_A                                    TRACE_ARG_SIZE(0,0,2,0,2)
#define FMT__L_D_A                                      TRACE_ARG_SIZE(0,1,1,0,1)
#define FMT__L_D_A_A                                    TRACE_ARG_SIZE(0,1,1,0,2)
#define FMT__L_L_A_A_H_L_H_H_H_H_H_H_H_H_H_H_H_H        TRACE_ARG_SIZE(13,0,3,0,2)
#define FMT__L_L_D_D                                    TRACE_ARG_SIZE(0,2,2,0,0)
#define FMT__L_L_H_L_H_H_H_H_H_H_H_H_H_H_H_H            TRACE_ARG_SIZE(13,0,3,0,0)
#define FMT__L_L_L_D                                    TRACE_ARG_SIZE(0,1,3,0,0)
#define FMT__L_L_L_D_L_D                                TRACE_ARG_SIZE(0,2,4,0,0)
#define FMT__L_L_L_L                                    TRACE_ARG_SIZE(0,0,4,0,0)
#define FMT__L_L_L_L_L                                  TRACE_ARG_SIZE(0,0,5,0,0)
#define FMT__L_L_L_L_L_L                                TRACE_ARG_SIZE(0,0,6,0,0)
#define FMT__L_L_L_L_L_L_L                              TRACE_ARG_SIZE(0,0,7,0,0)
#define FMT__L_P                                        TRACE_ARG_SIZE(0,0,1,1,0)
#define FMT__L_P_L                                      TRACE_ARG_SIZE(0,0,2,1,0)
#define FMT__P                                          TRACE_ARG_SIZE(0,0,0,1,0)
#define FMT__P_A                                        TRACE_ARG_SIZE(0,0,0,1,1)
#define FMT__P_D                                        TRACE_ARG_SIZE(0,1,0,1,0)
#define FMT__P_D_D                                      TRACE_ARG_SIZE(0,2,0,1,0)
#define FMT__P_D_D_D                                    TRACE_ARG_SIZE(0,3,0,1,0)
#define FMT__P_D_D_D_D                                  TRACE_ARG_SIZE(0,4,0,1,0)
#define FMT__P_D_D_H_D                                  TRACE_ARG_SIZE(1,3,0,1,0)
#define FMT__P_D_D_D_D_D                                TRACE_ARG_SIZE(0,5,0,1,0)
#define FMT__P_D_D_D_D_D_D                              TRACE_ARG_SIZE(0,6,0,1,0)
#define FMT__P_D_D_D_D_D_D_D                            TRACE_ARG_SIZE(0,7,0,1,0)
#define FMT__P_D_D_D_H_D                                TRACE_ARG_SIZE(1,4,0,1,0)
#define FMT__P_D_D_H                                    TRACE_ARG_SIZE(1,2,0,1,0)
#define FMT__P_D_D_L_L_P                                TRACE_ARG_SIZE(0,2,2,2,0)
#define FMT__P_D_H                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__P_D_P                                      TRACE_ARG_SIZE(0,1,0,2,0)
#define FMT__P_D_L                                      TRACE_ARG_SIZE(0,1,1,1,0)
#define FMT__P_D_P_D                                    TRACE_ARG_SIZE(0,2,0,2,0)
#define FMT__P_D_P_P                                    TRACE_ARG_SIZE(0,1,0,3,0)
#define FMT__P_H                                        TRACE_ARG_SIZE(1,0,0,1,0)
#define FMT__P_H_D                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__P_H_D_D                                    TRACE_ARG_SIZE(1,2,0,1,0)
#define FMT__P_H_D_D_H                                  TRACE_ARG_SIZE(2,2,0,1,0)
#define FMT__P_H_D_P                                    TRACE_ARG_SIZE(1,1,0,2,0)
#define FMT__P_H_D_H                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__P_H_D_L                                    TRACE_ARG_SIZE(1,1,1,1,0)
#define FMT__P_H_H                                      TRACE_ARG_SIZE(2,0,0,1,0)
#define FMT__P_H_H_D_D                                  TRACE_ARG_SIZE(2,2,0,1,0)
#define FMT__P_H_H_H                                    TRACE_ARG_SIZE(3,0,0,1,0)
#define FMT__P_H_H_H_A                                  TRACE_ARG_SIZE(3,0,0,1,1)
#define FMT__P_H_H_H_D                                  TRACE_ARG_SIZE(3,1,0,1,0)
#define FMT__P_H_H_H_H                                  TRACE_ARG_SIZE(4,0,0,1,0)
#define FMT__P_H_H_H_H_D                                TRACE_ARG_SIZE(4,1,0,1,0)
#define FMT__P_H_H_H_H_H_H                              TRACE_ARG_SIZE(6,0,0,1,0)
#define FMT__P_H_H_H_H_H_H_H                            TRACE_ARG_SIZE(7,0,0,1,0)
#define FMT__P_H_H_H_L                                  TRACE_ARG_SIZE(3,0,1,1,0)
#define FMT__P_H_H_L                                    TRACE_ARG_SIZE(2,0,1,1,0)
#define FMT__P_H_L                                      TRACE_ARG_SIZE(1,0,1,1,0)
#define FMT__P_H_L_H_L                                  TRACE_ARG_SIZE(2,0,2,1,0)
#define FMT__P_H_P                                      TRACE_ARG_SIZE(1,0,0,2,0)
#define FMT__P_H_P_D                                    TRACE_ARG_SIZE(1,1,0,2,0)
#define FMT__P_H_P_D_D                                  TRACE_ARG_SIZE(1,2,0,2,0)
#define FMT__P_H_P_D_D_D                                TRACE_ARG_SIZE(1,3,0,2,0)
#define FMT__P_H_P_H                                    TRACE_ARG_SIZE(2,0,0,2,0)
#define FMT__P_H_P_H_L                                  TRACE_ARG_SIZE(2,0,1,2,0)
#define FMT__P_H_P_P                                    TRACE_ARG_SIZE(1,0,0,3,0)
#define FMT__P_H_P_L                                    TRACE_ARG_SIZE(1,0,1,2,0)
#define FMT__P_H_P_P_H                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__P_H_P_P_P                                  TRACE_ARG_SIZE(1,0,0,4,0)
#define FMT__P_L                                        TRACE_ARG_SIZE(0,0,1,1,0)
#define FMT__P_L_D                                      TRACE_ARG_SIZE(0,1,1,1,0)
#define FMT__P_L_H                                      TRACE_ARG_SIZE(1,0,1,1,0)
#define FMT__P_L_P                                      TRACE_ARG_SIZE(0,0,1,2,0)
#define FMT__P_L_L_D                                    TRACE_ARG_SIZE(0,1,2,1,0)
#define FMT__P_L_L_L                                    TRACE_ARG_SIZE(0,0,3,1,0)
#define FMT__P_L_P_L                                    TRACE_ARG_SIZE(0,0,2,2,0)
#define FMT__P_L_P_P_P                                  TRACE_ARG_SIZE(0,0,1,4,0)
#define FMT__P_P                                        TRACE_ARG_SIZE(0,0,0,2,0)
#define FMT__P_P_L                                      TRACE_ARG_SIZE(0,0,1,2,0)
#define FMT__P_P_D                                      TRACE_ARG_SIZE(0,1,0,2,0)
#define FMT__P_P_D_D                                    TRACE_ARG_SIZE(0,2,0,2,0)
#define FMT__P_P_D_D_H                                  TRACE_ARG_SIZE(1,2,0,2,0)
#define FMT__P_P_D_D_P                                  TRACE_ARG_SIZE(0,2,0,3,0)
#define FMT__P_P_D_H_H                                  TRACE_ARG_SIZE(2,1,0,2,0)
#define FMT__P_P_D_H_P                                  TRACE_ARG_SIZE(1,1,0,3,0)
#define FMT__P_P_D_P                                    TRACE_ARG_SIZE(0,1,0,3,0)
#define FMT__P_P_D_P_D                                  TRACE_ARG_SIZE(0,2,0,3,0)
#define FMT__P_P_D_P_H                                  TRACE_ARG_SIZE(1,1,0,3,0)
#define FMT__P_P_H                                      TRACE_ARG_SIZE(1,0,0,2,0)
#define FMT__P_P_H_D                                    TRACE_ARG_SIZE(1,1,0,2,0)
#define FMT__P_P_H_H                                    TRACE_ARG_SIZE(2,0,0,2,0)
#define FMT__P_P_H_H_P                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__P_P_H_H_H                                  TRACE_ARG_SIZE(3,0,0,2,0)
#define FMT__P_P_H_P                                    TRACE_ARG_SIZE(1,0,0,3,0)
#define FMT__P_P_H_P_D_P                                TRACE_ARG_SIZE(1,2,0,4,0)
#define FMT__P_P_H_P_H                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__P_P_L_D                                    TRACE_ARG_SIZE(0,1,1,2,0)
#define FMT__P_P_L_L_H                                  TRACE_ARG_SIZE(1,0,2,2,0)
#define FMT__P_P_P                                      TRACE_ARG_SIZE(0,0,0,3,0)
#define FMT__P_P_P_H                                    TRACE_ARG_SIZE(1,0,0,3,0)
#define FMT__P_P_P_D                                    TRACE_ARG_SIZE(0,1,0,3,0)
#define FMT__P_P_P_D_P                                  TRACE_ARG_SIZE(0,1,0,4,0)
#define FMT__P_P_P_H_H                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__P_P_P_L_H                                  TRACE_ARG_SIZE(1,0,1,3,0)
#define FMT__P_P_P_L_D_H                                TRACE_ARG_SIZE(1,1,1,3,0)
#define FMT__P_P_P_L_H_H                                TRACE_ARG_SIZE(2,0,1,3,0)
#define FMT__P_P_P_P                                    TRACE_ARG_SIZE(0,0,0,4,0)
#define FMT__P_L_H_H                                    TRACE_ARG_SIZE(2,0,1,1,0)
#define FMT__P_P_P_P_P                                  TRACE_ARG_SIZE(0,0,0,5,0)
#define FMT__D_D_L_D                                    TRACE_ARG_SIZE(0,3,1,0,0)
#define FMT__D_D_L_L                                    TRACE_ARG_SIZE(0,2,2,0,0)
#define FMT__D_D_D_D_L_L                                TRACE_ARG_SIZE(0,4,2,0,0)
#define FMT__L_D_D_D_D_L_L                              TRACE_ARG_SIZE(0,4,3,0,0)
#define FMT__L_L_L_D_D                                  TRACE_ARG_SIZE(0,2,3,0,0)
#define FMT__D_H_P_H                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__H_D_H_H_D                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__D_A_B_H_H                                  TRACE_ARG_SIZE(2,1,0,0,3)
#define FMT__A_A_P                                      TRACE_ARG_SIZE(0,0,0,1,2)
#define FMT__B                                          TRACE_ARG_SIZE(0,0,0,0,2)
#define FMT__B_H_H                                      TRACE_ARG_SIZE(2,0,0,0,2)
#define FMT__B_D_H                                      TRACE_ARG_SIZE(1,1,0,0,2)
#define FMT__B_H_B                                      TRACE_ARG_SIZE(1,0,0,0,4)
#define FMT__B_B_B                                      TRACE_ARG_SIZE(0,0,0,0,6)
#define FMT__D_H_D_B                                    TRACE_ARG_SIZE(1,2,0,0,2)
#define FMT__D_H_H_B                                    TRACE_ARG_SIZE(2,1,0,0,2)
#define FMT__H_H_A_B                                    TRACE_ARG_SIZE(2,0,0,0,3)
#define FMT__A_D                                        TRACE_ARG_SIZE(0,1,0,0,1)
#define FMT__L_D_P_H                                    TRACE_ARG_SIZE(1,1,1,1,0)
#define FMT__D_D_H_H_H                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__P_B_D_D                                    TRACE_ARG_SIZE(0,2,0,1,2)
#define FMT__H_B_H_B                                    TRACE_ARG_SIZE(2,0,0,0,4)
#define FMT__B_A                                        TRACE_ARG_SIZE(0,0,0,0,3)
#define FMT__D_H_A                                      TRACE_ARG_SIZE(1,1,0,0,1)
#define FMT__D_H_A_H                                    TRACE_ARG_SIZE(2,1,0,0,1)
#define FMT__D_A_H                                      TRACE_ARG_SIZE(1,1,0,0,1)
#define FMT__D_D_D_A_B                                  TRACE_ARG_SIZE(0,3,0,0,3)
#define FMT__D_B_A                                      TRACE_ARG_SIZE(0,1,0,0,3)
#define FMT__H_B_A                                      TRACE_ARG_SIZE(1,0,0,0,3)
#define FMT__D_D_H_H_D_D_D_D                            TRACE_ARG_SIZE(2,6,0,0,0)
#define FMT__P_D_H_H_H_A                                TRACE_ARG_SIZE(3,1,0,1,1)
#define FMT__H_A_H_H_H_H                                TRACE_ARG_SIZE(5,0,0,0,1)
#define FMT__H_A_H_H_H_H_H                              TRACE_ARG_SIZE(6,0,0,0,1)
#define FMT__H_D_A_H                                    TRACE_ARG_SIZE(2,1,0,0,1)
#define FMT__D_A_H_D                                    TRACE_ARG_SIZE(1,2,0,0,1)
#define FMT__D_D_A_H                                    TRACE_ARG_SIZE(1,2,0,0,1)
#define FMT__P_H_H_H_H_H_H_H_H                          TRACE_ARG_SIZE(8,0,0,1,0)
#define FMT__D_D_H_L_H_H                                TRACE_ARG_SIZE(3,2,1,0,0)
#define FMT__D_H_L_H_H                                  TRACE_ARG_SIZE(3,1,1,0,0)
#define FMT__D_H_L_L_D_D_D                              TRACE_ARG_SIZE(1,4,2,0,0)
#define FMT__H_D_H_L_H_H                                TRACE_ARG_SIZE(4,1,1,0,0)
#define FMT__D_H_D_H_L_H_H                              TRACE_ARG_SIZE(4,2,1,0,0)
#define FMT__D_D_H_H_H_D_D_D_D                          TRACE_ARG_SIZE(3,6,0,0,0)
#define FMT__H_D_H_L                                    TRACE_ARG_SIZE(2,1,1,0,0)
#define FMT__H_H_L_L                                    TRACE_ARG_SIZE(2,0,2,0,0)
#define FMT__D_D_H_H_D_H_D_D_D_D                        TRACE_ARG_SIZE(3,7,0,0,0)
#define FMT__D_A_D_A                                    TRACE_ARG_SIZE(0,2,0,0,2)
#define FMT__H_H_H_L_H_H_H                              TRACE_ARG_SIZE(6,0,1,0,0)
#define FMT__H_D_D_D_D_D                                TRACE_ARG_SIZE(1,5,0,0,0)
#define FMT__P_P_D_D_L                                  TRACE_ARG_SIZE(0,2,1,2,0)
#define FMT__P_L_L                                      TRACE_ARG_SIZE(0,0,2,1,0)
#define FMT__P_P_L_L                                    TRACE_ARG_SIZE(0,0,2,2,0)
#define FMT__D_D_H_D_D_H                                TRACE_ARG_SIZE(2,4,0,0,0)
/* FIXME: will win_com_dump be able to decoed that? */
#define FMT__NC(N)                                      TRACE_ARG_SIZE(N,0,0,0,0)
#define FMT__H_L_H_H                                    TRACE_ARG_SIZE(3,0,1,0,0)
#define FMT__A_B                                        TRACE_ARG_SIZE(0,0,0,1,1)
#define FMT__D_H_H_L_D_D_D                              TRACE_ARG_SIZE(2,4,1,0,0)
#define FMT__H_H_D_D_H                                  TRACE_ARG_SIZE(3,3,0,0,0)
#define FMT__L_D_H_H_H_H                                TRACE_ARG_SIZE(4,1,1,0,0)
#define FMT__P_A_A                                      TRACE_ARG_SIZE(0,0,0,1,2)
#define FMT__D_D_P_H_H_H_H                              TRACE_ARG_SIZE(4,2,1,0,0)
#define FMT__P_P_D_D_D                                  TRACE_ARG_SIZE(0,3,0,2,0)
#define FMT__P_P_D_D_D_P                                TRACE_ARG_SIZE(0,3,0,3,0)
#define FMT__P_P_P_D_D_D_P                              TRACE_ARG_SIZE(0,3,0,4,0)
#define FMT__D_D_H_H_H_H                                TRACE_ARG_SIZE(2,4,0,0,0)
#define FMT__P_D_P_D_P_P                                TRACE_ARG_SIZE(4,2,0,0,0)
#define FMT__D_A_D                                      TRACE_ARG_SIZE(0,2,0,0,1)
#define FMT__D_H_D_D_D_D                                TRACE_ARG_SIZE(1,5,0,0,0)
#define FMT__H_D_D_D_H_H                                TRACE_ARG_SIZE(3,3,0,0,0)
#define FMT__P_H_D_D_D_H_H                              TRACE_ARG_SIZE(3,3,1,0,0)
#define FMT__H_H_H_D_H_L_H_H                            TRACE_ARG_SIZE(6,1,1,0,0)
#define FMT__D_D_P_P                                    TRACE_ARG_SIZE(0,2,0,2,0)
#define FMT__D_D_P_P_D_D                                TRACE_ARG_SIZE(0,4,0,2,0)
#define FMT__H_P_P_H_H                                  TRACE_ARG_SIZE(3,0,0,2,0)
#define FMT__D_P_H_H                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__H_P_H_H_P                                  TRACE_ARG_SIZE(3,0,0,2,0)
#define FMT__H_P_P                                      TRACE_ARG_SIZE(1,0,0,2,0)
#define FMT__H_P_P_H                                    TRACE_ARG_SIZE(2,0,0,2,0)
#define FMT__D_P_P_H_H                                  TRACE_ARG_SIZE(2,1,0,2,0)
#define FMT__D_P_D_D_P                                  TRACE_ARG_SIZE(0,3,0,2,0)
#define FMT__D_D_D_A_A                                  TRACE_ARG_SIZE(0,3,0,0,2)
#define FMT__P_H_P_H_D                                  TRACE_ARG_SIZE(2,1,0,0,2)
#define FMT__H_D_A_D_D_D                                TRACE_ARG_SIZE(1,4,0,0,1)
#define FMT__H_D_A_P                                    TRACE_ARG_SIZE(1,1,0,1,1)
#define FMT__H_H_H_H_H_H_H_H_H_H                        TRACE_ARG_SIZE(10,0,0,0,0)
#define FMT__P_D_P_D_D_D                                TRACE_ARG_SIZE(0,4,0,2,0)
#define FMT__D_P_D_H_H                                  TRACE_ARG_SIZE(2,2,0,1,0)
#define FMT__D_D_D_D_H_D                                TRACE_ARG_SIZE(1,5,0,0,0)
#define FMT__D_P_H                                      TRACE_ARG_SIZE(1,1,0,1,0)

/** @} */ /* TRACE_DATA_FORMAT */

/**
 *  @addtogroup TRACE_FIRST_ARG
 *  @{
 */
#define TRACE_COMMON1 TRACE_SUBSYSTEM_COMMON, 1U
#define TRACE_COMMON2 TRACE_SUBSYSTEM_COMMON, 2U
#define TRACE_COMMON3 TRACE_SUBSYSTEM_COMMON, 3U
#define TRACE_COMMON4 TRACE_SUBSYSTEM_COMMON, 4U

/* osif subsystem is nearly not used. Place it to the same with common and free
 * 1 bit for buffers. */
#define TRACE_OSIF1 TRACE_SUBSYSTEM_COMMON, 1U
#define TRACE_OSIF2 TRACE_SUBSYSTEM_COMMON, 2U
#define TRACE_OSIF3 TRACE_SUBSYSTEM_COMMON, 3U
#define TRACE_OSIF4 TRACE_SUBSYSTEM_COMMON, 4U

#define TRACE_MAC1 TRACE_SUBSYSTEM_MAC, 1U
#define TRACE_MAC2 TRACE_SUBSYSTEM_MAC, 2U
#define TRACE_MAC3 TRACE_SUBSYSTEM_MAC, 3U
#define TRACE_MAC4 TRACE_SUBSYSTEM_MAC, 4U

#define TRACE_MACSPLIT1 TRACE_SUBSYSTEM_MACSPLIT, 1U
#define TRACE_MACSPLIT2 TRACE_SUBSYSTEM_MACSPLIT, 2U
#define TRACE_MACSPLIT3 TRACE_SUBSYSTEM_MACSPLIT, 3U
#define TRACE_MACSPLIT4 TRACE_SUBSYSTEM_MACSPLIT, 4U

#define TRACE_MACLL1 TRACE_SUBSYSTEM_MACLL, 1U
#define TRACE_MACLL2 TRACE_SUBSYSTEM_MACLL, 2U
#define TRACE_MACLL3 TRACE_SUBSYSTEM_MACLL, 3U
#define TRACE_MACLL4 TRACE_SUBSYSTEM_MACLL, 4U

#define TRACE_NWK1 TRACE_SUBSYSTEM_NWK, 1U
#define TRACE_NWK2 TRACE_SUBSYSTEM_NWK, 2U
#define TRACE_NWK3 TRACE_SUBSYSTEM_NWK, 3U
#define TRACE_NWK4 TRACE_SUBSYSTEM_NWK, 4U

#define TRACE_APS1 TRACE_SUBSYSTEM_APS, 1U
#define TRACE_APS2 TRACE_SUBSYSTEM_APS, 2U
#define TRACE_APS3 TRACE_SUBSYSTEM_APS, 3U
#define TRACE_APS4 TRACE_SUBSYSTEM_APS, 4U

#define TRACE_AF1 TRACE_SUBSYSTEM_AF, 1U
#define TRACE_AF2 TRACE_SUBSYSTEM_AF, 2U
#define TRACE_AF3 TRACE_SUBSYSTEM_AF, 3U
#define TRACE_AF4 TRACE_SUBSYSTEM_AF, 4U

#define TRACE_ZDO1 TRACE_SUBSYSTEM_ZDO, 1U
#define TRACE_ZDO2 TRACE_SUBSYSTEM_ZDO, 2U
#define TRACE_ZDO3 TRACE_SUBSYSTEM_ZDO, 3U
#define TRACE_ZDO4 TRACE_SUBSYSTEM_ZDO, 4U

#define TRACE_SECUR1 TRACE_SUBSYSTEM_SECUR, 1U
#define TRACE_SECUR2 TRACE_SUBSYSTEM_SECUR, 2U
#define TRACE_SECUR3 TRACE_SUBSYSTEM_SECUR, 3U
#define TRACE_SECUR4 TRACE_SUBSYSTEM_SECUR, 4U

#define TRACE_ZCL1 TRACE_SUBSYSTEM_ZCL, 1U
#define TRACE_ZCL2 TRACE_SUBSYSTEM_ZCL, 2U
#define TRACE_ZCL3 TRACE_SUBSYSTEM_ZCL, 3U
#define TRACE_ZCL4 TRACE_SUBSYSTEM_ZCL, 4U

#define TRACE_ZLL1 TRACE_SUBSYSTEM_ZLL, 1U
#define TRACE_ZLL2 TRACE_SUBSYSTEM_ZLL, 2U
#define TRACE_ZLL3 TRACE_SUBSYSTEM_ZLL, 3U
#define TRACE_ZLL4 TRACE_SUBSYSTEM_ZLL, 4U

#define TRACE_ZGP1 TRACE_SUBSYSTEM_ZGP, 1U
#define TRACE_ZGP2 TRACE_SUBSYSTEM_ZGP, 2U
#define TRACE_ZGP3 TRACE_SUBSYSTEM_ZGP, 3U
#define TRACE_ZGP4 TRACE_SUBSYSTEM_ZGP, 4U

#define TRACE_ZSE1 TRACE_SUBSYSTEM_ZSE, 1U
#define TRACE_ZSE2 TRACE_SUBSYSTEM_ZSE, 2U
#define TRACE_ZSE3 TRACE_SUBSYSTEM_ZSE, 3U
#define TRACE_ZSE4 TRACE_SUBSYSTEM_ZSE, 4U

#ifndef DOXYGEN
#define TRACE_SPI1 TRACE_SUBSYSTEM_SPI, 1U
#define TRACE_SPI2 TRACE_SUBSYSTEM_SPI, 2U
#define TRACE_SPI3 TRACE_SUBSYSTEM_SPI, 3U
#define TRACE_SPI4 TRACE_SUBSYSTEM_SPI, 4U

#define TRACE_SSL1 TRACE_SUBSYSTEM_SSL, 1U
#define TRACE_SSL2 TRACE_SUBSYSTEM_SSL, 2U
#define TRACE_SSL3 TRACE_SUBSYSTEM_SSL, 3U
#define TRACE_SSL4 TRACE_SUBSYSTEM_SSL, 4U
#endif /* DOXYGEN */

#define TRACE_APP1 TRACE_SUBSYSTEM_APP, 1U
#define TRACE_APP2 TRACE_SUBSYSTEM_APP, 2U
#define TRACE_APP3 TRACE_SUBSYSTEM_APP, 3U
#define TRACE_APP4 TRACE_SUBSYSTEM_APP, 4U

#ifndef DOXYGEN
#define TRACE_SPECIAL1 TRACE_SUBSYSTEM_SPECIAL1, 1U
#define TRACE_SPECIAL2 TRACE_SUBSYSTEM_SPECIAL1, 2U
#define TRACE_SPECIAL3 TRACE_SUBSYSTEM_SPECIAL1, 3U
#define TRACE_SPECIAL4 TRACE_SUBSYSTEM_SPECIAL1, 4U

#define TRACE_USB1 TRACE_SUBSYSTEM_USB, 1U
#define TRACE_USB2 TRACE_SUBSYSTEM_USB, 2U
#define TRACE_USB3 TRACE_SUBSYSTEM_USB, 3U
#define TRACE_USB4 TRACE_SUBSYSTEM_USB, 4U

#define TRACE_MEMDBG1 TRACE_SUBSYSTEM_MEM, 1U
#define TRACE_MEMDBG2 TRACE_SUBSYSTEM_MEM, 2U
#define TRACE_MEMDBG3 TRACE_SUBSYSTEM_MEM, 3U
#define TRACE_MEMDBG4 TRACE_SUBSYSTEM_MEM, 4U

#define TRACE_CLOUD1 TRACE_SUBSYSTEM_CLOUD, 1U
#define TRACE_CLOUD2 TRACE_SUBSYSTEM_CLOUD, 2U
#define TRACE_CLOUD3 TRACE_SUBSYSTEM_CLOUD, 3U
#define TRACE_CLOUD4 TRACE_SUBSYSTEM_CLOUD, 4U

#define TRACE_HTTP1 TRACE_SUBSYSTEM_HTTP, 1U
#define TRACE_HTTP2 TRACE_SUBSYSTEM_HTTP, 2U
#define TRACE_HTTP3 TRACE_SUBSYSTEM_HTTP, 3U
#define TRACE_HTTP4 TRACE_SUBSYSTEM_HTTP, 4U

#define TRACE_JSON1 TRACE_SUBSYSTEM_JSON, 1U
#define TRACE_JSON2 TRACE_SUBSYSTEM_JSON, 2U
#define TRACE_JSON3 TRACE_SUBSYSTEM_JSON, 3U
#define TRACE_JSON4 TRACE_SUBSYSTEM_JSON, 4U

#define TRACE_LWIP1 TRACE_SUBSYSTEM_LWIP, 1U
#define TRACE_LWIP2 TRACE_SUBSYSTEM_LWIP, 2U
#define TRACE_LWIP3 TRACE_SUBSYSTEM_LWIP, 3U
#define TRACE_LWIP4 TRACE_SUBSYSTEM_LWIP, 4U

#define TRACE_VENSTAR1 TRACE_SUBSYSTEM_VENSTAR, 1U
#define TRACE_VENSTAR2 TRACE_SUBSYSTEM_VENSTAR, 2U
#define TRACE_VENSTAR3 TRACE_SUBSYSTEM_VENSTAR, 3U
#define TRACE_VENSTAR4 TRACE_SUBSYSTEM_VENSTAR, 4U

#define TRACE_TRANSPORT1 TRACE_SUBSYSTEM_TRANSPORT, 1U
#define TRACE_TRANSPORT2 TRACE_SUBSYSTEM_TRANSPORT, 2U
#define TRACE_TRANSPORT3 TRACE_SUBSYSTEM_TRANSPORT, 3U
#define TRACE_TRANSPORT4 TRACE_SUBSYSTEM_TRANSPORT, 4U

#define TRACE_UART1 TRACE_SUBSYSTEM_UART, 1U
#define TRACE_UART2 TRACE_SUBSYSTEM_UART, 2U
#define TRACE_UART3 TRACE_SUBSYSTEM_UART, 3U
#define TRACE_UART4 TRACE_SUBSYSTEM_UART, 4U

#define TRACE_BATTERY1 TRACE_SUBSYSTEM_BATTERY, 1U
#define TRACE_BATTERY2 TRACE_SUBSYSTEM_BATTERY, 2U
#define TRACE_BATTERY3 TRACE_SUBSYSTEM_BATTERY, 3U
#define TRACE_BATTERY4 TRACE_SUBSYSTEM_BATTERY, 4U

#define TRACE_OTA1 TRACE_SUBSYSTEM_OTA, 1U
#define TRACE_OTA2 TRACE_SUBSYSTEM_OTA, 2U
#define TRACE_OTA3 TRACE_SUBSYSTEM_OTA, 3U
#define TRACE_OTA4 TRACE_SUBSYSTEM_OTA, 4U

#define TRACE_MAC_API1 TRACE_SUBSYSTEM_MAC_API, 1U
#define TRACE_MAC_API2 TRACE_SUBSYSTEM_MAC_API, 2U
#define TRACE_MAC_API3 TRACE_SUBSYSTEM_MAC_API, 3U
#define TRACE_MAC_API4 TRACE_SUBSYSTEM_MAC_API, 4U

#define TRACE_ZBDIRECT1 TRACE_SUBSYSTEM_ZBDIRECT, 1U
#define TRACE_ZBDIRECT2 TRACE_SUBSYSTEM_ZBDIRECT, 2U
#define TRACE_ZBDIRECT3 TRACE_SUBSYSTEM_ZBDIRECT, 3U
#define TRACE_ZBDIRECT4 TRACE_SUBSYSTEM_ZBDIRECT, 4U

#define TRACE_DIAG1 TRACE_SUBSYSTEM_DIAGNOSTIC, 1U
#define TRACE_DIAG2 TRACE_SUBSYSTEM_DIAGNOSTIC, 2U
#define TRACE_DIAG3 TRACE_SUBSYSTEM_DIAGNOSTIC, 3U
#define TRACE_DIAG4 TRACE_SUBSYSTEM_DIAGNOSTIC, 4U

#define TRACE_NS1 TRACE_SUBSYSTEM_NS, 1U
#define TRACE_NS2 TRACE_SUBSYSTEM_NS, 2U
#define TRACE_NS3 TRACE_SUBSYSTEM_NS, 3U
#define TRACE_NS4 TRACE_SUBSYSTEM_NS, 4U

#define TRACE_NCP_TRANSPORT1 TRACE_SUBSYSTEM_NCP_TRANSPORT, 1U
#define TRACE_NCP_TRANSPORT2 TRACE_SUBSYSTEM_NCP_TRANSPORT, 2U
#define TRACE_NCP_TRANSPORT3 TRACE_SUBSYSTEM_NCP_TRANSPORT, 3U
#define TRACE_NCP_TRANSPORT4 TRACE_SUBSYSTEM_NCP_TRANSPORT, 4U

#define TRACE_TEST1 TRACE_SUBSYSTEM_TEST, 1U
#define TRACE_TEST2 TRACE_SUBSYSTEM_TEST, 2U
#define TRACE_TEST3 TRACE_SUBSYSTEM_TEST, 3U
#define TRACE_TEST4 TRACE_SUBSYSTEM_TEST, 4U

#define TRACE_ADDR1 TRACE_SUBSYSTEM_ADDR, 1U
#define TRACE_ADDR2 TRACE_SUBSYSTEM_ADDR, 2U
#define TRACE_ADDR3 TRACE_SUBSYSTEM_ADDR, 3U
#define TRACE_ADDR4 TRACE_SUBSYSTEM_ADDR, 4U

#define TRACE_PTA1 TRACE_SUBSYSTEM_PTA, 1U
#define TRACE_PTA2 TRACE_SUBSYSTEM_PTA, 2U
#define TRACE_PTA3 TRACE_SUBSYSTEM_PTA, 3U
#define TRACE_PTA4 TRACE_SUBSYSTEM_PTA, 4U

#endif /* DOXYGEN */

/** @} */ /* TRACE_FIRST_ARG */
/** @endcond */ /* DSR_TRACE */
/** @} */ /* Debug trace */

/** @endcond */ /* DOXYGEN_DEBUG_SECTION */

#endif /* ZB_LOGGER_H */
