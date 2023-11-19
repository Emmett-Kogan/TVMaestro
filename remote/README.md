# Remote
Currently the ADC portion of this is pretty much done as far as recording signals from the remote. For now it is simply recording the signal at 500ksps and saving it in a buffer, and then compressing that buffer into runs of high and low values to save space (about 20K*2 bytes vs 800 bytes). The compressed signal can be stored in signal structs that will later be used to reproduce the signals with the IR emitter. To prove that this actually works, below are a few screenshots of the signal from an oscilloscope, the signal being plotted in serial plot after being recorded, and the runs output of the signal:

### Signal as measured in waveforms
![Signal as measured in waveforms](screenshots/signal_waveforms.JPG)

### Signal as measured by the RP2040
![Signal as measured by the RP2040](screenshots/signal_sp.JPG)

### Signal runs compressed in the MCU
![Signal runs compressed in the MCU](screenshots/runs.JPG)

## TODO
I still need to reproduce a signal using the DAC, and make it turn on/off my TV, and that is what I am working towards, but first I'm going to be refactoring the ADC code so that it saves the compressed data into structs, and make a button struct that can be used later by the DAC code. To be honest, I think one ADC and one DAC function to record and then emit the signal should be fine, and later when I port these to the final MCU it will simply be a matter of calling these functions in a sequence with certain buttons as per the schedule.

## Known issues
The ADC probably does need to sample as fast as it is now, so I'll experiment with reducing the sampling rate once I get the transmissions working, and back down the sampling frequency until it barely works to save as much memory as possible, as this segment of the project is hardly the one that will be using the most.