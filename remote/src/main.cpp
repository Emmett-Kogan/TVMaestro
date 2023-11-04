#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    // Init peripherals
    stdio_init_all();

    while (1) {
        printf("\033[1;1H\033[2J");
        printf("Hello, World!\n");
        sleep_ms(500);
    }
      
}