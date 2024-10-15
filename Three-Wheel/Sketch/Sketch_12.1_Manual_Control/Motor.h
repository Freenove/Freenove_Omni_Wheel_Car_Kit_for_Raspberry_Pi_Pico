#ifndef __Motor_H__
#define __Motor_H__

#define ENTER               '\n'           //ENTER
#define INTERVAL_CHAR       '#'            //INTERVAL CHAR

#define CMD_LED_Bluetooth    "C"           //Car control command
#define CMD_MOTOR_Bluetooth  "A"           //Car control command
#define CMD_BUZZER_Bluetooth "D"           //Car control command
#define CMD_STATE_Bluetooth  "M"           //Car control command
#define CMD_CIRCLE           "B"           //Car control command

const char INTERVAL_CHAr[1] = {'#'};

void Motor_init();
void Turn_Control(int speed_v,int speed_a,int angle_v,int location);
void Motor_direction();

#endif