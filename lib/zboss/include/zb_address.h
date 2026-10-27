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
/* PURPOSE: Zigbee address management
*/

#ifndef ZB_ADDRESS_H
#define ZB_ADDRESS_H 1

/*! \addtogroup ZB_NWK_ADDR */
/*! @{ */


/*! @cond internals_doc */

#define ZB_UNKNOWN_SHORT_ADDR 0xFFFFU

#ifdef ZB_DEBUG_ADDR
#define TRACE_ADDR_PROTO_VOID    zb_uint16_t from_file, zb_uint16_t from_line
#define TRACE_ADDR_CALL_VOID     ZB_TRACE_FILE_ID, __LINE__
#define TRACE_ADDR_FORWARD_VOID  from_file, from_line
#define TRACE_ADDR_PROTO         TRACE_ADDR_PROTO_VOID ,
#define TRACE_ADDR_CALL          TRACE_ADDR_CALL_VOID ,
#define TRACE_ADDR_FORWARD       TRACE_ADDR_FORWARD_VOID ,
#else
#define TRACE_ADDR_PROTO_VOID
#define TRACE_ADDR_CALL_VOID
#define TRACE_ADDR_FORWARD_VOID
#define TRACE_ADDR_PROTO
#define TRACE_ADDR_CALL
#define TRACE_ADDR_FORWARD
#endif  /* ZB_DEBUG_ADDR */

/*! @endcond */


/**
   Pan ID reference

   Should be used inside protocol tables instead of 64-bit Pan ID
*/
typedef zb_uint8_t zb_address_pan_id_ref_t;

#define ZB_ADDRESS_PAN_ID_REF_NONE ((zb_address_pan_id_ref_t)(-1))

/**
   IEEE address reference
   Should be used inside protocol tables instead of 64/16-bit IEEE.
*/
#ifndef ZB_NO_BIG_NET
typedef zb_uint16_t zb_address_ieee_ref_t;
#else
typedef zb_uint8_t zb_address_ieee_ref_t;
#endif /* !ZB_NO_BIG_NET */

/*! @cond internals_doc */

/**
   Element index in sorted short addresses table
   Should be used to get short address by its index by
   \ref zb_address_by_sorted_table_index function.
*/
typedef zb_address_ieee_ref_t zb_sorted_address_idx_t;

/*! @endcond */

/**
   NONE IEEE address reference
*/
#define ZB_ADDRESS_IEEE_REF_NONE ((zb_address_ieee_ref_t)(-1))

/**
   First IEEE address reference
*/
#define ZB_ADDRESS_IEEE_REF_FIRST ((zb_address_ieee_ref_t)(0U))

#define ZB_ADDRESS_SORTED_IDX_NONE ((zb_sorted_address_idx_t)(-1))

/**
 * Neighbor reference (index in neighbor table)
 */
typedef zb_uint8_t zb_nwk_neighbor_ref_t;

#define ZB_NWK_NEIGHBOR_REF_NONE ((zb_uint8_t)-1)

/*! @cond internals_doc */

/**
 * @brief IEEE address location in NVRAM
 *
 * NVRAM offset divided by 4 (relative to the NVRAM page start)
 * (all NVRAM records are aligned by 4 bytes)
 */
typedef zb_uint16_t zb_address_location_t;

#define ZB_ADDRESS_LOCATION_NONE ((zb_uint16_t)-1)

/**
 * @name Address redirect type
 * @anchor zb_addr_redirect_type_t
 *
 * Note: These values were members of `enum zb_addr_redirect_type_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
/** Regular entry without redirect */
#define ZB_ADDR_REDIRECT_NONE 0x00U
/** Entry contains both short and IEEE addresses and linked with secondary entry that contains only short address */
#define ZB_ADDR_REDIRECT_PRIMARY 0x01U
/** Entry doesn't contain any address and redirects to primary entry with both addresses */
#define ZB_ADDR_REDIRECT_SECONDARY 0x02U
/** @} */

typedef zb_uint8_t zb_addr_redirect_type_t;

#ifdef ZB_DEBUG_ADDR_EXT

