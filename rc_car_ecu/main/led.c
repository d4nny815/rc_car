#include "led.h"

static int red_state = 0;


void red_led_init() {
    gpio_config_t led_cfg = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = ((uint64_t)1 << RED_LED_PIN),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&led_cfg);
}

void red_led_on() {
    red_state = 1;
    gpio_set_level(RED_LED_PIN, red_state);
}

void red_led_off() {
    red_state = 0;
    gpio_set_level(RED_LED_PIN, red_state);
}

void red_led_toggle() {
    red_state = red_state ^ 1;
    gpio_set_level(RED_LED_PIN, red_state);
}