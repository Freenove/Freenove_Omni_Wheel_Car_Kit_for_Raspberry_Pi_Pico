/*
* Sketch_18.1_BlueTooth_car.ino
*
* This sample code demonstrates multi-functional control for an omnidirectional smart car, 
* compatible with both three-wheel and four-wheel omnidirectional structural designs (Switching is implemented via the Car_4_wheel macro in Motor.h). 
* Key functional modules include: Manual Control, Free head mode, Lock head mode, Around mode, Ultrasonic mode, and RGB lighting effects display.
* The code adopts a modular architecture design, enabling seamless switching between three-wheel and four-wheel control algorithms through conditional compilation. 
* You can select the appropriate mode for functional verification and debugging based on their actual hardware configuration.
* 
* Author: [Vegetable-SYC]
* Date:   [2025-04-30]
*/

#include "Bluetooth.h"
#include "User_TIM.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                 // Set the serial port baud rate to 9600
  BlueTooth_Init();                   // Encoder initialization
  Encoder_Init();
  IMU_Init();
  Buzzer_Setup();                     // Buzzer initialization
  WS2812_Setup();                     // WS2812 initialization
  Motor_init();                       // Motor initialization
  Ultrasonic_init();                  // Ultrasonic initialization
  IR_Init();                          // IR initialization
  uint32_t current_millis = millis();
  while (millis() - current_millis < 500) {
    IMU_GetData();
  }
  tone(PIN_BUZZER, 2000, 200);
  tim_init();
}

void loop() {
  IR_Task();
  WS2812_Show(ws2812_task_mode);
  Bluetooth_Control();
}