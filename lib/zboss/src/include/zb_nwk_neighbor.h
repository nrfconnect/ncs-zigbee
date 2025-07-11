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
/* PURPOSE: Neighbor table (base and additional)
*/

#ifndef ZB_NWK_NEIGHBOR_H
#define ZB_NWK_NEIGHBOR_H 1

/*! @cond internals_doc */
/*! \addtogroup ZB_NWK */
/*! @{ */

/* to be able to limit addr_ref size to 1 byte, limit address translation table
 * size to 255  */
#ifndef ZB_CONFIGURABLE_MEM
ZB_ASSERT_COMPILE_DECL(ZB_IEEE_ADDR_TABLE_SIZE < ZB_UINT8_MAX);
#endif
ZB_ASSERT_COMPILE_DECL(ZB_NWK_ROUTER_AGE_LIMIT < 4); /* Check if fits in 2 bits */

#ifdef ZB_ROUTER_ROLE
#define ZB_MAX_CHILDREN ZB_NEIGHBOR_TABLE_SIZE
#define ZB_MAX_ED_CAPACITY_DEFAULT ((ZB_NEIGHBOR_TABLE_SIZE)/2u)
#endif

/* zb_neighbor_tbl_ent_t moved to zboss_api_internal.h for configurable memory feature. */

/* use nwkDiscoveryTable instead of old ext neighbor */
typedef zb_nwk_disc_tbl_ent_t zb_ext_neighbor_tbl_ent_t;

typedef struct zb_nbr_iterator_data_s {
  /*! Stores the neighbour table update counter. */
  zb_uint32_t update_count;
} zb_nbr_iterator_data_t;


/**
   Neighbor table
 */
typedef struct zb_neighbor_tbl_s
{
  /*! base (run-time) neighbor table */
#ifndef ZB_CONFIGURABLE_MEM
  zb_neighbor_tbl_ent_t     neighbor[ZB_NEIGHBOR_TABLE_SIZE];
  /*! array for addressing neighbor table by network address ref */
  zb_uint8_t                addr_to_neighbor[ZB_IEEE_ADDR_TABLE_SIZE];
  zb_nwk_disc_tbl_ent_t     disc_table[ZB_NWK_DISC_TABLE_SIZE];
#else
  zb_neighbor_tbl_ent_t     *neighbor;
  zb_uint8_t                *addr_to_neighbor;
  zb_nwk_disc_tbl_ent_t     *disc_table;
#endif
  zb_nbr_iterator_data_t    iterator_data;
  /*! clock pointer for incoming_frame_counter expire */
  zb_ushort_t               incoming_frame_counter_clock;
/*Pointer to the end device that expires first*/
  zb_neighbor_tbl_ent_t     *next_aging_end_device;
#ifdef ZB_ROUTER_ROLE
/*NBT element to start parent_annce from */
  zb_uint8_t parent_annce_position;
  zb_uint8_t max_ed_capacity; /*!< The number of ED children a device is allowed to have */
  zb_uint8_t ed_child_num;    /*!< Number of child ed devices */
  zb_uint8_t r_sibling_num;   /*!< Number of sibling router devices */
#endif
  zb_uint8_t transmit_failure_threshold;
  zb_uint8_t transmit_failure_timeout;
}
zb_neighbor_tbl_t;

#define ZB_NWK_NEIGHBOR_GET_INDEX_BY_ENTRY_ADDRESS(ent_addr) \
  ( ZB_ARRAY_IDX_BY_ELEM((zb_uint8_t *)&ZG->nwk.neighbor.neighbor[0], (zb_uint8_t *)ent_addr) / sizeof(*ent_addr) )

/**
   Neighbor table subsystem init.
 */
void zb_nwk_neighbor_init(void);

/**
   Stop work with extended neighbor table: move some entries to the base table,
   give all memory to the base
 */
void zb_nwk_exneighbor_stop(zb_uint16_t parent_short_addr);

/**
   Get extended neighbor table entry by short address

   @param panid_ref  - ext Pan ID ref
   @param short_addr - 16-bit device address
   @param enbt       - (out) allocated or found ext neighbor table entry

   @return RET_OK if success, error code if error
 */
zb_ret_t zb_nwk_exneighbor_by_short(zb_address_pan_id_ref_t panid_ref, zb_uint16_t short_addr, zb_ext_neighbor_tbl_ent_t **enbt);

/**
   Get extended neighbor table entry by long address

   @param panid_ref  - ext Pan ID ref
   @param long_addr  - 64-bit device address
   @param enbt       - (out) allocated or found ext neighbor table entry

   @return RET_OK if success, error code if error
 */
