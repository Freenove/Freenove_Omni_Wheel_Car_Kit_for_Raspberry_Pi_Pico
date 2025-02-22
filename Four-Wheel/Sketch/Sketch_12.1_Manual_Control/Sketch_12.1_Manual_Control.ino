/**********************************************************************
  Filename    : Sketch_12.1_Manual_Control
  Description : Use Raspberry Pi Pico implement the Manual control of the car
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
#include <RPi_Pico_TimerInterrupt.h>

#include <Arduino.h>
#include "Motor.h"
#include "Encoder.h"
#include <Wire.h>
#include "Pid.h"
#include "SoftwareSerial.h"

#define TIMER0_INTERVAL_MS 1000

int turn_speed, turn_location;

String CmdArray[8];
int paramters[8];
int bluetooth_state = 0, angle_out;

int  speed_out = 0, speed1val, speed2val, speed3val, speed4val;

RPI_PICO_Timer ITimer0(0);
SoftwareSerial bluetooth(1, 0);

void Car_Control() {
  if (paramters[1] == 0 && paramters[2] == 0) {
    angle_out = 0;
    speed_out = 0;
  }else  // If the parameters are not equal to 0
  {
    angle_out = paramters[1];
    speed_out = paramters[2];
  }
  if (paramters[1] == 0 && paramters[2] == 0 && paramters[3] == 0 && paramters[4] == 0) {
    turn_speed = 0;     
    turn_location = 0;   
  } else {
    turn_speed = paramters[4];     // Rotation speed
    turn_location = paramters[3];  // Direction of rotation
  }
}

bool timer_1ms_control(struct repeating_timer *t) {
  // 5ms detects and accumulates the encoder data once
  if (millis() % 5 == 0) {
    Getencoder_Data();
    speed_add();
  }
  if (millis() % 15 == 0) {
    speed_calculate();  // Find the average speed and filter the burrs
    Turn_Control(speed_out, angle_out, turn_speed, turn_location);
  }
  return true;
}
 
void setup() {
  // put your setup code here, to run once:
  // Set the serial port baud rate to 9600
  Serial.begin(9600);
  bluetooth.begin(115200);
  // Encoder initialization
  Encoder_Init();

  bluetooth.println("AT+NAME=BT05");  // Change Bluetooth device name
  delay(200);

  // Timer interrupt setting
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, timer_1ms_control);

  Motor_init();  // Motor initialization
}

void loop() {
  // Bluetooth processing
  if (bluetooth.available() > 0) {              // If the serial port receives data
    String message = bluetooth.readStringUntil('\n'); // Get the transferred instruction
    if(message == "CONNECT OK\r")
    {
      bluetooth_state = 1;
      Serial.println("Bluetooth connected");
    }
  }
  while(bluetooth_state)
  {
    if (bluetooth.available() > 0) {                    // If the serial port receives data
      String message = bluetooth.readStringUntil('\n'); // Get the transmitted instruction
      Serial.println(message);                          // print order

      Get_Command(message);
      if(message == "DISCONNECT\r")
      {
        bluetooth_state = 0;
        Serial.println("Bluetooth disconnected");
      }
      if(CmdArray[0] == CMD_MOTOR_Bluetooth)
      {
          Car_Control();
      }
      memset(CmdArray, 0, sizeof(CmdArray));
      memset(paramters, 0, sizeof(paramters));
    }
  }
}

void Get_Command(String inputStringTemp) {
  // Gets the instruction length
  int string_length = inputStringTemp.length() + 1;
  char str[string_length];
  // Converts an array of String type to a Char type array
  inputStringTemp.toCharArray(str, string_length);
  char *token = strtok(str, INTERVAL_CHAr);  // Split the array
  CmdArray[0] = String(token[0]);                    // Put the command into an array
  for (int i = 0; i < 5; i++) {
    if (token != NULL) {           
      // Continue splitting the array until # is not detected
      token = strtok(NULL, INTERVAL_CHAr);
    }
    // Convert the instruction to integer type
    paramters[i + 1] = atoi(token);  
  }
}

void speed_add() {
  speed1val += speed1;
  speed2val += speed2;
  speed3val += speed3;
  speed4val += speed4;
}

void speed_calculate() {
  speed1 = map(speed1val / 3,0,48,0,100);
  speed2 = map(speed2val / 3,0,48,0,100);
  speed3 = map(speed3val / 3,0,48,0,100);
  speed4 = map(speed4val / 3,0,48,0,100);

  speed1val = 0;
  speed2val = 0;
  speed3val = 0;
  speed4val = 0;
}