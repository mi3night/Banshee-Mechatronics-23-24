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
motor.dxlSetVelo([20, 20, 20, 20, 20],[0, 1, 2, 3, 4])
motor.simMotorRun([90, 223, 224, 131, 215], [0, 1, 2, 3, 4]) # OG: 215
time.sleep(1)
motor.motorRunWithInputs([150], [4])
time.sleep(2)
motor.motorRunWithInputs([31], [0])
time.sleep(1)

