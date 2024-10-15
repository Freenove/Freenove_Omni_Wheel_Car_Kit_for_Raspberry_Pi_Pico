/**********************************************************************
  Filename    : Sketch_05.1_WS2812
  Description : Use Raspberry Pi Pico Achieve flashing light effect
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
#include "LED.h"

const int LED_Pin = 28;  // Define pin WS2812
const int LED_Number = 4;// Define the number of WS2812 lamp beads
Adafruit_NeoPixel pixels(LED_Number, LED_Pin, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pixels.begin();// RGB initialization
  pixels.setBrightness(20);// Set RGB brightness
  WS2812_Setup();  // WS2812 initialization
  WS2812_Set_Color_1(0x0F, 0, 0, 255);// Set the WS2812 light bead color to Blue
}

void loop() {
  // put your main code here, to run repeatedly:
  WS2812_Show(1);// Achieve flashing light effect
}
