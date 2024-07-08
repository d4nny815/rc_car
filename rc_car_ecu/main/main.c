// main.c
#include <stdio.h>
#include "esp_task_wdt.h"
#include "esp_dsp.h"
#include "potentiometer.h"
#include "motor.h"
#include "button.h"
#include "led.h"


#define ADC_BUF_SIZE (100)

void fill_buffer(int* buf, int buf_size);

void app_main(void) {
    if (pot_init() == POT_INIT_FAIL) {
        printf("[ERROR] POTENTIOMETER INIT FAILED\n");
        while (1);
    }

    if (motor_init() == MOTOR_INIT_FAIL) {
        printf("[ERROR] MOTOR INIT FAILED\n");
        while (1);
    }

    button_init();
    red_led_init();

    // gpio_dump_io_configuration(stdout, ((uint64_t)1 << BTN_PIN));
    printf("Done with initialization\n");
    
    uint8_t percent;


    // * main loop 
    while (1) {
        percent = pot_get_percent();
        motor1_forward(percent);
        printf("%d, %hu\n", motor1_get_rpm(), percent);
        vTaskDelay(MOTOR1_TIME_PERIOD_MS / portTICK_PERIOD_MS);
    }
}


void fill_buffer(int* buf, int buf_size) {
    for (int i = 0; i < buf_size; i++) {
        buf[i] = pot_get_raw();
    }
    return;
}