/**********************************************************************
  Filename    : Sketch_04.1_Motor
  Description : Use Raspberry Pi Pico Control the motor forward for 1 
              second, stop for 1 second, then reverse for 1 second, and 
              cycle this process
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
const int wheel1_A_pin = 8;// Define motor1 drive pins
const int wheel1_B_pin = 9;// Define motor1 drive pins

void setup() {
  // put your setup code here, to run once:
  // Enable the motor drive pin to output mode
  analogWriteFreq(16000);// Set the appropriate trolley frequency
  pinMode(wheel1_A_pin,OUTPUT);
  pinMode(wheel1_B_pin,OUTPUT);
}

void loop() {
  // put you main code here, to run repeatedly:
  digitalWrite(wheel1_A_pin,1);  // Forward rotation
  digitalWrite(wheel1_B_pin,0);
  delay(1000); 

  digitalWrite(wheel1_A_pin,0);  //stop
  digitalWrite(wheel1_B_pin,0);
  delay(1000); 

  digitalWrite(wheel1_A_pin,0);  // Contrarotation
  digitalWrite(wheel1_B_pin,1);
  delay(1000);

  digitalWrite(wheel1_A_pin,0);  //stop
  digitalWrite(wheel1_B_pin,0);
  delay(1000); 
}