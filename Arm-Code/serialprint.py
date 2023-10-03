import serial
import syslog
import time

#The following line is for serial over GPIO

ser = serial.Serial(
    port='/dev/cu.usbserial-1140',
    baudrate=115200,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS
)

while(True):
    print(ser.readline())

ser.close()
