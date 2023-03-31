#pragma once

#include "!Project_library.h"


__weak void* IMU_Init(void);
__weak void init_termometer(void);
__weak void init_barometer(void);
__weak void init_magnitometer(void);
__weak void init_accelerometer(void);
__weak void init_gyroscope(void);
__weak void read_termometer(void);
__weak void read_barometer(void);
__weak void read_magnitometer(void);
__weak void read_accelerometer(void);
__weak void read_gyroscope(void);\
