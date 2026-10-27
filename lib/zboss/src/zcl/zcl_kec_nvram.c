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
/*  PURPOSE: Smart Energy Key Establishment Cluster, NVRAM routines
*/

#define ZB_TRACE_FILE_ID 3894

#include "zb_common.h"

#if defined ZB_ENABLE_SE_MIN_CONFIG && defined ZB_USE_NVRAM
/**
 * Common definitions for Server and Client
 */
#if defined ZB_SE_ENABLE_KEC_CLUSTER

#include "zcl/zb_zcl_kec_internal.h"
#include "zb_aps.h"
#include "zb_ecc.h"
#include "zb_error_indication.h"

zb_uint8_t *zb_kec_get_issuer(zb_uint8_t suite_no, zb_kec_icu_t *cert)
{
  zb_uint_t suite = (zb_uint_t)(1UL<<suite_no);
  if (suite == KEC_CS1 ) { return (zb_uint8_t *)cert->cs1.issuer; }
  if (suite == KEC_CS2 ) { return (zb_uint8_t *)cert->cs2.issuer; }

  TRACE_MSG(TRACE_ERROR, "zb_kec_get_issuer: suite not supported:%d ", (FMT__D, suite_no));
  return NULL;
}


zb_uint8_t *zb_kec_get_subject(zb_uint8_t suite_no, zb_kec_icu_t *cert)
{
  zb_uint_t suite = (zb_uint_t)(1UL<<suite_no);
  if (suite == KEC_CS1 ) { return (zb_uint8_t *)cert->cs1.subject; }
  if (suite == KEC_CS2 ) { return (zb_uint8_t *)cert->cs2.subject; }

  TRACE_MSG(TRACE_ERROR, "zb_kec_get_subject: suite not supported:%d ", (FMT__D, suite_no));
  return NULL;
}

#ifdef ZB_SE_KE_WHITELIST

/**
   Search for an ieee address in NVRAM whitelist

  @return RET_NOT_FOUND if ieee address is not found. return its index if found
*/
zb_int16_t zb_secur_search_ke_whitelist(zb_ieee_addr_t addr)
{
  zb_ret_t ret;
  zb_uint8_t page = KEC_CTX().ke_whitelist.page;
  zb_uint32_t pos = KEC_CTX().ke_whitelist.pos;
  zb_int16_t addr_idx = (zb_int16_t)RET_NOT_FOUND;
  zb_ieee_addr_t rec;
  zb_uint16_t i;

  for (i = 0; i < KEC_CTX().ke_whitelist.num_entries; i++)
  {
    ret = zb_osif_nvram_read(page, pos, (zb_uint8_t*) &rec, (zb_uint16_t)sizeof(zb_ieee_addr_t));

    /*cstat !MISRAC2012-Rule-14.3_a */
    /** @mdr{00007,18} */
    if (ret == RET_OK)
    {
      if (ZB_IEEE_ADDR_CMP(addr, rec))
      {
        addr_idx = (zb_int16_t)i;
        break;
      }
    }
    else
    {
      TRACE_MSG(TRACE_ERROR, "zb_osif_nvram_read failed", (FMT__0));
      /* No point in continue if NVRAM read failed */
      break;
    }

    pos += sizeof(zb_ieee_addr_t);

  }

  return addr_idx;
}


void zb_secur_ke_whitelist_add(zb_ieee_addr_t addr)
{
  if(KEC_CTX().ke_whitelist.num_entries >= ZB_SE_KE_WHITELIST_MAX_SIZE)
  {
    TRACE_MSG(TRACE_ERROR, "zb_secur_ke_whitelist_add: list is full", (FMT__0));
  }
  else
  {
    ZB_IEEE_ADDR_COPY(KEC_CTX().ke_whitelist.addr, addr);
    KEC_CTX().ke_whitelist.op = ZSE_KE_WHITELIST_ADD;
    TRACE_MSG(TRACE_COMMON2, "zb_secur_ke_whitelist_add addr " TRACE_FORMAT_64, (FMT__A, TRACE_ARG_64(addr)));
    /* If we fail, trace is given and assertion is triggered */
    (void)zb_nvram_write_dataset(ZB_NVRAM_KE_WHITELIST);
  }
}


