# Remote

## Goal
This part needs to essentially make a universal remote, that has a calibrate mode, and then a normal operation mode. The calibrate mode will effectivley be an IR receiver that records button presses that the user is instructed to do, which will be recorded and later used by the normal operation mode to interact with a TV. The normal operation mode will use a DAC to regurgitate these recorded transmissions with an IR emitter.

## Progress
So far, code for interacting with the rp2040's ADCs has been written and tested while interfacing with a TSOP4838 to record signals from a Roku stick. Typically, these signals need to be sampled at 38 kHz, so while a POC has been written in earlier commits, a version using the DMA to properly record a signal that can later be reused is in progress. The external DAC has also not arrived yet so interfacing with that hasn't been possible yet either (this component was ordered on Tuesday, 10/31 and was supposed to arrive by 11/3 but the UPS tracking information reports the package as being delayed).

The current circuit layout is just a TSOP4838 with it's ground and Vcc pins hooked up to the adafruit rp2040 feather's ground and Vcc pins, and the output pin is connected to the ADC0 channel's GPIO pin (A0). Furthermore, a DAD board oscilloscope channel is connected to the data out of the ADC so that the signal can be observed in waveforms as well.

## Known Bugs
The ADC is currently sampling too fast, so it needs to be properly configured to sample at 38kHz and not higher. I looked at the transmission with an oscilloscope and it took around 23ms, so with a sampling rate of 38kHz, around 900 samples need to be collected, so after a start condition collecting 1000 should be fine to reconstruct the signal later on.