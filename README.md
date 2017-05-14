# Prezi-Bee
  INTRODUCTION
 Prezi-Bee is a hardware device(A glove) that is used to control movement of slides of a presentation using hand gestures detection by the use of an IMU sensor on the glove. The hardware device detects a gesture and sends data to the application running on the PC to perform a function, like left arrow click.

 HARDWARE COMPONENTS 
 The glove also has a bluetooth module(HC-05), an ATmega8, a MPU6050 and a 5V Li-po battery along.
The code and the libraries are written for ATmega8 and Windows PC.
Pre-requisites 
1.Microsoft Visual Studio 2016 or higher should be installed in your PC.
2.Atmel Studio or any other IDE for embedded coding.

Basics
The baud rate of the device to communicate via USART is set at 9600. To run the code, add TWI_8.h, TWI_8.c, USART_8.h, USART_8.c and main.c  file to the project. The communication between the MPU6050 and the ATmega8 is via TWI whereas between PC and ATmega is via USART.

A single dimensional Kalman filter is used to remove the noise coming from the MPU accelerometer

How to open the software?

->Open "Serial Port Interface" vb file with the Microsoft Visual Studio 2016
How to setup the program?
->1.Click on the “Start button on the bar above”
  2.A window “Form 1” pops up.

  3.Set the baud rate to “9600”
  4.Select the COM Port (refer to #)
  5.CLick on "Connect" button
  6.A new window pops up which displays the ->
	* Current co-ordinates and the new co-ordinates of the cursor
	* The received data

7.Now the software is all set



#How to know the COM Port?
->1.Open the bluetooth settings of your pc

2.You will get a screen with various tabs

3.Select the COM Port Tab and the you will be looking at this

4.Keep in mind the COM port no. for “outgoing” direction.

