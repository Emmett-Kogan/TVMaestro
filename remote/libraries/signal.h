#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// Defines
#define SIGNAL_SIZE 200
#define CONVERT (3.3f / (1<<12))
#define BUFFER_SIZE 16000
#define THRESHHOLD 2.0

// Feather pins (silkscreen labels)
#define A0      0x1A
#define A2      0x1C    
#define SIGNAL_PIN 12

typedef struct button_t {
    uint32_t signal[SIGNAL_SIZE];  // store the compressed signal
    uint32_t  last;                  // last valid spot in signal
} button_t;

// Initializes the necessary IO
void signal_init(void);

// Waits for a signal to start and then saves it to a buffer, then compresses
// the information in the buffer and stores that into the signal array
void record_signal(button_t *button);

// Prints the signal array to the terminal
void print_signal(button_t *button);

// Uses the GPIO Pin to replay a recorded signal
void play_signal(button_t *button);
