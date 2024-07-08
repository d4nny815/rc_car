#ifndef BUTTON_H
#define BUTTON_H

#include "driver/gpio.h"

#define BTN_PIN (GPIO_NUM_40)

void button_init();
int button_read();

#endif