#define ZB_ADDR_USAGE_TYPE_NONE 0U
#define ZB_ADDR_USAGE_TYPE_ALLOC 1U
#define ZB_ADDR_USAGE_TYPE_DELETE 2U
#define ZB_ADDR_USAGE_TYPE_LOCK 3U
#define ZB_ADDR_USAGE_TYPE_UNLOCK 4U

typedef zb_uint8_t zb_addr_usage_type_t;

typedef struct zb_addr_usage_s
{
  zb_uint8_t type;
  zb_uint16_t file;
  zb_uint16_t line;
} zb_addr_usage_t;

typedef struct zb_addr_debug_ctx_s
{
  zb_bool_t verification_pending;

  zb_addr_usage_t usage_alloc;
  zb_addr_usage_t usage_delete;
  zb_addr_usage_t usages[ZB_DEBUG_ADDR_EXT_ADDR_USAGES_CNT];
} zb_addr_debug_ctx_t;

#endif /* ZB_DEBUG_ADDR_EXT */

/**
   64-bit / 16-bit address map (8 bytes)
*/
typedef ZB_PACKED_PRE struct zb_address_map_s
{
  zb_uint8_t lock_cnt;            /*!< lock counter (active references counter), not locked if 0 */
  zb_bitfield_t used:1;           /*!< if 0, this entry is free (never used)  */
  zb_bitfield_t redirect_type:2;  /*!< redirect type (see \ref zb_addr_redirect_type_t) */
  zb_bitfield_t padding:5;        /*!< Explicit padding bits */

#ifdef ZB_DEBUG_ADDR_EXT
  zb_addr_debug_ctx_t dbg_ctx;
#endif /* ZB_DEBUG_ADDR_EXT */

  /* primary or secondary (for redirected entries) data */
  union
  {
    /* Primary entry data */
    ZB_PACKED_PRE struct zb_address_map_primary_s
    {
      zb_address_location_t ieee_addr_location; /*!< IEEE address location in NVRAM,
                                                 *   used for primary entries */
      zb_uint16_t addr; /*!< 16-bit device address, used for primary entries */
      zb_nwk_neighbor_ref_t neighbor_ref; /*!< Reference to neighbor (index in neighbor table) */

      zb_bitfield_t has_address_conflict:1; /*!< Set to 1 if device discovers address conflict
                                             *   Cleared when conflict is resolved:
                                             *   - Device that discovers conflict sending Network Status
                                             *   - or another Network Status with identical payload was received */
      zb_bitfield_t ieee_req_in_progr:1; /*!< ieee request for that address is in progress */
      zb_bitfield_t clock:1;    /*!< clock value for the clock usage algorithm */
      zb_bitfield_t pending_for_delete:1;    /*!< record is pending for deletion */
      zb_bitfield_t padding:4; /*!< Explicit padding bits */
    } ZB_PACKED_STRUCT primary;

    /* Secondary entry data (for entries redirected to primary entries) */
    ZB_PACKED_PRE struct zb_address_map_secondary_s
    {
      zb_address_ieee_ref_t redirect_ref;  /*!< Reference to primary record */
      zb_uint8_t padding[4]; /*!< Explicit padding bits */
    } ZB_PACKED_STRUCT secondary;
  } u;
} ZB_PACKED_STRUCT zb_address_map_t;

#ifdef ZB_DEBUG_ADDR_EXT
ZB_ASSERT_COMPILE_DECL(sizeof(zb_address_map_t) == 8U + sizeof(zb_addr_debug_ctx_t));
#else
ZB_ASSERT_COMPILE_DECL(sizeof(zb_address_map_t) == 8U);
#endif

/**
   Add Pan ID to address storage and return reference.

   @param short_pan_id - 16-bit Pan ID identifier
   @param pan_id - 64-bit Pan ID identifier
   @param ref - (output) reference to Pan ID.

   @return RET_OK - when success, error code otherwise.

   @b Example
@code
        zb_address_pan_id_ref_t panid_ref;
        zb_ret_t ret;

        ret = zb_address_set_pan_id(mhr.src_pan_id, beacon_payload->extended_panid, &panid_ref);
        if (ret == RET_ALREADY_EXISTS)
        {
          ret = RET_OK;
        }
@endcode

 */
