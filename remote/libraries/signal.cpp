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
    return;
}

void record_signal(button_t *button) {
        // wait for falling edge (start of transmission)
        while(adc_read()*CONVERT > THRESHHOLD);
        
        // Buffer to record_signal signal in
        uint16_t temp = 0;
        uint16_t buffer[BUFFER_SIZE];

        // Record signal
        while(temp < BUFFER_SIZE) {
            // only high and low values, nothing else matters
            buffer[temp++] = adc_read() > 100 ? 255 : 0;
            sleep_us(2);        // adjust this to re-adjust sampling frequency
        }

        uint32_t count = 0;
        uint8_t index, prev;
        prev = buffer[0];

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
    // For each run value and length, write the value and wait for the duration
    for (uint8_t i = 0; i < button->last; i+=2) {
        gpio_put(SIGNAL_PIN, button->signal[i] ? 1 : 0);
        sleep_us(button->signal[i+1]*4);
    }
}
