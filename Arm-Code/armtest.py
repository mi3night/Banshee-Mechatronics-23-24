import math
import motorctrl_v1 as motor
import Movement_Calc_v2 as calculation
import numpy as np
import time
import time
import cv2

BASE_ID = 1
BICEP_ID = 2
FOREARM_ID = 3
WRIST_ID = 4
CLAW_ID = 0

PORT_NUM = '/dev/cu.usbserial-FT5NY9DI'
ADDR_PRESENT_POSITION = 132
BAUDRATE = 1000000

MOVEARM_MODE = 1

ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]


def checkMovement(ids):
    motorStatus = [0] * len(ids)
    finished = [1] * len(ids)
    firstPosition = 0
    secondPosition = 0
    while True:
        for id in (ids):
            firstPosition = motor.ReadMotorData(id, ADDR_PRESENT_POSITION)
            time.sleep(.1)
            secondPosition = motor.ReadMotorData(id, ADDR_PRESENT_POSITION)
            if (abs(firstPosition - secondPosition) < 5):
                motorStatus[id] = 1
        if (motorStatus == finished):
            print("finished")
            break

def pullout():
    print("pull out start")
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4]
                     )  # ALWAYS SET SPEED BEFORE ANYTHING
    
    motor.simMotorRun([110, 223, 270, 47, 272], [0, 1, 2, 3, 4])  # resting
    
    
    checkMovement(MOVE_IDs)
    motor.simMotorRun([187], [2])  # back to pull down more
    checkMovement(MOVE_IDs)
    motor.simMotorRun([150, 80], [2, 3])
    checkMovement(MOVE_IDs)
    motor.dxlSetVelo([20, 20, 20, 40, 26], [0, 1, 2, 3, 4])
    motor.simMotorRun([110, 223, 130, 130, 250], [0, 1, 2, 3, 4])  # push in
    motor.simMotorRun([110, 223, 90, 222, 194], [0, 1, 2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([110, 223, 90, 222, 194], [0, 1, 2, 3, 4])
    checkMovement(MOVE_IDs)
    
 
    motor.simMotorRun([31, 223, 90, 222, 190], [0, 1, 2, 3, 4])  # grab battery
    checkMovement(MOVE_IDs)
    motor.dxlSetVelo([20, 20, 20, 40, 26], [0, 1, 2, 3, 4])
    motor.simMotorRun([129, 104, 285], [2, 3, 4])  # pull out
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])
    motor.simMotorRun([150, 62, 285], [2, 3, 4])  # pull out more
    checkMovement(MOVE_IDs)
    motor.simMotorRun([30, 227, 270, 47, 272], [0, 1, 2, 3, 4])  # resting
    checkMovement(MOVE_IDs)
    print("pull out end")


def pushin():
    print("push in start")
    checkMovement(MOVE_IDs)
    motor.simMotorRun([187], [2])  # back to pull down more
    checkMovement(MOVE_IDs)
    motor.simMotorRun([150, 80], [2, 3])
    checkMovement(MOVE_IDs)
    motor.dxlSetVelo([20, 20, 20, 40, 26], [0, 1, 2, 3, 4])
    motor.simMotorRun([25, 227, 130, 130, 250], [0, 1, 2, 3, 4])  # push in
    motor.simMotorRun([25, 227, 90, 222, 194], [0, 1, 2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([90, 227, 90, 222, 194], [0, 1, 2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([30, 227, 270, 47, 272], [0, 1, 2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([30, 227, 301, 49, 143], [0, 1, 2, 3, 4])
    checkMovement(MOVE_IDs)
    print("push in end")

motor.portInitialization(PORT_NUM, ALL_IDs)



motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])
motor.simMotorRun([110, 223, 270, 47, 272], [0, 1, 2, 3, 4])  # resting
                 
                 
pullout()
pushin()