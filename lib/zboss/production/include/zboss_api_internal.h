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
/*  PURPOSE: ZBOSS internal data structures required for configurable memory.
*/

#ifndef ZB_ZBOSS_API_INTERNAL_H
#define ZB_ZBOSS_API_INTERNAL_H 1

#include "zb_vendor.h"
#include "zb_config.h"
#include "zb_types.h"
#include "zb_errors.h"
#include "zb_osif.h"
#include "zb_debug.h"
#include "zb_trace.h"
#include "zb_pooled_list.h"
#include "zb_ecc.h"

#ifndef ZB_MINIMAL_CONTEXT

#include "zb_address.h"

/* Data structures moved from subsystem internal headers for configurable memory implementation. */

/**
   Parameters for storing data in a pending queue

   Moved there from MAC to be able to implement MAC indirect quote size configuration.
*/
typedef struct zb_mac_pending_data_s
{
  zb_addr_u dst_addr;         /**< Destination address */
  zb_uint8_t      dst_addr_mode;    /**< Destination address mode @ref address_modes */
  zb_uint8_t      pending_param;                 /**< Pointer to pending data */
}
zb_mac_pending_data_t;


/**
   APS retransmissions
 */
typedef ZB_PACKED_PRE struct zb_aps_retrans_ent_s
{
  zb_uint16_t  clusterid;       /*!< Cluster ID*/
  zb_address_ieee_ref_t addr_ref;       /*!< Destination address*/
  zb_uint8_t   aps_counter;     /*!< APS counter */
  zb_uint8_t   src_endpoint;    /*!< Source endpoint */
  zb_uint8_t   dst_endpoint;    /*!< Destination endpoint */
  zb_uint8_t   buf;             /*!< Buffer index for retranslate */

  zb_bitfield_t aps_retries:3;  /*!< Number of attempts */
  zb_bitfield_t nwk_insecure:1; /*!< Flag 'Is NWK secure' */
  zb_bitfield_t state:3;        /*!< @see @ref aps_retrans_ent_state */
  zb_bitfield_t relay:1;
} ZB_PACKED_STRUCT zb_aps_retrans_ent_t;

#endif /* !ZB_MINIMAL_CONTEXT */

typedef ZB_PACKED_PRE struct zb_cb_q_ent_s
{
  union {
    zb_callback_t func_ptr;
    zb_callback2_t func2_ptr;
  } u;                           /*!< function to call  */
  zb_uint16_t user_param;        /*!< user parameter */
  zb_uint8_t param;              /*!< parameter to pass to 'func'  */
}
ZB_PACKED_STRUCT
zb_cb_q_ent_t;

/**
   Returns 'zb_cb_q_ent_t' members 'func_ptr' or 'func2_ptr' depending on whether the callback
   takes one or two parameters.

   @param ent - variable of type 'zb_cb_q_ent_t'
   @param is2param - boolean value, ZB_TRUE if the callback takes two parameters, ZB_FALSE otherwise

   @return 'func_ptr' or 'func2_ptr'
 */
#define ZB_CB_QENT_FPTR(ent, is2param) ((!(is2param)) ? (void*)((ent)->u.func_ptr) : (void*)((ent)->u.func2_ptr))

typedef ZB_PACKED_PRE struct zb_delayed_buf_q_ent_s
{
  union {
    zb_callback_t func_ptr;
    zb_callback2_t func2_ptr;
  } u;                           /*!< function to call  */
  zb_uint16_t   user_param;      /*!< user parameter */
  zb_bitfield_t buf_cnt:7;       /*!< number of buffers to allocate */
  zb_bitfield_t is_2param:1;     /*!< whether this is a 2param callback */
}
ZB_PACKED_STRUCT
zb_delayed_buf_q_ent_t;

/**
   Returns 'zb_delayed_buf_q_ent_t' members 'func_ptr' or 'func2_ptr' depending on whether the callback
   takes one or two parameters.

   @param ent - variable of type 'zb_delayed_buf_q_ent_t'

   @return 'func_ptr' or 'func2_ptr'
 */
#define ZB_DELAYED_BUF_QENT_FPTR(ent) (((ent)->is_2param == 0U) ? (void*)((ent)->u.func_ptr) : (void*)((ent)->u.func2_ptr))

#define ZB_LQA_ARR_SIZE 2

/**
   Delayed (scheduled to run after timeout) callbacks queue entry.
 */
