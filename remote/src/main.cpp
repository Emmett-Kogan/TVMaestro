#include "../libraries/signal.h"
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <pico/i2c_slave.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/flash.h"

// Macros
#define BIT(n) (0x1<<n)

// App related defines
#define MAX_BUTTONS 20
#define MAX_CHANNELS 100
#define USED_BUTTONS 12

// Communication defines
#define I2C_SLAVE_ADDRESS 0x42
#define SDA     0x02
#define SCL     0x03
#define BTN_PIN 13

// Flash defines
#define FLASH_TARGET_OFFSET (256 * 1024)
#define FLASH_TARGET_CONTENTS ((const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET))

// Global data structure defines
#define GDSB_SIZE (sizeof(button_t)*MAX_BUTTONS + sizeof(uint32_t)*(MAX_CHANNELS+1+1+1))
#define BUTTON_OFFSET 0
#define SCHEDULE_OFFSET (sizeof(button_t)*MAX_BUTTONS)
#define CHANNEL_COUNT_OFFSET (SCHEDULE_OFFSET+(sizeof(uint32_t)*MAX_CHANNELS))
#define CURRENT_CHANNEL_OFFSET (CHANNEL_COUNT_OFFSET+4)
#define VALID_FLAGS_OFFSET (CURRENT_CHANNEL_OFFSET+4)

// Buttons 0-9 will be the digits
#define POWER 10
#define VU 11
#define VD 12
// 13-19 will be reserved for furture use

void copy_config_to_flash(void);
void copy_config_from_flash(void);
void change_channel(uint32_t channel);

// Global data structures (these start at 0x20001234, and are 16,604 bytes total)
// This is just an easier way to guarantee the placement since I don't feel like refactoring everything else to use volatile
static uint8_t global_data_structures_buffer[GDSB_SIZE];
button_t *const buttons = (button_t *const) &global_data_structures_buffer[BUTTON_OFFSET];
uint32_t *const schedule = (uint32_t *const) &global_data_structures_buffer[SCHEDULE_OFFSET];
uint32_t *const channel_count = (uint32_t *const) &global_data_structures_buffer[CHANNEL_COUNT_OFFSET];
uint32_t *const current_channel = (uint32_t *const) &global_data_structures_buffer[CURRENT_CHANNEL_OFFSET];
uint32_t *const valid_flags = (uint32_t *const) &global_data_structures_buffer[VALID_FLAGS_OFFSET];
// Valid flags: BIT0 -> calibration, BIT1 -> schedule

// Communication variables
static char i2c_buffer[512];
static size_t i2c_index = 0;
static uint8_t i2c_flag = 0;
volatile uint8_t change_channel_flag = 0;

static void i2c_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    if (event == I2C_SLAVE_RECEIVE)
        i2c_buffer[i2c_index++] = i2c_read_byte_raw(i2c);
    else
        i2c_flag = 1;
}

static void button_handler(uint gpio, uint32_t events) {
    gpio_set_irq_enabled(BTN_PIN, GPIO_IRQ_EDGE_RISE, false);
    change_channel_flag = 1;
}

int main() {
    // Init peripherals
    stdio_init_all();

    // GPIO init
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE, true, &button_handler);

    // I2C init
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);
    i2c_init(i2c1, 100000); 
    i2c_slave_init(i2c1, I2C_SLAVE_ADDRESS, &i2c_handler);

    // Init signal library
    signal_init();

    // Grab whatever is in flash, it may be old config data, or it could be garbage
    copy_config_from_flash();

    while(1) {
        while(!i2c_flag && !change_channel) {
            sleep_ms(50); // I have no idea why but this makes a difference
        }

        // Handle case for button press
        if (change_channel_flag) {
            // Change the channel
            change_channel(schedule[(*current_channel) + 1 % (*channel_count)]);
            *current_channel = ((*current_channel)+1) % (*channel_count);     

            // Debaounce and cleanup
            gpio_set_irq_enabled(BTN_PIN, GPIO_IRQ_EDGE_RISE, true);
            sleep_ms(100);
            change_channel_flag = 0;
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
                break;
            case 'z':   // this is for debug/demo purposes
                // 
                if (!(*valid_flags & BIT(1)))
                    break;
                for (int i = 0; i < *channel_count; i++)
                    printf("Channel %d:\t%d\n", i, schedule[i]);
                break;
            case 'c':
            {
                // Calibrate
                for (int i = 0; i < USED_BUTTONS; i++)
                    record_signal(&buttons[i]);
                *(valid_flags) |= BIT(0);

                // Update flash
                copy_config_to_flash();

                break;
            }
            case 's':
            {
                // Configure schedule variable
                *current_channel = 0;
                *channel_count = 0;
                char *tmp = strtok(i2c_buffer, " ");
                while (1) {
                    tmp = strtok(NULL, " ");
                    if (!tmp) break;
                    schedule[*channel_count] = atoi(tmp);
                    *channel_count += 1;
                }
                *(valid_flags) |= BIT(1);

                // Write new version of schedule to flash
                copy_config_to_flash();

                // Go to new current channel
                change_channel(schedule[(*current_channel)]);

                break;
            }
            case 'v':
                // Adjust volume
                while(i2c_buffer[index++] != ' ');
                if (*valid_flags & BIT(0))
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

// Every time either the calibration or scheudle changes, write them to flash
void copy_config_to_flash(void) {
    flash_range_erase(FLASH_TARGET_OFFSET, GDSB_SIZE);
    flash_range_program(FLASH_TARGET_OFFSET, global_data_structures_buffer, GDSB_SIZE);
}

// On power up, load the existing calibration or schedule from flash if present
void copy_config_from_flash(void) {
    for (int i = 0; i < GDSB_SIZE; i++)
        global_data_structures_buffer[i] = FLASH_TARGET_CONTENTS[i];
}

// Will only handle the signal calls to change the channel
void change_channel(uint32_t channel) {
    printf("Change channel called!\n");


    return;
}
