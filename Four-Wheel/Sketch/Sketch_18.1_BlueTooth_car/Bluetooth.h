#ifndef _BlueTooth_H_
#define _BlueTooth_H_

#include <Arduino.h>
#include "SoftwareSerial.h"
#include "Control.h"
#include "Ultrasonic.h"
#include "Pico_W.h"
#include "LED.h"
#include "Motor.h"

#define UPLOAD_ultra_TIME 100
#define UPLOAD_VOL_TIME 3000

extern int bluetooth_state;
extern int imu_state;
extern int state;
extern bool isClockwise;
extern int Circle_r;

void Bluetooth_Control();
void BlueTooth_Init();
void Get_Command(String inputStringTemp);
#endif