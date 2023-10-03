



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

PORT_NUM = '/dev/tty.usbserial-FT5NY9DI'

BAUDRATE = 1000000

MOVEARM_MODE = 1

ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]

motor.portInitialization(PORT_NUM, ALL_IDs)
motor.dxlSetVelo([20, 20, 20, 20, 20],[0, 1, 2, 3, 4]) #ALWAYS SET SPEED BEFORE ANYTHING
motor.simMotorRun([25, 227, 270, 47, 272], [0,1,2,3,4]) #resting (not working) please test
time.sleep(5)
motor.simMotorRun([187], [2])  # back to pull down more
time.sleep(3)
motor.simMotorRun([150, 80], [2,3])
time.sleep(3)

motor.dxlSetVelo([20, 20, 20, 40, 26],[0, 1, 2, 3, 4])
#motor.simMotorRun([135, 104, 265], [2, 3, 4]) #back to pull down
#time.sleep(2)
motor.simMotorRun([25, 227, 130, 130, 250], [0, 1, 2, 3, 4])  #push in
motor.simMotorRun([25, 227, 90, 222, 194], [0, 1, 2, 3, 4])
#time.sleep(3)
#motor.dxlSetVelo([20, 20, 20, 20, 20],[0, 1, 2, 3, 4])
#motor.simMotorRun([90, 223, 90, 222, 194], [0, 1, 2, 3, 4]) # Let gos