typedef ZB_PACKED_PRE struct zb_tm_q_ent_s
{
  zb_callback_t func;           /*!< function to call  */
  zb_time_t run_time;           /*!< time to run at  */
  zb_uint8_t param;             /*!< parameter to pass to 'func'  */
  ZB_POOLED_LIST8_FIELD(next);
}
ZB_PACKED_STRUCT
zb_tm_q_ent_t;

#ifndef ZB_MINIMAL_CONTEXT
/**
   Installcode hash for TC

   Do not store installcode itself in RAM - keep only 4 bytes per entry in RAM.
 */
typedef ZB_PACKED_PRE struct zb_aps_installcode_storage_s
{
  zb_uint32_t    nvram_offset:28;      /*!< offset of zb_aps_installcode_nvram_t
                                          record in nvram. 28 bit supports 256k
                                          page - hope, it is enough  */
  zb_bitfield_t  nvram_page:4;         /*!< nvram page. Hope 16 pages are enough  */
} ZB_PACKED_STRUCT zb_aps_installcode_storage_t;


typedef ZB_PACKED_PRE struct zb_aps_device_key_pair_array_s
{
  /* Outgoing value for APS frame outgoing counter.
   * Counters are unique for each APS key */
  zb_uint32_t outgoing_frame_counter;

#ifndef ZB_NO_CHECK_INCOMING_SECURE_APS_FRAME_COUNTERS
  zb_uint32_t incoming_frame_counter; /*!< Incoming value,for APS frame incoming counter */
#endif

  zb_uint32_t    nvram_offset:27;     /*!< offset of zb_aps_device_key_pair_storage_t
                                       * record in nvram. 27 bit supports 128k
                                       * page - hope, it is enough  */

  zb_lbitfield_t verified_frame_counter:1;
  zb_lbitfield_t outgoing_frame_counter_valid:1;
  zb_lbitfield_t incoming_frame_counter_valid:1;

  /* This bits use during preparation for channel/panid change */
  zb_lbitfield_t handled:1;
  zb_lbitfield_t err_cnt_increased:1;

  /* The reserved value is uses 16 bits now,
   * but actual max value requires 10 bits */
  zb_uint16_t outgoing_frame_counter_reserved;

  /* This counter uses in challenge rsp, 16 bits should be enough */
  zb_uint16_t challenge_counter;
} ZB_PACKED_STRUCT zb_aps_device_key_pair_array_t;


/**
   Curve25519 keys lengths.
 */
#define ZB_ECC_CURVE25519_PRV_KEY_LEN 32U
#define ZB_ECC_CURVE25519_PUB_KEY_LEN ZB_ECC_CURVE25519_PRV_KEY_LEN

/**
   Storage of the data for key negotiation procedure.
 */
typedef struct zb_secur_ecdhe_common_ctx_s
{
  zb_address_ieee_ref_t ref;                       /* Will be used for tc multiple key negotiations */
  zb_uint8_t            key_negotiation_state;     /*!< attributes of the key
                                                    *   @ref zb_secur_key_negotiation_state_t
                                                    *   If entry is not used state is set to
                                                    *   ZB_SECUR_ANY_KEY_NEGOTIATION */

  zb_bitfield_t         is_initial_join:1;           /* with Network commissioning */
  zb_bitfield_t         is_rejoin:1;
  zb_bitfield_t         supported_key_neg_method_tlv_found:1;
  zb_bitfield_t         fragmentation_tlv_found:1;

  /* According to Zigbee Direct Specification, Device Capability Extension
   * is a bitmask and now has only ZVD capability bit. The bit is used
   * on Trust Center to determine whether joining device is ZVD */
  zb_bitfield_t         device_capability_extension_tlv_found:1;
  zb_bitfield_t         reserved:3;

  zb_uint8_t            supported_key_neg_method_tlv_value;
  zb_uint8_t            supported_secrets_tlv_value;

  zb_uint16_t           fragmentation_tlv_value;

  zb_uint16_t           device_capability_extension_tlv_value; /* See Zigbee Direct Specification for more information */

  zb_uint8_t            selected_key_neg_method; /*!< Selected by the device Key Negotiation Method
                                                  *  @see zb_tlv_key_negotiation_methods_t */
  zb_uint8_t            selected_psk_secret;     /*!< Selected by the device Key Negotiation Method
                                                  *  @see zb_tlv_psk_secrets_t */

  zb_uint8_t            public_key_point_i[ZB_ECC_PUB_KEY_MAX_LEN];  /* Initiator Public Key Point */
  zb_uint8_t            public_key_point_r[ZB_ECC_PUB_KEY_MAX_LEN];  /* Responder Public Key Point */
  zb_uint8_t            private_key[ZB_ECC_PRV_KEY_MAX_LEN];         /* Private key  */

  zb_uint8_t            psk[16];
  zb_uint8_t            nwk_key_seq_num;

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
  zb_bufid_t            start_key_neg_rsp_param;
  zb_bufid_t            confirm_key_param;
  zb_bufid_t            auth_token_rsp_param;
  zb_uint16_t           parent_addr;
#endif
} zb_secur_ecdhe_common_ctx_t;


