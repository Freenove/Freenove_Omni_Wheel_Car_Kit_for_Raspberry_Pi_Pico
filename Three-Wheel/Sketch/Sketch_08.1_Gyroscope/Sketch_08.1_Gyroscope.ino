/**********************************************************************
  Filename    : Sketch_08.1_Gyroscope
  Description : Use Raspberry Pi Pico read the yaw Angle of the MPU6050
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
#include "IMU_Fusion_SYC.h"

IMU imu(Wire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);// Set the serial port baud rate to 115200
  Wire.begin();
  imu.begin(CHOOSE_MPU6050);// MPU6050 init
  imu.MPU6050_CalcGyroOffsets();			// MPU6050 calibration
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:
  // Obtain MPU6050 data every 500ms and calculate the angles
  imu.Calculate();// MPU6050 update raw data
  Serial.print("AngleZ:");
  Serial.println(imu.getAngleZ());// Print yaw angle
  delay(500);
}