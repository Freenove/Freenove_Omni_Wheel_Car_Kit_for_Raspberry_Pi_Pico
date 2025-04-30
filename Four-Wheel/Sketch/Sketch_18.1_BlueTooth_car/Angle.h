#ifndef __ANGLE_h__
#define __ANGLE_h__

#include "IMU_Fusion_SYC.h"

extern float AccMagnitude;
extern int angle;

void IMU_Init(void);
void IMU_GetData(void);

#endif