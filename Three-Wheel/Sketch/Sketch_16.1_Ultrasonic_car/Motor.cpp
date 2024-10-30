#include "Motor.h"
#include "Arduino.h"

#include "PID.h"
#include "Ultrasonic.h"
#include "Encoder.h"

extern const int wheel1_A_pin = 8; // Motor1 drive pin
extern const int wheel1_B_pin = 9; // Motor1 drive pin

extern const int wheel2_A_pin = 12;// Motor2 drive pin
extern const int wheel2_B_pin = 13;// Motor2 drive pin

extern const int wheel3_A_pin = 15;// Motor3 drive pin
extern const int wheel3_B_pin = 14;// Motor3 drive pin

extern int angle,turn_output,angle_flag,angle_state,angle_temp,Ultrasonic_distance,state,angle_a_out;

int v1,v2,v3,v4,vx,vy,pid_v1,pid_v2,pid_v3,pid_v4,angle_v,angle_Ultrasonic,Ultrasonic_speed,Ultrasonic_index,Ultrasonic_mutex = 0,Ultrasonic_state,Ultrasonic_angle,Ultrasonic_delay;
int pid1,pid2,pid3,pid4;
int a1,b1,a2,b2,a3,b3,a4,b4,turn_output,turn_a1,turn_b1,turn_a2,turn_b2,turn_a3,turn_b3,turn_a4,turn_b4;
float L = 0.06;
int angle_head,angle_counter,location_state;
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
}

void Motor_direction()// Motor limiting and motor output 
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

void Ultrasonic_Run(int speed_v,int angle_a)
{
  angle_v = Angle_PID_Realize(angle_a,angle);
  if(angle_v > 80) angle_v = 80;else if(angle_v < -80) angle_v = -80;

  vx =  - speed_v * sin ( 0 * PI / 180 );
  vy =    speed_v * cos ( 0 * PI / 180 );

  v1 = -vx + angle_v;
  v2 = 0.5 * vx - sqrt(3) / 2 * vy + angle_v;
  v3 = 0.5 * vx + sqrt(3) / 2 * vy + angle_v;

  pid_v1 = Speed1_PID(v1,speed1);
  pid_v2 = Speed2_PID(v2,speed2);
  pid_v3 = Speed3_PID(v3,speed3);

  Motor_direction();
}


void Ultrasonic_control()
{
  delay(2);
  if((millis() - Ultrasonic_distance_time) > 20)
  {
    Ultrasonic_distance = Read_Distance();
    Ultrasonic_distance_time = millis();
  }
  if(Ultrasonic_mutex == 0)
  { 
    if(Ultrasonic_distance > 15)// The distance is greater than 15cm
    {
      Ultrasonic_speed = 20;
      Ultrasonic_angle = angle;
    }
    else{
      Ultrasonic_mutex = 1;
      Ultrasonic_speed = 0;
    }
  }
  else{
    if(Ultrasonic_state == 0)// turn left
    {
      Ultrasonic_angle = Ultrasonic_angle - 90;
      Ultrasonic_state = 1;
      Ultrasonic_time = millis();// Obtain the system running time
    }else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 1)// Turn 90 degrees to the left
    {
      Ultrasonic_compare[0] = Read_Distance();
      Ultrasonic_state = 2;
      Ultrasonic_time = millis();
    }else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 2)// Turn right back to the center
    {
      Ultrasonic_angle = Ultrasonic_angle + 90;
      Ultrasonic_state = 3;
      Ultrasonic_time = millis();
    }else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 3)// stop
    {
      Ultrasonic_state = 4;
      Ultrasonic_time = millis();
    }else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 4)// Turn 90 degrees to the right
    {
      Ultrasonic_angle = Ultrasonic_angle + 90;
      
      Ultrasonic_state = 5;
      Ultrasonic_time = millis();
    }
    else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 5)// Turn 90 degrees to the right
    {
      Ultrasonic_compare[1] = Read_Distance();
      Ultrasonic_state = 6;
      Ultrasonic_time = millis();
    }else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 6)
    {
      Ultrasonic_angle = Ultrasonic_angle - 90;// Go back to the middle
      Ultrasonic_state = 7;
      Ultrasonic_time = millis();
    }else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 7)
    {
      Ultrasonic_state = 8;
      Ultrasonic_time = millis();
    }else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 8)
    {
      if(Ultrasonic_compare[0] > Ultrasonic_compare[1])
      {
        Ultrasonic_angle = Ultrasonic_angle - 90;
      }else
      {
        Ultrasonic_angle = Ultrasonic_angle + 90;
      }
      Ultrasonic_state = 9;
      Ultrasonic_time = millis();
    }
    else if((millis() - Ultrasonic_time) > 1500 && Ultrasonic_state == 9)
    {
      Ultrasonic_speed = 20;
      Ultrasonic_state = 0;
      Ultrasonic_mutex = 0;
    }
  }
}