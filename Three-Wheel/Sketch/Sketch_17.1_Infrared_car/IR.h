#ifndef _INFRARED__H_
#define _INFRARED__H_

extern int IR_v ,IR_a,IR_angle_v;

void IR_Init(int pin);
void IR_Read();
unsigned long IR_Decode(int &code);
void IR_Release();
void IR_Receive(); 
void IR_Task();

#endif