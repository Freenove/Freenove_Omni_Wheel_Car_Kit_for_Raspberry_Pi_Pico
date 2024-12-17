/**********************************************************************
  Filename    : Sketch_16.1_Ultrasonic_car
  Description : Use Raspberry Pi Pico implement the Ultrasonic car
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
#include "Motor.h"

#include <RPi_Pico_TimerInterrupt.h>

#include "Encoder.h"
#include "Angle.h"
#include <Wire.h>
#include "Motor.h"
#include "Pid.h"

#include "Ultrasonic.h"

#define TIMER0_INTERVAL_MS 1000

int state = 4;

int angle_out = 0, speed1val, speed2val, speed3val, speed4val;

extern int angle,Ultrasonic_speed,Ultrasonic_angle;

int IMU_state = 0;

uint32_t current_millis;

RPI_PICO_Timer ITimer0(0);

bool timer_1ms_control(struct repeating_timer *t) {
  if (millis() % 5 == 0) {
    Getencoder_Data();
    speed_add();
  }
  if(millis() % 10 == 0)
  {
    IMU_state = 1;
  }
  if (millis() % 15 == 0) {
    speed_calculate();    // Find the average speed and filter the burrs
    if(Ultrasonic_angle > 360)
      Ultrasonic_angle -= 360;
    if(Ultrasonic_angle < 0)
      Ultrasonic_angle += 360;
    Ultrasonic_Run(Ultrasonic_speed, Ultrasonic_angle, 50);
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

  Ultrasonic_init();  // Ultrasonic initialization

  IMU_Init();
     
  int current_millis = millis();
  while(millis() - current_millis < 1000)
  {
    IMU_GetData();
  }

  // Timer interrupt setting
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, timer_1ms_control);
}

void loop() {
  Ultrasonic_control(20, 1500, 20);
  if(IMU_state == 1)
  {
    IMU_GetData();
    IMU_state = 0;
  }
}

void speed_add() {
  speed1val += speed1;
  speed2val += speed2;
  speed3val += speed3;
  speed4val += speed4;
}

void speed_calculate() {
  speed1 = map(speed1val / 3,0,50,0,100);
  speed2 = map(speed2val / 3,0,50,0,100);
  speed3 = map(speed3val / 3,0,50,0,100);
  speed4 = map(speed4val / 3,0,50,0,100);

  speed1val = 0;
  speed2val = 0;
  speed3val = 0;
  speed4val = 0;
}