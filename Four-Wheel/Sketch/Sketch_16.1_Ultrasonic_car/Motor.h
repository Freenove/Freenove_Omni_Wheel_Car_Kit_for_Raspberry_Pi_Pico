#ifndef __Motor_h__
#define __Motor_h__

void Motor_init();
void Ultrasonic_control(int speed, int turn_time, int distance);
void Motor_direction();
void Ultrasonic_Run(int speed_v, int angle_a, int turn_speed);

#endif