void zb_secur_ke_whitelist_del(zb_ieee_addr_t addr)
{
  ZB_IEEE_ADDR_COPY(KEC_CTX().ke_whitelist.addr, addr);
  KEC_CTX().ke_whitelist.op = ZSE_KE_WHITELIST_DEL;
  TRACE_MSG(TRACE_COMMON2, "zb_secur_ke_whitelist_del addr " TRACE_FORMAT_64, (FMT__A, TRACE_ARG_64(addr)));
  /* If we fail, trace is given and assertion is triggered */
  (void)zb_nvram_write_dataset(ZB_NVRAM_KE_WHITELIST);
}


void zb_secur_ke_whitelist_del_all()
{
  KEC_CTX().ke_whitelist.op = ZSE_KE_WHITELIST_DEL_ALL;
  /* If we fail, trace is given and assertion is triggered */
  (void)zb_nvram_write_dataset(ZB_NVRAM_KE_WHITELIST);
}


static zb_ret_t kec_cluster_nvram_write_ke_whitelist_dataset(zb_uint8_t page, zb_uint32_t pos)
{
  zb_ret_t ret = RET_OK;
  zb_uint16_t i;
  zb_ieee_addr_t rec;
  zb_uint32_t whitelist_pos;

  TRACE_MSG(TRACE_COMMON1,
            ">> kec_cluster_nvram_write_ke_whitelist_dataset page %hd pos %d num_entries %hu op %hu",
            (FMT__H_D_H_H, page, pos,
             KEC_CTX().ke_whitelist.num_entries, KEC_CTX().ke_whitelist.op));

  if (KEC_CTX().ke_whitelist.op == ZSE_KE_WHITELIST_DEL_ALL)
  {
    KEC_CTX().ke_whitelist.num_entries = 0;
    KEC_CTX().ke_whitelist.op = ZSE_KE_WHITELIST_NONE;
  }
  else if (KEC_CTX().ke_whitelist.op == ZSE_KE_WHITELIST_DEL)
  {
    /* Copy old data to new location except address to delete!
    (delete operation is performed by not copying it to new location) */

    for (i = 0; i < (KEC_CTX().ke_whitelist.num_entries); i++)
    {
      ret = zb_osif_nvram_read(KEC_CTX().ke_whitelist.page,
                         KEC_CTX().ke_whitelist.pos + (sizeof(zb_ieee_addr_t) * i),
                         (zb_uint8_t *)&rec,
                         (zb_uint16_t)sizeof(zb_ieee_addr_t));
      ZB_ASSERT(RET_OK == ret);

      if (KEC_CTX().ke_whitelist.op != ZSE_KE_WHITELIST_NONE) /* no need to compare after DEL operation is finished */
      {
        if (ZB_IEEE_ADDR_CMP(rec, KEC_CTX().ke_whitelist.addr))
        {
          TRACE_MSG(TRACE_COMMON2, "addr found i %hu num_entries %hu", (FMT__H_H, i, KEC_CTX().ke_whitelist.num_entries));

          KEC_CTX().ke_whitelist.op = ZSE_KE_WHITELIST_NONE;
          KEC_CTX().ke_whitelist.num_entries -= 1U;

          /* Check that the deleted address was the last one */
          if (i == KEC_CTX().ke_whitelist.num_entries)
          {
            break;
          }

          whitelist_pos = KEC_CTX().ke_whitelist.pos;
          whitelist_pos += (zb_uint32_t)KEC_CTX().ke_whitelist.num_entries * sizeof(zb_ieee_addr_t);

          /* copy last element to the current position where it is the address to delete */
          ret = zb_osif_nvram_read(KEC_CTX().ke_whitelist.page,
                                   whitelist_pos,
                                   (zb_uint8_t *)&rec,
                                   (zb_uint16_t)sizeof(zb_ieee_addr_t));
          ZB_ASSERT(RET_OK == ret);
        }
      }
      ret = zb_nvram_write_data(page,
                                pos + (sizeof(zb_ieee_addr_t) * i),
                                (zb_uint8_t *)&rec,
                                (zb_uint16_t)sizeof(zb_ieee_addr_t));
      ZB_ASSERT(RET_OK == ret);
    }
  }
  else
  {
    /* Nothing to delete, just copy  old data to new location */
    for (i = 0; i < KEC_CTX().ke_whitelist.num_entries; i++)
    {
      ret = zb_osif_nvram_read(KEC_CTX().ke_whitelist.page,
                               KEC_CTX().ke_whitelist.pos + (sizeof(zb_ieee_addr_t) * i),
                               (zb_uint8_t *)&rec,
                               (zb_uint16_t)sizeof(zb_ieee_addr_t));
      ZB_ASSERT(RET_OK == ret);

      /* If we fail, trace is given and assertion is triggered */
      ret = zb_nvram_write_data(page,
                                pos + (sizeof(zb_ieee_addr_t) * i),
                                (zb_uint8_t *)&rec,
                                (zb_uint16_t)sizeof(zb_ieee_addr_t));
    }

    /* Really need there  `if (KEC_CTX().ke_whitelist.op ==  )ZSE_KE_WHITELIST_ADD)`: operation might be `ZSE_KE_WHITELIST_NONE`
    in cases where Add is called with a already existent address or Delete is called for a non existent address
    */
    /* Check for ADD operation */
    if (KEC_CTX().ke_whitelist.op == ZSE_KE_WHITELIST_ADD)
    {
      /* write addr to free space in the end of the datasset */
      /* If we fail, trace is given and assertion is triggered */
      ret = zb_nvram_write_data(page,
                                pos + (KEC_CTX().ke_whitelist.num_entries * sizeof(zb_ieee_addr_t)),
                                (zb_uint8_t *)&KEC_CTX().ke_whitelist.addr,
                                (zb_uint16_t)sizeof(zb_ieee_addr_t));
      KEC_CTX().ke_whitelist.op = ZSE_KE_WHITELIST_NONE;
      KEC_CTX().ke_whitelist.num_entries += 1U;
    }
  }

  /* update context page/pos for new location */
  KEC_CTX().ke_whitelist.page = page;
  KEC_CTX().ke_whitelist.pos = pos;

  TRACE_MSG(TRACE_COMMON1, "<< kec_cluster_nvram_write_ke_whitelist_dataset, ret %d", (FMT__D, ret));

  return ret;
}


