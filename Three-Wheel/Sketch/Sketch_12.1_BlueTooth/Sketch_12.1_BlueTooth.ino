/**********************************************************************
  Filename    : Sketch_11.1_BlueTooth
  Description : Use Raspberry Pi Pico Read the information received by 
                Bluetooth and print it on the serial port monitor
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/

#include <Wire.h>
#include "SoftwareSerial.h"

SoftwareSerial bluetooth(1, 0);// Set pin TX to GP1 and pin RX to GP0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);     // Set the baud rate of the serial port to 9600
  bluetooth.begin(115200);// Set the baud rate of the Bluetooth to 115200
}

void loop() {
  // put your main code here, to run repeatedly:
  if (bluetooth.available() > 0) {
    Serial.println(bluetooth.readString());// Print the received characters
  }
}