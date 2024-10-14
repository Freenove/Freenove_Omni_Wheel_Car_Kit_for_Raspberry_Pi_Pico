#include "Motor.h"
#include "Arduino.h"

#include "PID.h"
#include "Encoder.h"

extern const int wheel1_A_pin = 9; // Motor1 drive pin
extern const int wheel1_B_pin = 8; // Motor1 drive pin

extern const int wheel2_A_pin = 12;// Motor2 drive pin
extern const int wheel2_B_pin = 13;// Motor2 drive pin

extern const int wheel3_A_pin = 15;// Motor3 drive pin
extern const int wheel3_B_pin = 14;// Motor3 drive pin

extern int angle,turn_output,angle_flag,angle_state,angle_temp,state,angle_a_out;

int v1,v2,v3,vx,vy,pid_v1,pid_v2,pid_v3,angle_v;
int pid1,pid2,pid3;
int a1,b1,a2,b2,a3,b3;
float L = 0.10;
int angle_head,angle_counter,location_state;

// Motor initialization
void Motor_init()
{
  analogWriteFreq(16000);// Set the appropriate trolley frequency
  pinMode(wheel1_A_pin,OUTPUT); 
  pinMode(wheel1_B_pin,OUTPUT);
  pinMode(wheel2_A_pin,OUTPUT);
  pinMode(wheel2_B_pin,OUTPUT);
  pinMode(wheel3_A_pin,OUTPUT);
  pinMode(wheel3_B_pin,OUTPUT);
}

void car_stop()
{
  analogWrite(wheel1_A_pin,LOW);
  analogWrite(wheel1_B_pin,LOW);

  analogWrite(wheel2_A_pin,LOW);
  analogWrite(wheel2_B_pin,LOW);

  analogWrite(wheel3_A_pin,LOW);
  analogWrite(wheel3_B_pin,LOW);
}

void Motor_direction()// Motor limiting and motor output orientation
{
  if(pid_v1 > 255) pid_v1 = 255;else if(pid_v1 < -255) pid_v1 = -255;
  if(pid_v1 > 0){ a1 = pid_v1; b1 = LOW; }else{ a1 = LOW; b1 = -pid_v1;}
  if(pid_v2 > 255) pid_v2 = 255;else if(pid_v2 < -255) pid_v2 = -255;
  if(pid_v2 > 0){ a2 = pid_v2; b2 = LOW; }else{ a2 = LOW; b2 = -pid_v2;}
  if(pid_v3 > 255) pid_v3 = 255;else if(pid_v3 < -255) pid_v3 = -255;
  if(pid_v3 > 0){ a3 = pid_v3; b3 = LOW; }else{ a3 = LOW; b3 = -pid_v3;}

  analogWrite(wheel1_A_pin,a1);
  analogWrite(wheel1_B_pin,b1);

  analogWrite(wheel2_A_pin,a2);
  analogWrite(wheel2_B_pin,b2);

  analogWrite(wheel3_A_pin,a3);
  analogWrite(wheel3_B_pin,b3);
}

void Turn_Control(int speed_v,int speed_a,int angle_v,int location)
{
  vx = - speed_v * sin ((angle_flag - angle_head + speed_a) * (PI / 180));
  vy = speed_v * cos ((angle_flag - angle_head + speed_a) * (PI / 180));
  if(angle_state == 0)
  {
    if( location >= 0 && location < 180)
    {
      angle_v = -angle_v;
    }
  }else if(angle_state == 1){// Push left joysticks only
    angle_v = Angle_PID_Realize(angle_flag,angle);// Angle PID control
    if(angle_v > 30) angle_v = 30;else if(angle_v < -30) angle_v = -30;
  }else if(angle_state == 2){// Push the left and right joysticks at the same time
    angle_counter++;
    if(angle_counter < 3)
    {
      if( location > 0 && location < 180)
      {
        angle_v = -angle_v;
        location_state = 1;
      }
      if( location > -180 && location < 0)
      {
        location_state = 2;
      }
      v1 = angle_v;
      v2 = angle_v;
      v3 = angle_v;
    }
    else if(angle_counter < 4)
    {
      angle_flag = angle;
      if(location_state == 1)
        angle_flag -= 10;
      else if(location_state == 2)
        angle_flag += 20;
      vx =  - speed_v * sin ( (angle_flag - angle_head + speed_a) * PI / 180 );
      vy =    speed_v * cos ( (angle_flag - angle_head + speed_a) * PI / 180 );
      location_state = 0;
      v1 = -vx;
      v2 = 0.5 * vx - sqrt(3) / 2 * vy;
      v3 = 0.5 * vx + sqrt(3) / 2 * vy;
    }
    else{
      angle_counter = 0;
    }
  }  
  if(angle_state != 2)
  {
    v1 = -vx + angle_v;
    v2 = 0.5 * vx - sqrt(3) / 2 * vy + angle_v;
    v3 = 0.5 * vx + sqrt(3) / 2 * vy + angle_v;
  }
  // Speed PID control
  pid_v1 = Speed1_PID(v1,speed1);
  pid_v2 = Speed2_PID(v2,speed2);
  pid_v3 = Speed3_PID(v3,speed3);
  Motor_direction();
}