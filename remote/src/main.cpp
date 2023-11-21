#include "../libraries/signal.h"

#define MAX_BUTTONS 10

static button_t buttons[MAX_BUTTONS];

int main() {
    // Init peripherals
    stdio_init_all();

    // Initializes peripherals for use by the signal library
    signal_init();

    buttons[0].ID = POWER;

    while(1) {
        // Clear previous output
        printf("\033[1;1H\033[2J");

        // Record button press: waits until start condition so this is blocking
        record_signal(&buttons[0]);

        // Prints the signal over UART
        print_signal(&buttons[0]);

        // Wait so we don't accidentally record twice (seems like for the ROKU
        //  stick there is a button up and button down signal for instance)
        sleep_ms(5000);
    }
}
