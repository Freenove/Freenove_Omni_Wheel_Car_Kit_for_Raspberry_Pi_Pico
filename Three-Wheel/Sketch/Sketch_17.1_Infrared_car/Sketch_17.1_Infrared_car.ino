/****************************************************************************
  Filename    : Sketch_17_Infrared_car
  Description : Use Raspberry Pi Pico realize infrared control car
  Auther      : www.freenove.com
  Modification: 2024/08/23
*****************************************************************************/
#include "Motor.h"
#include <RPi_Pico_TimerInterrupt.h>
#include <Arduino.h>
#include "Encoder.h"
#include <Wire.h>
#include "Motor.h"
#include "Pid.h"
#include "LED.h"
#include "IR.h"

#define TIMER0_INTERVAL_MS 1000

int speed1val, speed2val, speed3val;

const int LED_Pin = 28;  // Define RGB color light pins

RPI_PICO_Timer ITimer0(0);
Adafruit_NeoPixel pixels(4, LED_Pin, NEO_GRB + NEO_KHZ800);

bool timer_1ms_control(struct repeating_timer *t) {
  if (millis() % 5 == 0) {
    Getencoder_Data();
    speed_add();
  }
  if (millis() % 15 == 0) {
    speed_calculate();  // Find the average speed and filter the burrs
    IR_run(IR_v, IR_a, IR_angle_v);
  }
  return true;
}

void setup() {
  // put your setup code here, to run once:
  // Set the serial port baud rate to 9600
  Serial.begin(9600);

   // Encoder initialization
  Encoder_Init();

  // RGB initialization
  pixels.begin();
  
  // RGB Sets the brightness
  pixels.setBrightness(20);

  // WS2812 initialization
  WS2812_Setup();

  // Timer interrupt setting
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, timer_1ms_control);

  Motor_init();  // Motor initialization

  IR_Init(22);   // Infrared pin initialization
}

void loop() {
  IR_Receive();
  IR_Task();
  WS2812_Show(ws2812_task_mode);
}

void speed_add() {
  speed1val += speed1;
  speed2val += speed2;
  speed3val += speed3;
}

void speed_calculate() {
  speed1 = map(speed1val / 3,0,48,0,100);
  speed2 = map(speed2val / 3,0,48,0,100);
  speed3 = map(speed3val / 3,0,48,0,100);

  speed1val = 0;
  speed2val = 0;
  speed3val = 0;
}