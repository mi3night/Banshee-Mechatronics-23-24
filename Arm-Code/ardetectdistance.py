import cv2
import pyrealsense2
from realsense_depth import *
import matplotlib.pyplot as plt     

# Initialize Camera Intel Realsense
dc = DepthCamera()

# Create an AR marker detector (you may need to install the cv2_aruco package)
aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_100)
parameters = cv2.aruco.DetectorParameters_create()

# Initialize the point of interest
point = (400, 300)

while True:
    ret, depth_frame, color_frame = dc.get_frame()

    # Detect AR markers in the color frame
    corners, ids, _ = cv2.aruco.detectMarkers(color_frame, aruco_dict, parameters=parameters)

    if ids is not None and len(ids) > 0:
        # Get the center of the first detected AR marker
        marker_center = corners[0][0].mean(axis=0).astype(int)

        # Update the point based on the AR marker's center
        point = tuple(marker_center)

    # Show distance for the point of interest
    cv2.circle(color_frame, point, 4, (0, 0, 255))
    distance = depth_frame[point[1], point[0]]

    cv2.putText(color_frame, "{}mm".format(distance), (point[0], point[1] - 20), cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 0), 2)



    #filled_depth_image = np.asanyarray(filled_depth.get_data())


    normalized_depth = cv2.normalize(depth_frame, None, 0, 255, cv2.NORM_MINMAX, dtype=cv2.CV_8U)
    colored_depth = cv2.applyColorMap(normalized_depth, cv2.COLORMAP_JET)
    cv2.imshow("Depth Overlaid on Color", colored_depth)
    cv2.imshow("depth frame", depth_frame)
    cv2.imshow("Color frame", color_frame)

    key = cv2.waitKey(1)
    if key == 27:
        break
