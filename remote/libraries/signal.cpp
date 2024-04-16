#include "signal.h"

// Initializes ADC and GPIO
void signal_init(void) {
    // ADC init
    adc_init();
    adc_gpio_init(A2);
    adc_select_input(2);

    // GPIO init
    gpio_init(SIGNAL_PIN);
    gpio_set_dir(SIGNAL_PIN, GPIO_OUT);

    // Overdrive mode on GPIO pin (note that this increases power consumption)
    gpio_set_function(SIGNAL_PIN, GPIO_FUNC_SIO);
    gpio_set_slew_rate(SIGNAL_PIN, GPIO_SLEW_RATE_FAST);
    gpio_set_drive_strength(SIGNAL_PIN, GPIO_DRIVE_STRENGTH_12MA);
    gpio_set_input_enabled(SIGNAL_PIN, false);

    gpio_put(SIGNAL_PIN, 1);
    return;
}

void record_signal(button_t *button) {
    // Buffer to record_signal signal in
    uint16_t temp = 0;
    uint16_t buffer[BUFFER_SIZE];

    // wait for falling edge (start of transmission)
    while(adc_read()*CONVERT > THRESHHOLD);
    
    // Record signal
    while(temp < BUFFER_SIZE) {
        // only high and low values, nothing else matters
        buffer[temp++] = adc_read() > 100 ? 1 : 0;
    }

    uint32_t count = 0;
    uint8_t index = 0, prev = buffer[0];

    // Compress signal into runs
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] == prev && i != BUFFER_SIZE-1)
            count++;
        else {
            button->signal[index++] = prev;
            button->signal[index++] = count;
            count = 0;
            prev = buffer[i];
        }
    }

    // Set to index of last valid run
    button->last = index;
}

void print_signal(button_t *button) {
    for (int i = 0; i < button->last; i+=2)
        printf("%d: val %d, length %d\n", i/2, button->signal[i], 
                                               button->signal[i+1]);
}

void play_signal(button_t *button) {
    uint32_t *tmp = button->signal, bound = button->last; 
    uint8_t frac = 0;
    tmp[0] = 0;
    // For each run value and length, write the value and wait for the duration
    for (uint8_t i = 0; i < bound; i+=2) {
        gpio_put(SIGNAL_PIN, tmp[i]);
        sleep_us(tmp[i+1]*(2+frac));
        frac ^= 1;
    }
}
