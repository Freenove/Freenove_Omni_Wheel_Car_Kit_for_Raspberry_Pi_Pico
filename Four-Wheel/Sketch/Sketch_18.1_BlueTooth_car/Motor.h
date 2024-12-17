#ifndef __Motor_h__
#define __Motor_h__

#define Manual_Control   0
#define Free_hand        1
#define Lock_hand        2
#define Around_mode      3
#define Ultrasonic_mode  4

#define clockwise        0
#define anticlockwise    1

#define ENTER               '\n'                  //ENTER
#define INTERVAL_CHAR       '#'                   //INTERVAL CHAR

#define CMD_LED_Bluetooth    "C"           //Car control command
#define CMD_MOTOR_Bluetooth  "A"           //Car control command
#define CMD_BUZZER_Bluetooth "D"           //Car control command
#define CMD_STATE_Bluetooth  "M"           //Car control command
#define CMD_CIRCLE           "B"           //Car control command

const char INTERVAL_CHAr[1] = {'#'};

void Motor_init();
void car_stop();
void Turn_Control(int speed_v,int speed_a,int angle_v,int angle_a,int location,int mode);
void Ultrasonic_Run(int speed_v, int angle_a, int turn_speed);
void Ultrasonic_control(int speed, int turn_time, int distance);
void IR_run(int speed_v,int speed_a,int angle_v);
void Motor_direction();
void Circle_Control(float r, float V, float location);

#endif