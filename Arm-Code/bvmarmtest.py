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

PORT_NUM = '/dev/cu.usbserial-FT2N0BTD'
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
    motor.simMotorRun([168], [2])  # back to pull down more
    checkMovement(MOVE_IDs)
    motor.simMotorRun([150, 84, 269], [2, 3, 4])
    time.sleep(1)
    checkMovement([MOVE_IDs])
    motor.simMotorRun([145, 122, 233], [2, 3, 4])
    time.sleep(2)
    motor.simMotorRun([30], [0])
    time.sleep(1)
    checkMovement(MOVE_IDs)
    motor.simMotorRun([138, 75, 285], [2, 3, 4])
    time.sleep(1)
    checkMovement([MOVE_IDs])
    
    print("pull out end")


    
    
def testpullout():
        
    print("pull out start")
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4]
                     )  # ALWAYS SET SPEED BEFORE ANYTHING
    
    motor.simMotorRun([110, 223, 270, 47, 272], [0, 1, 2, 3, 4])  # resting
    checkMovement(MOVE_IDs)
    motor.simMotorRun([168], [2])  # back to pull down more
    checkMovement(MOVE_IDs)
    motor.simMotorRun([150, 84, 269], [2, 3, 4])
    time.sleep(1)
    checkMovement(MOVE_IDs)
    motor.simMotorRun([145, 122, 233], [2, 3, 4])
    time.sleep(1)
    checkMovement(MOVE_IDs)
    motor.simMotorRun([30], [0])
    time.sleep(1)
    motor.simMotorRun([138, 75, 285], [2, 3, 4])
    time.sleep(2)
    motor.simMotorRun([153, 50, 285], [2, 3, 4])
    time.sleep(2)
    motor.simMotorRun([265, 47, 170], [2, 3, 4])
    time.sleep(2)
    motor.simMotorRun([275], [4])
    time.sleep(2)
    

    

def testpushin():
    motor.simMotorRun([170], [4])
    time.sleep(4)
    motor.simMotorRun([153, 50, 285], [2, 3, 4])
    time.sleep(2)
    motor.simMotorRun([138, 75, 285], [2, 3, 4])
    time.sleep(2)
    motor.simMotorRun([145, 122, 233], [2, 3, 4])
    time.sleep(2)
    motor.simMotorRun([110], [0])
    time.sleep(1)
    motor.simMotorRun([145, 122, 233], [2, 3, 4])
    time.sleep(2)
    motor.simMotorRun([150, 84, 269], [2, 3, 4])
    time.sleep(1)



    

motor.portInitialization(PORT_NUM, ALL_IDs)



motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])
motor.simMotorRun([110, 223, 270, 47, 272], [0, 1, 2, 3, 4])  # resting
                 
testpullout()
testpushin()