/**
   NWK routing
*/
typedef ZB_PACKED_PRE struct zb_nwk_routing_s /* do not pack for IAR */
{
  zb_bitfield_t used:1; /*!< 1 if entry is used, 0 - otherwise */
  zb_bitfield_t status:2; /*!< The status of the route, see @ref nwk_route_state */
#ifndef ZB_LITE_NO_SOURCE_ROUTING
  zb_bitfield_t no_route_cache:1;        /*!< Dest does not store source routes. */
  zb_bitfield_t many_to_one:1;           /*!< Dest is the concentrator and many-to-one
                                          * request was used  */
  zb_bitfield_t route_record_required:1; /*!< Route record command frame should
                                          * be sent to the dest prior to the
                                          * next data packet */
#endif
#ifndef ZB_NO_NWK_MULTICAST
  zb_bitfield_t group_id_flag:1; /*!< Indicates that dest_addr is a Group ID */
#endif
#if ZB_NWK_ROUTING_TABLE_EXPIRY < 64U
  zb_bitfield_t expiry:6;  /*!< expiration time. max value -
                            * ZB_NWK_ROUTING_TABLE_EXPIRY (60) */
#else
  zb_uint8_t expiry;
#endif
#if !defined ZB_CONFIGURABLE_MEM && ZB_IEEE_ADDR_TABLE_SIZE <= 127U
  zb_bitfield_t next_hop_addr_ref:7;
#else
  zb_uint8_t next_hop_addr_ref; /*!< ref to network address of the next
                                            * hop on the way to the destination */
#endif
  zb_uint16_t dest_addr; /*!< 16-bit network address or Group ID of this route */
} ZB_PACKED_STRUCT
zb_nwk_routing_t;

/**
   NWK route discovery
*/
typedef struct zb_nwk_route_discovery_s /* do not pack for IAR */
{
  zb_bitfield_t used:1; /*!< 1 if entry is used, 0 - otherwise   */
  zb_bitfield_t expiration_time:7; /*!< Countdown timer indicating when route
                                    * discovery expires. ZB_NWK_ROUTE_DISCOVERY_EXPIRY 10 */
  zb_uint8_t request_id; /*!< Sequence number for a route request */
  /* TODO: use 1 byte - index in the translation table */
  zb_uint16_t source_addr; /*!< 16-bit network address of the route
                            * requests initiator */
  /* TODO: use 1 byte - index in the translation table */
  zb_uint16_t sender_addr; /*!< 16-bit network address of the device that
                            * has sent the most recent lowest cost route
                            * request */
  zb_uint16_t dest_addr; /*!< 16-bit network destination address of this
                          * request */
  zb_uint8_t forward_cost; /*!< Path cost from the source of the route request
                            * to the current device */
  zb_uint8_t residual_cost; /*!< Path cost from the current to the destination
                             * device */
} ZB_PACKED_STRUCT
zb_nwk_route_discovery_t;

#if defined ZB_PRO_STACK && !defined ZB_LITE_NO_SOURCE_ROUTING
/**
  Route Record Table Entry Format, Table 3.45
*/
typedef struct zb_nwk_rrec_s    /* do not pack for IAR */
{
  zb_bitfield_t used:1;                     /*!< Is record used */
  zb_bitfield_t expiry:7;                   /*!< Expiration time */
  zb_uint8_t  count;                        /*!< Count hops */
  zb_uint16_t addr;                         /*!< Destination address */
  zb_uint16_t path[ZB_NWK_MAX_PATH_LENGTH]; /*!< Full path */
} ZB_PACKED_STRUCT zb_nwk_rrec_t;

#endif

typedef ZB_PACKED_PRE struct zb_aps_dup_tbl_ent_s
{
  zb_uint8_t addr_ref;          /*!< Reference to addr_map */
  zb_uint8_t counter;                      /*!< APS frame counter */
  zb_bitfield_t clock:6;                   /*!< Expiry clock counter. Be sure it can hold 2*ZB_APS_DUP_INITIAL_CLOCK */
  zb_bitfield_t is_unicast:1;              /*!< Is delivery mode unicast */
  zb_bitfield_t is_command:1;              /*!< Is APS command frame */
} ZB_PACKED_STRUCT zb_aps_dup_tbl_ent_t;