zb_ret_t zb_nwk_exneighbor_by_ieee(zb_address_pan_id_ref_t panid_ref, zb_ieee_addr_t long_addr, zb_ext_neighbor_tbl_ent_t **enbt);

/**
   Check existence of neighbor table entry by IEEE address reference

   @param addr_ref - IEEE address reference

   @return ZB_TRUE if entry exists, ZB_FALSE otherwise
 */
zb_bool_t zb_nwk_neighbor_exists(zb_address_ieee_ref_t addr_ref);

/**
   Get neighbor table entry by address, create if absent

   @param addr_ref - address
   @param create_if_absent - create new entry if absent
   @param nbt - (out) neighbor table entry

   @return RET_OK if entry is found or was successfully created; RET_NOT_FOUND if entry doesn't exist; error code
   if error
 */
zb_ret_t zb_nwk_neighbor_get(zb_address_ieee_ref_t addr_ref, zb_bool_t create_if_absent, zb_neighbor_tbl_ent_t **nbt);


/**
   Get neighbor table entry by short address, does not create if absent

   @param short_addr - address
   @param nbt - (out) neighbor table entry

   @return RET_OK if found, else RET_NOT_FOUND
   if error
 */
zb_ret_t zb_nwk_neighbor_get_by_short(zb_uint16_t short_addr, zb_neighbor_tbl_ent_t **nbt);

/**
   Get neighbor table entry by long address, does not create if absent

   @param long_addr - address
   @param nbt - (out) neighbor table entry

   @return RET_OK if found, else RET_NOT_FOUND
   if error
 */
zb_ret_t zb_nwk_neighbor_get_by_ieee(const zb_ieee_addr_t long_addr, zb_neighbor_tbl_ent_t **nbt);


/**
   Remove entity by IEEE address

   @param ieee_ref - IEEE address reference

   @return RET_OK if success, error code if error
 */
zb_ret_t zb_nwk_neighbor_delete(zb_address_ieee_ref_t ieee_ref);

/**
   Remove all entities

   @return nothing
 */
void zb_nwk_neighbor_clear(void);


/**
   Remove all non-extneighbors.
*/
void zb_nwk_neighbor_remove_non_extnbrs(void);

/**
   Copy ext entry to base, to be able to send packet to it.

   @param eent - pointer to extended table entry
   @param nent - OUT pointer to pointer to the result - just created nbt entry. If NULL, caller don't care.

   @return RET_OK if success, error code if error
 */
zb_ret_t zb_nwk_neighbor_ext_to_base(zb_ext_neighbor_tbl_ent_t *eent, zb_neighbor_tbl_ent_t **nent);


/**
   Run one iteration of the incoming frame counter expiration clock algorithm

   The goal is to clear (drop to 0) counters for too old entries (with distance
   between it and key_seq_number > 3).
   Do it one entry at a time to exclude unpredictable delays.

   @param key_seq_number - current key sequence number
 */
void zb_nwk_neighbor_incoming_frame_counter_clock(zb_uint8_t key_seq_number);


/**
   Get next device with rx_on_when_idle == 1

   @param i - current index in the neighbor table. Use 0 to start iteration.
   @return index in the neighbor table (0 is valid index) or ~0 if no more such entry
 */
zb_ushort_t zb_nwk_neighbor_next_rx_on_i(zb_ushort_t i);

#define ZB_NWK_NEIGHBOR_BY_I(i) (&ZG->nwk.neighbor.neighbor[(i)])

/**
   Get next child to retransmit broadcast frame to

   @param addr - broadcast address
   @param i - current index in the neighbor table. Use 0 to start iteration.
   @return index in the neighbor table or ~0 if no more such children
 */
zb_ushort_t zb_nwk_nbr_next_ze_children_i(zb_uint16_t addr, zb_ushort_t i);

/**
   Get next child with rx_on_when_idle == 1

   @param i - current index in the neighbor table. Use 0 to start iteration.
   @return index in the neighbor table or ~0 if no more such children
 */
zb_ushort_t zb_nwk_nbr_next_ze_children_rx_off_i(zb_ushort_t i);

/**
   Get neighbor table size
*/
zb_uint8_t zb_nwk_neighbor_table_size(void);


zb_ret_t zb_nwk_exneigbor_sort_by_lqi(void);

zb_ret_t zb_nwk_exneighbor_remove_by_panid(zb_address_pan_id_ref_t panid_ref);

zb_uint8_t zb_nwk_neighbor_get_ed_cnt(void);
zb_uint8_t zb_nwk_neighbor_get_ed_short_list(zb_uint8_t start_index, zb_uint8_t max_records, zb_uint8_t** ed_list);

