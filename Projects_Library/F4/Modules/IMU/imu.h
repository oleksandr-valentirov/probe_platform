#pragma once

#include "!Project_library.h"


void* IMU_Init(void);

void IMU_SetCalibAX(int x);
void IMU_SetCalibAY(int y);
void IMU_SetCalibAZ(int z);

void IMU_SetCalibGX(int x);
void IMU_SetCalibGY(int y);
void IMU_SetCalibGZ(int z);