/**
   Global binding table - source part
*/
typedef ZB_PACKED_PRE struct zb_aps_bind_src_table_s
{
  zb_address_ieee_ref_t src_addr;   /*!< source address as ref from nwkAddressMap */
  zb_uint8_t            src_end;    /*!< source endpoint */
  zb_uint16_t           cluster_id; /*!< cluster id */
} ZB_PACKED_STRUCT zb_aps_bind_src_table_t;

/**
   Global binding table - destination part with 64-bit address
*/
typedef ZB_PACKED_PRE struct zb_aps_bind_long_dst_addr_s
{
  zb_address_ieee_ref_t dst_addr;        /*!< destination address as ref from nwkAddressMap */
  zb_uint8_t            dst_end;         /*!< destination endpoint */
} ZB_PACKED_STRUCT zb_aps_bind_long_dst_addr_t;

#ifndef ZB_CONFIGURABLE_MEM
#define ZB_APS_BIND_TRANS_TABLE_SIZE ((ZB_IOBUF_POOL_SIZE + 15U)/16U *4U)

/* it should be 4-byte aligned if it is stored in NVRAM */
#define ZB_SINGLE_TRANS_INDEX_SIZE (((ZB_APS_BIND_TRANS_TABLE_SIZE + 31U) / 32U) * 4U)
#endif

/**
   Global binding table - destination part
*/
typedef ZB_PACKED_PRE struct zb_aps_bind_dst_table_s
{
#ifdef ZB_CONFIGURABLE_MEM
  /* WARNING: this field will be rewritten if APS binding dataset is present in NVRAM */
  zb_uint8_t            *trans_index;
#endif /* defined ZB_CONFIGURABLE_MEM */

  ZB_PACKED_PRE union
  {
    zb_uint16_t group_addr;                /*!< group address */
    zb_aps_bind_long_dst_addr_t long_addr; /*!< @see zb_asp_long_dst_addr_t */
  } u;

#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t            trans_index[ZB_SINGLE_TRANS_INDEX_SIZE];
#endif /* defined ZB_CONFIGURABLE_MEM */

  zb_uint8_t            dst_addr_mode;   /*!< destination address mode flag, 0
                                          * - group address, otherwise long
                                          * address plus dest endpoint */
  zb_uint8_t            src_table_index; /*!< index from zb_asp_src_table_t */
#ifdef SNCP_MODE
  zb_uint8_t            id;              /* original index position when inserted, to identify
                                          *  entry even if moved with the array (on removal of
                                          *  other elements) */
  zb_uint8_t            align;
#endif
} ZB_PACKED_STRUCT zb_aps_bind_dst_table_t;

/**
   Neighbor table entry
 */
