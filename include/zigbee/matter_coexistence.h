/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZIGBEE_MATTER_COEXISTENCE_H_
#define ZIGBEE_MATTER_COEXISTENCE_H_

/** @file
 * @defgroup zigbee_matter_coexistence Matter + Zigbee coexistence runtime
 * @{
 *
 * @brief Orchestration layer that lets a single device host both Matter and
 * Zigbee stacks on the same 802.15.4 radio.
 *
 * The module owns two worker threads (Zigbee and Matter), handles the 802.15.4
 * callbacks-dispatcher handover on first Matter commissioning, and keeps the
 * persistent protocol state (see @ref zigbee_matter_protocol_state) up to date
 * across reboots and factory resets.
 *
 * Samples integrate the module by populating @ref zigbee_matter_coexistence_callbacks
 * and calling @ref zigbee_matter_coexistence_run from @c main().
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Callbacks supplied by the sample. */
struct zigbee_matter_coexistence_callbacks {
	/** Start the Matter application. Invoked from the Matter worker
	 *  thread. Required; must not be NULL.
	 */
	void (*matter_start)(void);

	/** Start the Zigbee application. Invoked from the Zigbee worker
	 *  thread only when the persisted protocol is Zigbee. Required;
	 *  must not be NULL.
	 */
	int (*zigbee_start)(void);

	/** Hook run on the Zigbee worker thread once the Matter board has
	 *  finished initialisation (first CHIPoBLE advertising-started event).
	 *  Typically used to register a chained Zigbee button handler with
	 *  the DK buttons library after @c dk_buttons_init() has completed.
	 *  May be NULL.
	 */
	void (*post_matter_board_init)(void);
};

/** @brief Start the coexistence runtime.
 *
 * Registers the Matter event handler, launches the (dormant) Zigbee and
 * Matter worker threads, and returns. Intended to be called once from
 * @c main() in a combined Matter + Zigbee sample.
 *
 * @param cb Callback table. Must not be NULL, and the @c matter_start and
 *           @c zigbee_start members must be set; passing an invalid table
 *           is a programming error and is asserted. The pointer must
 *           remain valid for the lifetime of the program; the module
 *           stores it internally.
 *
 * @retval 0 on success
 */
int zigbee_matter_coexistence_run(const struct zigbee_matter_coexistence_callbacks *cb);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ZIGBEE_MATTER_COEXISTENCE_H_ */
