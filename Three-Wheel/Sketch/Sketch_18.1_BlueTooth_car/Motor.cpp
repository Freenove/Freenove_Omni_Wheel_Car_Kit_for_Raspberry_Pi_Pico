#include "Motor.h"

int angle_a;
int v1, v2, v3, v4, vx, vy, pid_v1, pid_v2, pid_v3, pid_v4, angle_v, angle_Ultrasonic, Ultrasonic_speed, Ultrasonic_index, Ultrasonic_mutex = 0, Ultrasonic_state, Ultrasonic_angle, Ultrasonic_delay, angle_circle;
int pid1, pid2, pid3, pid4;
int a1, b1, a2, b2, a3, b3, a4, b4, turn_output;
int angle_head, angle_counter;
uint32_t Ultrasonic_time, Ultrasonic_distance_time;
int Ultrasonic_compare[2];

// Motor initialization
void Motor_init() {
  analogWriteFreq(16000);  // Set the appropriate trolley frequency
  pinMode(wheel1_A_pin, OUTPUT);
  pinMode(wheel1_B_pin, OUTPUT);

  pinMode(wheel2_A_pin, OUTPUT);
  pinMode(wheel2_B_pin, OUTPUT);

  pinMode(wheel3_A_pin, OUTPUT);
  pinMode(wheel3_B_pin, OUTPUT);

#ifdef Car_4_wheel
  pinMode(wheel4_A_pin, OUTPUT);
  pinMode(wheel4_B_pin, OUTPUT);
#endif
}

void car_stop() {
  analogWrite(wheel1_A_pin, LOW);
  analogWrite(wheel1_B_pin, LOW);

  analogWrite(wheel2_A_pin, LOW);
  analogWrite(wheel2_B_pin, LOW);

  analogWrite(wheel3_A_pin, LOW);
  analogWrite(wheel3_B_pin, LOW);

#ifdef Car_4_wheel
  analogWrite(wheel4_A_pin, LOW);
  analogWrite(wheel4_B_pin, LOW);
#endif
}

void Motor_direction()  // Motor limiting and motor output orientation
{
  if (pid_v1 > 255) pid_v1 = 255;
  else if (pid_v1 < -255) pid_v1 = -255;
  if (pid_v1 > 0) {
    a1 = pid_v1;
    b1 = LOW;
  } else {
    a1 = LOW;
    b1 = -pid_v1;
  }
  if (pid_v2 > 255) pid_v2 = 255;
  else if (pid_v2 < -255) pid_v2 = -255;
  if (pid_v2 > 0) {
    a2 = pid_v2;
    b2 = LOW;
  } else {
    a2 = LOW;
    b2 = -pid_v2;
  }
  if (pid_v3 > 255) pid_v3 = 255;
  else if (pid_v3 < -255) pid_v3 = -255;
  if (pid_v3 > 0) {
    a3 = pid_v3;
    b3 = LOW;
  } else {
    a3 = LOW;
    b3 = -pid_v3;
  }
#ifdef Car_4_wheel
  if (pid_v4 > 255) pid_v4 = 255;
  else if (pid_v4 < -255) pid_v4 = -255;
  if (pid_v4 > 0) {
    a4 = pid_v4;
    b4 = LOW;
  } else {
    a4 = LOW;
    b4 = -pid_v4;
  }
#endif

  analogWrite(wheel1_A_pin, a1);
  analogWrite(wheel1_B_pin, b1);

  analogWrite(wheel2_A_pin, a2);
  analogWrite(wheel2_B_pin, b2);

  analogWrite(wheel3_A_pin, a3);
  analogWrite(wheel3_B_pin, b3);

#ifdef Car_4_wheel
  analogWrite(wheel4_A_pin, a4);
  analogWrite(wheel4_B_pin, b4);
#endif
}

