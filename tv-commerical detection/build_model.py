import os
import cv2
import numpy as np
from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D, Flatten, Dense
from keras.preprocessing.image import ImageDataGenerator
from sklearn.model_selection import train_test_split
import concurrent.futures
import random

dataset_folder = 'dataset'
commercial_folder = os.path.join(dataset_folder, 'commercial')
non_commercial_folder = os.path.join(dataset_folder, 'non-commercial')
image_size = (224, 224)
batch_size = 12

model = Sequential()
model.add(Conv2D(32, (3, 3), input_shape=(image_size[0], image_size[1], 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dense(1, activation='sigmoid'))

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

# def frame_generator(folder, label, batch_size):
#     filenames = [filename for filename in os.listdir(folder) if filename.endswith(".mp4")]

#     batch_filenames = np.random.choice(filenames, batch_size, replace=False)
#     frames = []
#     labels = []
#     for filename in batch_filenames:
#         video_path = os.path.join(folder, filename)
#         cap = cv2.VideoCapture(video_path)
#         ret, frame = cap.read()
#         print(video_path)
#         num_frames = 200
#         while cap.isOpened():
#             ret, frame = cap.read()
#             if not ret or num_frames == 0:
#                 break
                    
#             processed_frame = preprocess_frame(frame)
#             frames.append(processed_frame)
#             labels.append(label)
#             num_frames = num_frames - 1
                
#         cap.release()
#     yield np.array(frames), to_categorical(labels, num_classes=2)

# # Function to preprocess a frame (replace with your actual preprocessing steps)
# def preprocess_frame(frame):
#     processed_frame = cv2.resize(frame, image_size)
#     processed_frame = processed_frame / 255.0  # Normalize pixel values to [0, 1]
#     return processed_frame

# # Load frames and labels from commercial and non-commercial folders
# commercial_frames, commercial_labels = next(frame_generator(commercial_folder, 1, batch_size))
# non_commercial_frames, non_commercial_labels = next(frame_generator(non_commercial_folder, 0, batch_size))

def frame_data_generator(folder, destination, num_frames=900):
    filenames = [filename for filename in os.listdir(folder) if filename.endswith(".mp4")]
    for filename in filenames:
        video_path = os.path.join(folder, filename)
        print(video_path)
        cap = cv2.VideoCapture(video_path)
    
        # frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
        # available_frames = list(range(1800))
        for _ in range(num_frames):
            # random_frame_index = random.choice(available_frames)
            # print(random_frame_index)
            # available_frames = list(range(frame_count))
            # available_frames.remove(random_frame_index)

            # cap.set(cv2.CAP_PROP_POS_FRAMES, random_frame_index)
            # cap.set(cv2.CAP_PROP_POS_FRAMES, _)
            ret, frame = cap.read()
            if ret:
                destination.append(cv2.resize(frame, image_size))

        cap.release()


# # commercial_frames = []
# # num_files = 80
# # for filename in os.listdir(commercial_folder):
# #     if filename.endswith(".mp4"):
# #         num_frames = 1800
# #         video_path = os.path.join(commercial_folder, filename)
# #         print(video_path)
# #         cap = cv2.VideoCapture(video_path)
# #         while cap.isOpened():
# #             ret, frame = cap.read()
# #             # print(num_frames)
# #             if not ret or num_frames == 0:
# #                 break
# #             commercial_frames.append(cv2.resize(frame, image_size))
# #             num_frames = num_frames - 1

# #         cap.release()
#         # num_files = num_files - 1
#         # if num_files == 0: 
#         #     break

# non_commercial_frames = []
# num_files = 80
# for filename in os.listdir(non_commercial_folder):
#     if filename.endswith(".mp4"):
#         num_frames = 400
#         video_path = os.path.join(non_commercial_folder, filename)
#         print(video_path)
#         cap = cv2.VideoCapture(video_path)
#         while cap.isOpened():
#             ret, frame = cap.read()
#             # print(num_frames)
#             if not ret or num_frames == 0:
#                 break
#             non_commercial_frames.append(cv2.resize(frame, image_size))
#             num_frames = num_frames - 1
            
#         cap.release()
#         num_files = num_files - 1
#         if num_files == 0: 
#             break
#     elif filename.endswith(".jpg"):
#         image_path = os.path.join(non_commercial_folder, filename)
#         frame = cv2.imread(image_path)
#         if frame is not None:
#             non_commercial_frames.append(cv2.resize(frame, image_size))


commercial_frames = []
frame_data_generator(commercial_folder, commercial_frames)
non_commercial_frames = []
frame_data_generator(non_commercial_folder, non_commercial_frames)

print(len(commercial_frames))
print(len(non_commercial_frames))

commercial_labels = np.ones(len(commercial_frames)) # (1 for commercial)
non_commercial_labels = np.zeros(len(non_commercial_frames)) # (0 for non-commercial)

X = np.array(commercial_frames + non_commercial_frames)
y = np.concatenate([commercial_labels, non_commercial_labels])

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

datagen = ImageDataGenerator(rescale=1./255)

train_generator = datagen.flow(X_train, y_train, batch_size=batch_size)
test_generator = datagen.flow(X_test, y_test, batch_size=batch_size)

model.fit_generator(
    train_generator,
    steps_per_epoch=len(X_train) // batch_size,
    epochs=5,
    validation_data=test_generator,
    validation_steps=len(X_test) // batch_size
)

model.save('commercial_detection_model.h5')