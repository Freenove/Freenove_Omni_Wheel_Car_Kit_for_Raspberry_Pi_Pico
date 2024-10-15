/**********************************************************************************
  Filename    : Sketch_15.1_Around_car
  Description : Use Raspberry Pi Pico Realize the circular rotation mode of the car
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

#define UPLOAD_ultra_TIME 100
#define UPLOAD_VOL_TIME 3000
long int lastUploadVoltageTime;
long int lastUploadultraTime;
#define UPLOAD_LIGHTVOL_TIME 500
long int lastUploadLIGHTADCTime;
#define UPLOAD_LINEVOL_TIME 200
long int lastUploadLINEVOLTime;
long int lastUploadSONARVOLTime;
int buzzer_frequency = 0;
int CAR_MODE_VOL = 0;
int LASt_CAR_MODE_VOL = 0;
int state_counter = 0, state = 3, num, turn_speed, turn_location, turn_conter, angle_temp,bluetooth_state;

String CmdArray[8];
int paramters[8];
int sendOnceModuleCheck = 1;
int cmdFlag, angle_a_out = 200;

int angle_out = 0, speed_out = 0, speed1val, speed2val, speed3val, speed4val;

static bool timer0Stopped = false;

extern int encoder1, encoder2, encoder3,angle_state;

extern int angle, turn_output, angle_v,Ultrasonic_speed,Ultrasonic_angle,Ultrasonic_distance;

int MPU_state = 0, angle_flag, angle_state = 0, angle_map,angle_flagstate = 0;

extern int flagCode, irValue;

extern int angle_hand,angle_absolute;

extern float speed_p, speed_i, speed_d;

float Circle_r;

RPI_PICO_Timer ITimer0(0);

bool timer_1ms_control(struct repeating_timer *t) {
  if (millis() % 5 == 0) {
    Getencoder_Data();
    speed_add();
  }
  if (millis() % 15 == 0) {
    speed_calculate();       // Find the average speed and filter the burrs
    Circle_Control(2,30,clockwise); 
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

  // Timer interrupt setting
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, timer_1ms_control);
}

void loop() {
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