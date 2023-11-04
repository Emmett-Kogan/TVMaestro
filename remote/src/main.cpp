#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// Goal:
// Sample a pin at an interval and print the result to the terminal
// test cached

#define INTERVAL 250
#define CONVERT (3.3f / (1<<12))
#define BUFFER_SIZE 900
#define THRESHHOLD 3.0

float buffer[BUFFER_SIZE];
uint8_t count = 0;

int main() {
    // Init peripherals
    stdio_init_all();
    adc_init();

    // init ADC pin
    adc_gpio_init(26);  // silkscreen label A0
    adc_select_input(0);

    // wait for a start condition, if voltage is not below 1V continue
    while (adc_read()*CONVERT > THRESHHOLD);

    while (1) {
        // transaction has started: sample to fill the buffer
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = adc_read()*CONVERT;
            sleep_us(INTERVAL);
        }

        // clear the terminal
        printf("\033[1;1H\033[2J");
        
        // print the buffer for serial plot
        // for (int i = 0; i < BUFFER_SIZE; i++) printf("%d", buffer[i]);

        // old print to be seen by a human
        printf("{");
        for (int i = 0; i < BUFFER_SIZE-1; i++) {
            if (i % 10 == 0 && i != 0) printf("\n");
            printf("%f, ", buffer[i]);
        }
        printf("%f}\n", buffer[BUFFER_SIZE-1]);

        // wait for a new transaction to start
        while(adc_read()*CONVERT > THRESHHOLD);
    }
}
