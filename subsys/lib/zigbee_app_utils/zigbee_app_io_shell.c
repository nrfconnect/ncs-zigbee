/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_string_conv.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/util.h>

#include <zigbee/zigbee_app_io_shell.h>

LOG_MODULE_REGISTER(zigbee_app_io_shell, CONFIG_ZIGBEE_APP_UTILS_LOG_LEVEL);

#define BUTTONS_NODE  DT_PATH(buttons)
#define LEDS_NODE     DT_PATH(leds)
#define PWM_LEDS_NODE DT_PATH(pwmleds)

#define MAX_HOLD_TIME_MS (60 * 1000)
#define EVENT_QUEUE_SIZE 8
#define LED_CHANGES      4

#if DT_NODE_EXISTS(BUTTONS_NODE)
#define BUTTON_COUNT DT_CHILD_NUM(BUTTONS_NODE)
#else
#define BUTTON_COUNT 0
#endif

#if DT_NODE_EXISTS(LEDS_NODE)
#define LED_COUNT DT_CHILD_NUM(LEDS_NODE)
#else
#define LED_COUNT 0
#endif

#if defined(CONFIG_PWM)
/* PWM LEDs whose PWM controller is disabled cannot be referenced. */
#define PWM_SPEC_AND_COMMA(node)                                                                   \
	COND_CODE_1(DT_NODE_HAS_STATUS(DT_PWMS_CTLR(node), okay), (PWM_DT_SPEC_GET(node),), ())

static const struct pwm_dt_spec pwm_leds[] = {
#if DT_NODE_EXISTS(PWM_LEDS_NODE)
	DT_FOREACH_CHILD_STATUS_OKAY(PWM_LEDS_NODE, PWM_SPEC_AND_COMMA)
#endif
};

#define PWM_LED_COUNT ARRAY_SIZE(pwm_leds)
#else
#define PWM_LED_COUNT 0
#endif

struct button_event {
	uint32_t state;
	uint32_t has_changed;
};

struct led_stats {
	bool on;
	uint8_t changes;
	/* Oldest change first. */
	uint32_t change_ms[LED_CHANGES];
};

struct pwm_led_stats {
	struct led_stats led;
	/* Level in percent. */
	uint8_t level;
};

static void event_work_fn(struct k_work *work);
static void release_work_fn(struct k_work *work);

static button_handler_t app_handler;
static atomic_t sim_buttons;
static struct led_stats led_stats[LED_COUNT];
static struct pwm_led_stats pwm_led_stats[PWM_LED_COUNT];

K_MSGQ_DEFINE(app_io_event_msgq, sizeof(struct button_event), EVENT_QUEUE_SIZE, 4);

static K_WORK_DEFINE(event_work, event_work_fn);
static K_WORK_DELAYABLE_DEFINE(release_work, release_work_fn);

uint32_t zigbee_app_io_get_buttons(void)
{
	return dk_get_buttons() | (uint32_t)atomic_get(&sim_buttons);
}

static void led_stats_update(struct led_stats *stats, bool on)
{
	if (stats->on == on) {
		return;
	}

	stats->on = on;

	for (uint8_t i = 0; i < LED_CHANGES - 1; i++) {
		stats->change_ms[i] = stats->change_ms[i + 1];
	}

	stats->change_ms[LED_CHANGES - 1] = k_uptime_get_32();
	if (stats->changes < LED_CHANGES) {
		stats->changes++;
	}
}

static uint32_t led_stats_toggle_rate(const struct led_stats *stats)
{
	uint32_t rate_ms;
	uint32_t newest_ms;
	uint32_t oldest_ms;

	if (stats->changes < LED_CHANGES) {
		return 0;
	}

	oldest_ms = stats->change_ms[0];
	newest_ms = stats->change_ms[LED_CHANGES - 1];
	rate_ms = (newest_ms - oldest_ms) / (LED_CHANGES - 1);

	/* An LED that has not changed for two intervals is no longer blinking. */
	if (k_uptime_get_32() - newest_ms > 2 * rate_ms) {
		return 0;
	}

	return rate_ms;
}

