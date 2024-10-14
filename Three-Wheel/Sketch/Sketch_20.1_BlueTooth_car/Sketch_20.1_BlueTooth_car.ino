#include "Motor.h"

#include <RPi_Pico_TimerInterrupt.h>

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "Encoder.h"
#include "Angle.h"
#include <Wire.h>
#include "Motor.h"
#include "Pid.h"
#include "SoftwareSerial.h"

#include "WS2812.h"
#include "Pico_W.h"
#include "Ultrasonic.h"
#include "IR.h"
#include "IMU_Fusion_SYC.h"

#define TIMER0_INTERVAL_MS 1000

#define UPLOAD_ultra_TIME 100
#define UPLOAD_VOL_TIME 3000
long int lastUploadVoltageTime;
long int lastUploadultraTime;

int buzzer_frequency = 0;
int CAR_MODE_VOL = 0;
int LASt_CAR_MODE_VOL = 0;
int state_counter = 0, state = 0, num, turn_speed, turn_location, turn_conter, angle_temp,bluetooth_state,angle_lock = 200;

String CmdArray[8];
int paramters[8];

int cmdFlag, angle_a_out = 200;

int angle_out = 0, speed_out = 0, speed1val, speed2val, speed3val, speed4val;

static bool timer0Stopped = false;

extern int angle, turn_output, angle_v,Ultrasonic_speed,Ultrasonic_angle,Ultrasonic_distance;

const int LED_Pin = 28;  // Define RGB color light pins

int imu_state = 0, angle_flag, angle_state = 0, angle_map,angle_flagstate = 0;

extern int flagCode, irValue;

extern int angle_head,angle_absolute;

extern float speed_p, speed_i, speed_d, AccMagnitude;

int Circle_r = 1,location,L = 10,W = 25;

Adafruit_NeoPixel pixels(4, LED_Pin, NEO_GRB + NEO_KHZ800);
RPI_PICO_Timer ITimer0(0);
SoftwareSerial bluetooth(1, 0);

void Car_Control() {
  // Control the translation of the car
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

void Car_2_Control() {
  if (paramters[1] == 0 && paramters[2] == 0) {
    angle_out = 0;
    speed_out = 0;
  }else  // If the parameters are not equal to 0
  {
    angle_out = paramters[1];
    speed_out = paramters[2];
  }
  if(paramters[1] == 0 && paramters[2] == 0 && paramters[3] == 0 && paramters[4] == 0  && abs(AccMagnitude - 0.98) <= 0.2) {
    turn_speed = 0;      
    turn_location = 0;   
    angle_flag = angle;  
    angle_state = 0;
  }else if (paramters[3] == 0 && paramters[4] == 0 && (paramters[1] != 0 || paramters[2] != 0))
  {
    if(angle_flagstate == 1)
    {
      angle_flag = angle;
      angle_flagstate = 0;
    }
    angle_state = 1;
  }else if ((paramters[3] != 0 || paramters[4] != 0) && paramters[1] == 0 && paramters[2] == 0)
  {
    angle_state = 0;
    angle_flag = angle;
    turn_speed = paramters[4];
    turn_location = paramters[3];  
  }
  else{
    angle_state = 2;
    angle_flag = angle;
    turn_speed = paramters[4];      // Rotation speed
    turn_location = paramters[3];   // Direction of rotation
    angle_flagstate = 1;           
  }
}

void Car_3_Control() {
  if (paramters[1] == 0 && paramters[2] == 0) {
    angle_out = 0;
    speed_out = 0;
  } 
  else               // If the parameters are not equal to 0
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
  if(bluetooth_state == 0)
  {
    if (millis() % 5 == 0) {
      Getencoder_Data();
      speed_add();
    }
    if (millis() % 15 == 0) {
      speed_calculate();  // Find the average speed and filter the burrs
      IR_run(IR_v, IR_a, IR_angle_v);
    }
  }else{
  if (state == Manual_Control) {
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
      Turn_Control(speed_out, angle_out, turn_speed, angle_a_out, turn_location, Manual_Control);
    }
  }else if (state == Free_hand) {
    if (millis() % 5 == 0) {
      Getencoder_Data();
      speed_add();
    }
    if (millis() % 10 == 0) {
      imu_state = 1;
    }
    if (millis() % 15 == 0) {
      speed_calculate();  
      Turn_Control(speed_out, angle_out, turn_speed, angle_a_out, turn_location, Free_hand);
    }
  }else if (state == Lock_hand) {
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
        Turn_Control(speed_out, angle_out, turn_speed ,angle_a_out ,turn_location ,Lock_hand);
      }
  }else if (state == Around_mode) {
      if (millis() % 5 == 0) {
        Getencoder_Data();
        speed_add();
      }
      if (millis() % 15 == 0) {
        speed_calculate();     
        Circle_Control(Circle_r,30,clockwise); 
      }
  }else if(state == Ultrasonic_mode) {
      if (millis() % 5 == 0) {
        Getencoder_Data();
        speed_add();
      }
      if(millis() % 10 == 0)
      {
        imu_state = 1;
      }
      if (millis() % 15 == 0) {
        speed_calculate();    // Find the average speed and filter the burrs
        if(Ultrasonic_angle > 360)
          Ultrasonic_angle -= 360;
        if(Ultrasonic_angle < 0)
          Ultrasonic_angle += 360;
        Ultrasonic_Run(Ultrasonic_speed, Ultrasonic_angle);
      }
    }
  }
  // 5ms detects and accumulates the encoder data once
  return true;
}

