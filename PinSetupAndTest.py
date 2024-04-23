import RPi.GPIO as GPIO
from time import sleep 

DIR = 10 #Dir+

STEP = 8 #PUL+

#Rest is ground 
#24V 1-5Amp

CW = 1
CCW = 0

GPIO.setmode(GPIO.BOARD)


GPIO.setup(DIR, GPIO.OUT)
GPIO.setup(STEP, GPIO.OUT)


GPIO.output(DIR, CW)

try:
        while True:  #Infinite Loop
            
            sleep(1.0)
            GPIO.output(DIR, CW)
            
            for x in range(200): #Will run for 200 steps
            
                GPIO.output(STEP, GPIO.HIGH)
                sleep(.0001) #Dictates the speed
                
                GPIO.output(STEP, GPIO.LOW)
                sleep(.0001) #Dictates the speed
                
            #Change Direction
            sleep(1.0)
            
            GPIO.output(DIR,CCW)
            
            for x in range(200): #Will run for 200 steps
            
                GPIO.output(STEP, GPIO.HIGH)
                sleep(.0001) #Dictates the speed
                
                GPIO.output(STEP, GPIO.LOW)
                sleep(.0001) #Dictates the speed
                
except KeyboardInterrupt:
        print("cleanup")
        GPIO.cleanup()

