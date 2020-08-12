## Overview
`Simple glitch` is the demo program used to show case basic functionality of a clock glitch attack on the Arty S7 FPGA running OpenTitan design. 
The password check routine has a termination condition that is true when a wrong character is received. This condition leads to timing differences for different inputs.
This timing differences is used to guess any password stored on the device.

The program does 2 main thing:
* Usually the program sticks in a while true loop
* A successful glitch allows to break out of this loop and prints '1234'.


## Stuck to a while true loop
First some prints are made to indicate start.
The program should stick in a while true loop and never be able to exist it.

## Clock glitch
Give a response whether password was correct or not.

## Show access via LEDs
LEDs indicate whether password was correct or not.
