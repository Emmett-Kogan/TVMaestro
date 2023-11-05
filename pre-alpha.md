# Pre-Alpha Build Report

### Repository Link
1. https://github.com/Emmett-Kogan/TVMaestro

### Time stamped evidence
1. Repo with issues/commit history: https://github.com/Emmett-Kogan/TVMaestro
2. Emmett's timesheet: https://docs.google.com/spreadsheets/d/1YhgjYHVoKcEeV9-Mo8SlOOsEO-r5k9PXPGEjSaAOhO4/edit#gid=0


## Architectural Elements

### External Interface
### Persistent State
Regarding persistent state, clearly preserving the button calibration across power cycles is ideal. When I ordered the DAC breakout board for prototyping I also purchased a microSD board for a seperate project, however, I think it could also be useful for developing this one, as storing the transmission data onto that with a basic metadata structure with pointers and bounds related to each transmission's recording, as well as some identifying information on what button's it corresponds to should be sufficient for preserving the calibration. Furthermore, the same hardware could be used to preserve schedules locally on the embedded device so that user's do not need to use their phones/the extenral app to reconfigure their schedules/preferences on the device as well, just in a seperate memory segment. The bounds of each segment will be easiest to determine once we have a better idea of how many button transmissions we need to record, and the size of the data structure necessary to keep track of them, and, an firmer concept on how we will track and store schedules in the same address space.

### Internal Systems
1. The calibration mode feature requires the ability to record IR transmissions sent from the remote to the television. At this point, proof of concept code has been written that can record any transmission, however, the sampling rate needs to be properly configured so that the number of samples collected accuratley records the entire signal to be repeated in the regular operation mode. So the ADCs have been interfaced with, and functionality to handle sampling with a DMA channel.
2. An internal buffer has been used to store the transmission sample raw data, that will later be used for a different dma transfer which will interface with the DAC and an IR emitter to then transmit the recorded transmission to replace the functionality provided by the remote.
3. This functionality will be packaged in a library once it is functional.

I'd like to note that there were several failed iterations of the ADC drivers before they worked properly, as well as a significant amount of experimentation and research that went into selecting the componenets used, understanding how to use them and their documentation that went into this work. This is why the current iteration is using a DMA channel as when I started this I did not realize the frequency that I would need to sample at, and how I would accomplsih this.

## Information Handling

### Communication
1. For now, the adafruit rp2040 feather is being used to interface with the TV remote that it will be replcaing in the calibration phase/mode, and then interfaceing with the TV and either a different MCU running the model discriminating between advertisements and broadcast footage, or the functionality in the rp2040 will need to be ported over to whatever other MCU we end up using (probably a beaglebone). Either way, ADCs are common peripherals, we will already have a good enough DAC breakout board by then for prototyping, and the same IR emitters/receivers will be sufficient.

### Integrity and Resilience