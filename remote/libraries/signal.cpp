#include "signal.h"

// Initializes ADC, I2C and external DAC
void signal_init(void) {
    // ADC init
    adc_init();
    adc_gpio_init(A0);  // silkscreen label A0
    adc_select_input(0);

    // I2C init

    // I'd like to configure this for fast mode plus I guess?
    // Will keep it in standard mode for now

    i2c_init(i2c_default, 100*1000);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);

    // DAC init
    // just make it output a test voltage
    // note that last 4 bits are don't cares
    sleep_ms(5000);
    printf("Starting I2C transaction\n");

    uint8_t transmission[3] = {MCP4725_WRITE_COMMAND, 0x00, 0x00};
    while(1) {
        printf("again\n");
        i2c_write_blocking(i2c_default, MCP4725_ADDR, transmission, 3, true);
        sleep_ms(500);
    }
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
        printf("%d: val %d, length %d\n", i/2, button->signal[i], button->signal[i+1]);
}

void play_signal(button_t *button) {
    // Do I2C writs to the DAC with shifted ADC values, so that it outputs 3.3V
    // while high and 0V when low for the signal, should only rewrite the thing
    // when it changes, so there should be some wait logic that uses the run
    // length value to determine how long to wait while high or low
}