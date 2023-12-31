# Remote
Currently the ADC portion of this is pretty much done as far as recording signals from the remote. For now it is simply recording the signal at 500ksps and saving it in a buffer, and then compressing that buffer into runs of high and low values to save space (about 20K*2 bytes vs 800 bytes). The compressed signal can be stored in signal structs that will later be used to reproduce the signals with the IR emitter. 

## Bill of materials
| Componenet              	| Count 	| Sourced from 	| Notes                                                                                           	|
|-------------------------	|-------	|--------------	|-------------------------------------------------------------------------------------------------	|
| TSOP4838                	| 1     	| Mouser       	|                                                                                                 	|
| SIR-56ST3F              	| 1     	| Mouser       	|                                                                                                 	|
| Adafruit Feather RP2040 	| 1     	| Adafruit     	| Any RP2040 will work, though the defines for each pin in the firmware will have to be modified and the Building/Running instructions assume this board. 	|
| MCP4725 Breakout Board  	| 1     	| Adafruit     	|                                                                                                 	|
| 1 $\mathsf{k\Omega}$ Resistor     	| 1     	| Any          	|                

## Construction of the circuit
The layout and connections of the circuit are shown as follows, where only the necessary IO pins and connections are shown for this module: \
![Circuit](screenshots/circuit.JPG)

## Building/Running
This assumes that the circuit has been constructed and the RP2040 has been connected to your computer. \
To build the remote module, first clone the repository into a linux system (WSL works fine). You need to have cloned the pico-sdk somewhere else on your system as well. Export the path to the pico-sdk directory, e.g. `export PICO_SDK_PATH=~/pico/pico-sdk` so that cmake can generate the correct paths for dependencies. Change directories to the `TVMaestro/remote/build` directory or make it if it does not yet exist, then type `cmake ..` to generate the build scripts. Type `make` to build the project, note that this will generate a `remote.uf2` binary that we will now copy to the RP2040 board. You can set the feather to bootloader mode by holding reset and the 'BOOTSEL' button at the same time, and releasing the reset button first. A USB device should show up on your system and you can simply drag the .uf2 to the device. \
To use the app there are two commands, `record` and `play` that you can send over a serial connection followed by a number identifier to record and playback signals of certain buttons, for instance, doing `record 0`, pointing a remote at the board and pressing the power button, then following this with `play 0` will replay the signal that was just recorded (the power button) and if aimed the corresponding TV, will turn it on or off.

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

## TODO
Working on fixing bugs as far as the consistency of the remote, further, I will be speaking with Blake about how he wants to do commands to the remote module, and possibly change how they are currently done. For now it is just over serial from a laptop but ultimatley an app talking to a different MCU over bluetooth will likely need to talk to this module for calibration and then a seperate thread of execution may need to invoke this module when running schedules.

## Known issues
I think the ADC needs to sample more often so that there is a better resolution for recording signals, so that replaying them works more consistently. It seems that some receivers are more forgiving than others.
