#include "button.h"

void button_init() {
    gpio_config_t btn_cfg = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = ((uint64_t)1 << BTN_PIN),
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&btn_cfg);
}

int button_read() {
    return gpio_get_level(BTN_PIN);
}