zb_ret_t zb_address_set_pan_id(zb_uint16_t short_pan_id, const zb_ext_pan_id_t pan_id, zb_address_pan_id_ref_t *ref);


/**
   Get extended Pan ID with reference.

   @param pan_id_ref - reference to Pan ID
   @param pan_id - (output) Pan ID.

   @return nothing

   @b Example
@code
    zb_uint8_t i;
    ZB_BUF_INITIAL_ALLOC((zb_bufid_t )ZB_BUF_FROM_REF(param),
           sizeof(*discovery_confirm) + sizeof(*network_descriptor) * ZB_PANID_TABLE_SIZE,
                           discovery_confirm);
    zb_nlme_network_descriptor_t *network_descriptor = (zb_nlme_network_descriptor_t *)(discovery_confirm + 1);
    for (i = 0 ; i < ZG->nwk.neighbor.ext_neighbor_used ; ++i)
    {
         zb_address_get_pan_id(ZG->nwk.neighbor.ext_neighbor[i].panid_ref, network_descriptor[j].extended_pan_id);
         network_descriptor[j].logical_channel = ZG->nwk.neighbor.ext_neighbor[i].logical_channel;
         ...
         n_nwk_dsc++;
    }
    discovery_confirm->network_count = n_nwk_dsc;
    discovery_confirm->status = (zb_mac_status_t)((zb_bufid_t )ZB_BUF_FROM_REF(param))->u.hdr.status;
    ZB_SCHEDULE_CALLBACK(zb_nlme_network_discovery_confirm, param);
@endcode

 */
void zb_address_get_pan_id(zb_address_pan_id_ref_t pan_id_ref, zb_ext_pan_id_t pan_id);

/**
   Clears Pan ID table except own pan_id.

   @param pan_id - (our) Pan ID.

   @return nothing

 */
void zb_address_clear_pan_id_table(const zb_ext_pan_id_t pan_id);

/**
   Clears whole Pan ID table

   @return nothing

 */
void zb_address_reset_pan_id_table(void);

/*! @endcond */

/**
   Get Pan ID reference with extended Pan ID.

   @param pan_id -  Pan ID
   @param ref - (output) reference to Pan ID

   @return RET_OK - when success, error code otherwise.

   @b Example
@code
    zb_address_pan_id_ref_t my_panid_ref;
    if ( zb_address_get_pan_id_ref(ZB_NIB_EXT_PAN_ID(), &my_panid_ref) != RET_OK )
    {
      TRACE_MSG(TRACE_NWK1, "Pan ID " TRACE_FORMAT_64 " not in Pan ID arr - ?", (FMT__A,
                TRACE_ARG_64(ZB_NIB_EXT_PAN_ID())));
    }
@endcode
 */
zb_ret_t zb_address_get_pan_id_ref(const zb_ext_pan_id_t pan_id, zb_address_pan_id_ref_t *ref);

/**
   Get short Pan ID with reference.

   @param pan_id_ref - reference to Pan ID
   @param pan_id_p - (output) Pan ID.


   @b Example
@code
    zb_uint16_t nt_panid;
    zb_address_get_short_pan_id(ZG->nwk.neighbor.ext_neighbor[i].panid_ref, &nt_panid);
    if (nt_panid == pan_id)
    {
        TRACE_MSG(TRACE_NWK1, "pan_id %d is on ch %hd", (FMT__D_H, pan_id, channel));
        unique_pan_id = 0;
    }
@endcode
 */
void zb_address_get_short_pan_id(zb_address_pan_id_ref_t pan_id_ref, zb_uint16_t *pan_id_p);


/**
   Compare Pan ID in the source form with Pan ID reference.

   @param pan_id_ref - Pan ID ref
   @param pan_id     - Pan ID (64-bit)

   @return ZB_TRUE if addresses are equal, ZB_FALSE otherwise

   @b Example
@code
   for (i = 0 ; i < ZG->nwk.neighbor.ext_neighbor_used ; ++i)
    {
        for (j = 0 ; j < n_nwk_dsc &&
                   !zb_address_cmp_pan_id_by_ref(ZG->nwk.neighbor.ext_neighbor[i].panid_ref, network_descriptor[j].extended_pan_id) ;
                 ++j)
        {
            ...
        }
    }
@endcode
 */
