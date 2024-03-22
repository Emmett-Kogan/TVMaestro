#include "../libraries/signal.h"
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <pico/i2c_slave.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#define MAX_BUTTONS 20
#define USED_BUTTONS 12
#define I2C_SLAVE_ADDRESS 0x42
#define SDA     0x02
#define SCL     0x03
#define BTN_PIN 13

// Buttons 0-9 will be the digits
#define POWER 10
#define VU 11
#define VD 12
// 13-19 will be reserved for furture use

// Commands
//     !record <button id>
//     !play <button id>
//     !display <button id>
//     !identify
    
//     setup for:
//     !calibrate
//     !schedule <schedule>
//     !vol <+/->

static button_t buttons[MAX_BUTTONS];
static uint8_t calibrared = 0;

// Communbication variables for commands
static char i2c_buffer[512];
static size_t i2c_index = 0;
static uint8_t i2c_flag = 0;

// Flag for when told to change channels by ML MCU
volatile uint8_t change_channel = 0;

// Schedule
std::vector<int> schedule;
static uint32_t current_channel = 0;

static void i2c_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    if (event == I2C_SLAVE_RECEIVE)
        i2c_buffer[i2c_index++] = i2c_read_byte_raw(i2c);
    else
        i2c_flag = 1;
}

static void button_handler(uint gpio, uint32_t events) {
    gpio_set_irq_enabled(BTN_PIN, GPIO_IRQ_EDGE_RISE, false);
    change_channel = 1;
}

int main() {
    // Init peripherals
    stdio_init_all();

    // I2C init
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);
    i2c_init(i2c1, 100000); 
    i2c_slave_init(i2c1, I2C_SLAVE_ADDRESS, &i2c_handler);

    // GPIO init
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE, true, &button_handler);
    
    // Init signal library
    signal_init();

    // If there is an SD card plugged in, try to load a schedule from it
    // If not a valid schedule, do not mark as calibrated
    // If a valid schedule, load into the schedule variable

    while(1) {
        while(!i2c_flag && !change_channel) {
            sleep_ms(50); // I have no idea why but this makes a difference
        }

        // Handle case for button press
        if (change_channel) {
            // Change the channel
            printf("Changing channel\n");
            
            // Need to change channel to whatever number in schedule[current_channel+1]
            uint32_t new_channel = schedule[(current_channel + 1) % schedule.size()];

            // Need to get each digit of new channel and send them in high to low order

            current_channel = current_channel + 1 % schedule.size();

            // Debaounce and cleanup
            gpio_set_irq_enabled(BTN_PIN, GPIO_IRQ_EDGE_RISE, true);
            sleep_ms(100);
            change_channel = 0;
            continue;
        }

        // Clear previous output
        printf("\033[1;1H\033[2J");
        for (int i = 0; i < i2c_index; i++)
            printf("%c", i2c_buffer[i]);
        
        // Only when specifying button do we care about this
        uint32_t index = 0, button;
        if (i2c_buffer[1] == 'r' || i2c_buffer[1] == 'p' || i2c_buffer[1] == 'd') {
            while (i2c_buffer[index++] != ' ');
            button = atoi(&i2c_buffer[index]);
            printf("Button specified: %d\n", button);
        }

        switch(i2c_buffer[1]) {
            case 'r':
                // Record a signal
                record_signal(&buttons[button]);
                printf("Button %d recorded\n", button);
                break;
            case 'p':
                // Play a signal
                if (!buttons[button].last)
                    printf("Error: button signal has not been recorded");
                else
                    play_signal(&buttons[button]);
                break;
            case 'd':
                // Display a signal
                print_signal(&buttons[button]);
                break;
            case 'i':
                // Identify
                printf("This is a TVMaestro!\n");
            case 'c':
            {
                // Calibrate
                for (int i = 0; i < USED_BUTTONS; i++)
                    record_signal(&buttons[i]);
                calibrared = 1;
                break;
            }
            case 's':
            {
                // Configure schedule variable
                schedule.clear();
                char *tmp = strtok(i2c_buffer, " ");
                while (1) {
                    tmp = strtok(NULL, " ");
                    if (!tmp) break;
                    schedule.push_back(atoi(tmp));
                }

                // Write new version of schedule to SD card

                break;
            }
            case 'v':
                // Adjust volume
                while(i2c_buffer[index++] != ' ');
                if (calibrared)
                    play_signal(&buttons[(i2c_buffer[index] == '+') ? VU : VD]);

                break;
            default:
                printf("Error: Invalid command");
        }

        // Reset buffer index and flag so we wait for the interrupt to be triggered again
        i2c_index = 0;
        i2c_flag = 0;
    }
}