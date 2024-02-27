import math
import motorctrl_v1 as motor
import Movement_Calc_v2 as calculation
import numpy as np
import time
import cv2
import socket

BASE_ID = 1
BICEP_ID = 2
FOREARM_ID = 3
WRIST_ID = 4
CLAW_ID = 0


PORT_NUM = '/dev/ttyUSB0'  # for rpi
MOVEARM_MODE = 1
ADDR_PRESENT_POSITION = 132
ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]

motor.portInitialization(PORT_NUM, ALL_IDs)
SERVER_HOST = '192.168.1.61'
SERVER_PORT = 3300

def checkMovement(ids):
    time.sleep(0.5)
    motorStatus = [0] * len(ids)
    finished = [1] * len(ids)
    firstPosition = 0
    secondPosition = 0
    while True:
        for id in (ids):
            firstPosition = motor.ReadMotorData(id, ADDR_PRESENT_POSITION)
            time.sleep(.1)
            secondPosition = motor.ReadMotorData(id, ADDR_PRESENT_POSITION)
            if (abs(firstPosition - secondPosition) < 2):
                motorStatus[id] = 1
        if (motorStatus == finished):
            print("finished")
            break
        
    
def pullout():
        
    print("pull out start")
    motor.dxlSetVelo([30, 30, 30, 30, 30], [0, 1, 2, 3, 4])  # ALWAYS SET SPEED BEFORE ANYTHING
    motor.simMotorRun([110, 223, 270, 47, 272], [0, 1, 2, 3, 4])  # resting
    checkMovement(MOVE_IDs)
    motor.simMotorRun([168], [2])  # back to pull down more
    checkMovement(MOVE_IDs)
    motor.simMotorRun([150, 84, 269], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([145, 122, 233], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([30], [0])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([138, 75, 285], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([153, 50, 285], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([265, 47, 170], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([275], [4])
    checkMovement(MOVE_IDs)

def pushin():
    print("Push In Start")
    motor.dxlSetVelo([30, 30, 30, 30, 30], [0, 1, 2, 3, 4])  # ALWAYS SET SPEED BEFORE ANYTHING
    motor.simMotorRun([170], [4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([153, 50, 285], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([138, 75, 285], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([145, 122, 233], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([110], [0])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([145, 122, 233], [2, 3, 4])
    checkMovement(MOVE_IDs)
    motor.simMotorRun([150, 84, 269], [2, 3, 4])
    checkMovement(MOVE_IDs)

while True:
    uin = int(input("Push(0) or Pull (1)"))
    if uin == 0: pushin()
    elif uin == 1: pullout()
    else: print("Invalid Input")