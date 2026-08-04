/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <string.h>

#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/util.h>

#ifdef CONFIG_PARTITION_MANAGER_ENABLED
#include <pm_config.h>
#endif

#include <zboss_api.h>

#ifdef ZB_USE_NVRAM

/* RRAM (nRF54) requires write offset and length aligned to write block size (16 B) */
#define NVRAM_MAX_WRITE_BLOCK_SIZE 16

#ifdef CONFIG_PARTITION_MANAGER_ENABLED
#define ZBOSS_NVRAM_PARTITION_SIZE PM_ZBOSS_NVRAM_SIZE
#define ZBOSS_NVRAM_FLASH_AREA_ID  PM_ZBOSS_NVRAM_ID
#define ZBOSS_PRODUCT_CONFIG_FLASH_AREA_ID PM_ZBOSS_PRODUCT_CONFIG_ID
#else
#define ZBOSS_NVRAM_PARTITION_SIZE	      PARTITION_SIZE(zboss_nvram)
#define ZBOSS_NVRAM_FLASH_AREA_ID	      PARTITION_ID(zboss_nvram)
#define ZBOSS_PRODUCT_CONFIG_FLASH_AREA_ID    PARTITION_ID(zboss_product_config)

BUILD_ASSERT(FIXED_PARTITION_EXISTS(zboss_nvram),
	     "Devicetree must define fixed partition node zboss_nvram when Partition Manager is disabled.");
#ifdef ZB_PRODUCTION_CONFIG
BUILD_ASSERT(FIXED_PARTITION_EXISTS(zboss_product_config),
	     "Devicetree must define fixed partition node zboss_product_config for production config "
	     "when Partition Manager is disabled.");
#endif
#endif

/* Size of logical ZBOSS NVRAM page in bytes. */
#define ZBOSS_NVRAM_PAGE_SIZE (ZBOSS_NVRAM_PARTITION_SIZE / CONFIG_ZIGBEE_NVRAM_PAGE_COUNT)
#define PHYSICAL_PAGE_SIZE 0x1000
BUILD_ASSERT((ZBOSS_NVRAM_PAGE_SIZE % PHYSICAL_PAGE_SIZE) == 0,
	     "The size must be a multiply of physical page size.");

LOG_MODULE_DECLARE(zboss_osif, CONFIG_ZBOSS_OSIF_LOG_LEVEL);

/* ZBOSS callout that should be called once flash erase page operation
 * is finished.
 */
void zb_nvram_erase_finished(zb_uint8_t page);

static const struct flash_area *fa; /* ZBOSS nvram */

#ifdef ZB_PRODUCTION_CONFIG
static const struct flash_area *fa_pc; /* production config */
#endif

void zb_osif_nvram_init(const zb_char_t *name)
{
	ARG_UNUSED(name);
	int ret;

	ret = flash_area_open(ZBOSS_NVRAM_FLASH_AREA_ID, &fa);
	if (ret) {
		LOG_ERR("Can't open ZBOSS NVRAM flash area");
	}

#ifdef ZB_PRODUCTION_CONFIG
	ret = flash_area_open(ZBOSS_PRODUCT_CONFIG_FLASH_AREA_ID, &fa_pc);
	if (ret) {
		LOG_ERR("Can't open product config flash area");
	}
#endif
}

zb_uint32_t zb_get_nvram_page_length(void)
{
	return ZBOSS_NVRAM_PAGE_SIZE;
}

zb_uint8_t zb_get_nvram_page_count(void)
{
	return CONFIG_ZIGBEE_NVRAM_PAGE_COUNT;
}

static zb_uint32_t get_page_base_offset(int page_num)
{
	return (page_num * zb_get_nvram_page_length());
}

static int nvram_flash_write(const struct flash_area *area, off_t off,
			     const void *data, size_t len)
{
	uint32_t write_block = flash_area_align(area);
	const uint8_t *src = data;
	uint8_t block_buf[NVRAM_MAX_WRITE_BLOCK_SIZE];
	int err;

	/* Trivial case: alignment does not matter. */	
	if (write_block <= 1) {
		return flash_area_write(area, off, data, len);
	}

	__ASSERT_NO_MSG(write_block <= sizeof(block_buf));

	/* Unaligned start: flash requires writes on block boundaries (4 B or 16 B).
	 Read-modify-write one block.
	 */
	if (off & (write_block - 1)) {
		off_t block_start = off & ~(write_block - 1);
		size_t block_off = off - block_start;
		size_t chunk = MIN(len, write_block - block_off);

		/* Read the full block from flash. */
		err = flash_area_read(area, block_start, block_buf, write_block);
		if (err) {
			return err;
		}

		/* Patch in the new bytes at the correct offset. */
		memcpy(block_buf + block_off, src, chunk);

		/* Write the whole block back. */
		err = flash_area_write(area, block_start, block_buf, write_block);
		if (err) {
			return err;
		}

		off += chunk;
		src += chunk;
		len -= chunk;
	}

	size_t aligned_len = len & ~((size_t)write_block - 1);

	/* Aligned middle: write the largest contiguous aligned chunk in one
	 * flash_area_write(). This avoids the need to read-modify-write multiple
	 * blocks.
	 */
	if (aligned_len) {
		err = flash_area_write(area, off, src, aligned_len);
		if (err) {
			return err;
		}

		off += aligned_len;
		src += aligned_len;
		len -= aligned_len;
	}

	/* Unaligned tail: same read-modify-write as the head. */
	if (len) {
		err = flash_area_read(area, off, block_buf, write_block);
		if (err) {
			return err;
		}

		memcpy(block_buf, src, len);
		err = flash_area_write(area, off, block_buf, write_block);
		if (err) {
			return err;
		}
	}

	return 0;
}

