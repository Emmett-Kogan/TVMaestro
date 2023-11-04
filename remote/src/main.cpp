#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// Goal:
// Sample a pin at an interval and print the result to the terminal
// test

#define INTERVAL 1000
#define CONVERT (3.3f / (1<<12))

int main() {
    // Init peripherals
    stdio_init_all();
    adc_init();

    // init ADC pin
    adc_gpio_init(26);  // silkscreen label A0
    adc_select_input(0);

    while (1) {

        uint16_t temp = adc_read();

        printf("\033[1;1H\033[2J");
        printf("ADC raw val %d\n", temp);
        printf("ADC converted val %f V\n", temp*CONVERT);
        sleep_ms(INTERVAL);
    }
      
}
