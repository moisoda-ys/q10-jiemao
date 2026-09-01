/*
 * Copyright (c) 2023 ZitaoTech
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/activity.h>
#include <zmk/keymap.h>
#include <zmk/events/activity_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

BUILD_ASSERT(DT_HAS_CHOSEN(zmk_keyboard_backlight),
             "keyboard_backlight: No zmk_keyboard_backlight chosen node found");

static const struct device *const indiled_dev = DEVICE_DT_GET(DT_CHOSEN(zmk_keyboard_backlight));

#define CHILD_COUNT(...) +1
#define DT_NUM_CHILD(node_id) (DT_FOREACH_CHILD(node_id, CHILD_COUNT))
#define INDICATOR_LED_NUM_LEDS (DT_NUM_CHILD(DT_CHOSEN(zmk_keyboard_backlight)))

#define BRT_MAX 90
#define BRT_BLINK_HIGH 100
#define BRT_BLINK_LOW 10
#define BLINK_INTERVAL_MS 125

static bool prev_active = false;
static int prev_layer = -1;
static bool blink_on = false;

static struct k_work_delayable polling_work;
static struct k_work_delayable blink_work;

static void set_led_brightness(uint8_t level) {
    if (!device_is_ready(indiled_dev)) {
        LOG_ERR("Indicator LED device not ready");
        return;
    }
    for (int i = 0; i < INDICATOR_LED_NUM_LEDS; i++) {
        int err = led_set_brightness(indiled_dev, i, level);
        if (err < 0) {
            LOG_ERR("Failed to set LED[%d] brightness: %d", i, err);
        }
    }
}

/* Blink whenever a non-base layer is active. */
static void blink_work_handler(struct k_work *work) {
    if (prev_layer == 0) {
        set_led_brightness(0);
        return;
    }

    blink_on = !blink_on;
    set_led_brightness(blink_on ? BRT_BLINK_HIGH : BRT_BLINK_LOW);

    k_work_reschedule(&blink_work, K_MSEC(BLINK_INTERVAL_MS));
}

static void polling_work_handler(struct k_work *work) {
    bool active = (zmk_activity_get_state() == ZMK_ACTIVITY_ACTIVE);
    int current_layer = zmk_keymap_highest_layer_active();

    if (current_layer != prev_layer || active != prev_active) {
        prev_layer = current_layer;
        prev_active = active;

        k_work_cancel_delayable(&blink_work);
        blink_on = false;
        if (current_layer == 0) {
            set_led_brightness(BRT_MAX);
        } else {
            blink_on = false;
            set_led_brightness(BRT_BLINK_LOW);
            k_work_reschedule(&blink_work, K_MSEC(BLINK_INTERVAL_MS));
        }
    }

    k_work_reschedule(&polling_work, K_MSEC(100));
}

static int keyboardbacklight_init(void) {
    if (!device_is_ready(indiled_dev)) {
        LOG_ERR("LED indicator device not ready");
        return -ENODEV;
    }

    prev_active = (zmk_activity_get_state() == ZMK_ACTIVITY_ACTIVE);
    prev_layer = -1;

    k_work_init_delayable(&polling_work, polling_work_handler);
    k_work_init_delayable(&blink_work, blink_work_handler);

    k_work_reschedule(&polling_work, K_MSEC(100));
    return 0;
}

SYS_INIT(keyboardbacklight_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
