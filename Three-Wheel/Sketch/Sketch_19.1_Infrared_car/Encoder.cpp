#include "Arduino.h"
#include "Encoder.h"

const int encoderA1Pin = 6; // Encoder1 A pin
const int encoderB1Pin = 7; // Encoder1 B pin

const int encoderA2Pin = 10;// Encoder2 A pin
const int encoderB2Pin = 11;// Encoder2 B pin

const int encoderA3Pin = 16;// Encoder3 A pin
const int encoderB3Pin = 17;// Encoder3 B pin

const int wheel1_A_pin = 9; // Motor1 drive pin
const int wheel1_B_pin = 8; // Motor1 drive pin

const int wheel2_A_pin = 12;// Motor2 drive pin
const int wheel2_B_pin = 13;// Motor2 drive pin

const int wheel3_A_pin = 15;// Motor3 drive pin
const int wheel3_B_pin = 14;// Motor3 drive pin

int encoder1Pos = 0,encoder2Pos = 0,encoder3Pos = 0;
int lastEncoded1 = 0,lastEncoded2 = 0,lastEncoded3 = 0;
int speed1,speed2,speed3;

int encoder1,encoder2,encoder3;


void Encoder_Init()
{
  pinMode(encoderA1Pin,INPUT);
  pinMode(encoderB1Pin,INPUT);
  pinMode(encoderA2Pin,INPUT);
  pinMode(encoderB2Pin,INPUT);
  pinMode(encoderA3Pin,INPUT);
  pinMode(encoderB3Pin,INPUT);

  attachInterrupt(digitalPinToInterrupt(encoderA1Pin), Getencoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB1Pin), Getencoder, CHANGE);

  attachInterrupt(digitalPinToInterrupt(encoderA2Pin), Getencoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB2Pin), Getencoder, CHANGE);

  attachInterrupt(digitalPinToInterrupt(encoderA3Pin), Getencoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB3Pin), Getencoder, CHANGE);
}

// The interrupt function that gets the encoder encoded value
void Getencoder()
{
  int MSB1 = digitalRead(encoderA1Pin);
  int LSB1 = digitalRead(encoderB1Pin);

  int MSB2 = digitalRead(encoderA2Pin);
  int LSB2 = digitalRead(encoderB2Pin);
  
  int MSB3 = digitalRead(encoderA3Pin);
  int LSB3 = digitalRead(encoderB3Pin);

  int encoded1 = (MSB1 << 1) | LSB1;
  int sum1 = (lastEncoded1 << 2) | encoded1;

  if (sum1 == 0b1101 || sum1 == 0b0100 || sum1 == 0b0010 || sum1 == 0b1011) encoder1Pos++;
  if (sum1 == 0b1110 || sum1 == 0b0111 || sum1 == 0b0001 || sum1 == 0b1000) encoder1Pos--;

  int encoded2 = (MSB2 << 1) | LSB2;
  int sum2 = (lastEncoded2 << 2) | encoded2;

  if (sum2 == 0b1101 || sum2 == 0b0100 || sum2 == 0b0010 || sum2 == 0b1011) encoder2Pos++;
  if (sum2 == 0b1110 || sum2 == 0b0111 || sum2 == 0b0001 || sum2 == 0b1000) encoder2Pos--;

  int encoded3 = (MSB3 << 1) | LSB3;
  int sum3 = (lastEncoded3 << 2) | encoded3;

  if (sum3 == 0b1101 || sum3 == 0b0100 || sum3 == 0b0010 || sum3 == 0b1011) encoder3Pos++;
  if (sum3 == 0b1110 || sum3 == 0b0111 || sum3 == 0b0001 || sum3 == 0b1000) encoder3Pos--;

  lastEncoded1 = encoded1;
  lastEncoded2 = encoded2;
  lastEncoded3 = encoded3;
}

void Getencoder_Data()
{
  // Get the encoder code value
  speed1 = encoder1Pos;
  speed2 = encoder2Pos;
  speed3 = encoder3Pos;
  
  encoder1Pos = 0;
  encoder2Pos = 0;
  encoder3Pos = 0;
}