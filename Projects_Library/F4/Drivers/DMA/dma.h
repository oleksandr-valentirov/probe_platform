#pragma once

#include "!Project_library.h"

void DMA_GPSoutInit(uint32_t* gps_buf_ptr);
void DMA_GPSoutTransfer(uint8_t size);