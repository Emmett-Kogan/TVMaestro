import os
import cv2
import numpy as np
from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D, Flatten, Dense
from keras.preprocessing.image import ImageDataGenerator
from sklearn.model_selection import train_test_split

dataset_folder = 'dataset'
commercial_folder = os.path.join(dataset_folder, 'commercial')
non_commercial_folder = os.path.join(dataset_folder, 'non-commercial')
image_size = (224, 224)
batch_size = 32

model = Sequential()
model.add(Conv2D(32, (3, 3), input_shape=(image_size[0], image_size[1], 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dense(1, activation='sigmoid'))

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

commercial_frames = []
for filename in os.listdir(commercial_folder):
    if filename.endswith(".mp4"):
        video_path = os.path.join(commercial_folder, filename)
        cap = cv2.VideoCapture(video_path)
        ret, frame = cap.read()
        if ret:
            commercial_frames.append(cv2.resize(frame, image_size))
        cap.release()

non_commercial_frames = []
for filename in os.listdir(non_commercial_folder):
    if filename.endswith(".jpg"):
        image_path = os.path.join(non_commercial_folder, filename)
        frame = cv2.imread(image_path)
        if frame is not None:
            non_commercial_frames.append(cv2.resize(frame, image_size))


commercial_labels = np.ones(len(commercial_frames)) # (1 for commercial)
non_commercial_labels = np.zeros(len(non_commercial_frames)) # (0 for non-commercial)

X = np.array(commercial_frames + non_commercial_frames)
y = np.concatenate([commercial_labels, non_commercial_labels])

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

datagen = ImageDataGenerator(rescale=1./255)

train_generator = datagen.flow(X_train, y_train, batch_size=batch_size)
test_generator = datagen.flow(X_test, y_test, batch_size=batch_size)

model.fit_generator(
    train_generator,
    steps_per_epoch=len(X_train) // batch_size,
    epochs=10,
    validation_data=test_generator,
    validation_steps=len(X_test) // batch_size
)

model.save('commercial_detection_model.keras')