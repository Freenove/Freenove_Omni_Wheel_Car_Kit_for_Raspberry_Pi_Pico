#ifndef __Motor_h__
#define __Motor_h__

#define clockwise     1
#define anticlockwise 2 

void Motor_init();
void Circle_Control(float r, float V, float location);
void Motor_direction();

#endif