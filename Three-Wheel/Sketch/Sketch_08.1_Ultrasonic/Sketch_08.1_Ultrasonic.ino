/**********************************************************************
  Filename    : Sketch_08.1_Ultrasonic
  Description : Use Raspberry Pi Pico read the distance obtained by the 
                ultrasonic module
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
const int Trig = 3;// Define the Trig pin number of the Ultrasonic module
const int Echo = 2;// Define the Echo pin number of the Ultrasonic module

int Ultrasonic_distance;

void Ultrasonic_init()
{
  // Enable the Ultrasonic module Pin
  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);
}

void Read_Distance()
{
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);

  Ultrasonic_distance = pulseIn(Echo,HIGH) / 58.8;// Calculated distance
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);// Set the serial port baud rate to 115200
  Ultrasonic_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  Read_Distance();
  Serial.print("distance:");
  Serial.print(Ultrasonic_distance);//The value of the print distance, in centimeters (cm)
  Serial.println("cm");
  delay(500);// Delay 0.5 second
}