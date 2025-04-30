#include "Angle.h"

float AccMagnitude;
int angle;

IMU imu(Wire);

void IMU_Init() 
{
  Wire.begin();
  imu.QMC5883L_SetOffsets(96.5,571,0);
  imu.QMC5883L_SetScales(1,1,0);
  imu.Heading_Offset(360); // Set offest
  imu.begin(CHOOSE_ALL);// IMU initialization
  imu.MPU6050_CalcGyroOffsets();// MPU6050 calibration
}

void IMU_GetData() 
{
  imu.Calculate(); // Calculate the raw data
  angle = imu.Data_Fusion(); // Get fusion data
}