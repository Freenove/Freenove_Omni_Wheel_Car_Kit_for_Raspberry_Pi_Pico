#include "Ultrasonic.h"
#include "Arduino.h"

int Ultrasonic_distance;

void Ultrasonic_init()
{
  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);
}

int Read_Distance()
{
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);

  Ultrasonic_distance = pulseIn(Echo,HIGH) / 58.8;
  return Ultrasonic_distance;
}

