#include "Pid.h"
#include "Arduino.h"

float speed_p = 2.0,speed_i = 0,speed_d = 3.4;
float angle_p = 0.95,angle_i = 0 ,angle_d = 3.1;

int speed1_error,speed1_lasterror,speed1_integral,speed1_Maxintegral,speed1_output,speed1_Maxoutput;

int speed2_error,speed2_lasterror,speed2_integral,speed2_Maxintegral,speed2_output,speed2_Maxoutput;

int speed3_error,speed3_lasterror,speed3_integral,speed3_Maxintegral,speed3_output,speed3_Maxoutput;

int Angle_error,Angle_lasterror,Angle_integral,Angle_output;

float Speed1_PID(float target,float feedback)// Primary PID calculation
{
    // Get the error value
    speed1_error = target - feedback;

    if(speed1_error < 1 && speed1_error > -1) speed1_error = 0;// PID with dead zone

    // Integral term accumulation
    speed1_integral += speed1_error;

    // Integral limiting
    if(speed_i * speed1_integral < -speed1_Maxintegral) 
      speed1_integral = -speed1_Maxintegral / speed_i;
    else if(speed_i * speed1_integral > speed1_Maxintegral) 
      speed1_integral = speed1_Maxintegral / speed_i;

    //  Empty points when braking
    if(target == 0) 
      speed1_integral = 0; //  Empty i while braking

    // increasing PID control algorithm
    speed1_output += (speed_p * speed1_error) + (speed_i * speed1_integral)
                   + (speed_d * (speed1_error - speed1_lasterror));

    speed1_lasterror = speed1_error;

    //  Direct output 0 when braking
    if(target == 0) 
      speed1_output = 0; 
    
    return speed1_output;
}
float Speed2_PID(float target,float feedback)//Primary PID calculation
{
    // Get the error value
    speed2_error = target - feedback;

    if(speed2_error < 1 && speed2_error > -1) speed2_error = 0;// PID with dead zone

    // Integral term accumulation
    speed2_integral += speed2_error;
    
    // Integral limiting
    if(speed_i * speed2_integral < -speed2_Maxintegral) 
      speed2_integral = -speed2_Maxintegral / speed_i;
    else if(speed_i * speed2_integral > speed2_Maxintegral) 
      speed2_integral = speed2_Maxintegral / speed_i;

    //  Empty points when braking
    if(target == 0) 
      speed2_integral = 0; //  Empty i while braking

    // increasing PID control algorithm
    speed2_output += (speed_p * speed2_error) + (speed_i * speed2_integral)
                   + (speed_d * (speed2_error - speed2_lasterror));

    speed2_lasterror = speed2_error;

    //  Direct output 0 when braking
    if(target == 0) 
      speed2_output = 0; 
    
    return speed2_output;
}
float Speed3_PID(float target,float feedback)//Primary PID calculation
{
    // Get the error value
    speed3_error = target - feedback;

    if(speed3_error < 1 && speed3_error > -1) speed3_error = 0;// PID with dead zone

    // Integral term accumulation
    speed3_integral += speed3_error;

    // Integral limiting
    if(speed_i * speed3_integral < -speed3_Maxintegral) 
      speed3_integral = -speed3_Maxintegral / speed_i;
    else if(speed_i * speed3_integral > speed3_Maxintegral) 
      speed3_integral = speed3_Maxintegral / speed_i;

    //  Empty points when braking
    if(target == 0) 
      speed3_integral = 0; //  Empty i while braking

    // increasing PID control algorithm
    speed3_output += (speed_p * speed3_error) + (speed_i * speed3_integral)
                   + (speed_d * (speed3_error - speed3_lasterror));

    speed3_lasterror = speed3_error;

    //  Direct output 0 when braking
    if(target == 0) 
      speed3_output = 0; 
    
    return speed3_output;
}