void Circle_Control(float r, float V, float location) {
  if (r < 1)
    r = 1;
  angle_circle = V / r;
  if (location == clockwise) {
    vx = -V * sin(90 * (PI / 180));
    // vy = V * cos (90 * (PI / 180));
    calculateWheelSpeed(angle_circle);
  }
  if (location == anticlockwise) {
    vx = -V * sin(-90 * (PI / 180));
    // vy = V * cos (-90 * (PI / 180));
    calculateWheelSpeed(-angle_circle);
  }
  // Calculate the target speed for each motor using the PID control algorithm.
  updatePID();
  Motor_direction();  // Motor limiting and motor output orientation
}

void Turn_Control(int speed_v, int speed_a, int angle_v, int angle_a, int location, int mode) {
  if (mode == Manual_Control) {
    vx = -speed_v * sin(speed_a * PI / 180);
    vy = speed_v * cos(speed_a * PI / 180);
  }
  if (mode == Free_hand) {
    vx = -speed_v * sin((angle_flag - angle_head + speed_a) * PI / 180);
    vy = speed_v * cos((angle_flag - angle_head + speed_a) * PI / 180);
  }
  if (mode == Manual_Control || mode == Free_hand) {
    if (angle_state == 0) {
      if (location >= 0 && location < 180) {
        angle_v = -angle_v;
      }
    } else if (angle_state == 1) {  // Push the left joysticks only
      angle_v = Angle_PID_Realize(angle_flag, angle);
      if (angle_v > 50) angle_v = 50;
      else if (angle_v < -50) angle_v = -50;
    } else if (angle_state == 2) {  // Push the left and right joysticks at the same time
      angle_counter++;
      if (angle_counter < 3) {
        if (location > 0 && location < 180) {
          angle_v = -angle_v;
        }
        v1 = angle_v;
        v2 = angle_v;
        v3 = angle_v;
        #ifdef Car_4_wheel
          v4 = angle_v;
        #endif
      } else if (angle_counter < 4) {
        angle_flag = angle;
        vx = -speed_v * sin((angle_flag - angle_head + speed_a) * PI / 180);
        vy = speed_v * cos((angle_flag - angle_head + speed_a) * PI / 180);

        calculateWheelSpeed();
      } else {
        angle_counter = 0;
      }
      angle_state = 0;
    }
    if (angle_state != 2) {
      calculateWheelSpeed(angle_v);
    }
    // Calculate the target speed for each motor using the PID control algorithm.
    updatePID();
  }
  if (mode == Lock_hand) {
    angle_v = Angle_PID_Realize(angle_a, angle);

    if (angle_v > 50) angle_v = 50;
    else if (angle_v < -50) angle_v = -50;

    vx = -speed_v * sin((angle - angle_lock + speed_a) * PI / 180);
    vy = speed_v * cos((angle - angle_lock + speed_a) * PI / 180);

    calculateWheelSpeed(angle_v);

    // Calculate the target speed for each motor using the PID control algorithm.
    updatePID();
  }
  Motor_direction();  // Motor limiting and motor output orientation
}
/*
  function: Ultrasonic_Run
  parameter: 
    speed_v: Target speed of the car.
    angle_a: Target rotation angle of the car.
    turn_speed: Target rotation speed of the car.
*/
void Ultrasonic_Run(int speed_v, int angle_a, int turn_speed) {
  angle_v = Angle_PID_Realize(angle_a, angle);
  if (angle_v > turn_speed) angle_v = turn_speed;
  else if (angle_v < -turn_speed) angle_v = -turn_speed;

  vx = -speed_v * sin(0 * PI / 180);
  vy = speed_v * cos(0 * PI / 180);

  calculateWheelSpeed(angle_v);

  // Calculate the target speed for each motor using the PID control algorithm.
  updatePID();
  Motor_direction();
}
/*
  function: Ultrasonic_control
  parameter: 
    speed: The cruising speed of the car, the parameter size range is from 0 to 100.
    turn_time: The time interval between the turns of the car, the unit is in milliseconds.
    distance: the distance measurement of obstacles by the car, the unit is in centimeters.
*/
void Ultrasonic_control(int speed, int turn_time, int distance) {
  delay(2);
  if ((millis() - Ultrasonic_distance_time) > 20)  // Check every 20 ms.
  {
    Ultrasonic_distance = Read_Distance();
    Ultrasonic_distance_time = millis();
  }
  if (Ultrasonic_mutex == 0) {
    if (Ultrasonic_distance > distance)  // The distance is greater than 15cm
    {
      Ultrasonic_speed = speed;
      Ultrasonic_angle = angle;
    } else {
      Ultrasonic_mutex = 1;
      Ultrasonic_speed = 0;
    }
  } else {
    if (Ultrasonic_state == 0)  // turn left
    {
      Ultrasonic_angle = Ultrasonic_angle - 90;
      Ultrasonic_state = 1;
      Ultrasonic_time = millis();                                                  // Obtain the system running time
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 1)  // Turn 90 degrees to the left
    {
      Ultrasonic_compare[0] = Read_Distance();
      Ultrasonic_state = 2;
      Ultrasonic_time = millis();
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 2)  // Turn right back to the center
    {
      Ultrasonic_angle = Ultrasonic_angle + 90;
      Ultrasonic_state = 3;
      Ultrasonic_time = millis();
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 3)  // stop
    {
      Ultrasonic_state = 4;
      Ultrasonic_time = millis();
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 4)  // Turn 90 degrees to the right
    {
      Ultrasonic_angle = Ultrasonic_angle + 90;

      Ultrasonic_state = 5;
      Ultrasonic_time = millis();
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 5)  // Turn 90 degrees to the right
    {
      Ultrasonic_compare[1] = Read_Distance();
      Ultrasonic_state = 6;
      Ultrasonic_time = millis();
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 6) {
      Ultrasonic_angle = Ultrasonic_angle - 90;  // Go back to the middle
      Ultrasonic_state = 7;
      Ultrasonic_time = millis();
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 7) {
      Ultrasonic_state = 8;
      Ultrasonic_time = millis();
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 8) {
      if (Ultrasonic_compare[0] > Ultrasonic_compare[1]) {
        Ultrasonic_angle = Ultrasonic_angle - 90;
      } else {
        Ultrasonic_angle = Ultrasonic_angle + 90;
      }
      Ultrasonic_state = 9;
      Ultrasonic_time = millis();
    } else if ((millis() - Ultrasonic_time) > turn_time && Ultrasonic_state == 9) {
      Ultrasonic_speed = speed;
      Ultrasonic_state = 0;
      Ultrasonic_mutex = 0;
    }
  }
}

