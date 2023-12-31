import math
import motorctrl_v1 as motor
import Movement_Calc_v2 as calculation
import numpy as np
import time
import serial
import cv2
import socket
import RPi.GPIO as GPIO
import serial.tools.list_ports

GPIO.setmode(GPIO.BOARD)
GPIO.setup(16, GPIO.OUT)
GPIO.output(16, GPIO.LOW)
ADDR_PRESENT_POSITION = 132 #Address of the positions of the motors
BASE_ID = 1
BICEP_ID = 2
FOREARM_ID = 3
WRIST_ID = 4
CLAW_ID = 0
#GPIO 23 physical pin 16 arduino reset pin

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

PORT_NUM = '/dev/ttyUSB0'  # for rpi
serial.Serial('/dev/ttyUSB1', 9600, timeout=1)

for onePort in ports:
    port = str(onePort)
    print(port)
    if 'USB1 - USB <->' in port:
        PORT_NUM = '/dev/ttyUSB1'  # for rpi
    if 'USB0 - USB Serial' in port:
        ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

serialInst.close()


# PORT_NUM = '/dev/cu.usbserial-FT5NY9DI'  #for mac
BAUDRATE = 1000000
MOVEARM_MODE = 1
ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]

print('port num is ' + PORT_NUM)
motor.portInitialization(PORT_NUM, ALL_IDs)

def trajectoryMove(angles, ids):
    print('starting trajectory')
    currents = []
    index = 0
    differences = []
    for id in ids:
        currents.append(motor._map(motor.ReadMotorData(id, 132), 0, 4095, 0, 360))
        differences.append((angles[index] - currents[index])/10)
        index += 1
    loop = 0
    print(currents)
    print(differences)
    while (loop < 10):
        currents = [x + y for x, y in zip(currents, differences)]
        print(currents)
        motor.simMotorRun(currents, [0, 1, 2, 3, 4])
        loop += 1 
    print('finished')
        
    


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
            break

def initializePosition():
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])  # ALWAYS SET SPEED BEFORE ANYTHING
    # motor.simMotorRun([90, 225, 177, 226, 180], [0, 1, 2, 3, 4])  # set arm straight up
    # checkMovement(MOVE_IDs)
    # motor.simMotorRun([90, 223, 300, 48, 147], [0, 1, 2, 3, 4])
    # checkMovement(MOVE_IDs)
    # print('initialize completed')
    trajectoryMove([90, 225, 177, 226, 180], [0, 1, 2, 3, 4])
    checkMovement(MOVE_IDs)
    trajectoryMove([90, 223, 300, 48, 147], [0, 1, 2, 3, 4])
    checkMovement(MOVE_IDs)

def pullout():
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])  # ALWAYS SET SPEED BEFORE ANYTHING
    motor.simMotorRun([90, 223, 90, 222, 185], [0, 1, 2, 3, 4])  # set chamber
    checkMovement(MOVE_IDs)
    motor.simMotorRun([31, 223, 90, 222, 190], [0, 1, 2, 3, 4])  # grab battery
    checkMovement(MOVE_IDs)
    motor.dxlSetVelo([20, 20, 20, 40, 26], [0, 1, 2, 3, 4])
    motor.simMotorRun([129, 104, 285], [2, 3, 4])  # pull out
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])
    motor.simMotorRun([137, 62, 285], [2, 3, 4])  # pull out more
    checkMovement(MOVE_IDs)
    motor.simMotorRun([30, 227, 270, 47, 272], [0, 1, 2, 3, 4])  # resting
    checkMovement(MOVE_IDs)

def pushin():
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

# arduinoinput = ''
initializePosition()
# pullout()
# GPIO.output(16, GPIO.HIGH)
# ser.write(b'g')  # Tell Arduino it's good to go

# Wait for arduino to send s, means it has arrived at BVM
# while True:
#     response = ser.readline().strip()
#     arduinoinput = response.decode()
#     print(arduinoinput[0] + " flag ")
#     if arduinoinput[0] == 's':
#         print("push battery into BVM!")
#         break


# Push battery into BVM
# pushin()
# GPIO.cleanup()