#ifndef _IR_H_
#define _IR_H_

extern int IR_v ,IR_a,IR_angle_v;
extern bool IR_Enable;

extern int flagCode, irValue;

void IR_Init();
void IR_Read();
unsigned long IR_Decode(int &code);
void IR_Release();
int IR_Receive();
void IR_Task();

#endif