import cv2
import numpy as np
from keras.models import load_model
from keras.preprocessing import image
from keras.applications.resnet50 import preprocess_input

model = load_model('commercial_detection_model.keras')

def preprocess_frame(frame):
    img = cv2.resize(frame, (224, 224))
    return img

video_path = '28 August 2023 9Go! commercial break (6).ia.mp4'
cap = cv2.VideoCapture(video_path)

if not cap.isOpened():
    print("Error opening video file")

frame_count = 0
while cap.isOpened():
    ret, frame = cap.read()

    if not ret:
        break

    if frame_count % 4 != 0:
       continue

    processed_frame = preprocess_frame(frame)

    prediction = model.predict(np.expand_dims(processed_frame, axis=0))[0, 0]

    threshold = 0.5
    label = "Commercial" if prediction < threshold else "Non-commercial"

    cv2.putText(frame, f"Predicted: {prediction}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv2.putText(frame, label, (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
    cv2.imshow('Frame', frame)
    cv2.waitKey(1)

    if label == "Commercial":
        print("Commercial detected at frame:", int(cap.get(cv2.CAP_PROP_POS_FRAMES)))

cap.release()
