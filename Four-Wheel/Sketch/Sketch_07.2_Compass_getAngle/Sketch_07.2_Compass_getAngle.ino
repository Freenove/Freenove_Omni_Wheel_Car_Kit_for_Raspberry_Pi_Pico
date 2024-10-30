/**********************************************************************
  Filename    : Sketch_07.2_Magnetometer_getAngle
  Description : Use Raspberry Pi Pico read QMC5883L magnetic declination
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
#include "IMU_Fusion_SYC.h"

IMU imu(Wire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  imu.QMC5883L_SetOffsets(76.5,452.5,0);// Fill in the offset obtained by calibration
  imu.QMC5883L_SetScales(1,0.95,0);// Fill in the Scale obtained by calibration
  imu.Heading_Offset(360); 			// Set Heading offest
  imu.begin(CHOOSE_QMC5883L);		// Only QMC5883L is initialized
}

void loop(){
  imu.Calculate();
  Serial.print("heading:");
  Serial.println(imu.getHeading());

}