#include "motor.h"

static volatile uint32_t encA_cnt = 0;
static int encA_cnt_per_dt = 0;
static pcnt_unit_handle_t pcnt_handle;
static enum motor_state {MOTOR_STOPPED, MOTOR_FORWARD, MOTOR_REVERSE} motor1_state = MOTOR_STOPPED;

int motor_init() {
    ledc_timer_config_t pwm_timer_cfg = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&pwm_timer_cfg);
    
    // init pin 1
    ledc_channel_config_t motor1_cfg = {
        .channel = LEDC_CHANNEL_0,
        .duty = 0,
        .gpio_num = MOTOR1_PIN1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = LEDC_TIMER_0,
    };
    ledc_channel_config(&motor1_cfg);

    // init pin 2
    ledc_channel_config_t motor2_cfg = {
        .channel = LEDC_CHANNEL_1,
        .duty = 0,
        .gpio_num = MOTOR1_PIN2,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = LEDC_TIMER_0,
    };
    ledc_channel_config(&motor2_cfg);

    motor1_state = MOTOR_STOPPED;

    // init encoder pins with pcnt
    pcnt_unit_config_t pcnt_cfg = {
        .low_limit = -32768,
        .high_limit = 32767,
        .intr_priority = 0,
        .flags.accum_count = 0, 
    };
    pcnt_new_unit(&pcnt_cfg, &pcnt_handle);

    pcnt_chan_config_t pcnt_chan_cfg = {
        .edge_gpio_num = MOTOR1_ENCA_PIN,
        .level_gpio_num = -1,
        .flags.invert_edge_input = 0,
        .flags.invert_level_input = 0,
        .flags.virt_edge_io_level = 0,
        .flags.virt_level_io_level = 0,
        .flags.io_loop_back = 0,
    };
    pcnt_channel_handle_t pcnt_chan_handle;
    pcnt_new_channel(pcnt_handle, &pcnt_chan_cfg, &pcnt_chan_handle);
    pcnt_channel_set_edge_action(pcnt_chan_handle, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD);
    pcnt_unit_enable(pcnt_handle);
    pcnt_unit_start(pcnt_handle);

    // init timer
    gptimer_handle_t timer_handle;
    gptimer_config_t timer_cfg = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1 * 1000 * 1000, // 1 MHz, 1 us resolution
        .intr_priority = 0,
        .flags.intr_shared = 0,
    };
    gptimer_new_timer(&timer_cfg, &timer_handle);

    gptimer_event_callbacks_t cbs = {
        .on_alarm = encA_timer_isr,
    };
    gptimer_register_event_callbacks(timer_handle, &cbs, NULL);

    gptimer_enable(timer_handle);

    gptimer_alarm_config_t alarm_cfg = {
        .alarm_count = MOTOR1_TIME_PERIOD_S * 1000 * 1000, // 5 sec * 1000 ms / 1 sec * 1000 us / 1 ms
        .reload_count = 0,
        .flags.auto_reload_on_alarm = 1,
    };
    gptimer_set_alarm_action(timer_handle, &alarm_cfg);

    gptimer_start(timer_handle);

    return MOTOR_INIT_PASS;
}


bool encA_timer_isr(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    pcnt_unit_stop(pcnt_handle);
    pcnt_unit_get_count(pcnt_handle, &encA_cnt_per_dt);
    pcnt_unit_clear_count(pcnt_handle);
    pcnt_unit_start(pcnt_handle);
    return true;
}


void motor1_forward(uint8_t duty_cycle) {
    if (motor1_state == MOTOR_REVERSE) motor1_stop();

    // turn on switching pin
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, GET_TIMER_DC(duty_cycle));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    motor1_state = MOTOR_FORWARD;
}


void motor1_reverse(uint8_t duty_cycle) {
    if (motor1_state == MOTOR_FORWARD) motor1_stop();

    // turn on switching pin
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, GET_TIMER_DC(duty_cycle));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

    motor1_state = MOTOR_REVERSE;
}


int motor1_get_rpm() {
    // return (encA_cnt_per_dt);
    return (encA_cnt_per_dt * REVOLUTIONS_PER_MINUTE) / (MOTOR1_TIME_PERIOD_S * MOTOR1_ENC_PPR) * MOTOR1_CORRECTION_FACTOR;
}


void motor1_stop() {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    while (motor1_get_rpm() != 0);
    
    motor1_state = MOTOR_STOPPED;
}



