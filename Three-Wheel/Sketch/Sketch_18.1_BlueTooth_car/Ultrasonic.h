#ifndef _Ultrasonic__H_
#define _Ultrasonic__H_

const int Trig = 3;
const int Echo = 2;

extern int Ultrasonic_speed,Ultrasonic_angle,Ultrasonic_distance, Ultrasonic_mutex, Ultrasonic_state;

void Ultrasonic_init();
int Read_Distance();
void Ultrasonic_Run(int speed_v,int speed_a,int angle_v);
#endif