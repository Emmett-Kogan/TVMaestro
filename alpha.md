# Alpha Build Report

### Repository Link
1. https://github.com/Emmett-Kogan/TVMaestro

### Time stamped evidence
1. Repo with issues/commit history: https://github.com/Emmett-Kogan/TVMaestro
2. Timesheet: https://docs.google.com/spreadsheets/d/1YhgjYHVoKcEeV9-Mo8SlOOsEO-r5k9PXPGEjSaAOhO4/edit#gid=0

## Usability

### Interface
In terms of the UI for the mobile application, a user can access available devices to pair to via BLE as well as the options for scheduling such as selecting channel schedules or enabling/disabling add detections.
### Navigation
Current navigation through the mobile application is very intuitive. The two different activities (Home and Schedule) selectable on the bottom naviagtion bar allow users to quickly swap between setup and running options which are displayed to the user upon selection of the particular activity.
### Perception
Changes within the mobile applications UI are indicated visually with either a popup window or by displaying the currently selected option. A currently selected option is located adjacent to its respective selection menu for clarity sake. 
### Responsiveness
Currently the application is not having to wait for any responses, however in the future when this is implemented, these types of processes will be pushed to the forefront of currently occuring operations.
## Build Quality

### Robustness
During regular use, a user will not cause the mobile application to crash.
### Consistency
In terms of the ad detection feature, we are still trying to build an acceptable ad-detecting model. During this sprint, it was a goal to try to perform text classification text from television ads and regular television programs. At the point of submission, text has been gathered and separated, but the model has not yet been built.
### Aesthetic Rigor
The remote can be tested through the Arduino IDE. Assets, in terms of text and images are accessible.
## Vertical Features

### External Interface
The external interface of the remote module has been ported onto the Adafruit NRF52840 feather board. For now, it was implemented just using Arduino IDE and, should work exactly the same as it did in the previous implementation, however, it has not been exhaustivley tested yet. While the new implementation will eventually use a uSD card to store schedule information across power cycles, the driver for the nrf52840 still needs to be completed, as playing with bluetooth communication between a phone and the board was a higher priority for this sprint.

### Persistent State
As far as the remote module, the major use case for persistent state would be storing schedule information. We have components to interface with a uSD card to accomplish this, but, during this sprint, we focused on getting bluetooth connection between the MCU and app over this, but, once we standardize how a scheudle will be stored, writing this externally and reading this upon reset should be trivial. Communications between the MCU and the model will need to be stored as well because there will probably be some kind of control/data flow for when to send certain signals in response to the model's results. 

### Internal Systems
For the primary remote use case, the recording and replaying action is close to finished, where a stream of input data is being compressed and reproduced using an ADC and a GPIO pin (I took another look at the ADC input and since it was binary I just hooked up a GPIO pin to the emitter instead of using a DAC). Further, we've sent messages between the nrf52840 and Adafruit's Bluefruit, so the main work for getting bluetooth communication working before implementing each set of commands for the app and MCU is just getting the app to talk to the MCU the same way Bluefruit does. In terms of the gathering data for the model, an HDMI capture card was used to record live television broadcast off of a Roku using OBS. The videos were converted into audio files and then transcribed using OpenAI's Whisper. Then the transcriptions were separated into ads sentences and non-ads sentences through human labeling.
