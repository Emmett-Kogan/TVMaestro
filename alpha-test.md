# Alpha Test Plan

## Expected Behavior

### Remote Module
The remote module is expected to be able to listen for and record IR signals, and then reproduce them such that the reproduced signals behave the same way as the original ones when transmitted to the TV set. In the source code and previous doucments submitted, this functionality has been outlined and demonstrated with comparisons between an oscope measuring the original signal and the reproduced output also on an oscope, as well as the original signals data being displayed using SerialPlot to demonstrate consistency throughout the various steps of the program.
Furthermore, we also need to be testing the bluetooth component of the remote module now to measure the consistency of the messages to determine what kind of checksum/redundancy scheme we need to use when sending/receiving schedules. The expectation is that they can be received correctly from a reasonable distance at a highly consistent rate.

### TV Commercial Detection
The machine learning model for performing TV commercial detection is slated to be run on a Raspberry Pi 4B. The microcomputer is installed with an HDMI input from Geekworm, the C790, that recognizes HDMI sources and captures the video feed on which the machine learning model will make its detection. The additional hardware is ongoing kernel driver installation and presently recognizes HDMI feed. Since the Raspberry Pi 4B provides an HDMI source of its own, the video feed can be passed through for seemless interruption of TV broadcasting. The result of the model will be transmitted to the microcontroller via a communication protocol like UART.

### Mobile Application
The mobile application is expected to be able to discover nearby bluetooth low energy devices, or more specifically, our microcontroller. It should then be able to show that the microcontroller is available to connect to. The device should then be able to be selected, and then a connection update message should be displayed to the user.

## Test Procedures

### Remote Module
1. In order to demonstrate that the reproduced signals are of high enough quality to work with the TV set, all we need to do is perform an end-to-end test on the remote module by simply issueing it commands over UART to tell it when to record, and when to playback certain signals. Depending on the results of the test, the IR component of the remote module may be done, or, it may need to be tuned to take more samples/record the original signal at a higher resolution so that the reproduced signals is of a higher quality.
2. To test the bluetooth component of the module, we will send blocks of data, of at least the same size as what a schedule ultimatley be, and checking on the receiving end for differences between the data sent and what it originally was, and producing data that suggests how consistent the bluetooth communications fully work. This will inform what kind of checking/redundancy scheme we need to build on top just the basic transmitting and receiving functionality, e.g., sending an additional hash or checksum, and making a mechanism for the remote module to request the app to resend a communication if it originally failed.

### Mobile Application
1. To test the bluetooth module on the mobile application is working we must first navigate to the pairing screen and select the pair option, and confirm that the microcontroller is being shown as avialable for connection. In the event that the microcontroller is not available, it should not be shown as a connection option.
2. To test pairing, we should select the microcontroller, and ensure that the appropriate pairing message is displayed. In the negative test case, an error message should be displayed.
3. Ensure that connection is continued via the displayed connection status. In the case that connection is broken, such as a power down or out of range situation, the status should update.
