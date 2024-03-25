# Remote
The BLE portion is complete, where it advertises a bluetooth connection, and receives command strings over it. It forwards these command strings to the pico over I2C afterwards. This is the stage where a checksum, if necessary, will be implemented. Through testing so far, there haven't been issues with sending arbitrary amounts of data through this communication pipeline, however, I also haven't tested this for a variety of ranges/enough times to be able to say whether or not some form of error checking is necessary.

The pico application is also pretty much done now as far as recording signals, receiving communications with both the ML MCU and BLE board, and most functionality has been implemted, i.e. the commands for calibrating specific buttons, replaying certain buttons, some quality of life commands that were added, and setup for a calibration command that steps through with the app to record all necessary buttons, as well as schedule configuration has been implemented. The signals are done the same way as before, however, the DAC has been optimized out as the signals are just high or low, there is no need to be able to write a range of voltages to the emmitter, so the latency involved with the DAC is now gone.

## Bill of materials
| Componenet              	| Count 	| Sourced from 	| Notes                                                                                           	|
|-------------------------	|-------	|--------------	|-------------------------------------------------------------------------------------------------	|
| TSOP4838                	| 1     	| Mouser       	|                                                                                                 	|
| SIR-56ST3F              	| 1     	| Mouser       	|                                                                                                 	|
| Adafruit Feather RP2040 	| 1     	| Adafruit     	| Any RP2040 will work, though the defines for each pin in the firmware will have to be modified and the Building/Running instructions assume this board. 	|
| Adafruit Feather NRF52840 |1      | Adafruit | The code related to this board was built using ArduinoIDE, so the libraries used specify this specefic board. |   	|
| 1 $\mathsf{k\Omega}$ Resistor     	| 3     	| Any          	|    |            
| Push button | 1    | Any | This was simply used to simulate communication from the ML MCU and is unnecessary in the final build |

## Construction of the circuit
The layout and connections of the circuit are shown as follows, where only the necessary IO pins and connections are shown for this module: \
![Circuit](screenshots/circuit.JPG)

Note that the I2C Dac is gone, and the NRF52840 is now the master of the I2C bus. Also note that the NRF52840 requires external pullup resistors for the I2C bus.

## Building/Running
This assumes that the circuit has been constructed and the RP2040 has been connected to your computer.  Furhter, this also assumes the same for the NRF52840 and that the ArduinoIDE [setup](https://learn.adafruit.com/introducing-the-adafruit-nrf52840-feather/arduino-bsp-setup) guide has also been followed\
To build the remote module, first clone the repository into a linux system (WSL works fine). You need to have cloned the pico-sdk somewhere else on your system as well. Export the path to the pico-sdk directory, e.g. `export PICO_SDK_PATH=~/pico/pico-sdk` so that cmake can generate the correct paths for dependencies. Change directories to the `TVMaestro/remote/build` directory or make it if it does not yet exist, then type `cmake ..` to generate the build scripts. Type `make` to build the project, note that this will generate a `remote.uf2` binary that we will now copy to the RP2040 board. You can set the feather to bootloader mode by holding reset and the 'BOOTSEL' button at the same time, and releasing the reset button first. A USB device should show up on your system and you can simply drag the .uf2 to the device. \
Furthermore, to build the BLE code and upload it to the NRF52840 feather, assuming the NRF52840 development guide with ArduinoIDE has been followed, you should simply be able to open the `controller` files in ArduinoIDE and compile/upload them to the board. I have found that this board has an error where sometimes when uploading to the board it will return some DFU error when updating the firmware. If this happens, simply put the board into the bootloader write mode by pressing the reset button twice quickly (if it is in bootloader mode, the neopixel LED will be green), update the COM port in the IDE to be the new NRF52840 port, and upload again.

## Commands
| Command | Arguements | Explanation |
|-------------------------	|-------	|--------------	|
| !record | button index | Overwrite the button signal |
| !play | button index | Transmit the signal of the button |
| !display | button index | Print internal representation of the button's signal |
| !identify | N/A | Prints a string to USB |
| !calibrate | N/A | Initiates a calibration sequence where the user must press 0-9, power, then volume up followed by volume down to record each of the buttons |
| !schedule | schedule | Configure the module's schedule, where the scheudle is a space seperated list of channel numbers |
| !vol | +/- | Adjust the volume by sending this command follwed by + or - to adjust up or down respectivley |

To use the app, you can issue the commands listed above using a phone connected to the BLE board over bluetooth.

## Oscope screenshots of the module running
To prove that this actually works, below are a few screenshots of the signal from an oscilloscope, the signal being plotted in serial plot after being recorded, and the runs output of the signal:

### Signal as measured in waveforms
![Signal as measured in waveforms](screenshots/signal_waveforms.JPG)

### Signal as measured by the RP2040
![Signal as measured by the RP2040](screenshots/signal_sp.JPG)

### Signal runs compressed in the MCU
![Signal runs compressed in the MCU](screenshots/runs.JPG)

### Comparison of regenerated and original signals
![](screenshots/original-vs-recreated.JPG)

[Remote module release candidate demo link](https://youtu.be/ckK17YBWTac)

## Known issues
1. The ADC samples and toggling the GPIO pin are not synchronzied correctly when sending/receiving a signal. This causes a bit of variance that prevents the remote from working all of the time.
2. The current button setup for simulating signals from the ML MCU is not being debounced properly so there are times where repeated change channel events occur. Currently the interrupt is just being disabled for 100ms, but, I haven't had a chance to look at how long the button bounces for. This should not be an issue moving forward as a GPIO pin sending the signal from the ML MCU should not bounce as severly (or at all). 