zb_ret_t zb_osif_nvram_read(zb_uint8_t page, zb_uint32_t pos, zb_uint8_t *buf,
			    zb_uint16_t len)
{
	if (page >= zb_get_nvram_page_count()) {
		return RET_PAGE_NOT_FOUND;
	}

	if (pos + len > zb_get_nvram_page_length()) {
		return RET_INVALID_PARAMETER;
	}

	if (!buf) {
		return RET_INVALID_PARAMETER_3;
	}

	if (!len) {
		return RET_INVALID_PARAMETER_4;
	}
	LOG_DBG("Function: %s, page: %d, pos: %d, len: %d",
		__func__, page, pos, len);

	uint32_t flash_addr = get_page_base_offset(page) + pos;

	int err = flash_area_read(fa, flash_addr, buf, len);

	if (err) {
		LOG_ERR("Read error: %d", err);
		return RET_ERROR;
	}
	return RET_OK;
}

zb_ret_t zb_osif_nvram_write(zb_uint8_t page, zb_uint32_t pos, void *buf,
			     zb_uint16_t len)
{
	uint32_t flash_addr = get_page_base_offset(page) + pos;

	if (page >= zb_get_nvram_page_count()) {
		return RET_PAGE_NOT_FOUND;
	}

	if (pos + len > zb_get_nvram_page_length()) {
		return RET_INVALID_PARAMETER;
	}

	if (!buf) {
		return RET_INVALID_PARAMETER_3;
	}

	if (len == 0) {
		return RET_OK;
	}

	if (!(len >> 2)) {
		return RET_INVALID_PARAMETER_4;
	}

	LOG_DBG("Function: %s, page: %d, pos: %d, len: %d",
		__func__, page, pos, len);

	int err = nvram_flash_write(fa, flash_addr, buf, len);

	if (err) {
		LOG_ERR("Write error: %d", err);
		return RET_ERROR;
	}

	return RET_OK;
}

zb_ret_t zb_osif_nvram_erase_async(zb_uint8_t page)
{
	zb_ret_t ret = RET_OK;

	if (page < zb_get_nvram_page_count()) {
		int err = flash_area_erase(fa, get_page_base_offset(page),
					   zb_get_nvram_page_length());
		if (err) {
			LOG_ERR("Erase error: %d", err);
			ret = RET_ERROR;
		}
	}
	zb_nvram_erase_finished(page);
	return ret;
}

void zb_osif_nvram_wait_for_last_op(void)
{
	/* empty for synchronous erase and write */
}

void zb_osif_nvram_flush(void)
{
	/* empty for synchronous erase and write */
}


#ifdef ZB_PRODUCTION_CONFIG

#define ZB_OSIF_PRODUCTION_CONFIG_MAGIC             { 0xE7, 0x37, 0xDD, 0xF6 }
#define ZB_OSIF_PRODUCTION_CONFIG_MAGIC_SIZE        4

zb_bool_t zb_osif_prod_cfg_check_presence(void)
{
	zb_uint8_t hdr[ZB_OSIF_PRODUCTION_CONFIG_MAGIC_SIZE] =
		ZB_OSIF_PRODUCTION_CONFIG_MAGIC;
	zb_uint8_t buffer[ZB_OSIF_PRODUCTION_CONFIG_MAGIC_SIZE] = {0};

	int err = flash_area_read(fa_pc, 0, buffer,
				  ZB_OSIF_PRODUCTION_CONFIG_MAGIC_SIZE);

	if (!err) {
		return ((zb_bool_t) !memcmp(buffer, hdr, sizeof(buffer)));

	} else {
		return ZB_FALSE;
	}
}

zb_ret_t zb_osif_prod_cfg_read_header(zb_uint8_t *prod_cfg_hdr,
				      zb_uint16_t hdr_len)
{
	int err = flash_area_read(fa_pc, ZB_OSIF_PRODUCTION_CONFIG_MAGIC_SIZE,
				  prod_cfg_hdr, hdr_len);

	if (err) {
		LOG_ERR("Prod conf header read error: %d", err);
		return RET_ERROR;
	}
	return RET_OK;
}


zb_ret_t zb_osif_prod_cfg_read(zb_uint8_t *buffer,
			       zb_uint16_t len,
			       zb_uint16_t offset)
{
	uint32_t pc_offset = ZB_OSIF_PRODUCTION_CONFIG_MAGIC_SIZE + offset;
	int err = flash_area_read(fa_pc, pc_offset, buffer, len);

	if (err) {
		LOG_ERR("Prod conf read error: %d", err);
		return RET_ERROR;
	}
	return RET_OK;
}

#endif  /* ZB_PRODUCTION_CONFIG */

#endif  /* ZB_USE_NVRAM */
