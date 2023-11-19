#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define CONVERT (3.3f / (1<<12))
#define BUFFER_SIZE 6500
#define THRESHHOLD 2.0

int main() {
    // Init peripherals
    stdio_init_all();

    adc_init();
    adc_gpio_init(26);  // silkscreen label A0
    adc_select_input(0);

    uint16_t temp = 0;
    uint8_t buffer[BUFFER_SIZE];
    while(1) {
        while(adc_read()*CONVERT > THRESHHOLD);
        printf("\033[1;1H\033[2J");

        printf("Starting\n");
        while(temp < BUFFER_SIZE) {
            buffer[temp++] = adc_read();
            sleep_us(2);
        }

        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d, ", buffer[i]);
            if (i % 10 == 9) printf("\n");
        }
        printf("done");
    }
}