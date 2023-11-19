#include "signal.h"

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

        print_signal(button);
}

void print_signal(button_t *button) {
    for (int i = 0; i < button->last; i+=2)
        printf("%d: val %d, length %d\n", i/2, button->signal[i], button->signal[i+1]);
}