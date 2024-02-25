# Alpha Test Plan

# Remote Module
### Alpha Test Results
1. The end-to-end tests are somewhat working. The communication between all of the components of the remote module work well, i.e. the BLE communication with a phone, I2C communication between NRD52840 and RP2040, and the setup for the change channel signal. So far, I haven't figured out a better way to automate sending large blocks of data from my phone to the BLE board, however, this should be much easier to do with the app. As far as testing the commands that have been implemented, there have been no errors when issueing them from the bluetooth communications. Further, the I2C communication has also not encountered an error. That said, these also haven't been stress tested, the largest message propopgated through these paths has been arouond thirty bytes, where a schedule with five channels is around that length. The main issue has been with the IR signals as timing issues that were not present before have developed as a result of optimizing out the DAC, so slight adjustments need to be made to `play_signal` to resolve this.
2. To repeat, the blocks of data sent over bluetooth haven't had any issues yet, but this also hasn't been stress tested. Once the app can send an arbitrary amount of data, I'd like to send a packet that occupies the entire message length and see how often the BLE board receives a malformed packet. This test wasn't fully completed during the Beta sprint as there were still issues being reolved with porting over the remote module to the new board, which are now resolved.

### Expected Behavior
The remote module is expected to be able to listen for and record IR signals, and then reproduce them such that the reproduced signals behave the same way as the original ones when transmitted to the TV set. In the source code and previous doucments submitted, this functionality has been outlined and demonstrated with comparisons between an oscope measuring the original signal and the reproduced output also on an oscope, as well as the original signals data being displayed using SerialPlot to demonstrate consistency throughout the various steps of the program.
Furthermore, we also need to be testing the bluetooth component of the remote module now to measure the consistency of the messages to determine what kind of checksum/redundancy scheme we need to use when sending/receiving schedules. The expectation is that they can be received correctly from a reasonable distance at a highly consistent rate.

### Test Procedures
1. In order to demonstrate that the reproduced signals are of high enough quality to work with the TV set, all we need to do is perform a functional test on the remote module by simply issueing it commands over UART to tell it when to record, and when to playback certain signals. Depending on the results of the test, the IR component of the remote module may be done, or, it may need to be tuned to take more samples/record the original signal at a higher resolution so that the reproduced signals is of a higher quality.
2. To test the bluetooth component of the module, we will send blocks of data, of at least the same size as what a schedule ultimatley be, and checking on the receiving end for differences between the data sent and what it originally was, and producing data that suggests how consistent the bluetooth communications fully work. This will inform what kind of checking/redundancy scheme we need to build on top just the basic transmitting and receiving functionality, e.g., sending an additional hash or checksum, and making a mechanism for the remote module to request the app to resend a communication if it originally failed.
3. An end-to-end test that involves calibrating the remote, setting a schedule, and actually changing channels should be completed. This would involve sending it various commands from a phone, followed by either simulating change channel signals from the ML MCU using a button, or actually getting those signals from the ML MCU depending on its progress. This will specifically be testing schedule/calibration functionality, but it will also require the previous tests to be passed in order to complete.

# TV Commercial Detection

### Alpha Test Results
### Expected Behavior
The machine learning model for performing TV commercial detection is slated to be run on a Raspberry Pi 4B. The microcomputer is installed with an HDMI input from Geekworm, the C790, that recognizes HDMI sources and captures the video feed on which the machine learning model will make its detection. The additional hardware is ongoing kernel driver installation and presently recognizes HDMI feed. Since the Raspberry Pi 4B provides an HDMI source of its own, the video feed can be passed through for seemless interruption of TV broadcasting. The result of the model will be transmitted to the microcontroller via a communication protocol like UART. The model 
will send for each sentence received whether it is an ad or not an ad. If a certain(undecided) number of hits are sent to the microcontroller in a certain(undecided) segment of time, the microcontroller will communicate to the remote module to send a signal.

### Test Procedures
1. Transcriptions of ads and non ads will be fed to the model, and the model will either mark the transcription as an ad or not an ad and will be accurate to reality.
2. If we decide to pursue image classification, a similar test will be conducted with advertisement images and non-advertisement images.

# Mobile Application

### Alpha Test Results
### Expected Behavior
The mobile application is expected to be able to discover nearby bluetooth low energy devices, or more specifically, our microcontroller. It should then be able to show that the microcontroller is available to connect to. The device should then be able to be selected, and then a connection update message should be displayed to the user.

### Test Procedures
1. To test the bluetooth module on the mobile application is working we must first navigate to the pairing screen and select the pair option, and confirm that the microcontroller is being shown as avialable for connection. In the event that the microcontroller is not available, it should not be shown as a connection option.
2. To test pairing, we should select the microcontroller, and ensure that the appropriate pairing message is displayed. In the negative test case, an error message should be displayed.
3. Ensure that connection is continued via the displayed connection status. In the case that connection is broken, such as a power down or out of range situation, the status should update.
