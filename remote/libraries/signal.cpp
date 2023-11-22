#include "signal.h"

// Initializes ADC, I2C and external DAC
void signal_init(void) {
    // ADC init
    adc_init();
    adc_gpio_init(A0);  // silkscreen label A0
    adc_select_input(0);

    // I2C init
    i2c_init(i2c1, 1000*1000); 
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);
    
    // DAC init
    DAC_write(0x0FFF);

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
    uint8_t i, j;
    
    printf("last = %d\n", button->last);

    // For each run value and length, write the value and wait for the duration
    for (i = 0; i < button->last; i+=2) {
        DAC_write(button->signal[i] ? 0x0FFF : 0x000);
        sleep_us(button->signal[i+1]*4);
    }
}

void DAC_write(uint16_t v) {
    uint8_t transmission[3] = {MCP4725_WRITE_COMMAND, (uint8_t) (v & 0xFF00 >> 8), (uint8_t) (v & 0x00FF)};
    i2c_write_blocking(i2c1, MCP4725_ADDR, transmission, 3, false);
}