int zigbee_app_io_set_led(uint8_t led_idx, uint32_t val)
{
	int err;

	err = dk_set_led(led_idx, val);
	if (err || led_idx >= ARRAY_SIZE(led_stats)) {
		return err;
	}

	led_stats_update(&led_stats[led_idx], val != 0);
	return 0;
}

#if defined(CONFIG_PWM)
int zigbee_app_io_set_pwm(const struct pwm_dt_spec *spec, uint32_t period, uint32_t pulse)
{
	uint32_t level;
	int err;

	err = pwm_set_dt(spec, period, pulse);
	if (err) {
		return err;
	}

	for (size_t i = 0; i < PWM_LED_COUNT; i++) {
		if (pwm_leds[i].dev == spec->dev && pwm_leds[i].channel == spec->channel) {
			level = period ? (uint32_t)((uint64_t)pulse * 100U / period) : 0;
			pwm_led_stats[i].level = (uint8_t)MIN(level, 100U);
			led_stats_update(&pwm_led_stats[i].led, level > 0);
			break;
		}
	}

	return 0;
}
#endif

static void event_work_fn(struct k_work *work)
{
	struct button_event evt;

	ARG_UNUSED(work);

	while (k_msgq_get(&app_io_event_msgq, &evt, K_NO_WAIT) == 0) {
		app_handler(evt.state, evt.has_changed);
	}
}

static int event_queue(uint32_t state, uint32_t has_changed)
{
	struct button_event evt = {
		.state = state,
		.has_changed = has_changed,
	};

	return k_msgq_put(&app_io_event_msgq, &evt, K_NO_WAIT);
}

static void release_work_fn(struct k_work *work)
{
	uint32_t mask = (uint32_t)atomic_get(&sim_buttons);
	int err;

	ARG_UNUSED(work);

	if (mask == 0) {
		return;
	}

	/* Queue the release before clearing the simulated state, so that a new
	 * press cannot be queued ahead of this release.
	 */
	err = event_queue(dk_get_buttons(), mask);
	atomic_clear(&sim_buttons);

	if (err) {
		LOG_ERR("Button event queue full, release of 0x%x lost", mask);
		return;
	}

	k_work_submit(&event_work);
}

static int press_start(const struct shell *sh, uint32_t id, unsigned long hold_ms)
{
	uint32_t mask = BIT(id);
	int err;

	if (!atomic_cas(&sim_buttons, 0, mask)) {
		shell_error(sh, "Another button press is in progress");
		return -EBUSY;
	}

	err = event_queue(zigbee_app_io_get_buttons(), mask);
	if (err) {
		atomic_clear(&sim_buttons);
		shell_error(sh, "Button event queue full, try again");
		return err;
	}

	k_work_submit(&event_work);
	k_work_reschedule(&release_work, K_MSEC(hold_ms));
	return 0;
}

static int parse_id(const struct shell *sh, const char *str, size_t count, const char *what,
		    uint32_t *id)
{
	int err = 0;
	unsigned long val = shell_strtoul(str, 0, &err);

	if (count == 0) {
		shell_error(sh, "No %ss available", what);
		return -ENODEV;
	}

	if (err || val >= count) {
		shell_error(sh, "Invalid %s ID, must be <0, %u>", what, (unsigned int)count - 1);
		return -EINVAL;
	}

	*id = (uint32_t)val;
	return 0;
}

static int button_press(const struct shell *sh, const char *id_str, unsigned long hold_ms)
{
	uint32_t id;
	int err;

	if (app_handler == NULL) {
		shell_error(sh, "Not initialized, zigbee_app_io_shell_init() was not called");
		return -ENODEV;
	}

	err = parse_id(sh, id_str, BUTTON_COUNT, "button", &id);
	if (err) {
		return err;
	}

	return press_start(sh, id, hold_ms);
}

static int cmd_button_press(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);

	return button_press(sh, argv[1], 0);
}

static int cmd_button_hold(const struct shell *sh, size_t argc, char **argv)
{
	int err = 0;
	long hold_ms;

	ARG_UNUSED(argc);

	hold_ms = shell_strtol(argv[2], 0, &err);
	if (err || !IN_RANGE(hold_ms, 0, MAX_HOLD_TIME_MS)) {
		shell_error(sh, "Invalid hold time, must be <0, %u> ms", MAX_HOLD_TIME_MS);
		return -EINVAL;
	}

	return button_press(sh, argv[1], hold_ms);
}