zb_bool_t zb_address_cmp_pan_id_by_ref(zb_address_pan_id_ref_t pan_id_ref, const zb_ext_pan_id_t pan_id);

/**
   Update long/short address pair. Create the pair if not exist. Optionally, lock.
   Reaction on device announce etc. Long and short addresses are present. Must
   synchronize the address translation table with this information.

   @note Never call zb_address_update() with empty (zero) ieee_address or empty
   (-1) short_address.

   @param ieee_address - long address
   @param short_address - short address
   @param lock - if TRUE, lock address entry
   @param ref_p - (out) address reference

   @return RET_OK or error code

   @b Example
@code
  zb_address_ieee_ref_t addr_ref;
  zb_uint16_t nwk_addr;
  zb_zdo_nwk_addr_resp_head_t *resp = (zb_zdo_nwk_addr_resp_head_t*)zb_buf_begin(buf);
  if (resp->status == ZB_ZDP_STATUS_SUCCESS)
  {
    ZB_LETOH64(ieee_addr, resp->ieee_addr);
    ZB_LETOH16(&nwk_addr, &resp->nwk_addr);
    zb_address_update(ieee_addr, nwk_addr, ZB_TRUE, &addr_ref);
  }
@endcode

 */
#define zb_address_update(ieee_address, short_address, lock, ref_p) \
  zb_address_update_func(TRACE_ADDR_CALL (ieee_address), (short_address), (lock), (ref_p))

zb_ret_t zb_address_update_func(TRACE_ADDR_PROTO
  const zb_ieee_addr_t ieee_address,
  zb_uint16_t short_address,
  zb_bool_t lock,
  zb_address_ieee_ref_t *ref_p);


/**
   Update address pair if one or oth of its component were unknown.

   @param ieee_address - long address
   @param short_address - short address
   @return RET_OK or error code
 */
#define zb_address_update_if_absent(ieee_address, short_address) \
  zb_address_update_if_absent_func(TRACE_ADDR_CALL (ieee_address), (short_address))

zb_ret_t zb_address_update_if_absent_func(TRACE_ADDR_PROTO const zb_ieee_addr_t ieee_address, zb_uint16_t short_address);

#define zb_long_address_update_by_ref(ieee_address, ref) \
  zb_long_address_update_by_ref_func(TRACE_ADDR_CALL (ieee_address), (ref))

void zb_long_address_update_by_ref_func(TRACE_ADDR_PROTO const zb_ieee_addr_t ieee_address, zb_address_ieee_ref_t ref);

/**
   Get address with address reference.

   Get existing IEEE (long) and short addresses with address reference. Update address alive
   time if it not locked.

   @param ieee_address  - (out) long address
   @param short_address_p - (out) short address
   @param ref - address reference

   @b Example
@code
    zb_address_ieee_ref_t addr_ref;
    zb_nlme_join_indication_t *resp = ZB_BUF_GET_PARAM((zb_bufid_t )ZB_BUF_FROM_REF(param), zb_nlme_join_indication_t);
    zb_address_by_ref(resp->extended_address, &resp->network_address, addr_ref);
@endcode

 */
#define zb_address_by_ref(ieee_address, short_address_p, ref) \
  zb_address_by_ref_func(TRACE_ADDR_CALL (ieee_address), (short_address_p), (ref))

void zb_address_by_ref_func(
  TRACE_ADDR_PROTO zb_ieee_addr_t ieee_address,
  zb_uint16_t *short_address_p,
  zb_address_ieee_ref_t ref);

/**
   Get IEEE address with address reference.

   Get existing IEEE address(long address) with address reference. Update address alive time if it not locked.

   @param ieee_address  - (out) long address
   @param ref - address reference

   @b Example
@code
void func(zb_neighbor_tbl_ent_t *nbt)
{
  zb_ieee_addr_t ieee_addr;
  zb_address_ieee_by_ref(ieee_addr, nbt->addr_ref);
  ...
}
@endcode

 */
#define zb_address_ieee_by_ref(ieee_address, ref) \
  zb_address_ieee_by_ref_func(TRACE_ADDR_CALL (ieee_address), (ref))