typedef ZB_PACKED_PRE struct zb_neighbor_tbl_ent_s /* not need to pack it at IAR */
{
  zb_bitfield_t             used:1;         /*!< Record has used */

  zb_bitfield_t             device_type:2; /*!< Neighbor device type - @see @ref nwk_device_type */

  zb_bitfield_t             depth:4; /*!< The network depth of this
                                       device. A value of 0x00
                                       indicates that this device is the
                                       Zigbee coordinator for the
                                       network.  */

  zb_bitfield_t             send_via_routing:1;  /*!< Due to bad link to that device send packets
                                                  *   via NWK routing.
                                                  */

  zb_bitfield_t             relationship:3; /*!< The relationship between the
                                              neighbor and the current device:
                                              0x00=neighbor is the parent
                                              0x01=neighbor is a child
                                              0x02=neighbor is a sibling
                                              0x03=none of the above
                                              0x04=previous child
                                              0x05=unauthenticated child
                                              This field shall be present in every
                                              neighbor table entry.
                                              see @ref nwk_relationship
                                            */

  zb_bitfield_t             need_rejoin:1; 	/*!< Need send rejoin response without receive request */

  /* there was send_via_routing field which marked asymmetrical links when we
   * can head the device but it can't hear us. Now that functionality is
   * implemented using outgoing_cost field. */

  zb_bitfield_t             keepalive_received:1; /*!< This value indicates at least one keepalive
                                                   *   has been received from the end device since
                                                   *   the router has rebooted.
                                                   */

  zb_bitfield_t             mac_iface_idx:5;  /*!< An index into the MAC Interface Table
                                               * indicating what interface the neighbor or
                                               * child is bound to. */

  zb_bitfield_t             transmit_failure_cnt:4;  /*!< Transmit failure counter (used to initiate
                                                      * device address
                                                      * search). */
  zb_bitfield_t             zvd_ephemeral_session_is_started:1; /*!< ZDD should be able to track ZVD session */
  zb_bitfield_t             reserved:1;

  zb_uint8_t                lqa;  /*!< Link quality. Also used to calculate
                                   * incoming cost */
  zb_int8_t                 rssi;  /*!< Received signal strength indicator */
  zb_uint8_t                key_seq_number; /*!< key number for which
                                             * incoming_frame_counter is valid  */

  zb_address_ieee_ref_t     addr_ref;       /*!< address translation entry */

  zb_uint8_t                prev_lqa_array[ZB_LQA_ARR_SIZE];

  zb_uint32_t               incoming_frame_counter; /*!< incoming NWK frame counter
                                                     * for this device after
                                                     * key change */

#if !defined ZB_ED_ROLE && defined ZB_MAC_DUTY_CYCLE_MONITORING
  zb_bitbool_t is_subghz:1;        /*!< if 1, this is Sub-GHz device */
  zb_bitbool_t suspended:1;        /*!< if 1, SuspendZCLMessages was sent to the device */
  zb_lbitfield_t pkt_count:22;       /*!< count of packets received from this device */
#define MAX_NBT_PKT_COUNT ((1u<<22U)-1U)
  zb_lbitfield_t subghz_ep:8;        /*!< endpoint with Sub-GHz cluster on remote device */
#endif

  union dev_u
  {
    ZB_PACKED_PRE struct r_s
    {
      zb_bitfield_t           permit_joining:1; /*!< A value of TRUE indicates that at
                                                    least one Zigbee router on the
                                                    network currently permits joining,
                                                    i.e. its NWK has been issued an
                                                    NLME-PERMIT-JOINING
                                                    primitive and, the time limit if
                                                    given, has not yet expired.  */
      /* Following fields present only if nwkSymLink = TRUE - this is PRO, not 2007 */
      zb_bitfield_t           outgoing_cost:3;  /*!< The cost of an outgoing
                                                  * link. Got from link status. */
      /* In other words, if a device fails to receive nwkRouterAgeLimit link status
        messages from a router neighbor in a row, the old outgoing cost information is
        discarded. In this case, the neighbor entry is considered stale and may be reused if
        necessary to make room for a new neighbor. */
      /* actually ZB_NWK_ROUTER_AGE_LIMIT is 3, so 2 bits is enough */
      zb_bitfield_t           age:2; /*!< The number of nwkLinkStatusPeriod intervals since a
                                        * link status command was received */

      zb_bitfield_t           reserved:2;

      zb_uint8_t             router_information;

      zb_bitfield_t          router_connectivity:4; /*!< An indicator for how well this router neighbor is connected to other
                                                     * routers in its vicinity. Higher numbers indicate better connectivity */
      zb_bitfield_t          router_nb_set_diversity:4; /*!< An indicator for how different the sibling router's set of neighbors
                                                         * is compared to the local router's set of neighbors */
      zb_bitfield_t          router_outbound_act:4; /*!< Incremented whenever this neighbor is used as a next hop for a data packet;
                                                     * decremented once every nwkLinkStatusPeriod */
      zb_bitfield_t          router_inbound_act:4; /*!< Incremented whenever the local device is used by this neighbor as a next hop
                                                    * for a data packet; decremented once every nwkLinkStatusPeriod */
    } ZB_PACKED_STRUCT r;

    ZB_PACKED_PRE struct ed_s
    {
      zb_lbitfield_t          rx_on_when_idle:1; /*!< Indicates if neighbor receiver
                                                      enabled during idle periods:
                                                      TRUE = Receiver is on
                                                      FALSE = Receiver is off
                                                      This field should be present for
                                                      entries that record the parent or
                                                      children of a Zigbee router or
                                                      Zigbee coordinator.  */

      zb_lbitfield_t          keepalive_received:1; /*!< This value indicates at least one keepalive
                                                       *   has been received from the end device since
                                                       *   the router has rebooted.
                                                       */

      zb_lbitfield_t          nwk_timeout:4; /*!< End device timeout - @see @ref nwk_requested_timeout */

      zb_lbitfield_t          time_to_expire:26; /*Time stamp for ED aging*/
    } ZB_PACKED_STRUCT ed;
  } dev;
} ZB_PACKED_STRUCT
zb_neighbor_tbl_ent_t;


/**
   Determines whether receiver is enabled on the neighbor device if it is idle.
   Status is always true for routers and coordinators.

   @param nbt - pointer to 'zb_neighbor_tbl_ent_t'
   @return RX On When Idle status
 */
