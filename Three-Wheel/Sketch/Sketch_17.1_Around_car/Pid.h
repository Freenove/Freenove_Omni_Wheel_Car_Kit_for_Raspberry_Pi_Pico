#ifndef _Pid_H_
#define _Pid_H_

extern float speed_p,speed_i,speed_d,angle_p,angle_i,angle_d;

float Speed1_PID(float target,float feedback);
float Speed2_PID(float target,float feedback);
float Speed3_PID(float target,float feedback);
float Speed4_PID(float target,float feedback);
float Angle_PID_Realize(float target, float feedback);

#endif