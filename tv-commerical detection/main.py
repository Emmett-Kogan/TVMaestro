import cv2
import numpy as np
import subprocess
import time
import threading
import RPi.GPIO as GPIO
from collections import deque
from keras.models import load_model
from keras.applications.resnet50 import ResNet50, preprocess_input, decode_predictions

label = None
prediction = None
average = None
prediction_results = deque(maxlen=10)

model = load_model('commercial_detection_model.h5')

def execute_command(command):
    try:
        subprocess.run(command, shell=True, check=True)
        time.sleep(1)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        return False
    return True

def average_prediction(predictions):
    total = 0.0
    for prediction in predictions:
        total += prediction
    num_predictions = len(predictions)
    return (total / num_predictions)

def preprocess_frame(frame):
    img = cv2.resize(frame, (224, 224))
    return img

def predict_commercial(frame):
    global label
    global prediction
    global average
        
    processed_frame = preprocess_frame(frame)

    prediction = model.predict(np.expand_dims(processed_frame, axis=0))[0, 0]
    prediction_results.append(prediction)
    average = average_prediction(prediction_results)

    threshold = 0.5
    label = "Non-commercial" if prediction < threshold else "Commercial"
    prediction = f"Predicted: {prediction}"

def detect_commercial_transition():
    return (average > 0.5)

def main():
    execute_command('v4l2-ctl --set-edid=file=edid.txt --fix-edid-checksums')
    execute_command('v4l2-ctl --query-dv-timings')
    execute_command('v4l2-ctl -V')
    execute_command('v4l2-ctl --set-dv-bt-timings query')
    execute_command('v4l2-ctl -v pixelformat=UYVY')

    GPIO.setmode(GPIO.BCM)
    GPIO.setup(14, GPIO.OUT)

    cap = cv2.VideoCapture("/dev/video0")

    if not cap.isOpened():
        print("Error: Could not open video device.")
        return

    cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('U', 'Y', 'V', 'Y'))

    frame_count = 0
    while cap.isOpened():
        ret, frame = cap.read()

        if not ret:
            print("Error: Couldn't read frame.")
            break

        if prediction is not None:
            cv2.putText(frame, prediction, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            cv2.putText(frame, f"Avg: {average}", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            cv2.putText(frame, label, (10, 90), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
            if len(prediction_results) == prediction_results.maxlen and detect_commercial_transition():
                cv2.rectangle(frame, (300, 200), (frame.shape[1], 200), (214, 208, 34), -1)
                cv2.putText(frame, "Transition to TV commercial detected!", (770, 65), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2, cv2.LINE_AA)
                GPIO.output(14, GPIO.HIGH)
                print(f"GPIO pin 14 asserted (set to HIGH).")
            else:
                GPIO.output(14, GPIO.LOW)
                print(f"GPIO pin 14 reset (set to LOW).")  
        else:
            cv2.putText(frame, "Predicting...", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        
        cv2.imshow('Frame', frame)

        frame_count += 1

        if frame_count % 30 == 0:
            thread = threading.Thread(target=predict_commercial, args=(frame,))
            thread.start()

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    GPIO.cleanup()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()