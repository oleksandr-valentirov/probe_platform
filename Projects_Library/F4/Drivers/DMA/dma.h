#pragma once

#include "!Project_library.h"

void DMA_GPSoutInit(uint32_t* gps_buf_ptr);
void DMA_GPSinInit(uint32_t* gps_buf_ptr);
void DMA_GPSoutTransfer(uint8_t size);
void DMA_GPSinTransferStart(uint16_t size);
void DMA_GPSinTransferStop(void);
uint16_t DMA_GPSinGetRemainingDataCounter(void);