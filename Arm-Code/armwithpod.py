import math
import motorctrl_v1 as motor
import Movement_Calc_v2 as calculation
import numpy as np
import time
import serial
import cv2

BASE_ID = 1
BICEP_ID = 2
FOREARM_ID = 3
WRIST_ID = 4
CLAW_ID = 0

arduinoinput = ''

#Need to make a function that automatically searches for the port number
#similar to the one in serial lookup.py but use string search to auto
#select arm serial port and arduino serial port
#Might not have to do since after testing the arm stays at /dev/ttyUSB0
#PORT_NUM = '/dev/cu.usbserial-FT5NY9DI'  #for mac
PORT_NUM = '/dev/ttyUSB0' #for arm
ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1) #for arduino


BAUDRATE = 1000000

MOVEARM_MODE = 1

ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]



#TCP IP request from GCS
time.sleep(4)




motor.portInitialization(PORT_NUM, ALL_IDs)
motor.dxlSetVelo([20, 20, 20, 20, 20],[0, 1, 2, 3, 4]) #ALWAYS SET SPEED BEFORE ANYTHING
motor.simMotorRun([90, 223, 90, 222, 185], [0, 1, 2, 3, 4]) #set chamber
time.sleep(4)
motor.simMotorRun([31, 223, 90, 222, 190], [0, 1, 2, 3, 4]) #grab battery
time.sleep(2)
motor.dxlSetVelo([20, 20, 20, 40, 26],[0, 1, 2, 3, 4]) 
motor.simMotorRun([129, 104, 285], [2, 3, 4]) #pull out
motor.dxlSetVelo([20, 20, 20, 20, 20],[0, 1, 2, 3, 4])
motor.simMotorRun([137, 62, 285], [2, 3, 4]) #pull out more
time.sleep(3)
motor.simMotorRun([30, 227, 270, 47, 272], [0,1,2,3,4]) #resting 


#push battery back in

ser.write(b'g')

while arduinoinput != 's':
    response = ser.readline().strip()
    arduinoinput = response.decode()

time.sleep(7)
motor.simMotorRun([187], [2])  # back to pull down more
time.sleep(3)
motor.simMotorRun([150, 80], [2,3])
time.sleep(3)
motor.dxlSetVelo([20, 20, 20, 40, 26],[0, 1, 2, 3, 4])
motor.simMotorRun([25, 227, 130, 130, 250], [0, 1, 2, 3, 4])  #push in
motor.simMotorRun([25, 227, 90, 222, 194], [0, 1, 2, 3, 4])
time.sleep(3)
motor.simMotorRun([90, 227, 90, 222, 194], [0, 1, 2, 3, 4])
time.sleep(3)
motor.simMotorRun([30, 227, 270, 47, 272], [0,1,2,3,4]) 
time.sleep(7)
motor.simMotorRun([30, 227, 301, 49, 143], [0,1,2,3,4]) 
