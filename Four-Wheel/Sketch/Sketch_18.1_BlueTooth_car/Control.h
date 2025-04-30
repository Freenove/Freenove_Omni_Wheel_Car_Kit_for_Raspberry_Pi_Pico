#ifndef _Control__H_
#define _Control__H_

#include <Arduino.h>
#include "Angle.h"

extern int paramters[8];
extern int angle_out, speed_out;
extern int turn_speed, turn_location;
extern int angle_flag, angle_state, angle_flagstate;
extern int angle_lock;
extern int angle_a_out;
extern int angle_temp;

void Car_Control();
void Car_2_Control();
void Car_3_Control();

#endif