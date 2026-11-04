/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZIGBEE_APP_IO_SHELL_H__
#define ZIGBEE_APP_IO_SHELL_H__

#include <stdbool.h>
#include <stdint.h>
#include <zephyr/sys/util.h>
#include <dk_buttons_and_leds.h>

#if defined(CONFIG_PWM)
#include <zephyr/drivers/pwm.h>
#endif

/** @file zigbee_app_io_shell.h
 *
 * @defgroup zigbee_app_io_shell Zigbee application button and LED shell.
 * @{
 * @brief Shell commands for simulating button presses and reading LED states.
 *
 * @details Provides the @c app_io shell command. Simulated presses are passed
 * to the application button handler and are reported by
 * @ref zigbee_app_io_get_buttons. LED changes are recorded only if they are
 * made with @ref zigbee_app_io_set_led, and PWM LED changes only if they are
 * made with @ref zigbee_app_io_set_pwm.
 */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_ZIGBEE_APP_IO_SHELL)

/**@brief Initialize the shell module.
 *
 * @note Must be called after @c dk_buttons_init.
 *
 * @param[in] handler Button handler passed to @c dk_buttons_init.
 */
void zigbee_app_io_shell_init(button_handler_t handler);

/**@brief Get the state of buttons, including simulated presses.
 *
 * @return Bitmask of pressed buttons, as returned by @c dk_get_buttons.
 */
uint32_t zigbee_app_io_get_buttons(void);

/**@brief Set the state of an LED and record the change.
 *
 * @note Use this function instead of @c dk_set_led, so that LED changes are
 *       visible to the shell module.
 *
 * @param[in] led_idx Index of the LED.
 * @param[in] val     Value for the LED: 1 - turn on, 0 - turn off.
 *
 * @return Same as @c dk_set_led.
 */
int zigbee_app_io_set_led(uint8_t led_idx, uint32_t val);

#if defined(CONFIG_PWM)
/**@brief Set a PWM LED and record the change.
 *
 * @note Use this function instead of @c pwm_set_dt, so that PWM LED changes are
 *       visible to the shell module. Only the PWM LEDs from the @c pwmleds
 *       devicetree node are recorded. The PWM LED is on if the PWM level, as a
 *       percentage of the period, is greater than 0.
 *
 * @param[in] spec   PWM specification of the LED.
 * @param[in] period Period in nanoseconds.
 * @param[in] pulse  Pulse width in nanoseconds.
 *
 * @return Same as @c pwm_set_dt.
 */
int zigbee_app_io_set_pwm(const struct pwm_dt_spec *spec, uint32_t period, uint32_t pulse);
#endif

#else /* CONFIG_ZIGBEE_APP_IO_SHELL */

static inline void zigbee_app_io_shell_init(button_handler_t handler)
{
	ARG_UNUSED(handler);
}

static inline uint32_t zigbee_app_io_get_buttons(void)
{
	return dk_get_buttons();
}

static inline int zigbee_app_io_set_led(uint8_t led_idx, uint32_t val)
{
	return dk_set_led(led_idx, val);
}

#if defined(CONFIG_PWM)
static inline int zigbee_app_io_set_pwm(const struct pwm_dt_spec *spec, uint32_t period,
					uint32_t pulse)
{
	return pwm_set_dt(spec, period, pulse);
}
#endif

#endif /* CONFIG_ZIGBEE_APP_IO_SHELL */

/**@brief Turn an LED on and record the change.
 *
 * @param[in] led_idx Index of the LED.
 *
 * @return Same as @c dk_set_led_on.
 */
static inline int zigbee_app_io_set_led_on(uint8_t led_idx)
{
	return zigbee_app_io_set_led(led_idx, 1);
}

/**@brief Turn an LED off and record the change.
 *
 * @param[in] led_idx Index of the LED.
 *
 * @return Same as @c dk_set_led_off.
 */
static inline int zigbee_app_io_set_led_off(uint8_t led_idx)
{
	return zigbee_app_io_set_led(led_idx, 0);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZIGBEE_APP_IO_SHELL_H__ */
