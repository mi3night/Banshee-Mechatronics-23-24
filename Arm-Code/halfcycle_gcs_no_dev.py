import time
import socket

SERVER_HOST = '192.168.1.94'
SERVER_PORT = 7777
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((SERVER_HOST, SERVER_PORT))

def pullout():
    time.sleep(26)
    print("Arm pushed in")

def pushin():
    time.sleep(26)
    print("Arm pushed in")
    
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

# Wait for arduino to send s, means it has arrived at BVM
while True:
    print("wait for arduino input")
    arduinoinput = 's'
    time.sleep(3)
    if arduinoinput == 's':
        print("push battery into BVM!")
        break


# Push battery into BVM
pushin()