void updatePID() {
  pid_v1 = Speed1_PID(v1, speed1);
  pid_v2 = Speed2_PID(v2, speed2);
  pid_v3 = Speed3_PID(v3, speed3);
#ifdef Car_4_wheel
  pid_v4 = Speed4_PID(v4, speed4);
#endif
}

void calculateWheelSpeed(int angle_point) {
#ifdef Car_4_wheel
  v1 = -vx + angle_point;
  v2 = -vy + angle_point;
  v3 = vx + angle_point;
  v4 = vy + angle_point;
#else
  v1 = -vx + angle_point;
  v2 = 0.5 * vx - sqrt(3) / 2 * vy + angle_point;
  v3 = 0.5 * vx + sqrt(3) / 2 * vy + angle_point;
#endif
}

void IR_run(int speed_v, int speed_a, int angle_v) {
  vx = -speed_v * sin(speed_a * PI / 180);
  vy = speed_v * cos(speed_a * PI / 180);

  v1 = -vx + angle_v;
  v2 = 0.5 * vx - sqrt(3) / 2 * vy + angle_v;
  v3 = 0.5 * vx + sqrt(3) / 2 * vy + angle_v;

  // Calculate the target speed for each motor using the PID control algorithm.
  pid_v1 = Speed1_PID(v1, speed1);
  pid_v2 = Speed2_PID(v2, speed2);
  pid_v3 = Speed3_PID(v3, speed3);

  Motor_direction();  // Motor limiting and motor output orientation
}