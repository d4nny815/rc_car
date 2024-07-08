#ifndef LED_H
#define LED_H

// #include <stdio.h>
#include "driver/gpio.h"


#define RED_LED_PIN (GPIO_NUM_17)

void red_led_init();
void red_led_on();
void red_led_off();
void red_led_toggle();

#endif