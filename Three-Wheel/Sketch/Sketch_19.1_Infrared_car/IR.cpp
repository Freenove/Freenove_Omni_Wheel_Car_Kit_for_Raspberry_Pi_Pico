#include "IR.h" 
#include "Arduino.h"
#include "Motor.h"
#include "WS2812.h"

int logList[32]; 
unsigned long startTime; 
int endTime, end2Time,irPin,IR_v ,IR_a,IR_angle_v; 
int flagCode = 0,irValue; 
bool irState = true; 
extern int state;
 
void IR_Init(int pin) { 
  irPin = pin; 
  pinMode(irPin, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(irPin), IR_Read, CHANGE); 
} 
 
void IR_Read() 
{ 
  if (irState == true) { 
    unsigned long lowTime, highTime, intervalTime; 
    int num = 0; 
    while (digitalRead(irPin) == LOW) { 
      startTime = micros(); 
      while (digitalRead(irPin) == LOW) { 
        lowTime = micros(); 
      } 
      intervalTime = lowTime - startTime; 
      while (digitalRead(irPin) == HIGH) { 
        highTime = micros(); 
        intervalTime = highTime - lowTime; 
        if (intervalTime > 10000) { 
          end2Time = millis(); 
          if (num == 32) { 
            flagCode = 1; 
            endTime = millis(); 
          } 
          else if (num == 0 && end2Time - endTime > 300 && end2Time - endTime < 400) { 
            flagCode = 2; 
            endTime = millis(); 
          } 
          return; 
        } 
      } 
      if (intervalTime < 2000) { 
        if (intervalTime < 700) { logList[num ++] = 0; 
        } 
        else { 
          logList[num ++] = 1; 
        } 
      } 
    } 
  } 
}

unsigned long IR_Decode(int &code) { 
  unsigned long irData = 0; 
  irState=false; 
  if (code == 1) { 
    code = 0; 
    for (int i = 0; i < 32; i ++) { 
      if (logList[i] == 0) { 
        irData <<= 1; 
      } 
      else { 
        irData <<= 1; 
        irData ++; 
      } 
      logList[i] = 0; 
    } 
 
  } 
  if (code == 2) { 
    code = 0; 
    irData = 0xffffffff; 
  } 
  return irData; 
} 
 
void IR_Release(){ 
  irState=true; 
}

void IR_Receive()
{
  if(flagCode){ 
    irValue = IR_Decode(flagCode); 
    Serial.println(irValue, HEX); 
    IR_Release(); 
  } 
}

void IR_Task()
{
  switch (irValue) {
      case 0xFF02FD:  // go straight
        IR_v = 30;
        IR_a = 0;
        IR_angle_v = 0;
        break;
      case 0xFFE01F:  // Pan to the left
        IR_v = 30;
        IR_a = 90;
        IR_angle_v = 0;
        break;
      case 0xFF906F:  // Pan to the right
        IR_v = 30;
        IR_a = -90;
        IR_angle_v = 0;
        break;
      case 0xFF9867:  // Back up
        IR_v = 30;
        IR_a = 180;
        IR_angle_v = 0;
        break;
      case 0xFFA857:  // Stop
        IR_v = 0;
        IR_a = 0;
        IR_angle_v = 0;
        break;
      case 0xFF22DD:  // 45 degrees left
        IR_v = 30;
        IR_a = 45;
        IR_angle_v = 0;
        break;
      case 0xFFC23D:  //-45 degrees right
        IR_v = 30;
        IR_a = -45;
        IR_angle_v = 0;
        break;
      case 0xFF6897:  // 135 degrees left
        IR_v = 30;
        IR_a = 135;
        IR_angle_v = 0;
        break;
      case 0xFFB04F:  // -135 degrees right
        IR_v = 30;
        IR_a = -135;
        IR_angle_v = 0;
        break;
      case 0xFFA25D:  // anticlockwise
        IR_v = 0;
        IR_a = 0;
        IR_angle_v = 30;
        break;
      case 0xFFE21D:  // clockwise
        IR_v = 0;
        IR_a = 0;
        IR_angle_v = -30;
        break;
      case 0xFF18E7:
        ws2812_task_mode++;
        if(ws2812_task_mode > 5)
          ws2812_task_mode = 0;
        break;
  }
  irValue = 0;
}