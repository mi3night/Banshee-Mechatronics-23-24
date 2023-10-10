
# Version Description/History

## V0_1
	
Basic test of the Battery Transfer Pod's(BTP) motors using the Hobbywing QuicRun WP 880 dual brushed ESC<br>
using Arduino's servo library.<br>
**NOTE** this code does not work.

## V0_2

Continuation of V0_1 Test using potentiometer.<br>

## V0_3

Continuation of V0_1 however the 5 second set up time was added and slowly increase speed.<br>
**NOTE** The 5 second setup time is needed due to the ESC's safety features.

## V0_4

BTP forward and reverse movement test.<br>
**NOTE** this code does not work, the motors will not move in reverse.

## V0_5

### V0_5_1

Test intergration of one Ultrasonic Distance Sensor HC-SR04.

### V0_5_2

Technically a continuation of V0_4 forward and reverse movement test using more debuging techniques.

### V0_5_3

Reset point test.<br>

## V0_6

Continuation of V_0_5_1 test using two HC-SR04 instead of one.<br>
**NOTE** Still unable to reverse.

## V0_7

Continuation of V0_6, redone to use function to make code more readable and easier to debug.<br>
Added relative direction detection.<br>
**NOTE** Still unable to reverse.

## V0_8

BTP half cycle test.<br>
**NOTE** Still unable to reverse.

## V0_9

Added graidentControl function which slowly ramps the motors to the speed parameter given.<br>
Added reverse function which comply's with ESC's reverse protocol.<br>
To reverse: brake->graidentControl to midpoint->graidentControl to desired reverse speed.<br>
**NOTE** The motors were still unable to reverse until the LED Program Card was used to set the ESCs<br>
into forward-reverse-brake mode for more information on modes refer to ESC user manual section 3 "Programable Items".

## V1_0

Added serial communcation with BTP Raspberry Pi to detemine when the BTP shold move to another station.<br>
Full cycle funcationality without arm integration.