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
