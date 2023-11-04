#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

// Followung the dma_capture example from pico-examples

#define INTERVAL 250
#define CONVERT (3.3f / (1<<12))
#define BUFFER_SIZE 1000
#define THRESHHOLD 3.0

uint8_t capture_buffer[BUFFER_SIZE];

int main() {
    // Init peripherals
    stdio_init_all();
    adc_init();

    // init ADC pin
    adc_gpio_init(26);  // silkscreen label A0
    adc_select_input(0);

    adc_fifo_setup(
        true,   // write each conversion to sample FIFO
        true,   // enable dma data request
        1,      // DREQ (and IRQ) asserted when at least 1 sample present
        false,  // won't see err bit so disable it
        true    // shift each sample to 8 bits when pushing to FIFO
    );

    adc_set_clkdiv(0);

    uint dma_chan = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_chan);

    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);

    channel_config_set_dreq(&cfg, DREQ_ADC);

    dma_channel_configure(dma_chan, &cfg,
        capture_buffer,
        &adc_hw->fifo,
        BUFFER_SIZE,
        true    
    );

    // Loop start
    while(1) {
        // Wait for start condition
        while (adc_read()*CONVERT > THRESHHOLD);

        // Clear previous output
        printf("\033[1;1H\033[2J");
        printf("Starting capture on falling edge");

        // Start capture
        adc_run(true);

        // Wait for capture to complete
        dma_channel_wait_for_finish_blocking(dma_chan);
        adc_run(false);

        // Cleanup
        printf("Capture finished");
        adc_fifo_drain();

        // Print capture data to terminal
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%f, ", capture_buffer[i]*CONVERT);
            if (i % 10 == 9) printf("\n");
        }
    }
}
