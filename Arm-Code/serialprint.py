import serial
import time

ser = serial.Serial('COM4', 115200, timeout=1)
time.sleep(1)
counter = 0

while True:
    ser.write((str(counter)).encode())
    print("Sent:", counter)
    counter += 1

    time.sleep(0.1)  # Add a small delay (e.g., 100 ms) to allow Arduino to process and clear the buffer

    response = ser.readline().strip()
    print("Received:", response.decode())

    time.sleep(0.1)  # Add another small delay before sending the next value