static void kec_cluster_nvram_read_ke_whitelist_dataset(zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length,
    zb_nvram_ver_t ver, zb_uint16_t ds_ver)
{
  ZVUNUSED(ver);
  ZVUNUSED(ds_ver);

  /* Update page pos and number of entries in ZSE_CTX, ieee addresses are not stored in ZSE_CTX */
  KEC_CTX().ke_whitelist.page = page;
  KEC_CTX().ke_whitelist.pos = pos;
  KEC_CTX().ke_whitelist.num_entries = (length/(zb_uint16_t)sizeof(zb_ieee_addr_t));
}


/**
  Returns the size of all ieee addresses stored in nvram plus the size of one more or one less
    address depending on outstanding insertion/removal operations
*/
static zb_uint16_t kec_cluster_nvram_ke_whitelist_length(void)
{
  zb_size_t length = (KEC_CTX().ke_whitelist.num_entries * sizeof(zb_ieee_addr_t));

  switch(KEC_CTX().ke_whitelist.op)
  {
    case ZSE_KE_WHITELIST_ADD:
      /* Check for a duplicate */
      if(zb_secur_search_ke_whitelist(KEC_CTX().ke_whitelist.addr) != RET_NOT_FOUND)
      {
        /* address already in list, no point in adding */
        KEC_CTX().ke_whitelist.op = ZSE_KE_WHITELIST_NONE;
      }
      else
      {
        length += sizeof(zb_ieee_addr_t);
      }
      break;

    case ZSE_KE_WHITELIST_DEL:
      if(zb_secur_search_ke_whitelist(KEC_CTX().ke_whitelist.addr) != RET_NOT_FOUND)
      {
        /* address is in list, length will be -1 address */
        length -= sizeof(zb_ieee_addr_t);
      }
      else
      {
        /* element is not in list, nothing to delete */
        KEC_CTX().ke_whitelist.op = ZSE_KE_WHITELIST_NONE;
      }
      break;

    case ZSE_KE_WHITELIST_DEL_ALL:
      length = 0U;
      break;

    default:
      /* MISRA rule 16.4 - Mandatory default label */
      break;
  }

  return (zb_uint16_t)length;
}

