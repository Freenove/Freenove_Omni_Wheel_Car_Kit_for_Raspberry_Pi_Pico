#include "Control.h"

int paramters[8];
int angle_out = 0, speed_out = 0;
int turn_speed, turn_location;
int angle_flag, angle_state = 0 ,angle_flagstate = 0;
int angle_lock = 200;
int angle_a_out = 200;
int angle_temp;

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