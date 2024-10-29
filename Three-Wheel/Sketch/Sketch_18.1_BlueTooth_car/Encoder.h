#ifndef __encoder_H__
#define __encoder_H__

extern int speed1,speed2,speed3;
extern int encoder1, encoder2, encoder3,v3,speed3,v1;

void Getencoder();
void Encoder_Init();
void Getencoder_Data();
void speed_calculator();

#endif