#endif /* ZB_SE_KE_WHITELIST */


/**
   Read certdb dataset and create certdb pointers to nvram (hash data structure)
 */
static void kec_cluster_nvram_read_certdb_dataset(
    zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length, zb_nvram_ver_t ver, zb_uint16_t ds_ver)
{
  zb_kec_cert_nvram_t rec;
  zb_uint_t i, count, idx, n;
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_SECUR1, "> kec_cluster_nvram_read_certdb_dataset %d pos %ld length %d ver %d",
     (FMT__H_L_D_D, page, pos, length, ver));

  ZVUNUSED(ver);
  ZVUNUSED(ds_ver);

  count = length / sizeof(rec);

  TRACE_MSG(TRACE_SECUR3, "%d records", (FMT__D, count));

  for (i = 0 ; i < count && ret == RET_OK ; i++)
  {
    ret = zb_nvram_read_data(page, pos, (zb_uint8_t*)&rec, (zb_uint16_t)sizeof(rec));
    if (ret == RET_OK)
    {
      zb_uint8_t *t_issuer = zb_kec_get_issuer(rec.suite_no, (zb_kec_icu_t *)&rec.cert);
      /*zb_uint16_t suite = 1<<rec.suite_no;
      if (suite == KEC_CS1 ) t_issuer = (zb_uint8_t *)rec.cert.cs1.issuer;
      if (suite == KEC_CS2 ) t_issuer = (zb_uint8_t *)rec.cert.cs2.issuer;*/
      if (t_issuer == NULL)
      {
       /* unsupported suite */
       TRACE_MSG(TRACE_ERROR, "Crypto suite %X not implemented", (FMT__D, 1<<rec.suite_no));
       ret = RET_NOT_IMPLEMENTED;
      }
      idx = zb_64bit_hash(t_issuer) % ZB_ARRAY_SIZE(KEC_CTX().certdb);
      TRACE_MSG(TRACE_SECUR3, "page %d pos %d issuer " TRACE_FORMAT_64 ", hash idx %d", (FMT__D_D_A_D, page, pos, TRACE_ARG_64(t_issuer), idx));
      n = 0;
      /* Seek for free slot */
      while (KEC_CTX().certdb[idx].nvram_offset != 0U && n < ZB_ARRAY_SIZE(KEC_CTX().certdb))
      {
       idx = (idx + 1U) % ZB_ARRAY_SIZE(KEC_CTX().certdb);
       n++;
      }
      if (KEC_CTX().certdb[idx].nvram_offset != 0U)
      {
       /* no free space */
       TRACE_MSG(TRACE_ERROR, "No free space for certdb load", (FMT__0));
       ret = RET_NO_MEMORY;
      }
      else
      {
        KEC_CTX().certdb[idx].nvram_offset = pos;
        KEC_CTX().certdb[idx].nvram_page = page;
        TRACE_MSG(TRACE_SECUR3, "for certdb nvram_offset %d nvram_page %d; issuer " TRACE_FORMAT_64 " hash idx %d",
                   (FMT__D_D_A_D, KEC_CTX().certdb[idx].nvram_offset, KEC_CTX().certdb[idx].nvram_page,
                    TRACE_ARG_64(t_issuer), idx));
      }
    }
    pos += sizeof(rec);
  }

  TRACE_MSG(TRACE_ZCL1, "< kec_cluster_nvram_read_certdb_dataset", (FMT__0));
}


