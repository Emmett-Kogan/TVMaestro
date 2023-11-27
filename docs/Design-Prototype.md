# TVMaestro: Design Prototype

## Time Breakdown
The timesheet's of each member are located [here](https://docs.google.com/spreadsheets/d/1YhgjYHVoKcEeV9-Mo8SlOOsEO-r5k9PXPGEjSaAOhO4/edit#gid=0) and reflect on the time spent on research, and developing interfaces and systems throughout the milestone.

## Building and Running
Remote Module: See the README file in the remote directory for documentation on components required, construction of the circuits, and how to build and run the firmware.\
Companion App: Locate the README file in the CompanionApp directory for information on how to build and run the app.

## Evidence of Soundness
Our model was tested through human verification. A dataset of images of many different classes including images that are advertisements were run through our model. At the moment our model is correctly marking the advertisement images as ads, but it is also tagging images that are clearly not ads as ads. The natural interpretation is that our datasets for training the model are not robust enough to single out only advertisement images as ads. We need to improve upon these datasets. On 11/26/2023, a dataset of ~65,000 advertisement images, and an 11.2 GB nonAds dataset was found here: https://people.cs.pitt.edu/~kovashka/ads/readme_images.txt
### Theoretical Background
The theoretical background on the ML side is a coursera course by Andrew Ng, a former ML professor at Stanford. This course gives a lot of valuable knowledge on machine learning and gives practical problems to students to build different kinds of models.
### Prior Art
There are several instances of machine learning being used to classify advertisements. NSF CISE CRII: RI: Automatically Understanding the Messages and Goals of Visual Media (Award #1566270) is a study that attempted to do exactly that with a more complex goal of "automatically answering questions about the messages of the ads". The dataset that this work used is the one listed in the Evidence of Soundness section which we intend to use, and is allowed to be used if we properly cite.
## External Interfaces
### Presentation
The external interface for the TVMaestro consists of a remote and a companion android application. The remote module is a tactile interface as it includes buttons for programming options. The app is a graphical interface that allows users to view bluetooth pairing/connectivity options, as well as schedule options and the current state of these options. 

### Perception
Feedback from the TVMaestro will be given through the application. The app visually will change based on what the user interacts with. For instance, upon opening the application a user is prompted to connect to their TVMaestro via bluetooth. Once this connection is established the app shows a connected message and allows the user to begin assigning schedules on the schedule page.

### Usability
The prototype companion app acts as expected, as it shows that the user should connect to a TVMaestro to begin scheduling. Additionally, the scheduling page allows for selection/creation of schedules. None of these exhibit random behavior through testing.
The remote module of the prototype behaves predictably, as during normal operation, the user can use the commands record or play followed by a button ID to either trigger calibrating a button or playing back the signal with no randomness.

## Internal Systems
### Componenet Architectures
The architecture of the remote module is described in the directory's README.md, further, a schematic of the final circuit is also listed. In the building/running section, it outlines the method of running the code as well as lists the required circuit to run it properly. The companion application is a android app that is currently being run through Android Studio's native testing/simulation tools. The app can also be ran on a android device with developer options enabled. Instructions on how to run this are in the README.md of the CompanionApp directory. The ML model uses the tensorflow library to train, build, and test the model. Instructions to work with the model is in the buildModel.md in the tv-commercial detection section.

### Communication Mechansims
The TVMaestro will communicate with TV's via the remote module, which uses IR to issue commands to the TV just like any other remote. It first calibrates by acting as an IR receiver from the actual remote to get signals to retransmit when told to by other parts of the project (e.g. it receives a signal to change the channel from the ML model so it sends the requisite signals to change the channel to the next in line in the schedule). Once it is in scheudle mode it will actually use the recorded signals to modfiy the state of the TV. There will be internal communication to faciliate this, e.g. the ML model and app components will need to calibrate and issue commands to the remote module throughout operation. A demonstration of the remote module can be found in the remote/README.md, where a power on signal was recorded using an ADC, and regurgitated using the MCP4725 DAC, which is then shown with an oscope and fed into a diode that emits the message.

### Resilience
As far as the remote module of TVMaestro, error correction is not critical to demonstrate it's functionality, however, later in the project, we can use the ML model to produce feedback to the remote module on whether or not certain signals are working, which should then prompt the user to recalibrate the remote (either entirely or perhaps just the signals that were failing). As the remote module is the slave device, it can be commanded to recalibrate any time with additional `record n` commands where n is the ID of the button that needs to be recalibarted. Furthermore, in the pre-alpha build it was discussed for persistent state that an SD card would be used to preserve data across power cycles, and while this has not been implemented yet, using checksums to validate the data on the SD card and fetched off of it during operation will be necessary as well. ML error correction will be an iterative process, if the model is sending incorrect messages on when an ad is occurring there could possibly be a button on the remote that can be pressed to say that the model has sent the wrong message.
