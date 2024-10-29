/**********************************************************************
  Filename    : Sketch_07.2_Compass_getAngle
  Description : Use Raspberry Pi Pico read Compass Angle
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
#include "IMU_Fusion_SYC.h"

IMU imu(Wire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  imu.QMC5883L_SetOffsets(212.5,562.5,0);
  imu.QMC5883L_SetScales(1,1,0);
  imu.Heading_Offset(360); 			// Set Heading offest
  imu.begin(CHOOSE_QMC5883L);		// Only QMC5883L is initialized
}

void loop(){
  imu.Calculate();
  Serial.print("heading:");
  Serial.println(imu.getHeading());
}