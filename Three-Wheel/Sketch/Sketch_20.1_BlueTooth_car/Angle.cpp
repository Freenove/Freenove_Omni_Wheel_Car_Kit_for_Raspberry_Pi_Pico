#include "IMU_Fusion_SYC.h"

float AccMagnitude;

int angle;

IMU imu(Wire);

void IMU_Init() 
{
  Wire.begin();
  #ifdef Car_4_wheel
    imu.QMC5883L_SetOffsets(348.5,-145.5,0);// Fill in the offset obtained by calibration
    imu.QMC5883L_SetScales(1,1.04,0);// Fill in the Scale obtained by calibration
  #else
    imu.QMC5883L_SetOffsets(-933.5,-105,0);// Fill in the offset obtained by calibration
    imu.QMC5883L_SetScales(1,0.97,0);// Fill in the Scale obtained by calibration
  #endif 
  imu.Heading_Offset(360); // Set offest
  imu.begin(CHOOSE_ALL);// IMU initialization
  imu.MPU6050_CalcGyroOffsets();// MPU6050 calibration
}

void IMU_GetData() 
{
    imu.Calculate(); // Calculate the raw data
    float newValue = imu.Data_Fusion(); // Get fusion data

    // Define the smoothing factor
    float alpha = 0.1; // 0 < alpha < 1, the smaller the smoother, the larger the more sensitive

    // Calculate the difference between the current Angle and the new value
    float diff = newValue - angle;

    // Handle cases that cross 0 degrees
    if (diff > 180) {
        diff -= 360;
    } else if (diff < -180) {
        diff += 360;
    }

    // Use weighted average to update angles
    angle += alpha * diff;

    // Make sure the result is in the range [0, 360)
    angle = fmod(angle + 360, 360);
}