static zb_uint16_t kec_cluster_nvram_certdb_length(void)
{
  zb_uint16_t ret;
  zb_uint16_t n = 0;
  zb_uint16_t n_free = 0;
  zb_uint16_t i;
  for (i = 0 ; i < ZB_ARRAY_SIZE(KEC_CTX().certdb) ; ++i)
  {
    n += ZB_B2U(KEC_CTX().certdb[i].nvram_offset != 0U);
    /* Check for free slots: if no free slots, dataset write will skip installcode */
    n_free += ZB_B2U(KEC_CTX().certdb[i].nvram_offset == 0U);
  }

  ret = ZB_B2U(KEC_CTX().cert_to_add != NULL && !ZB_U2B(KEC_CTX().cert_to_add->do_update) && n_free != 0U);
  ret += n;
  ret *= (zb_uint16_t)sizeof(zb_kec_cert_nvram_t);

  return ret;
}


static zb_ret_t kec_cluster_nvram_write_certdb_dataset(zb_uint8_t page, zb_uint32_t pos)
{
  zb_kec_cert_nvram_t rec;
  zb_ret_t ret = RET_OK;
  zb_uint_t idx, i, n;

  zb_uint8_t *t_issuer = NULL;

  TRACE_MSG(TRACE_SECUR1, "> kec_cluster_nvram_write_certdb_dataset %d pos %ld", (FMT__H_L, page, pos));

  /* -1 to do not match with any offset in the hash */
  idx = (zb_uint_t)(~0U);
  if ((KEC_CTX().cert_to_add != NULL)
      && !ZB_U2B(KEC_CTX().cert_to_add->do_update))
  {
    t_issuer = zb_kec_get_issuer(KEC_CTX().cert_to_add->full_cert.suite_no, (zb_kec_icu_t *)&KEC_CTX().cert_to_add->full_cert.cert);

    /* Verify t_issuer assignment. It will unrecoverably fail if t_issuer is NULL */
    ZB_VERIFY(t_issuer != NULL, RET_UNINITIALIZED);

    idx = zb_64bit_hash(t_issuer) % ZB_ARRAY_SIZE(KEC_CTX().certdb);
    n = 0;
    /* Seek for free slot */
    while (KEC_CTX().certdb[idx].nvram_offset != 0U && n < ZB_ARRAY_SIZE(KEC_CTX().certdb))
    {
      idx = (idx + 1U) % ZB_ARRAY_SIZE(KEC_CTX().certdb);
      n++;
    }
    if (KEC_CTX().certdb[idx].nvram_offset != 0U)
    {
      TRACE_MSG(TRACE_SECUR1, "No free hash slot!", (FMT__0));
      idx = (zb_uint_t)(~0U);
    }
    else
    {
      TRACE_MSG(TRACE_SECUR3, "got free hash slot %d", (FMT__D, idx));
    }
  }

  for (i = 0; i < ZB_ARRAY_SIZE(KEC_CTX().certdb) && ret == RET_OK ; ++i)
  {
    if (i == idx)
    {
      /* This is the record we want to add, so fill it from the user's data */
      ZB_MEMCPY(&rec, &KEC_CTX().cert_to_add->full_cert, sizeof(rec));
      TRACE_MSG(TRACE_SECUR1, "idx %d add entry for address " TRACE_FORMAT_64, (FMT__D_A, idx, TRACE_ARG_64(t_issuer)));
    }
    else if (KEC_CTX().certdb[i].nvram_offset != 0U)
    {
      /* Read record from nvram using address stored in certdb hash
         Seems, record migration will not produce any problems because old page
         still exists during migration.
       */
      ret = zb_osif_nvram_read(KEC_CTX().certdb[i].nvram_page,
          KEC_CTX().certdb[i].nvram_offset,
                               (zb_uint8_t *)&rec,
                               (zb_uint16_t)sizeof(rec));
      /*cstat !MISRAC2012-Rule-14.3_b */
      /** @mdr{00007,19} */
      if (ret != RET_OK)
      {
        TRACE_MSG(TRACE_ERROR, "failed to read a certdb record", (FMT__0));
        break;
      }
      /* Find certificate to update by suite #, issuer and subject.
         These parameters allow to uniquely determine the certificate.
       */
      if ((KEC_CTX().cert_to_add != NULL)
          && ZB_U2B(KEC_CTX().cert_to_add->do_update)
          && (rec.suite_no == KEC_CTX().cert_to_add->suite_no)
          && (ZB_MEMCMP(zb_kec_get_issuer(rec.suite_no, &rec.cert), KEC_CTX().cert_to_add->issuer, 8) == 0)
          && (ZB_MEMCMP(zb_kec_get_subject(rec.suite_no, &rec.cert), KEC_CTX().cert_to_add->subject, 8) == 0)
        )
      {
        /* SEAP: at this point, t_issuer can be uninitialized, since the if block before the for loop might not be executed
           So, I am adding a t_issuer assignment verification here. If the case where t_issuer is not initialized is applicable
           here, probably will be best to remove this verification and the dereference of it in the TRACE_MSG below */
      
        /* Verify t_issuer assignment. It will unrecoverably fail if t_issuer is NULL */
        ZB_VERIFY(t_issuer != NULL, RET_UNINITIALIZED);

        ZB_MEMCPY(&rec, &KEC_CTX().cert_to_add->full_cert, sizeof(rec));
        TRACE_MSG(TRACE_SECUR1, "update certdb i %d for suite no %d for issuer " TRACE_FORMAT_64, (FMT__D_D_A, i, rec.suite_no, TRACE_ARG_64(t_issuer)));
      }
      else
      {
        TRACE_MSG(TRACE_SECUR1, "read by i %d : suite %d for issuer " TRACE_FORMAT_64,
          (FMT__D_D_A, i, rec.suite_no, TRACE_ARG_64(
            zb_kec_get_issuer(rec.suite_no, (zb_kec_icu_t *)&rec.cert))));
      }
    }
    else
    {
      continue;
    }
    /* If we fail, trace is given and assertion is triggered */
    ret = zb_nvram_write_data(page, pos, (zb_uint8_t*)&rec, (zb_uint16_t)sizeof(rec));
    /* Update certdb pointers hash */
    KEC_CTX().certdb[i].nvram_page = page;
    KEC_CTX().certdb[i].nvram_offset = pos;
    TRACE_MSG(TRACE_SECUR1, "idx %d: update as nvram_page %d nvram_offset %d", (FMT__D_D_D, i, KEC_CTX().certdb[i].nvram_page, KEC_CTX().certdb[i].nvram_offset));
    pos += sizeof(rec);
  }
  TRACE_MSG(TRACE_SECUR1, "< kec_cluster_nvram_write_certdb_dataset 0x%lx", (FMT__L, ret));
  return ret;
}


