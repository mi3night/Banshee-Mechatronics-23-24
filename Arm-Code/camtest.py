import numpy as np
import time
import cv2
import math
import motorctrl_v1 as motor
import Movement_Calc_v2 as calculation


ARUCO_DICT = {
    "DICT_4X4_50": cv2.aruco.DICT_4X4_50,
    "DICT_4X4_100": cv2.aruco.DICT_4X4_100,
    "DICT_4X4_250": cv2.aruco.DICT_4X4_250,
    "DICT_4X4_1000": cv2.aruco.DICT_4X4_1000,
    "DICT_5X5_50": cv2.aruco.DICT_5X5_50,
    "DICT_5X5_100": cv2.aruco.DICT_5X5_100,
    "DICT_5X5_250": cv2.aruco.DICT_5X5_250,
    "DICT_5X5_1000": cv2.aruco.DICT_5X5_1000,
    "DICT_6X6_50": cv2.aruco.DICT_6X6_50,
    "DICT_6X6_100": cv2.aruco.DICT_6X6_100,
    "DICT_6X6_250": cv2.aruco.DICT_6X6_250,
    "DICT_6X6_1000": cv2.aruco.DICT_6X6_1000,
    "DICT_7X7_50": cv2.aruco.DICT_7X7_50,
    "DICT_7X7_100": cv2.aruco.DICT_7X7_100,
    "DICT_7X7_250": cv2.aruco.DICT_7X7_250,
    "DICT_7X7_1000": cv2.aruco.DICT_7X7_1000,
    "DICT_ARUCO_ORIGINAL": cv2.aruco.DICT_ARUCO_ORIGINAL,
    "DICT_APRILTAG_16h5": cv2.aruco.DICT_APRILTAG_16h5,
    "DICT_APRILTAG_25h9": cv2.aruco.DICT_APRILTAG_25h9,
    "DICT_APRILTAG_36h10": cv2.aruco.DICT_APRILTAG_36h10,
    "DICT_APRILTAG_36h11": cv2.aruco.DICT_APRILTAG_36h11
}


frameX = 0
objX = 1000
frameY = 0
objY = 1000
test = "out"
def aruco_display(corners, ids, rejected, image):
    if len(corners) > 0:
        ids = ids.flatten()
        
        for (markerCorner, markerID) in zip(corners, ids):
            corners = markerCorner.reshape((4, 2))
            (topLeft, topRight, bottomRight, bottomLeft) = corners
            
            topRight = (int(topRight[0]), int(topRight[1]))
            bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
            bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
            topLeft = (int(topLeft[0]), int(topLeft[1]))

            cv2.line(image, topLeft, topRight, (255, 0, 255), 2)
            cv2.line(image, topRight, bottomRight, (255, 0, 255), 2)
            cv2.line(image, bottomRight, bottomLeft, (255, 0, 255), 2)
            cv2.line(image, bottomLeft, topLeft, (255, 0, 255), 2)

            #Object's center pixel coordinates    
            h,w,_ = img.shape
            global objX
            global frameX
            global objY
            global frameY
            fX=int(w/2)
            frameX = fX
            fY=int(h/2)
            frameY = fY
            cX = int((topLeft[0] + bottomRight[0]) / 2.0)
            objX = cX
            cY = int((topLeft[1] + bottomRight[1]) / 2.0)
            objY = cY
            cv2.circle(image, (cX, cY), 4, (0, 0, 255), -1)
            objectCord = "(" + str(cX) + ", " + str(cY) + ")" 
            cv2.putText(image, objectCord, (cX,cY), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255,0,255), 2)
            print("At pixel coordinates ({}, {})".format(cX,cY))

            # Calculate distance
            
            marker_size = np.linalg.norm(np.array(topRight) - np.array(topLeft))
            distance_feet, distance_per_pixel = calculate_distance(marker_size)
            distance_feet_rounded = round(distance_feet, 2)
            distance_per_pixel_rounded = round(distance_per_pixel, 6)
            arX = distance_per_pixel * (cX - fX) / 3.6
            arY = distance_feet_rounded
            arX = round(arX, 4)
            arY = round(arY, 4)
            print("[Inference] ArUco marker ID: {}, Distance: {} feet, Distance per pixel: {} feet/pixel\n, X coord: {}, Y coord: {}\n".format(markerID, distance_feet_rounded, distance_per_pixel_rounded, arX, arY))
            outlineText = "ID: " + str(markerID) + " at " +  str(distance_feet_rounded) + " feet, " +  str(distance_per_pixel_rounded) + " ft/pixel" 
            outlineText2 = "X Axis: " + str(arX) + " Y Axis: " + str(arY)   
            

            cv2.putText(image, outlineText,(topLeft[0], topLeft[1] - 10), cv2.FONT_HERSHEY_SIMPLEX,
                0.6, (255, 0, 255), 2)
            cv2.putText(image, outlineText2, (topLeft[0], topLeft[1] + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 255), 2)
    return image

