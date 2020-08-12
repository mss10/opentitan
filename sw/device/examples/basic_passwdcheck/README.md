## Overview
`Basic password check` is the demo program used to show case basic functionality of a simple power side-channel attack on the Arty S7 FPGA running OpenTitan design.
The Chipwhisperer is triggered via GPIO of the FPGA and starts to capture power trace. Then the password check routine is executed that has a termination condition. 
The routine stops when a wrong character is received. This condition leads to timing differences for different inputs.
This timing differences and the knowledge about the length of the password is used to guess any password stored on the device.

The program does 4 main things:
* Trigger the Chipwhisperer
* Read a password via UART.
* Reply via UART whether the received password was correct or not.
* Show access via LEDs

## Read a password via UART
First some prints are made to indicate start.
This function is able to receive a null-terminated ('\0') password via UART.

## Reply via UART
Give a response whether password was correct or not.

## Show access via LEDs
LEDs indicate whether password was correct or not.
