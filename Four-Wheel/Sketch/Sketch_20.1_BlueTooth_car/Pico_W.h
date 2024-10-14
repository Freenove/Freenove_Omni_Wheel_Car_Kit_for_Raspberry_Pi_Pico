#ifndef __PICO_H_
#define __PICO_H_

#include <Arduino.h> 

//Buzzer pin definition             
#define PIN_BUZZER 26                   //Define the pins for the Pico W control buzzer
#define BUZZER_FREQUENCY 2000           //Define the resonant frequency of the buzzer 
void Buzzer_Setup(void);                //Buzzer initialization
void Buzzer_Alert(int beat, int rebeat);//Buzzer alarm function
void freq(int PIN, int freqs, int times); 
void Buzzer_Alarm(bool enable);          //Buzzer alarm function
void Buzzer_Variable(int frequency, int time, int times);

////////////////////Battery drive area/////////////////////////////////////
#define PIN_BATTERY        27        //Set the battery detection voltage pin
#define LOW_VOLTAGE_VALUE  525       //Set the minimum battery voltage
#define BAT_VOL_STANDARD	8.4
#define OA_SPEED_OFFSET_PER_V	3
extern int oa_VoltageCompensationToSpeed;

extern float batteryCoefficient;    //Set the proportional coefficient

int Get_Battery_Voltage_ADC(void);   //Gets the battery ADC value
float Get_Battery_Voltage(void);     //Get the battery voltage value
void Set_Battery_Coefficient(float coefficient);//Set the partial pressure coefficient
void oa_CalculateVoltageCompensation(void);

#endif