void zb_address_ieee_by_ref_func(TRACE_ADDR_PROTO zb_ieee_addr_t ieee_address, zb_address_ieee_ref_t ref);


/**
   Get short address by address reference.

   Get existing short address with address reference. Update address alive time if it not locked.

   @param short_address_p  - (out) short address
   @param ref - address reference

   @b Example
@code
    zb_neighbor_tbl_ent_t *nbt;
    if(zb_nwk_neighbor_with_address_conflict(&nbt)==RET_OK)
    {
        zb_uint16_t addr;
        zb_address_short_by_ref(&addr, nbt->addr_ref);
        func(addr);
    }
@endcode

 */
#define zb_address_short_by_ref(short_address_p, ref) \
  zb_address_short_by_ref_func(TRACE_ADDR_CALL (short_address_p), (ref))

void zb_address_short_by_ref_func(TRACE_ADDR_PROTO zb_uint16_t *short_address_p, zb_address_ieee_ref_t ref);

/**
   Get address ref by long address, optionally create if not exist, optionally lock.
   Update address alive time if not locked.
   @param ieee - IEEE device address
   @param create - if TRUE, create address entry if it does not exist
   @param lock - if TRUE, lock address entry
   @param ref_p - (out) address reference

   @note: never call zb_address_by_ieee() with empty (zero) ieee_address

   @return RET_OK or error code

   @b Example
@code
  void test_get_short_addr(zb_uint8_t param)
  {
    zb_address_ieee_ref_t ref_p;
    zb_bufid_t buf = ZB_BUF_FROM_REF(param);

    if (zb_address_by_ieee(g_ieee_addr_r2, ZB_TRUE, ZB_FALSE, &ref_p) == RET_OK)
    {
      ...
    }
  }
@endcode

 */
#define zb_address_by_ieee(ieee, create, lock, ref_p) \
  zb_address_by_ieee_func(TRACE_ADDR_CALL (ieee), (create), (lock), (ref_p))

zb_ret_t zb_address_by_ieee_func(
   TRACE_ADDR_PROTO const zb_ieee_addr_t ieee,
   zb_bool_t create,
   zb_bool_t lock,
   zb_address_ieee_ref_t *ref_p);


/**
   Get address ref by long address, do not lock.

   This is a read-only version of zb_address_by_ieee(). Always use it if address not need to be created.
   @param ieee - IEEE device address
   @param ref_p - (out) address reference

   @note: never call zb_address_by_ieee() with empty (zero) ieee_address

   @return RET_OK or RET_NOT_FOUND
*/
#define zb_address_get_by_ieee(ieee, ref_p) \
  zb_address_get_by_ieee_func(TRACE_ADDR_CALL (ieee), (ref_p))

zb_ret_t zb_address_get_by_ieee_func(TRACE_ADDR_PROTO const zb_ieee_addr_t ieee, zb_address_ieee_ref_t *ref_p);



/**
   Get address ref by long address, lock the address

   This is a read-only version of zb_address_by_ieee(). Always use it if address not need to be created.
   @param ieee - IEEE device address
   @param ref_p - (out) address reference

   @note: never call zb_address_by_ieee() with empty (zero) ieee_address

   @return RET_OK or RET_NOT_FOUND
*/
#define zb_address_get_by_ieee_lk(ieee, ref_p) \
  zb_address_get_by_ieee_lk_func(TRACE_ADDR_CALL (ieee), (ref_p))

zb_ret_t zb_address_get_by_ieee_lk_func(
  TRACE_ADDR_PROTO const zb_ieee_addr_t ieee,
  zb_address_ieee_ref_t *ref_p);


/**
   Get short address by IEEE address (long).

   @param ieee_address - long address

   @return short address if ok, -1 otherwise.

   @par Example
   @snippet thermostat/thermostat_zc/thermostat_zc.c default_short_addr
   @snippet thermostat/thermostat_zc/thermostat_zc.c address_short_by_ieee
   @par

 */
#define zb_address_short_by_ieee(ieee_address) \
  zb_address_short_by_ieee_func(TRACE_ADDR_CALL (ieee_address))

zb_uint16_t zb_address_short_by_ieee_func(TRACE_ADDR_PROTO const zb_ieee_addr_t ieee_address);


