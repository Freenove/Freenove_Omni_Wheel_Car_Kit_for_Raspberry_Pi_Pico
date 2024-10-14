/**********************************************************************
  Filename    : Sketch_05.1_Buzzer
  Description : Use Raspberry Pi Pico control buzzer emits a sound of 
                100Hz every 500 milliseconds
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
const int PIN_BUZZER = 26;// Define the buzzer pin number

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_BUZZER,OUTPUT);// Enable the buzzer pin to be in output mod
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(PIN_BUZZER,100);// The buzzer sounds at 100Hz
  delay(500);          // Delay 0.5 second
  tone(PIN_BUZZER,0);  // The buzzer makes no sound
  delay(500);          // Delay 0.5 second
}
