/**********************************************************************
  Filename    : Sketch_04.2_Motor_PWM
  Description : Use Raspberry Pi Pico Control motor to change speed
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/

const int wheel1_A_pin = 8;// Define motor1 drive pins
const int wheel1_B_pin = 9;// Define motor1 drive pins

void setup() {
  // put your setup code here, to run once:
  // Set the motor PWM frequency to 16 kHz
  analogWriteFreq(16000);
  // Enable the motor drive pin to output mode
  pinMode(wheel1_A_pin,OUTPUT);
  pinMode(wheel1_B_pin,OUTPUT);
}

void loop() {
  // put you main code here, to run repeatedly:
  // Stop
  analogWrite(wheel1_A_pin,0);
  analogWrite(wheel1_B_pin,0);
  delay(1500);// Delay 1.5 second
  // The speed is 200
  analogWrite(wheel1_A_pin,200);
  analogWrite(wheel1_B_pin,0);
  delay(1500);// Delay 1.5 second
  // The speed is 255
  analogWrite(wheel1_A_pin,255);
  analogWrite(wheel1_B_pin,0);
  delay(1500);// Delay 1.5 second
}