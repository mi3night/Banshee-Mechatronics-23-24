import cv2
from cv2 import aruco
import numpy
import pyrealsense2

# 1. Load the ArUco dictionary
aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_100)  # Choose your desired dictionary
parameters = cv2.aruco.DetectorParameters_create()

# 2. Access video capture
video_capture = cv2.VideoCapture(0)  # Use 0 for webcam, or file path for a video file

while True:
    # 3. Capture a frame
    ret, frame = video_capture.read()

    # 4. Detect ArUco markers
    corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(frame, aruco_dict, parameters=parameters)

    # 5. Draw detected markers on the frame
    if ids is not None:
       for i in range(len(ids)):
           cv2.aruco.drawDetectedMarkers(frame, corners, ids)

    # 6. Display the frame
    cv2.imshow('ArUco Marker Detection', frame)

    # 7. Exit on 'q' key press
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 8. Release resources
video_capture.release()
cv2.destroyAllWindows()