void setup() {
  // put your setup code here, to run once:
  // Set the serial port baud rate to 9600
  Serial.begin(9600);
  bluetooth.begin(115200);

  // RGB initialization
  pixels.begin();
  
  // RGB Sets the brightness
  pixels.setBrightness(20);

  // Encoder initialization
  Encoder_Init();

  IMU_Init();

  // Buzzer initialization
  Buzzer_Setup();

  // WS2812 initialization
  WS2812_Setup();

  // Motor initialization
  Motor_init();  

  // Ultrasonic initialization
  Ultrasonic_init();  

  // IR initialization
  IR_Init(22);  

  int current_millis = millis();
  while(millis() - current_millis < 500)
  {
    IMU_GetData();
    angle_head = angle;
  }

  tone(PIN_BUZZER, 2000, 200);

  // Timer interrupt Settings
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, timer_1ms_control);
}

void loop() {
  IR_Receive();
  IR_Task();
  WS2812_Show(ws2812_task_mode);
  // Bluetooth processing
  if (bluetooth.available() > 0) {              // If the serial port receives data
    String message = bluetooth.readStringUntil('\n'); // Get the transferred instruction
    if(message == "CONNECT OK\r")
    {
      bluetooth_state = 1;
      state = 0;
      car_stop();
      Serial.println("Bluetooth connected");
    }
  }
  while(bluetooth_state)
  {
    if(state == Ultrasonic_mode)
      Ultrasonic_control();
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
        car_stop();
        if(state == Free_hand)
        {
          angle_head = angle;
          angle_flag = angle_head;
        }
      }
      if(CmdArray[0] == CMD_CIRCLE)
      {
        Circle_r = paramters[2] / 10;
        location = paramters[1];
      }
      if(CmdArray[0] == CMD_LED_Bluetooth)
      {
        WS2812_SetMode(paramters[1]);
        WS2812_Set_Color_1(255, paramters[2], paramters[3], paramters[4]);
      }
      if(CmdArray[0] == CMD_MOTOR_Bluetooth)
      {
        if(state == Manual_Control)
          Car_Control();
        if(state == Free_hand)
          Car_2_Control();
        if(state == Lock_hand)
          Car_3_Control();
      }
      if(CmdArray[0] == CMD_BUZZER_Bluetooth)
      {
        tone(PIN_BUZZER, paramters[1]);
      }
      memset(CmdArray, 0, sizeof(CmdArray));
      memset(paramters, 0, sizeof(paramters));
    }
    if (millis() - lastUploadVoltageTime > UPLOAD_VOL_TIME) {
        int Battery = analogRead(27)*3.3/1023*4*1000;
        bluetooth.print("F#" + String(Battery) + '#' +"\n");
        lastUploadVoltageTime = millis();
    }
    if (millis() - lastUploadultraTime > UPLOAD_ultra_TIME) {
        bluetooth.print("E#" + String(Ultrasonic_distance) + '#' + "\n");
        lastUploadultraTime = millis();
    }
    WS2812_Show(ws2812_task_mode);
  }
  if (imu_state == 1) {
    IMU_GetData();
    imu_state = 0;
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
