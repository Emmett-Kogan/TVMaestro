#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// Goal:
// Sample a pin at an interval and print the result to the terminal
// test cached

#define INTERVAL 50
#define CONVERT (3.3f / (1<<12))
#define BUFFER_SIZE 10

float buffer[BUFFER_SIZE];
uint8_t count = 0;

int main() {
    // Init peripherals
    stdio_init_all();
    adc_init();

    // init ADC pin
    adc_gpio_init(26);  // silkscreen label A0
    adc_select_input(0);

    while (1) {
        // Poll until the voltage drops below some threshhold
        // A low voltage seems to be the start condition
        uint16_t temp = adc_read();

        if (temp*CONVERT < 3.0) {
            buffer[count++] = temp*CONVERT;
        }

        // temp print statements just to look at the receiver output
        printf("\033[1;1H\033[2J");
        printf("ADC raw val %d\n", temp);
        printf("ADC converted val %f V\n", temp*CONVERT);

        // If buffer is full, print it, otherwise continue to fill it
        if (count == BUFFER_SIZE) break;

        sleep_ms(INTERVAL);
    }

    // Printf the buffer once it is full
    while (1) {
        printf("\033[1;1H\033[2J");
        printf("\n{%f", buffer[0]);
        for (int i = 1; i < count; i++) {
            printf(", %f", buffer[i]);
            if (i % 8 == 4) printf("\n");
        }
        printf("}\n");
        sleep_ms(5000);
    }
      
}
