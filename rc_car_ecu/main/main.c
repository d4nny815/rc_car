// main.c
#include <stdio.h>
#include "esp_task_wdt.h"
#include "potentiometer.h"


#define ADC_BUF_SIZE (100)

int pot_init();
void fill_buffer(int* buf, int buf_size);

// void app_main(void) {
//     esp_task_wdt_deinit();  // Disable the watchdog timer
//     printf("Done with initialization\n");

// }

void app_main(void) {

    
    // * init code
    esp_task_wdt_deinit();  // Disable the watchdog timer

    if (pot_init() == POT_INIT_FAIL) {
        printf("[ERROR] POTENTIOMETER INIT FAILED\n");
        while (1);
    }

    printf("Done with initialization\n");
    int adc_val = 0;
    int adc_vals[ADC_BUF_SIZE];
    char uart_buf[256];

    uint32_t sum;
    
    // * main loop 
    while (1) {

        fill_buffer(adc_vals, ADC_BUF_SIZE);

        sum = 0;
        for (int i = 0; i < ADC_BUF_SIZE; i++) {
            sum += adc_vals[i];
        }
        sum /= ADC_BUF_SIZE;

        adc_val = CONVERT_TO_UV(sum);

        sprintf(uart_buf, "read %lu on the ADC, uV: %.03f", sum, adc_val / 1000000.0);
        printf("%s\n", uart_buf);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void fill_buffer(int* buf, int buf_size) {
    for (int i = 0; i < buf_size; i++) {
        buf[i] = pot_get_raw();
    }
    printf("done filling up buffer\n");
    return;
}

//TODO:
/**
 * 
 * print start up message
 * read adc and get the data
 * map it 0 - 100%?
 * output a pwm signal
 * change pwm signal
 * pot -> pwm pin
 */