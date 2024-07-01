#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "esp_adc/adc_oneshot.h"

#define POT_INIT_SUCCESS (0) 
#define POT_INIT_FAIL (1)
#define POT_PIN (GPIO_NUM_4)
#define POT_CHAN (ADC_CHANNEL_3)
#define POT_ADC_BITS (13)
#define POT_ADC_MAX ((1 << POT_ADC_BITS) - 1)
#define POT_VREF_MV (3300)

#define POT_MAGIC_CAL_X (314)
#define POT_MAGIC_CAL_Y (-4241)

#define CONVERT_TO_UV(x) (x * POT_MAGIC_CAL_X - POT_MAGIC_CAL_Y)


int pot_init();
uint16_t pot_get_raw();
uint32_t pot_get_uV();

#endif