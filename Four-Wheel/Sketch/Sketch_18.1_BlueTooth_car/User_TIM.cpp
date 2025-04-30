#include "User_TIM.h"

static bool timer0Stopped = false;

RPI_PICO_Timer ITimer0(0);

void tim_init()
{
  // Timer interrupt Settings
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, timer_1ms_control);
}

bool timer_1ms_control(struct repeating_timer *t) {
  if(bluetooth_state == 0)
  {
    if (millis() % 5 == 0) {
      Getencoder_Data();
      speed_add();
    }
    if (millis() % 15 == 0) {
      speed_calculate();  // Find the average speed and filter the burrs
      IR_run(IR_v, IR_a, IR_angle_v);
    }
  }else{
  if (state == Manual_Control) {
    if (millis() % 5 == 0) {
      Getencoder_Data();
      speed_add();
    }
    // 10ms Detects the current Angle once
    if (millis() % 10 == 0) {
      imu_state = 1;
    }
    if (millis() % 15 == 0) {
      speed_calculate();  // Find the average speed and filter the burrs
      Turn_Control(speed_out, angle_out, turn_speed, angle_a_out, turn_location, Manual_Control);
    }
  }else if (state == Free_hand) {
    if (millis() % 5 == 0) {
      Getencoder_Data();
      speed_add();
    }
    if (millis() % 10 == 0) {
      imu_state = 1;
    }
    if (millis() % 15 == 0) {
      speed_calculate();  
      Turn_Control(speed_out, angle_out, turn_speed, angle_a_out, turn_location, Free_hand);
    }
  }else if (state == Lock_hand) {
      if (millis() % 5 == 0) {
        Getencoder_Data();
        speed_add();
      }
      // 10ms Detects the current Angle once
      if (millis() % 10 == 0) {
        imu_state = 1;
      }
      if (millis() % 15 == 0) {
        speed_calculate();  // Find the average speed and filter the burrs
        Turn_Control(speed_out, angle_out, turn_speed ,angle_a_out ,turn_location ,Lock_hand);
      }
  }else if (state == Around_mode) {
      if (millis() % 5 == 0) {
        Getencoder_Data();
        speed_add();
      }
      if (millis() % 15 == 0) {
        speed_calculate();     
        Circle_Control(Circle_r,30,isClockwise); 
      }
  }else if(state == Ultrasonic_mode) {
      if (millis() % 5 == 0) {
        Getencoder_Data();
        speed_add();
      }
      if(millis() % 10 == 0)
      {
        imu_state = 1;
      }
      if (millis() % 15 == 0) {
        speed_calculate();    // Find the average speed and filter the burrs
        if(Ultrasonic_angle > 360)
          Ultrasonic_angle -= 360;
        if(Ultrasonic_angle < 0)
          Ultrasonic_angle += 360;
        Ultrasonic_Run(Ultrasonic_speed, Ultrasonic_angle, 50);
      }
    }
  }
  // 5ms detects and accumulates the encoder data once
  return true;
}
