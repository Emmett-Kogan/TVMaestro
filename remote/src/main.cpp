#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define CONVERT (3.3f / (1<<12))
#define BUFFER_SIZE 20000
#define THRESHHOLD 2.0

int main() {
    // Init peripherals
    stdio_init_all();

    adc_init();
    adc_gpio_init(26);  // silkscreen label A0
    adc_select_input(0);

    

    while(1) {
        // wait for falling edge (start of transmission)
        while(adc_read()*CONVERT > THRESHHOLD);

        // Clear previous output
        //printf("\033[1;1H\033[2J");

        //printf("Starting\n");

        
        uint16_t temp = 0;
        uint8_t buffer[BUFFER_SIZE];

        // Record signal
        while(temp < BUFFER_SIZE) {
            // only high and low values, nothing else matters
            buffer[temp++] = adc_read() > 100 ? 255 : 0;
            sleep_us(2);
        }

        uint32_t runs[200];
        uint32_t count = 0;
        uint8_t index, prev;
        prev = buffer[0];

        for (int i = 0; i < BUFFER_SIZE; i++) {
            if (buffer[i] == prev)
                count++;
            else {
                runs[index++] = prev;
                runs[index++] = count;
                count = 0;
                prev = buffer[i];
            }
            printf("%x", buffer[i]);    // used for serial plot
        }

        // for (int i = 0; i < index; i+=2) {
        //     printf("%d: val: %d, length: %d\n", i/2, runs[i], runs[i+1]);
        // }
        
        sleep_ms(5000);
        //printf("done");

        // there are 34 lows in the signal, need to make sure there are 34 low regions here and a subsequent high
        // to make sure I'm recording the entire signal

    }
}