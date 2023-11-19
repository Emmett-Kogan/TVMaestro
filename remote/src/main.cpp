#include "../libraries/signal.h"

#define MAX_BUTTONS 10

static button_t buttons[MAX_BUTTONS];

int main() {
    // Init peripherals
    stdio_init_all();

    adc_init();
    adc_gpio_init(26);  // silkscreen label A0
    adc_select_input(0);

    buttons[0].ID = POWER;

    while(1) {
        record_signal(&buttons[0]);
        sleep_ms(5000);
    }
}