/**
   Get IEEE address (long) with short address.

   @param short_addr - short address
   @param ieee_address - (out)long address

   @return RET_OK or RET_NOT_FOUND

   @b Example
   @snippet light_sample_HA_1_2_bulb/light_coordinator_HA_1_2_bulb/light_zc_HA_1_2_bulb.c address_ieee_by_short

 */
#define zb_address_ieee_by_short(short_addr, ieee_address) \
  zb_address_ieee_by_short_func(TRACE_ADDR_CALL (short_addr), (ieee_address))

zb_ret_t zb_address_ieee_by_short_func(TRACE_ADDR_PROTO zb_uint16_t short_addr, zb_ieee_addr_t ieee_address);


/**
   Get address reference by short address. Create the reference if it does not exist.
   Optionally, lock the address. Update address alive time if not locked.
   @param short_address - 16bit device address
   @param create - if TRUE, create address entry if it does not exist
   @param lock - if TRUE, lock address entry
   @param ref_p - (out) address reference

   @note Never call zb_address_by_short() with empty (-1) short_address

   @return RET_OK or error code

   @b Example
   @snippet simple_gw/simple_gw.c address_by_short

 */
#define zb_address_by_short(short_address, create, lock, ref_p) \
  zb_address_by_short_func(TRACE_ADDR_CALL (short_address), (create), (lock), (ref_p))


zb_ret_t zb_address_by_short_func(
   TRACE_ADDR_PROTO zb_uint16_t short_address,
   zb_bool_t create,
   zb_bool_t lock,
   zb_address_ieee_ref_t *ref_p);


/**
   Get address reference by short address, do not lock.

   This is a read-only version of zb_address_by_short().
   @param short_address - 16bit device address
   @param ref_p - (out) address reference

   @note Never call zb_address_by_short() with empty (-1) short_address

   @return RET_OK or RET_NOT_FOUND

 */
#define zb_address_get_by_short(short_address, ref_p) \
  zb_address_get_by_short_func(TRACE_ADDR_CALL (short_address), (ref_p))

zb_ret_t zb_address_get_by_short_func(TRACE_ADDR_PROTO zb_uint16_t short_address, zb_address_ieee_ref_t *ref_p);

/**
   Get address reference by short address, lock address

   This is a read-only version of zb_address_by_short() adding an address lock.
   @param short_address - 16bit device address
   @param ref_p - (out) address reference

   @note Never call zb_address_by_short() with empty (-1) short_address

   @return RET_OK or RET_NOT_FOUND

 */
#define zb_address_get_by_short_lk(short_address, ref_p) \
  zb_address_get_by_short_lk_func(TRACE_ADDR_CALL (short_address), (ref_p))

zb_ret_t zb_address_get_by_short_lk_func(TRACE_ADDR_PROTO zb_uint16_t short_address, zb_address_ieee_ref_t *ref_p);


/*! @cond internals_doc */
/**
   Get address ref by index from short_sorted table.
   @param index - index address short_sorted table
   @param ref_p - (out) address reference

   @return RET_OK or error code

   @b Example
@code
    zb_address_ieee_ref_t ref_p;
    if( ZG->nwk.neighbor.base_neighbor_used > 0 &&
              zb_address_by_sorted_table_index(ZG->nwk.neighbor.send_link_status_index, &ref_p)==RET_OK
    )
    {
        ...
    }
@endcode
 */
zb_ret_t zb_address_by_sorted_table_index(zb_sorted_address_idx_t index, zb_address_ieee_ref_t *ref_p);
/*! @endcond */

/**
   Check that address is locked (has lock counter > 0)

   @param ref - IEEE/network address pair reference

   @return ZB_TRUE if address is locked
 */
zb_bool_t zb_address_is_locked(zb_address_ieee_ref_t ref);


#define zb_address_get_primary_entry(addr_ref) \
  zb_address_get_primary_entry_func(TRACE_ADDR_CALL (addr_ref))

zb_address_map_t *zb_address_get_primary_entry_func(
  TRACE_ADDR_PROTO zb_address_ieee_ref_t addr_ref);

/**

   Increase address lock counter, when it used in some table.
   Address must be already locked.

   @param ref - IEEE/network address pair reference

   @return RET_OK or RET_ERROR
 */
