import math
import motorctrl_v1 as motor
import Movement_Calc_v2 as calculation
import numpy as np
import time
import serial
import cv2
import socket
import RPi.GPIO as GPIO
BASE_ID = 1
BICEP_ID = 2
FOREARM_ID = 3
WRIST_ID = 4
CLAW_ID = 0


# # PORT_NUM = '/dev/cu.usbserial-FT5NY9DI'  #for mac
# PORT_NUM = '/dev/ttyUSB0'  # for rpi
# ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1)  # for rpi

# BAUDRATE = 1000000
# MOVEARM_MODE = 1
# ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
# MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]

#SERVER_HOST = '107.77.227.224'
#SERVER_HOST = '192.168.1.26'
SERVER_HOST = '192.168.1.50'
#SERVER_HOST = '172.20.10.3'
SERVER_PORT = 12345
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((SERVER_HOST, SERVER_PORT))
GPIO.setmode(GPIO.BOARD)
GPIO.setup(16, GPIO.OUT)
GPIO.output(16, GPIO.LOW)
def pullout():
#     motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])  # ALWAYS SET SPEED BEFORE ANYTHING
#     motor.simMotorRun([90, 223, 90, 222, 185], [0, 1, 2, 3, 4])  # set chamber
#     time.sleep(4)
#     motor.simMotorRun([31, 223, 90, 222, 190], [0, 1, 2, 3, 4])  # grab battery
#     time.sleep(2)
#     motor.dxlSetVelo([20, 20, 20, 40, 26], [0, 1, 2, 3, 4])
#     motor.simMotorRun([129, 104, 285], [2, 3, 4])  # pull out
#     motor.dxlSetVelo([20, 20, 20, 20, 20], [0, 1, 2, 3, 4])
#     motor.simMotorRun([137, 62, 285], [2, 3, 4])  # pull out more
#     time.sleep(3)
#     motor.simMotorRun([30, 227, 270, 47, 272], [0, 1, 2, 3, 4])  # resting
    time.sleep(9)
    print("Arm pulled out")


def pushin():
    # time.sleep(7)
    # motor.simMotorRun([187], [2])  # back to pull down more
    # time.sleep(3)
    # motor.simMotorRun([150, 80], [2, 3])
    # time.sleep(3)
    # motor.dxlSetVelo([20, 20, 20, 40, 26], [0, 1, 2, 3, 4])
    # motor.simMotorRun([25, 227, 130, 130, 250], [0, 1, 2, 3, 4])  # push in
    # motor.simMotorRun([25, 227, 90, 222, 194], [0, 1, 2, 3, 4])
    # time.sleep(3)
    # motor.simMotorRun([90, 227, 90, 222, 194], [0, 1, 2, 3, 4])
    # time.sleep(3)
    # motor.simMotorRun([30, 227, 270, 47, 272], [0, 1, 2, 3, 4])
    # time.sleep(7)
    # motor.simMotorRun([30, 227, 301, 49, 143], [0, 1, 2, 3, 4])
    time.sleep(26)
    print("Arm pushed in")

arduinoinput = ''
# TCP IP request from GCS.
while True:
    response = client_socket.recv(1024)
    if response.decode() == 'ready':
        break

# Take Battery from GCS
pullout()
GPIO.output(16, GPIO.HIGH)
time.sleep(8)
# ser.write(b'g')  # Tell Arduino it's good to go

# Wait for arduino to send s, means it has arrived at BVM
while True:
    # response = ser.readline().strip()
    # arduinoinput = response.decode()
    print("wait for arduino input")
    arduinoinput = 's'
    time.sleep(3)
    if arduinoinput == 's':
        print("push battery into BVM!")
        break


# Push battery into BVM
pushin()
GPIO.cleanup()
