#include "Bluetooth.h"

bool isClockwise;
int state = 0;
char CmdArray[8];
int bluetooth_state = 0;
int imu_state = 0;
long int lastUploadVoltageTime;
long int lastUploadultraTime;
int Circle_r = 1;

SoftwareSerial bluetooth(1, 0);

void BlueTooth_Init()
{
  bluetooth.begin(115200);
  bluetooth.println("AT+NAME=BT05");  // Change Bluetooth device name
  delay(200);
}

void Bluetooth_Control()
{
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
      Ultrasonic_control(20, 1500, 20);
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
        if(state == Ultrasonic_mode)
        {
          Ultrasonic_mutex = 0;
          Ultrasonic_state = 0;
        }
      }
      if(CmdArray[0] == CMD_CIRCLE)
      {
        Circle_r = paramters[2] / 10;
        isClockwise = paramters[1];
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
  CmdArray[0] = token[0];                    // Put the command into an array               
  for (int i = 0; i < 5; i++) {
    if (token != NULL) {
      token = strtok(NULL, INTERVAL_CHAr);
    }
    paramters[i + 1] = atoi(token);
  }
}