def calculate_distance(marker_size):
    # Constants for your specific camera setup
    # You need to calibrate these values for your camera
    marker_size_at_one_meter = 0.1055  # Adjust this value based on the actual marker size at 1 meter distance
    focal_length = 226  # Focal length of your camera in pixels
    
    # Convert meters to feet
    distance_in_feet = marker_size_at_one_meter * focal_length / marker_size * 3.28084
    distance_per_pixel = distance_in_feet / focal_length

    return distance_in_feet, distance_per_pixel



aruco_type = "DICT_5X5_100"
arucoDict = cv2.aruco.Dictionary_get(ARUCO_DICT[aruco_type])
arucoParams = cv2.aruco.DetectorParameters_create()

cap = cv2.VideoCapture(1)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

def pullout():
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4]
                     )  # ALWAYS SET SPEED BEFORE ANYTHING
    motor.simMotorRun([90, 223, 90, 222, 185], [0, 1, 2, 3, 4])  # set chamber
    time.sleep(4)
    motor.simMotorRun([31, 223, 90, 222, 190], [0, 1, 2, 3, 4])  # grab battery
    time.sleep(2)
    motor.dxlSetVelo([20, 20, 20, 40, 26], [0, 1, 2, 3, 4])
    motor.simMotorRun([129, 104, 285], [2, 3, 4])  # pull out
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])
    motor.simMotorRun([137, 62, 285], [2, 3, 4])  # pull out more
    time.sleep(3)
    motor.simMotorRun([30, 227, 270, 47, 272], [0, 1, 2, 3, 4])  # resting


def pushin():
    motor.simMotorRun([30, 227, 270, 47, 272], [0, 1, 2, 3, 4])
    time.sleep(4)
    motor.simMotorRun([187], [2])  # back to pull down more
    time.sleep(3)
    motor.simMotorRun([150, 80], [2, 3])
    time.sleep(3)
    motor.dxlSetVelo([20, 20, 20, 40, 26], [0, 1, 2, 3, 4])
    motor.simMotorRun([25, 227, 130, 130, 250], [0, 1, 2, 3, 4])  # push in
    motor.simMotorRun([25, 227, 90, 222, 194], [0, 1, 2, 3, 4])
    time.sleep(3)
    motor.simMotorRun([90, 227, 90, 222, 194], [0, 1, 2, 3, 4])
    time.sleep(3)
    motor.simMotorRun([30, 227, 270, 47, 272], [0, 1, 2, 3, 4])
    time.sleep(7)
    motor.simMotorRun([30, 227, 301, 49, 143], [0, 1, 2, 3, 4])



BASE_ID = 1
BICEP_ID = 2
FOREARM_ID = 3
WRIST_ID = 4
CLAW_ID = 0

PORT_NUM = '/dev/cu.usbserial-FT5NY9DI'

BAUDRATE = 1000000

MOVEARM_MODE = 1

ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]

motor.portInitialization(PORT_NUM, ALL_IDs)
motor.dxlSetVelo([20, 20, 20, 20, 20],[0, 1, 2, 3, 4]) #ALWAYS SET SPEED BEFORE ANYTHING
motor.simMotorRun([90, 227, 280, 49, 143], [0, 1, 2, 3, 4])




while (MOVEARM_MODE):
        while cap.isOpened():
            ret, img = cap.read()
            h, w, _ = img.shape
            width = 1000
            height = int(width*(h/w))
            img = cv2.resize(img, (width, height), interpolation=cv2.INTER_CUBIC)
            h,w,_ = img.shape
            fX=int(w/2)
            fY=int(h/2)
            cv2.circle(img, (fX,fY), 3, (255, 0, 0), -1)
            cv2.putText(img," (" + str(fX) + " , " + str(fY) + ")", (fX,fY), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            corners, ids, rejected = cv2.aruco.detectMarkers(img, arucoDict, parameters=arucoParams)
            detected_markers = aruco_display(corners, ids, rejected, img)
            
            cv2.imshow("Image", detected_markers)
            
            print(abs(objX - frameX))
            if (abs(objX - frameX) < 50 ) and (test == "out"):
                pullout()
                print("out")
                test = "in"
                time.sleep(8)
                motor.simMotorRun([90, 227, 280, 49, 143], [0, 1, 2, 3, 4]) # back to pull down more so that the camera can see the AR Marker
                objX = 51
            
            
            if (abs(objX - frameX) < 50 ) and (test == "in"):
                pushin()
                print("out")
                test = "out"
                objX = 51     
            key = cv2.waitKey(1) & 0xFF
            if key == ord("q"):
                break
        break

cv2.destroyAllWindows()
cap.release()