#define ZB_NEIGHBOR_ENT_RX_ON_WHEN_IDLE(nbt)                                              \
  (((nbt)->device_type != ZB_NWK_DEVICE_TYPE_ED) ||                                       \
    ((nbt)->device_type == ZB_NWK_DEVICE_TYPE_ED && ZB_U2B((nbt)->dev.ed.rx_on_when_idle)))

/**
   Returns outgoing cost of route for the neighbor device.
   Default cost value for devices with unspecified type is 'ZB_NWK_STATIC_PATH_COST'

   @param nbt - pointer to 'zb_neighbor_tbl_ent_t'
   @return Outgoing cost of route
 */
#define ZB_NEIGHBOR_ENT_GET_OUTGOING_COST(nbt)                                              \
  ((ZB_NWK_IS_ZC_OR_ZR((nbt)->device_type)) ? (nbt)->dev.r.outgoing_cost : ZB_NWK_STATIC_PATH_COST)

/**
   Determines whether the neighbor device permits joining.
   Default value for devices with unspecified type is 'ZB_FALSE'

   @param nbt - pointer to 'zb_neighbor_tbl_ent_t'
   @return Permit Joining flag
 */
#define ZB_NEIGHBOR_ENT_PERMIT_JOINING(nbt)                                               \
  ((ZB_NWK_IS_ZC_OR_ZR((nbt)->device_type)) ? ZB_U2B((nbt)->dev.r.permit_joining) : ZB_FALSE)

/**
   nwkDiscoveryTable entry

The stack MUST maintain a separate table for storing potential
networks and parents during join and rejoin operations.  The minimum
size of this table is 6 entries.  This table is described in Table
3-64

 */
typedef ZB_PACKED_PRE struct zb_nwk_disc_tbl_ent_s
{
  zb_ieee_addr_compressed_t long_addr; /*!< 64-bit address (packed) */
  zb_uint16_t               short_addr; /*!< 16-bit network address of the device. If -1, entry is free */
  zb_bitfield_t             used:1;         /*!< Record has used */
  zb_bitfield_t             panid_ref:5; /*!< ref to the extended Pan id  */
  zb_bitfield_t             permit_joining:1; /*!< A value of TRUE indicates that at
                                                least one Zigbee router on the
                                                network currently permits joining,
                                                i.e. its NWK has been issued an
                                                NLME-PERMIT-JOINING
                                                primitive and, the time limit if
                                                given, has not yet expired.  */
  zb_bitfield_t             router_capacity:1; /*!< This value is set to TRUE if this
                                                     device is capable of accepting
                                                     join requests from router-
                                                     capable devices and is set to
                                                     FALSE otherwise.   */
  zb_bitfield_t             end_device_capacity:1; /*!< This value is set to TRUE if the
                                                         device is capable of accepting
                                                         join requests from end devices
                                                         seeking to join the network and
                                                         is set to FALSE otherwise.  */
  zb_bitfield_t             potential_parent:1; /*!< This field usage - see
                                                      3.6.1.4.1.1  Child Procedure:
                                                      If the Status parameter indicates a refusal to permit
                                                      joining on the part of the neighboring device (that is, PAN at capacity or PAN
                                                      access denied), then the device attempting to join should set the Potential parent
                                                      bit to 0 in the corresponding neighbor table entry to indicate a failed join attempt.
                                                    */
  zb_bitfield_t             logical_channel:7; /*!< The current logical channel
                                                     occupied by the network.  */
  zb_bitfield_t             tlv_found:1;
  zb_bitfield_t             support_kn_methods_tlv_found:1;

  zb_bitfield_t             stack_profile:2; /*!< Zigbee stack profile id.   */
  zb_bitfield_t             channel_page:5; /*!< The current channel page occupied by the network. Max page # 31. Page 0 - 2.4GHz */

  zb_bitfield_t             device_type:2; /*!< Neighbor device type - \see
                                            * zb_nwk_device_type_t */
  zb_bitfield_t             depth:4; /*!< The network depth of this
                                       device. A value of 0x00
                                       indicates that this device is the
                                       Zigbee coordinator for the
                                       network.  */

  zb_uint8_t                lqi;  /*!< Link quality. Also used to calculate
                                   * incoming cost */
  zb_int8_t                 rssi;
  zb_uint8_t                update_id; /*!< This field reflects the value of nwkUpdateId from the NIB.  */

  zb_uint8_t                kn_secrets;
  zb_uint8_t                kn_methods;
  zb_uint16_t               classification_mask; /* that is router_information. Keep name to simplify mergw with WWAH code from r22 */
  /* a.	If ParentPreference indicates ?Key Negotiation Support? is preferred then parents advertising the ?Supported Key Negotiation Method TLV? in their Beacon Appendix SHALL be preferred.
b.	If  ParentPreference indicates ?CSL Support? preferred, then parents advertising ?CSL Support? set to 1 in the Router Information TLV of the Beacon Appendix SHALL be preferred.
 */
  zb_bitfield_t             mac_iface_idx:2;  /*!< An index into the MAC Interface Table
                                               * indicating what interface the neighbor or
                                               * child is bound to. */
  zb_bitfield_t             reserved:5;
} ZB_PACKED_STRUCT
zb_nwk_disc_tbl_ent_t;

