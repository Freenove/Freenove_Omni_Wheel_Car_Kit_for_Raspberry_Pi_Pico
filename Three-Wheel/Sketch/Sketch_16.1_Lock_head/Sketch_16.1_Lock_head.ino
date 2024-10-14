/************************************************************************
  Filename    : Sketch_15.1_Lock_hand
  Description : Use Raspberry Pi Pico implement the Lock_hand of the car
  Auther      : www.freenove.com
  Modification: 2024/08/23
************************************************************************/
#include "Motor.h"

#include <RPi_Pico_TimerInterrupt.h>

#include "Encoder.h"
#include "Angle.h"
#include <Wire.h>
#include "Motor.h"
#include "Pid.h"
#include "SoftwareSerial.h"

#include "IMU_Fusion_SYC.h"

#define TIMER0_INTERVAL_MS 1000

const int PIN_BUZZER = 26;

int state_counter = 0, state = 0, num, turn_speed, turn_location, turn_conter, angle_temp,bluetooth_state,angle_lock = 200;

String CmdArray[8];
int paramters[8];
int sendOnceModuleCheck = 1;
int cmdFlag, angle_a_out = 200;

int angle_out = 0, speed_out = 0, speed1val, speed2val, speed3val;

static bool timer0Stopped = false;

extern int encoder1, encoder2, encoder3,angle_state;

extern int angle, turn_output, angle_v;

const int LED_Pin = 28;  // Define RGB color light pins

int imu_state = 0, angle_flag, angle_state = 0, angle_map,angle_flagstate = 0;

extern int flagCode, irValue;

extern int angle_head,angle_absolute;

extern float speed_p, speed_i, speed_d, accMagnitude;

int Circle_r,location;
RPI_PICO_Timer ITimer0(0);
SoftwareSerial bluetooth(1, 0);

void Car_3_Control() {
  if (paramters[1] == 0 && paramters[2] == 0) {
    angle_out = 0;
    speed_out = 0;
  } 
  else//If the parameters are not equal to 0
  {
    angle_out = paramters[1];
    speed_out = paramters[2];
  }
  if(paramters[1] == 0 && paramters[2] == 0 && paramters[3] == 0 && paramters[4] == 0)
  {
    angle_a_out = angle_lock;
  }
  else{
    angle_temp = paramters[3];
    if(angle_temp > -180 && angle_temp <= 0)
    {
      angle_a_out = map(angle_temp,0,-180,angle_lock,angle_lock + 180);
      if(angle_a_out > 360) 
      {
        angle_a_out -= 360;
      }
    }
    if(angle_temp >= 0 && angle_temp < 180)
    {
      angle_a_out = map(angle_temp,0,180,angle_lock,angle_lock - 180); 
    }
  }
}

bool timer_1ms_control(struct repeating_timer *t) {
  // 5ms detects and accumulates the encoder data once
  if (millis() % 5 == 0) {
    Getencoder_Data();
    speed_add();
  }
  // 10ms Detects the current Angle once
  if (millis() % 10 == 0) {
    imu_state = 1;
  }
  if (millis() % 15 == 0) {
    speed_calculate();  // Find the average speed and filter the burrs
    Turn_Control(speed_out, angle_out, turn_speed ,angle_a_out);
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

  IMU_Init();

  // Motor initialization
  Motor_init();  

  int current_millis = millis();
  while(millis() - current_millis < 500)
  {
    IMU_GetData();
  }

  tone(PIN_BUZZER, 2000, 200);

  // Timer interrupt Settings
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, timer_1ms_control);
}

void loop() {
  // Bluetooth processing
  if (imu_state == 1) {
      IMU_GetData();
      imu_state = 0;
  }
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
    if (imu_state == 1) {
      IMU_GetData();
      imu_state = 0;
    }
    if (bluetooth.available() > 0) {                    // If the serial port receives data
      String message = bluetooth.readStringUntil('\n'); // Get the transmitted instruction
      Serial.println(message);                          // print order
      Get_Command(message);
      if(message == "DISCONNECT\r")
      {
        bluetooth_state = 0;
        Serial.println("Bluetooth disconnected");
      }
      if(CmdArray[0] == CMD_STATE_Bluetooth)
      {
        state = paramters[1];
        if(state == 2)
          angle_head = angle;
        car_stop();
      }
      if(CmdArray[0] == CMD_MOTOR_Bluetooth)
      {
        if(state == 2)
          Car_3_Control();
      }
      memset(CmdArray, 0, sizeof(CmdArray));
      memset(paramters, 0, sizeof(paramters));
    }
  }
}

void Get_Command(String inputStringTemp) {
  int string_length = inputStringTemp.length() + 1;
  char str[string_length];
  inputStringTemp.toCharArray(str, string_length);
  char *token = strtok(str, INTERVAL_CHAr); 
  CmdArray[0] = token[0];                  
  for (int i = 0; i < 5; i++) {
    if (token != NULL) {
      token = strtok(NULL, INTERVAL_CHAr);
    }
    paramters[i + 1] = atoi(token);
  }
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
void Buzzer_Setup(void) {
  pinMode(PIN_BUZZER, OUTPUT);
}