#define zb_address_lock(ref) \
  zb_address_lock_func(TRACE_ADDR_CALL (ref))

zb_ret_t zb_address_lock_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref);


/**

   Unlock address counter. Decrease lock counter.

   @param ref - IEEE/network address pair reference
 */
#define zb_address_unlock(ref) \
  zb_address_unlock_func(TRACE_ADDR_CALL (ref))

void zb_address_unlock_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref);

/**
   Delete address.

   @return RET_OK or RET_ERROR

   @param ref - IEEE/network address pair reference
 */
#define zb_address_delete(ref) \
  zb_address_delete_func(TRACE_ADDR_CALL (ref))

zb_ret_t zb_address_delete_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref);


/*! @cond internals_doc */

/**
   Check that two address refs refer to the one address.
   In this case one record is regular, second - redirect.
   Also returns returns true if addr_ref_a is equal to addr_ref_b.

   @param addr_ref_a Address ref to compare.
   @param addr_ref_b Address ref to compare.
   @return zb_bool_t ZB_FALSE if these address refs refer to different addresses.
                     ZB_TRUE otherwise.
 */
zb_bool_t zb_address_cmp_two_refs(zb_address_ieee_ref_t addr_ref_a, zb_address_ieee_ref_t addr_ref_b);

/**
 * @brief Check that address reference is used
 *
 * @param ref Address ref to check
 * @return zb_bool_t ZB_TRUE if address is used, ZB_FALSE otherwise.
 */
zb_bool_t zb_address_in_use(zb_address_ieee_ref_t ref);

/**
 * @brief Check if address tables have enough memory for the new address
 *
   @param new_addr - new IEEE address
 * @return zb_bool_t ZB_TRUE if there is enough memory, ZB_FALSE otherwise.
 */
zb_bool_t zb_address_check_mem_for_new_addr(const zb_ieee_addr_t new_addr);


/**
   Reset address map.

   It is the caller's responsibility to ensure that no addr ref is in use since all addr refs will be invalidated.

   @return RET_OK or error code
 */
void zb_address_reset(void);

#define zb_address_setup_ieee_disc(short_addr) zb_address_setup_ieee_disc_func(TRACE_ADDR_CALL (short_addr))
zb_bool_t zb_address_setup_ieee_disc_func(TRACE_ADDR_PROTO zb_uint16_t short_addr);

#define zb_address_done_ieee_disc(short_addr) zb_address_done_ieee_disc_func(TRACE_ADDR_CALL (short_addr))
void zb_address_done_ieee_disc_func(TRACE_ADDR_PROTO zb_uint16_t short_addr);

#define zb_address_setup_short_disc(ref) zb_address_setup_short_disc_func(TRACE_ADDR_CALL (ref))
zb_bool_t zb_address_setup_short_disc_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref);

#define zb_address_done_short_disc(ref) zb_address_done_short_disc_func(TRACE_ADDR_CALL (ref))
void zb_address_done_short_disc_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref);

#define zb_address_setup_addr_conflict(ref) zb_address_setup_addr_conflict_func(TRACE_ADDR_CALL (ref))
zb_bool_t zb_address_setup_addr_conflict_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref);

#define zb_address_done_addr_conflict(ref) zb_address_done_addr_conflict_func(TRACE_ADDR_CALL (ref))
zb_bool_t zb_address_done_addr_conflict_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref);

/* Neighbor table reference */

void zb_address_clear_neighbor_refs(void);

#define zb_address_get_neighbor_ref(ref) \
  zb_address_get_neighbor_ref_func(TRACE_ADDR_CALL (ref))

zb_nwk_neighbor_ref_t zb_address_get_neighbor_ref_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref);

#define zb_address_set_neighbor_ref(ref, neighbor_ref) \
  zb_address_set_neighbor_ref_func(TRACE_ADDR_CALL (ref), (neighbor_ref))

void zb_address_set_neighbor_ref_func(TRACE_ADDR_PROTO zb_address_ieee_ref_t ref, zb_nwk_neighbor_ref_t neighbor_ref);

/*! @endcond */

/*! @} */


#endif /* ZB_ADDRESS_H */
