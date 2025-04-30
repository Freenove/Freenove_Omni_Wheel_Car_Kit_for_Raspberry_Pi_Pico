#ifndef __encoder_H__
#define __encoder_H__

extern const int wheel1_A_pin;  // Motor1 drive pin
extern const int wheel1_B_pin;  // Motor1 drive pin

extern const int wheel2_A_pin;  // Motor2 drive pin
extern const int wheel2_B_pin;  // Motor2 drive pin

extern const int wheel3_A_pin;  // Motor3 drive pin
extern const int wheel3_B_pin;  // Motor3 drive pin

extern const int wheel4_A_pin;// Motor4 drive pin
extern const int wheel4_B_pin;// Motor4 drive pin

extern int speed1,speed2,speed3,speed4;
extern int encoder1, encoder2, encoder3, encoder4;
extern int speed1val, speed2val, speed3val, speed4val;

void Getencoder();
void Encoder_Init();
void Getencoder_Data();
void speed_calculator();
void speed_add();
void speed_calculate();

#endif
