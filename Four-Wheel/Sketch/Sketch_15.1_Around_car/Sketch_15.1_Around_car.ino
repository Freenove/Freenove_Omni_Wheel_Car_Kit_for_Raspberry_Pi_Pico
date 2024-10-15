/**********************************************************************************
  Filename    : Sketch_15.1_Around_car
  Description : Use Raspberry Pi Pico Realize the Around mode of the car
  Auther      : www.freenove.com
  Modification: 2024/08/23
***********************************************************************************/
#include "Motor.h"

#include <RPi_Pico_TimerInterrupt.h>

#include <Arduino.h>

#include "Encoder.h"
#include <Wire.h>
#include "Motor.h"
#include "Pid.h"

#define TIMER0_INTERVAL_MS 1000

int angle_out = 0, speed_out = 0, speed1val, speed2val, speed3val, speed4val;

float Circle_r;

int r = 20;

RPI_PICO_Timer ITimer0(0);

bool timer_1ms_control(struct repeating_timer *t) {
  if (millis() % 5 == 0) {
    Getencoder_Data();
    speed_add();
  }
  if (millis() % 15 == 0) {
    speed_calculate();       // Find the average speed and filter the burrs
    Circle_Control(2, 40, clockwise);
  }
  return true;
}

void setup() {
  // put your setup code here, to run once:
  // Set the serial port baud rate to 9600
  Serial.begin(9600);

  // Encoder initialization
  Encoder_Init();

  Motor_init();  // Motor initialization

  delay(1000);

  // Timer interrupt setting
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, timer_1ms_control);
}

void loop() {
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