static zb_bool_t kec_cluster_nvram_ds_filter(zb_uint16_t ds_type)
{
  zb_bool_t ret;

  TRACE_MSG(TRACE_COMMON1, ">> kec_cluster_nvram_ds_filter, ds_type %d", (FMT__D, ds_type));

  switch(ds_type)
  {
    case ZB_NVRAM_DATASET_SE_CERTDB:
    case ZB_NVRAM_KE_WHITELIST:
      ret = ZB_TRUE;
      break;
    default:
      ret = ZB_FALSE;
      break;
  }

  TRACE_MSG(TRACE_COMMON1, "<< kec_cluster_nvram_ds_filter, ret %d", (FMT__D, ret));

  return ret;
}


static zb_size_t kec_cluster_nvram_ds_get_length(zb_uint16_t ds_type)
{
  zb_size_t ret;

  TRACE_MSG(TRACE_COMMON1, ">> kec_cluster_nvram_ds_get_length, ds_type %d", (FMT__D, ds_type));

  switch(ds_type)
  {
    case ZB_NVRAM_DATASET_SE_CERTDB:
      ret = kec_cluster_nvram_certdb_length();
      break;
#ifdef ZB_SE_KE_WHITELIST
    case ZB_NVRAM_KE_WHITELIST:
      ret = kec_cluster_nvram_ke_whitelist_length();
      break;
#endif /* ZB_SE_KE_WHITELIST */
    default:
      ZB_ASSERT(0);
      ret = (zb_size_t)(-1);
      break;
  }

  TRACE_MSG(TRACE_COMMON1, "<< kec_cluster_nvram_ds_get_length, ret %d", (FMT__D, ret));

  return ret;
}


