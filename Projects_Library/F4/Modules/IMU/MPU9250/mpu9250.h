#pragma once

#include "!Project_library.h"

void* IMU_SensorInit(void);
void IMU_poll_accel(int* ax, int* ay, int* az);