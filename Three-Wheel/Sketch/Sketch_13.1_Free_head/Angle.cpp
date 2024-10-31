#include "IMU_Fusion_SYC.h"

const int windowSize = 10; // Window size
float data[windowSize]; // An array to store data
int index_ = 0; // Current index
float sum = 0; // Sum of current Windows
float AccMagnitude;

int angle;

IMU imu(Wire);

void IMU_Init() 
{
  Wire.begin();
  imu.QMC5883L_SetOffsets(212.5,562.5,0);
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
