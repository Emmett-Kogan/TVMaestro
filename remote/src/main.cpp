#include "../libraries/signal.h"
#include <stdlib.h>

#define MAX_BUTTONS 10

#define CR  0x0D
#define LF  0x0A
#define BS  0x08
#define DEL 0x7F

static button_t buttons[MAX_BUTTONS];

int main() {
    // Init peripherals
    stdio_init_all();
    
    // Initializes peripherals for use by the signal library
    signal_init();

    buttons[0].ID = POWER;

    // while(1) {
    //     // Clear previous output
    //     printf("\033[1;1H\033[2J");

    //     // Record button press: waits until start condition so this is blocking
    //     record_signal(&buttons[0]);

    //     // Prints the signal over UART
    //     print_signal(&buttons[0]);

    //     play_signal(&buttons[0]);

    //     // Wait so we don't accidentally record twice (seems like for the ROKU
    //     //  stick there is a button up and button down signal for instance)
    //     sleep_ms(5000);
    // }

    while(1) {
        // Clear previous output
        printf("\033[1;1H\033[2J");

        // Get command
        char command[256];
        uint8_t index = 0;
        while(1) {
            command[index] = getchar();
            printf("%c", command[index]);

            if ((command[index] == BS || command[index] == DEL) && index > 0) {
                command[--index] = 0;
            }
            else if (command[index] == CR) {
                command[++index] = 0;
                printf("\n");
                break;
            }
            else
                index++;
        }

        // Get thing to do the command on
        index = 0;
        while(command[index++] != ' ');
        uint8_t button = atoi(&command[index]);

        // Parse commands
        switch(command[0]) {
            case 'r':
                printf("Recording signal for button %d\n", button);
                record_signal(&buttons[button]);
                break;

            case 'p':
                if (!buttons[button].last) {
                    printf("Error: button signal has not been recorded");
                    continue;
                }

                printf("Playing signal for button %d\n", button);
                play_signal(&buttons[button]);
                break;

            case 'd':
                printf("Displaying signal for button %d\n", button);
                print_signal(&buttons[button]);
                break;

            default:
                // do nothing
                break;
            
        }

        sleep_ms(1000);
    }
}