/***************************************************************************
  Filename    : Sketch_07.1_Compass_calibration
  Description : Use Raspberry Pi Pico read the yaw Angle of the Magnetometer
  Auther      : www.freenove.com
  Modification: 2024/08/23
****************************************************************************/
#include "IMU_Fusion_SYC.h"

IMU imu(Wire);

void setup() {
  Serial.begin(9600);         // Set the baud rate to 9600
  Serial.print("\n");
  Wire.begin();
  imu.begin(CHOOSE_QMC5883L); // Select QMC5883L
}

void loop() {
  imu.QMC5883L_Calibration(); // Compass automatic calibration
}