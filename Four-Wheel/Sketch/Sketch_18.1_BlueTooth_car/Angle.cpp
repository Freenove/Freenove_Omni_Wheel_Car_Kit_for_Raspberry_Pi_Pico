#include "IMU_Fusion_SYC.h"
#define Car_4_wheel

float AccMagnitude;

int angle;

IMU imu(Wire);

void IMU_Init() 
{
  Wire.begin();
  imu.QMC5883L_SetOffsets(76.5,452.5,0);// Fill in the offset obtained by calibration
  imu.QMC5883L_SetScales(1,0.95,0);// Fill in the Scale obtained by calibration
  imu.Heading_Offset(360); // Set offest
  imu.begin(CHOOSE_ALL);// IMU initialization
  imu.MPU6050_CalcGyroOffsets();// MPU6050 calibration
}

void IMU_GetData() 
{
    imu.Calculate(); // Calculate the raw data
    angle = imu.Data_Fusion(); // Get fusion data
}