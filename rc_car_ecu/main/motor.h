#ifndef MOTOR_H
#define MOTOR_H

#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "driver/pulse_cnt.h"
#include "led.h"

#define MOTOR_INIT_PASS (0)
#define MOTOR_INIT_FAIL (1)

#define MOTOR1_PIN1 (41)
#define MOTOR1_PIN2 (42)
#define MOTOR1_ENCA_PIN (38)
#define MOTOR1_ENCB_PIN (39)

#define GET_TIMER_DC(x) ((1 << 14) - 1) * (x / 100.0)
#define REVOLUTIONS_PER_MINUTE (60)
#define MOTOR1_TIME_PERIOD_MS (100)
#define MOTOR1_TIME_PERIOD_S (MOTOR1_TIME_PERIOD_MS / 1000.0)
// #define MOTOR1_TIME_PERIOD_S (60)
#define MOTOR1_ENC_PPR (48)
#define MOTOR1_CORRECTION_FACTOR (.65)

int motor_init();
void motor1_forward(uint8_t duty_cycle);
void motor1_reverse(uint8_t duty_cycle);
int motor1_get_rpm();
void motor1_stop();


void motor1_encA_isr_handler(void* arg);
void motor1_encB_isr_handler(void* arg);
bool encA_timer_isr(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

#endif