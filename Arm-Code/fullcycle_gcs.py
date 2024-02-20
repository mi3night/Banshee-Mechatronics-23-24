import math
import motorctrl_v1 as motor
import Movement_Calc_v2 as calculation
import numpy as np
import time
import serial
import cv2
import socket
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
GPIO.setup(18, GPIO.OUT)
GPIO.output(18, GPIO.LOW)

BASE_ID = 1
BICEP_ID = 2
FOREARM_ID = 3
WRIST_ID = 4
CLAW_ID = 0


# PORT_NUM = '/dev/cu.usbserial-FT5NY9DI'  #for mac
PORT_NUM = '/dev/ttyUSB0'  # for rpi
ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1)  # for rpi

BAUDRATE = 1000000
MOVEARM_MODE = 1
ADDR_PRESENT_POSITION = 132
ALL_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]
MOVE_IDs = [BASE_ID, BICEP_ID, FOREARM_ID, WRIST_ID, CLAW_ID]

motor.portInitialization(PORT_NUM, ALL_IDs)
SERVER_HOST = '192.168.1.61'
SERVER_PORT = 3300

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

def pushin():
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


client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((SERVER_HOST, SERVER_PORT))


arduinoinput = ''
# TCP IP request from GCS.
client_socket.send("Mech".encode('utf-8'))
while True:
    response = client_socket.recv(1024)
    if response.decode() == 'Go':
        break

# Take Battery from GCS
pullout()
GPIO.output(18, GPIO.HIGH)
time.sleep(8)
ser.write(b'b')  # Tell Arduino it's good to go

# Wait for arduino to send s, means it has arrived at BVM
while True:
    print("waiting for s")
    response = ser.readline().strip()
    arduinoinput = response.decode()
    if arduinoinput == '1':
        print("push battery into BVM!")
        break


# Push battery into BVM
ser.flush()
time.sleep(3)
pushin()

time.sleep(5)  # let BVM cycle battery

# Take battery out of BVM
pullout()
time.sleep(3)
print("sending b to arduino")
ser.write(b'g')  # Tell Arduino it's good to go
time.sleep(0.5)
response = ser.readline().strip().decode()
print(response)

# Wait for arduino to send s, means it has arrived at GCS
while True:
    response = ser.readline().strip()
    arduinoinput = response.decode()
    if arduinoinput == 's':
        print("push battery into GCS!")
        break

# Push battery into GCS

pushin()

# Tell GCS it's good to go
message = 'Finished'
client_socket.send(message.encode())
GPIO.cleanup()