zb_uint8_t zb_nwk_neighbor_get_zc_zr_cnt(void);
zb_ret_t zb_nwk_neighbor_get_by_idx(zb_uint8_t idx, zb_neighbor_tbl_ent_t **nbt);

zb_ret_t zb_nwk_get_sorted_neighbor(zb_uint8_t *index, zb_neighbor_tbl_ent_t **p_nbt);

void zb_nwk_neighbor_complete_deletion(zb_address_ieee_ref_t ieee_ref, zb_uint8_t neighbor_index);

#ifdef ZB_ROUTER_ROLE
void zb_nwk_neighbour_rssi_store(zb_neighbor_tbl_ent_t *nbt, zb_int8_t rssi);

#ifdef ZB_NWK_NEIGHBOUR_PATH_COST_RSSI_BASED
zb_uint8_t zb_nwk_neighbour_get_path_cost(zb_neighbor_tbl_ent_t *nbt);
#endif /* ZB_NWK_NEIGHBOUR_PATH_COST_RSSI_BASED */

#endif /* ZB_ROUTER_ROLE */

zb_uint8_t zb_nwk_neighbor_update_lqa(zb_neighbor_tbl_ent_t *nbt, zb_uint8_t raw_lqa);

void zb_nwk_neighbor_update_lqa_rssi(zb_neighbor_tbl_ent_t *nbt, zb_uint8_t lqi, zb_int8_t rssi);

#if !defined ZB_ED_ROLE && defined ZB_MAC_DUTY_CYCLE_MONITORING
zb_uint8_t zb_nwk_neighbor_get_subghz_count(void);
zb_address_ieee_ref_t zb_nwk_neighbor_get_top_ith_device(zb_uint8_t i);
/** @fn zb_uint8_t zb_nwk_neighbor_get_subghz_list(zb_uint8_t *ref_list, zb_uint8_t max_records)
 *  @brief Builds list of Sub-GHz devices
 *  @details Returns list of references to Sub-GHz devices entries in neighbor table sorted by
 *    pkt_count in descending order.
 *  @param[out]  ref_list - pointer to the array which will store references to the neighbor table entries
 *  @param[in]   max_records - max number of elements that ref_list can store
 *  @return number of items in ref_list
 */
zb_uint8_t zb_nwk_neighbor_get_subghz_list(zb_address_ieee_ref_t *ref_list, zb_uint8_t max_records);
#endif

/**
   Update the neighbour table update counter.
 */
void zb_nwk_nbr_iterator_update_counter(void);

/*! @} */
/*! @endcond */

/* reserve slot 0 for the parent */
#define EXN_START_I 1U

#ifdef ZB_ROUTER_ROLE
zb_bool_t zb_nwk_ed_or_zvd_is_our_child(zb_uint16_t addr);
#endif /* ZB_ROUTER_ROLE */

void zb_nwk_nbt_print(void);

#ifdef ZB_ROUTER_ROLE

#define ZB_ROUTER_ACTIVITY_THRESHOLD 2u

void zb_nwk_nbt_inc_child_num(zb_bool_t is_router);

void zb_nwk_nbt_dec_child_num(zb_bool_t is_router);

void zb_nwk_nbt_reset_child_num(void);

zb_bool_t zb_nwk_nbt_have_space_for_device(zb_bool_t is_router);

zb_bool_t zb_nwk_nbt_have_space_for_ed(void);

zb_bool_t zb_nwk_nbt_have_space_for_r(void);

zb_bool_t zb_nwk_nbt_capacity_is_full(void);

zb_bool_t zb_nwk_nbt_have_ed_children(void);

zb_uint16_t zb_nwk_nbt_get_rank_of_router(zb_neighbor_tbl_ent_t *nbt);

/* This function is based on zb_nwk_neighbor_get with create_if_absent == ZB_TRUE
 * The difference is in finding candidate entry. Here it is based on device_type.
 *
 * PLEASE NOTE that returned nbt entry is initialized with default values.
 * is_router parameter is neither incrementing nbt devs count nor sets device type of nbt entry.
 * It is up to the user of this function to take care of, just like with zb_nwk_neighbor_get. */
zb_ret_t zb_nwk_nbt_alloc_ent_for_device(zb_address_ieee_ref_t addr_ref, zb_bool_t is_router, zb_neighbor_tbl_ent_t** nbt);

zb_neighbor_tbl_ent_t* zb_nwk_nbt_get_entry_for_substitution(void);

#endif /* ZB_ROUTER_ROLE */

#endif /* ZB_NWK_NEIGHBOR_H */