static int cmd_button_get(const struct shell *sh, size_t argc, char **argv)
{
	uint32_t id;
	int err;

	ARG_UNUSED(argc);

	err = parse_id(sh, argv[1], BUTTON_COUNT, "button", &id);
	if (err) {
		return err;
	}

	shell_print(sh, "Button %u: %u", id, (zigbee_app_io_get_buttons() & BIT(id)) ? 1 : 0);
	return 0;
}

static int cmd_led_get(const struct shell *sh, size_t argc, char **argv)
{
	uint32_t id;
	int err;

	ARG_UNUSED(argc);

	err = parse_id(sh, argv[1], LED_COUNT, "LED", &id);
	if (err) {
		return err;
	}

	shell_print(sh, "LED %u: %u", id, led_stats[id].on ? 1 : 0);
	return 0;
}

static int cmd_led_toggle_rate(const struct shell *sh, size_t argc, char **argv)
{
	uint32_t id;
	int err;

	ARG_UNUSED(argc);

	err = parse_id(sh, argv[1], LED_COUNT, "LED", &id);
	if (err) {
		return err;
	}

	shell_print(sh, "LED %u toggle_rate: %u", id, led_stats_toggle_rate(&led_stats[id]));
	return 0;
}

static int cmd_pwm_get(const struct shell *sh, size_t argc, char **argv)
{
	uint32_t id;
	int err;

	ARG_UNUSED(argc);

	err = parse_id(sh, argv[1], PWM_LED_COUNT, "PWM LED", &id);
	if (err) {
		return err;
	}

	shell_print(sh, "PWM LED %u: %u", id, pwm_led_stats[id].level);
	return 0;
}

static int cmd_pwm_toggle_rate(const struct shell *sh, size_t argc, char **argv)
{
	uint32_t id;
	int err;

	ARG_UNUSED(argc);

	err = parse_id(sh, argv[1], PWM_LED_COUNT, "PWM LED", &id);
	if (err) {
		return err;
	}

	shell_print(sh, "PWM LED %u toggle_rate: %u", id,
		    led_stats_toggle_rate(&pwm_led_stats[id].led));
	return 0;
}

void zigbee_app_io_shell_init(button_handler_t handler)
{
	app_handler = handler;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_button,
	SHELL_CMD_ARG(press, NULL,
		      "<id>\nShort press of a button",
		      cmd_button_press, 2, 0),
	SHELL_CMD_ARG(hold, NULL,
		      "<id> <time_ms>\nHold a button for time_ms, then release it",
		      cmd_button_hold, 3, 0),
	SHELL_CMD_ARG(get, NULL,
		      "<id>\nPrint the button state (0 or 1)",
		      cmd_button_get, 2, 0),
	SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(sub_led,
	SHELL_CMD_ARG(get, NULL,
		      "<id>\nPrint the LED state (0 or 1)",
		      cmd_led_get, 2, 0),
	SHELL_CMD_ARG(get_toggle_rate, NULL,
		      "<id>\nPrint the average time between the last four LED changes\n"
		      "in ms, or 0 if the LED is not blinking",
		      cmd_led_toggle_rate, 2, 0),
	SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(sub_pwm,
	SHELL_CMD_ARG(get, NULL,
		      "<id>\nPrint the PWM LED level (0-100)",
		      cmd_pwm_get, 2, 0),
	SHELL_CMD_ARG(get_toggle_rate, NULL,
		      "<id>\nPrint the average time between the last four PWM LED\n"
		      "on/off changes in ms, or 0 if the PWM LED is not blinking",
		      cmd_pwm_toggle_rate, 2, 0),
	SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(sub_app_io,
	SHELL_CMD(button, &sub_button, "Simulate buttons and read their state", NULL),
	SHELL_CMD(led, &sub_led, "Read LED states", NULL),
	SHELL_COND_CMD(CONFIG_PWM, pwm, &sub_pwm, "Read PWM LED states", NULL),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(app_io, &sub_app_io, "Simulate buttons and read LED states", NULL);