static zb_uint16_t kec_cluster_nvram_ds_get_version(zb_uint16_t ds_type)
{
  zb_uint16_t ret;

  TRACE_MSG(TRACE_COMMON1, ">> kec_cluster_nvram_ds_get_version, ds_type %d", (FMT__D, ds_type));

  switch(ds_type)
  {
    case ZB_NVRAM_DATASET_SE_CERTDB:
      ret = ZB_NVRAM_SE_CERTDB_DS_VER;
      break;
#ifdef ZB_SE_KE_WHITELIST
    case ZB_NVRAM_KE_WHITELIST:
      ret = ZB_NVRAM_DATA_SET_VERSION_NOT_AVAILABLE;
      break;
#endif /* ZB_SE_KE_WHITELIST */
    default:
      ZB_ASSERT(0);
      ret = 0;
      break;
  }

  TRACE_MSG(TRACE_COMMON1, "<< kec_cluster_nvram_ds_get_version, ret %d", (FMT__D, ret));

  return ret;
}


static void kec_cluster_nvram_ds_read(zb_uint16_t ds_type,
                                      zb_uint8_t page,
                                      zb_uint32_t pos,
                                      zb_uint16_t len,
                                      zb_nvram_ver_t nvram_ver,
                                      zb_uint16_t ds_ver)
{
  TRACE_MSG(TRACE_COMMON1,
            ">> kec_cluster_nvram_ds_read, ds_type %d, page %d, "
            "pos %d, len %d, nvram_ver %d, ds_ver %d",
            (FMT__D_D_D_D_D_D, ds_type, page, pos, len, nvram_ver, ds_ver));

  switch(ds_type)
  {
    case ZB_NVRAM_DATASET_SE_CERTDB:
      kec_cluster_nvram_read_certdb_dataset(page, pos, len, nvram_ver, ds_ver);
      break;
#ifdef ZB_SE_KE_WHITELIST
    case ZB_NVRAM_KE_WHITELIST:
      kec_cluster_nvram_read_ke_whitelist_dataset(page, pos, len, nvram_ver, ds_ver);
      break;
#endif /* ZB_SE_KE_WHITELIST */
    default:
      ZB_ASSERT(0);
      break;
  }

  TRACE_MSG(TRACE_COMMON1, "<< kec_cluster_nvram_ds_read", (FMT__0));
}


static zb_ret_t kec_cluster_nvram_ds_write(zb_uint16_t ds_type,
                                           zb_uint8_t page,
                                           zb_uint32_t pos)
{
  zb_ret_t ret;

  TRACE_MSG(TRACE_COMMON1,
            ">> kec_cluster_nvram_ds_write, ds_type %d, page %d, pos %d",
            (FMT__D_D_D, ds_type, page, pos));

  switch(ds_type)
  {
    case ZB_NVRAM_DATASET_SE_CERTDB:
      ret = kec_cluster_nvram_write_certdb_dataset(page, pos);
      break;
#ifdef ZB_SE_KE_WHITELIST
    case ZB_NVRAM_KE_WHITELIST:
      ret = kec_cluster_nvram_write_ke_whitelist_dataset(page, pos);
      break;
#endif /* ZB_SE_KE_WHITELIST */
    default:
      ZB_ASSERT(0);
      ret = RET_ERROR;
      break;
  }

  TRACE_MSG(TRACE_COMMON1, "<< kec_cluster_nvram_ds_read", (FMT__0));

  return ret;
}


void zb_kec_cluster_nvram_init(void)
{
  TRACE_MSG(TRACE_COMMON1, ">> zb_kec_cluster_nvram_init", (FMT__0));

  (void)zb_nvram_custom_ds_register(kec_cluster_nvram_ds_filter,
                                    kec_cluster_nvram_ds_get_length,
                                    kec_cluster_nvram_ds_get_version,
                                    kec_cluster_nvram_ds_read,
                                    kec_cluster_nvram_ds_write);

  TRACE_MSG(TRACE_COMMON1, "<< zb_kec_cluster_nvram_init", (FMT__0));
}

#endif /* ZB_SE_ENABLE_KEC_CLUSTER */

#endif /* ZB_ENABLE_SE_MIN_CONFIG && ZB_USE_NVRAM */