/**
   Kind of negotiation before TX
*/

/**
 * @name MAC TX wait
 * @anchor mac_tx_wait
 */
/** @{ */
#define ZB_MAC_TX_WAIT_ACK               0U
#define ZB_MAC_TX_WAIT_CSMACA            1U
#define ZB_MAC_TX_WAIT_ZGP               2U
#define ZB_MAC_TX_WAIT_NONE              3U
/** @} */

/**
 * @name MAC TX status
 * @anchor mac_tx_status
 */
/** @{ */
#define ZB_TRANS_TX_OK                   0U
#define ZB_TRANS_CHANNEL_BUSY_ERROR      1U
#define ZB_TRANS_TX_RETRY_COUNT_EXCEEDED 2U
#define ZB_TRANS_TX_LBT_TO               3U
#define ZB_TRANS_NO_ACK                  4U
/** @} */

zb_uint32_t zb_get_channel_mask(void);

/* MAC diagnostics info */
typedef ZB_PACKED_PRE struct zb_mac_diagnostic_info_s
{
  zb_uint32_t mac_rx_bcast;       /* A counter that is incremented each time
                                   * the MAC layer receives a broadcast. */
  zb_uint32_t mac_tx_bcast;       /* A counter that is incremented each time
                                   * the MAC layer transmits a broadcast. */
  zb_uint32_t mac_rx_ucast;       /* A counter that is incremented each time the
                                   * MAC layer receives a unicast. */

  /* These 3 counters are required not to break
   * ZDO channel management logic that
   * uses normalized counters values.
   */
  zb_uint32_t mac_tx_ucast_total_zcl;    /* The same as mac_tx_ucast_total, but non-normalized */
  zb_uint16_t mac_tx_ucast_failures_zcl; /* The same as mac_tx_ucast_failures, but non-normalized */
  zb_uint16_t mac_tx_ucast_retries_zcl;  /* The same as mac_tx_ucast_retries, but non-normalized*/

  zb_uint16_t mac_tx_ucast_total; /* Total number of Mac Tx Transactions to
                                   * attempt to send a message (but not
                                   * counting retries) */
  zb_uint16_t mac_tx_ucast_failures; /* Total number of failed Tx
                                      * Transactions. So if the Mac send a
                                      * single packet, it will be retried 4
                                      * times without ack, that counts as 1 failure */
  zb_uint16_t mac_tx_ucast_retries; /* Total number of Mac Retries regardless of
                                     * whether the transaction resulted in
                                     * success or failure. */

  zb_uint16_t phy_to_mac_que_lim_reached;  /* A counter that is incremented each time when MAC RX queue if full. */

  zb_uint16_t mac_validate_drop_cnt; /* How many times the packet was dropped at the packet
                                      * validation stage for length or bad formatting. */

  zb_uint16_t phy_cca_fail_count;   /* Number of the PHY layer was unable
                                     * to transmit due to a failed CCA */

  zb_uint8_t period_of_time;    /* Time period over which MACTx results are measured */
  zb_uint8_t last_msg_lqi;      /* LQI value of the last received packet */
  zb_int8_t last_msg_rssi;      /* RSSI value of the last received packet */
  zb_uint32_t cca_retries;         /* Total number of CCA retries */
  zb_uint32_t pta_lo_pri_req;      /* Total number of low priority PTA request */
  zb_uint32_t pta_hi_pri_req;      /* Total number of high priority PTA request */
  zb_uint32_t pta_lo_pri_denied;   /* Total number of low priority PTA request denied by master */
  zb_uint32_t pta_hi_pri_denied;   /* Total number of high priority PTA request denied by master */
  zb_uint32_t pta_denied_rate;     /* PTA deny rate*/
} ZB_PACKED_STRUCT
zb_mac_diagnostic_info_t;

