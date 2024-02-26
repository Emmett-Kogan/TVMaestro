# Beta Build Report

### Repository Link
1. https://github.com/Emmett-Kogan/TVMaestro

### Time stamped evidence
1. Repo with issues/commit history: https://github.com/Emmett-Kogan/TVMaestro
2. Timesheet: https://docs.google.com/spreadsheets/d/1YhgjYHVoKcEeV9-Mo8SlOOsEO-r5k9PXPGEjSaAOhO4/edit#gid=0

## Usability

### Interface
In terms of the UI for the mobile application, a user can access available devices to pair to via BLE as well as the options for scheduling such as selecting channel schedules or enabling/disabling add detections, as well as options for controlling the hardware unit. The hardware options will be added in the near future. Below is some screenshots from an android device running the application with some of the relevant UI pages.
![Screenshot_20240225_203002_AndroidApp (1)](https://github.com/Emmett-Kogan/TVMaestro/assets/80291937/d455b137-35ef-4984-9da7-d94336fb88c1)
### Navigation
Current navigation through the mobile application is very intuitive. The two different activities (Home and Schedule) selectable on the bottom naviagtion bar allow users to quickly swap between setup and running options which are displayed to the user upon selection of the particular activity. This is depicted in the photo above.
### Perception
Changes within the mobile applications UI are indicated visually with either a popup window or by displaying the currently selected option. A currently selected option is located adjacent to its respective selection menu for clarity sake. In the below photos of the mobile application it is clear that the UI changes as a user selects their different options.
![Screenshot_20240225_203032_AndroidApp](https://github.com/Emmett-Kogan/TVMaestro/assets/80291937/679865f5-389c-4f0c-9ead-53cd40a8d52a)
![Screenshot_20240225_205158_AndroidApp](https://github.com/Emmett-Kogan/TVMaestro/assets/80291937/962a0291-096e-4335-a153-f3880010d4c2)

### Responsiveness
Currently the application has immediate response time in terms of navigation and selection options. When a user begins scanning for devices there is a required wait time to scan for devices. During this wait time a popup is displayed, notifiying the user that a scan is in progress. This is currently set to 5 seconds to enable a thorough scanning of nearby devices for connection. Connecting to a device will also require a waiting/loading screen as we will need to make sure the connection is secure before allowing the user to continue in the app. This will also utilize some sort of popup or loading animation until the connection is established as secure. Below is a screenshot from the mobile application showing some of the mentioned UI elements.
![Screenshot_20240225_203023_AndroidApp](https://github.com/Emmett-Kogan/TVMaestro/assets/80291937/f9e4d6d1-a3aa-4bee-8e11-f9fe535e5c29)
![Screenshot_20240225_203009_AndroidApp](https://github.com/Emmett-Kogan/TVMaestro/assets/80291937/4079ce52-a2d8-4087-a28b-9a74dab73927)
## Build Quality

### Robustness
During regular use, a user will find it hard to crash the application as the available options are relatively limited and usage of the Activity and Fragment models allow for realtive reliability. This is prevented as the app uses variables that are set and checked in order to allow the user to do things in the UI. When we have to begin validating if our BLE connections are secure or not, we will use tools within the android bluetooth API that allow for checking of connection state, and other possible error scenarios. This has been tested so far via mimicing what regular user behavior. 
### Consistency
In terms of the ad detection feature, we are still trying to build an acceptable ad-detecting model. During this sprint, it was a goal to try to perform text classification text from television ads and regular television programs. At the point of submission, text has been gathered and separated, but the model has not yet been built.
### Aesthetic Rigor
The remote can be tested through the Arduino IDE. Assets, in terms of text and images are accessible. In terms of the mobile application, a uniform aesthetic has been selected to keep the UI clear and concise (refer to the images in the Useability section). At the moment some of the popup's are not completely consistent with the total aesthetic (differing in color, size, or shape), and we plan to address this once development of other features has progressed more.
## Vertical Features

### External Interface
The remote module is now implemented using an NRF52840 to handle all bluetooth communication to a phone, which communicates with an RP2040 which handles any adc/signal processing and storing schedules. The BLE module was implemented using ArduinoIDE, and the pico module was implemented using the sdk in C. It is functionally similair to the pre-alpha build, but instead of communicating to a laptop over USB, the remote module advertises it's own bluetooth connection, and has a series of commands implemnted to configure and replay buttons, initiate a calibration sequence, and schedule configuration. The two MCUs communicate using I2C, as the NRF52840 doesn't actually have a secondary accessible UART, so the NRF52840 stores commands in a buffer and issues them to the pico. Eventually, the pico will also need to be signaled by the ML MCU for when to change channels, a GPIO pin interrupt has been implemented to cause a channel cahnge on a rising edge, so any MCU is capable of sending this signal, and for now, it is simply connected to a button for demo purposes. For more information on the current version of the remote module, please see the README in the corresponding directory.

### Persistent State
Because of the difficulties early in the sprint with trying to integrate all of the remote functionality on the NRF52840, a lot of time spent was not used effectivley regarding the current state of the project. So, a SPI driver to interface with the micro SD card reader and to store and load schedule information from an SD card has not been implmented yet. However, the schedule's internal data struture has been finalized, and the way a schedule is expected to be received from the app, has also been finalized. So storing either of those representations onto the SD card, adn then fetching it from the SD card on startup, and overwriting the current config on the SD card whenever a schedule is written to the remote module should be straightforward.

Communications between the MCU and the model will need to be stored as well because there will probably be some kind of control/data flow for when to send certain signals in response to the model's results. 

### Internal Systems
For the primary remote use case, the recording and replaying action is close to finished, where a stream of input data is being compressed and reproduced using an ADC and a GPIO pin (I took another look at the ADC input and since it was binary I just hooked up a GPIO pin to the emitter instead of using a DAC). This functionality will remain on the RP2040 for this project, however, future iterations should work to integrate this with the NRF52840 board to reduce the complexity of the remote module. Communication between a phone sending messages over bluetooth to all remote components has been finished, and while testing the performance and reliablity of these messages needs to be tested, the start to implementing a checksum on the messages has been implemented as well as a method of sending messages from the MCU back to the phone to request the previous command message again is in place. The remote module has also been setup for an internal signal to signify when to change channels that will come from the ML MCU. Again, for more information on the current state of the remote module, please see the corresponding README.

In terms of the gathering data for the model, an HDMI capture card was used to record live television broadcast off of a Roku using OBS. The videos were converted into audio files and then transcribed using OpenAI's Whisper. Then the transcriptions were separated into ads sentences and non-ads sentences through human labeling.
