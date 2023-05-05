#pragma once

#include "!Project_library.h"


#define SECTOR_7    0x08060000
#define SIZE        10
#define IMU_AX      ((int*)(SECTOR_7))
#define IMU_AY      ((int*)(SECTOR_7 + 4))
#define IMU_AZ      ((int*)(SECTOR_7 + 8))
#define IMU_GX      ((int*)(SECTOR_7 + 12))
#define IMU_GY      ((int*)(SECTOR_7 + 16))
#define IMU_GZ      ((int*)(SECTOR_7 + 20))

void MEM_Write(void* src, void* dst, size_t size);
