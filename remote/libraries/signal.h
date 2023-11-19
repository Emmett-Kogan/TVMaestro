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

typedef struct button_t {
    uint32_t ID;                    // Button ID, see button enum
    uint32_t signal[SIGNAL_SIZE];  // store the compressed signal
    uint8_t  last;                  // last valid spot in signal
} button_t;

// Button ID enum
enum {
    POWER = 0
};

void record_signal(button_t *button);
void print_signal(button_t *button);