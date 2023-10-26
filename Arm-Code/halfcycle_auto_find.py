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
BASE_ID = 1
BICEP_ID = 2
FOREARM_ID = 3
WRIST_ID = 4
CLAW_ID = 0
#GPIO 23 physical pin 16 arduino reset pin

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

portsList = []

for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))
if(portsList[0]== "/dev/ttyUSB1 - USB Serial"):
    PORT_NUM = '/dev/ttyUSB0'  # for rpi
    ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1)
else:
    PORT_NUM = '/dev/ttyUSB1'  # for rpi
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

serialInst.close()


# PORT_NUM = '/dev/cu.usbserial-FT5NY9DI'  #for mac
PORT_NUM = '/dev/ttyUSB0'  # for rpi
ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1)  # for rpi

BAUDRATE = 1000000
MOVEARM_MODE = 1
ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]

motor.portInitialization(PORT_NUM, ALL_IDs)



def pullout():
    motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])  # ALWAYS SET SPEED BEFORE ANYTHING
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
    time.sleep(7)
    # while (True):
    #     if (motor.dxlGetVelo(ALL_IDs) == [0,0,0,0,0]):
    #         break

def pushin():
    time.sleep(7)
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
    # while (True):
    #     if (motor.dxlGetVelo(ALL_IDs) == [0,0,0,0,0]):
    #         break
arduinoinput = ''

pullout()
GPIO.output(16, GPIO.HIGH)
time.sleep(8)
ser.write(b'g')  # Tell Arduino it's good to go

# Wait for arduino to send s, means it has arrived at BVM
while True:
    response = ser.readline().strip()
    arduinoinput = response.decode()
    print(arduinoinput[0] + " flag ")
    if arduinoinput[0] == 's':
        print("push battery into BVM!")
        break


# Push battery into BVM
pushin()
GPIO.cleanup()