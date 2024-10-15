#include "Motor.h"
#include "Arduino.h"

#include "PID.h"
#include "Encoder.h"

extern const int wheel1_A_pin = 8;
extern const int wheel1_B_pin = 9;

extern const int wheel2_A_pin = 13;
extern const int wheel2_B_pin = 12;

extern const int wheel3_A_pin = 15;
extern const int wheel3_B_pin = 14;

extern const int wheel4_A_pin = 20;
extern const int wheel4_B_pin = 21;

extern int angle,turn_output,angle_flag,angle_state,angle_temp,state,angle_a_out;

int v1,v2,v3,v4,vx,vy,pid_v1,pid_v2,pid_v3,pid_v4,angle_v,angle_circle;
int pid1,pid2,pid3,pid4;
int a1,b1,a2,b2,a3,b3,a4,b4,turn_output,turn_a1,turn_b1,turn_a2,turn_b2,turn_a3,turn_b3,turn_a4,turn_b4;
float L = 0.06;
int angle_hand,angle_counter,location_state;
uint32_t Ultrasonic_time,Ultrasonic_distance_time;
int Ultrasonic_compare[2];

// Motor initialization
void Motor_init()
{
  analogWriteFreq(16000);// Set the appropriate PWM frequency
  pinMode(wheel1_A_pin,OUTPUT); 
  pinMode(wheel1_B_pin,OUTPUT);
  pinMode(wheel2_A_pin,OUTPUT);
  pinMode(wheel2_B_pin,OUTPUT);
  pinMode(wheel3_A_pin,OUTPUT);
  pinMode(wheel3_B_pin,OUTPUT);
  pinMode(wheel4_A_pin,OUTPUT);
  pinMode(wheel4_B_pin,OUTPUT);
}

void Motor_direction()// Motor limiting and motor output orientation
{
  if(pid_v1 > 255) pid_v1 = 255;else if(pid_v1 < -255) pid_v1 = -255;
  if(pid_v1 > 0){ a1 = pid_v1; b1 = LOW; }else{ a1 = LOW; b1 = -pid_v1;}
  if(pid_v2 > 255) pid_v2 = 255;else if(pid_v2 < -255) pid_v2 = -255;
  if(pid_v2 > 0){ a2 = pid_v2; b2 = LOW; }else{ a2 = LOW; b2 = -pid_v2;}
  if(pid_v3 > 255) pid_v3 = 255;else if(pid_v3 < -255) pid_v3 = -255;
  if(pid_v3 > 0){ a3 = pid_v3; b3 = LOW; }else{ a3 = LOW; b3 = -pid_v3;}
  if(pid_v4 > 255) pid_v4 = 255;else if(pid_v4 < -255) pid_v4 = -255;
  if(pid_v4 > 0){ a4 = pid_v4; b4 = LOW; }else{ a4 = LOW; b4 = -pid_v4;}

  analogWrite(wheel1_A_pin,a1);
  analogWrite(wheel1_B_pin,b1);

  analogWrite(wheel2_A_pin,a2);
  analogWrite(wheel2_B_pin,b2);

  analogWrite(wheel3_A_pin,a3);
  analogWrite(wheel3_B_pin,b3);

  analogWrite(wheel4_A_pin,a4);
  analogWrite(wheel4_B_pin,b4);
}

void Circle_Control(float r, float V, float location)
{
  if(location == clockwise)
  {
    angle_circle = V / r;
    vx = - V * sin (90 * (PI / 180));
    vy = V * cos (90 * (PI / 180));
    v1 = -vx + angle_circle;
    v2 = -vy + angle_circle;
    v3 = vx + angle_circle;
    v4 = vy + angle_circle;
  }
  if(location == anticlockwise)
  {
    angle_circle = V / r;
    vx = - V * sin (-90 * (PI / 180));
    vy = V * cos (-90 * (PI / 180));
    v1 = -vx + angle_circle;
    v2 = -vy + angle_circle;
    v3 = vx + angle_circle;
    v4 = vy + angle_circle;
  }
  // Speed PID control
  pid_v1 = Speed1_PID(v1,speed1);
  pid_v2 = Speed2_PID(v2,speed2);
  pid_v3 = Speed3_PID(v3,speed3);
  pid_v4 = Speed4_PID(v4,speed4);

  Motor_direction();// Motor limiting and motor output orientation
}