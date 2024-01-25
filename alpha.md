# Pre-Alpha Build Report

### Repository Link
1. https://github.com/Emmett-Kogan/TVMaestro

### Time stamped evidence
1. Repo with issues/commit history: https://github.com/Emmett-Kogan/TVMaestro
2. Timesheet: https://docs.google.com/spreadsheets/d/1YhgjYHVoKcEeV9-Mo8SlOOsEO-r5k9PXPGEjSaAOhO4/edit#gid=0

## Usability

### Interface
### Navigation
### Perception
### Responsiveness

## Build Quality

### Robustness
### Consistency
### Aesthetic Rigor

## Vertical Features

### External Interface
The external interface of the remote module has been ported onto the Adafruit NRF52840 feather board. For now, it was implemented just using Arduino IDE and, should work exactly the same as it did in the previous implementation, however, it has not been exhaustivley tested yet. While the new implementation will eventually use a uSD card to store schedule information across power cycles, the driver for the nrf52840 still needs to be completed, as playing with bluetooth communication between a phone and the board was a higher priority for this sprint.

### Persistent State
As far as the remote module, the major use case for persistent state would be storing schedule information. We have components to interface with a uSD card to accomplish this, but, during this sprint, we focused on getting bluetooth connection between the MCU and app over this, but, once we standardize how a scheudle will be stored, writing this externally and reading this upon reset should be trivial.

### Internal Systems
For the primary remote use case, the recording and replaying action is close to finished, where a stream of input data is being compressed and reproduced using an ADC and a GPIO pin (I took another look at the ADC input and since it was binary I just hooked up a GPIO pin to the emitter instead of using a DAC). Further, we've sent messages between the nrf52840 and Adafruit's Bluefruit, so the main work for getting bluetooth communication working before implementing each set of commands for the app and MCU is just getting the app to talk to the MCU the same way Bluefruit does.