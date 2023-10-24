import math
import numpy as np
import time
import cv2
import socket

SERVER_HOST = '192.168.1.94'
SERVER_PORT = 7777

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((SERVER_HOST, SERVER_PORT))


def pullout():
   print("Pull out Start")
   time.sleep(7)
   print("Pull out Done")


def pushin():
    print("Push in Start")
    time.sleep(7)
    print("Push in Done")


arduinoinput = ''
# TCP IP request from GCS.
client_socket.send("Mech".encode('utf-8'))
while True:
    response = client_socket.recv(1024)
    if response.decode() == 'Go':
        break

# Take Battery from GCS
pullout()
time.sleep(8)
arduinoinput = 'g'

# Wait for arduino to send s, means it has arrived at BVM
while True:
    time.sleep(5)
    arduinoinput = 's'
    if arduinoinput == 's':
        print("push battery into BVM!")
        break


# Push battery into BVM
pushin()

time.sleep(5)  # let BVM cycle battery

# Take battery out of BVM
pullout()

arduinoinput = 'g'

# Wait for arduino to send s, means it has arrived at GCS
while True:
    time.sleep(5)
    arduinoinput = 's'
    if arduinoinput == 's':
        print("push battery into GCS!")
        break


# Push battery into GCS

pushin()

# Tell GCS it's good to go
message = 'Finished'
client_socket.send(message.encode())