/* MAC diagnostics info extended struct */
typedef ZB_PACKED_PRE struct zb_mac_diagnostic_ex_info_s
{
  zb_mac_diagnostic_info_t mac_diag_info;
  /* Internal variables/counters that should be transferred
   * from MAC to ZDO and should not go to the NHLE */
  zb_uint32_t mac_tx_for_aps_messages; /* Internal counter used to calculate
                                          average_mac_retry_per_aps_message_sent in ZDO */
} ZB_PACKED_STRUCT
zb_mac_diagnostic_ex_info_t;

/*! @brief Structure saved diagnostic counters except MAC
 * See the ZCL Diagnostics Cluster -> ZCLr7 spec, chapter 3.15 */
typedef ZB_PACKED_PRE struct zdo_diagnostics_info_s
{
  /*! @brief An attribute that is incremented
   *         each time the device resets. */
  zb_uint16_t number_of_resets;

  /*! @brief A counter that is incremented each time
   *         the APS layer receives a broadcast. */
  zb_uint16_t aps_tx_bcast;

  /*! @brief A counter that is incremented each time
   *         the APS layer successfully transmits a unicast. */
  zb_uint16_t aps_tx_ucast_success;

  /*! @brief A counter that is incremented each time the
   *  APS layer retries the sending of a unicast. */
  zb_uint16_t aps_tx_ucast_retry;

  /*! @brief A counter that is incremented each time
   *         the APS layer fails to send a unicast. */
  zb_uint16_t aps_tx_ucast_fail;

  /*! @brief A counter that is incremented each time the network
   *  layer submits a route discovery message to the MAC. */
  zb_uint16_t route_disc_initiated;

  /*! @brief A counter that is incremented each time
   *  an entry is added to the neighbor table. */
  zb_uint16_t nwk_neighbor_added;

  /*! @brief A counter that is incremented each time
   *  an entry is removed from the neighbor table. */
  zb_uint16_t nwk_neighbor_removed;

  /*! @brief A counter that is incremented each time a neighbor table entry
   *  becomes stale because the neighbor has not been heard from. */
  zb_uint16_t nwk_neighbor_stale;

  /*! @brief A counter that is incremented each time
   *         a node joins or rejoins the network via this node. */
  zb_uint16_t join_indication;

  /*! @brief A counter that is incremented each time an entry
   *  is removed from the child table. */
  zb_uint16_t childs_removed;

  /*! @brief A counter that is incremented each time a message is
   *         dropped at the network layer because the APS frame counter
   *         was not higher than the last message seen from that source. */
  zb_uint16_t nwk_fc_failure;

  /*! @brief A counter that is incremented each time a message is
   *         dropped at the APS layer because the APS frame counter was
   *         not higher than the last message seen from that source. */
  zb_uint16_t aps_fc_failure;

  /*! @brief A counter that is incremented each time a message is dropped
   *  at the APS layer because it had APS encryption but the key associated
   *  with the sender has not been authenticated, and thus the key is not
   *  authorized for use in APS data messages. */
  zb_uint16_t aps_unauthorized_key;

  /*! @brief A counter that is incremented each time a NWK encrypted message
   *         was received but dropped because decryption failed. */
  zb_uint16_t nwk_decrypt_failure;

  /*! @brief A counter that is incremented each time an APS encrypted message was
   *  received but dropped because decryption failed. */
  zb_uint16_t aps_decrypt_failure;

  /*! @brief A counter that is incremented each time
   *         the stack failed to allocate a packet buffers. */
  zb_uint16_t packet_buffer_allocate_failures;

  /*! @brief A counter that is equal to the average number
   *         of MAC retries needed to send an APS message. */
  zb_uint16_t average_mac_retry_per_aps_message_sent;

  /*! @brief A counter that is incremented on the NWK layer
   *         each time tries number of a packet resending are gone.
   */
  zb_uint16_t nwk_retry_overflow;

  /** A non-standard counter of the number of times the NWK broadcast was
   *  dropped because the broadcast table was full.
   *  01/15/2021 In ZBOSS fired if any of the broadcast_transaction or
   *  broadcast_retransmission tables are full */
  zb_uint16_t nwk_bcast_table_full;

} ZB_PACKED_STRUCT zdo_diagnostics_info_t;

#if defined NCP_MODE && !defined NCP_MODE_HOST
typedef ZB_PACKED_PRE struct zb_ncp_pending_calls_s
{
  zb_uint8_t tsn;
  zb_uint16_t call_id;
} ZB_PACKED_STRUCT
zb_ncp_pending_calls_t;

#endif /* NCP_MODE && !NCP_MODE_HOST */

#endif /* !ZB_MINIMAL_CONTEXT */

#endif /* ZB_ZBOSS_API_INTERNAL_H */
