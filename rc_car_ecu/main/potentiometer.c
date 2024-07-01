#include "potentiometer.h"

static adc_oneshot_unit_handle_t pot_handle;


int pot_init() {
    adc_oneshot_unit_init_cfg_t pot_cfg = {
        .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
        .unit_id = ADC_UNIT_1,
    };
    if (adc_oneshot_new_unit(&pot_cfg, &pot_handle) == ESP_FAIL) return POT_INIT_FAIL;

    adc_oneshot_chan_cfg_t pot_chan_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_13,
    };
    if (adc_oneshot_config_channel(pot_handle, POT_CHAN, &pot_chan_cfg)) return POT_INIT_FAIL;

    return POT_INIT_SUCCESS;
}


uint16_t pot_get_raw() {
    int val;
    adc_oneshot_read(pot_handle, POT_CHAN, &val);
    return (uint16_t)val;
}


uint32_t pot_get_uV() {
    uint16_t raw = pot_get_raw();
    return CONVERT_TO_UV(raw);
}
