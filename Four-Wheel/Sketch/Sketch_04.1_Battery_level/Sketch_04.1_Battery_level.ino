/**********************************************************************
  Filename    : Sketch_04.1_Battery_level
  Description : Use Raspberry Pi Pico Reading battery voltage
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/

const int Battery_ADC = 27;// Define the ADC sampling pin
float Battery;

void setup() {
  // put your setup code here, to rnu once:
  pinMode(Battery_ADC,INPUT);// Enables the ADC sampling pin to be input mode
  Serial.begin(9600);      // Set the serial port baud rate to 115200
}

void loop() {
  // put your main code here, to run repeatedly:
  Battery = analogRead(Battery_ADC)*3.3/1023*4;// Get the sampled value and calculate the real-time voltage value
  Serial.print("Battery:");
  Serial.print(Battery);// Print voltage
  Serial.println("V");
  delay(1000);// Delay 1 second
}
