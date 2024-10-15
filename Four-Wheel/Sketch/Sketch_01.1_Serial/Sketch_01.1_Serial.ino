/**************************************************************************************
  Filename    : Sketch_01.1_Serial
  Description : Use Raspberry Pi Pico to Output information in the serial port monitor
  Auther      : www.freenove.com
  Modification: 2024/08/23
**************************************************************************************/
int counter;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Initialize the serial port, set the baud rate to 115200
  delay(3000);
  Serial.println("Raspberry Pi Pico Ready");// print the string "Raspberry Pi Pico Ready"
}

void loop() {
  // put you main code here, to run repeatedly:
  Serial.print("counter:");
  Serial.println(counter++);
  delay(1000);
}

