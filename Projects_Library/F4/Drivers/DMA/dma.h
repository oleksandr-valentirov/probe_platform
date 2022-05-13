#pragma once

#include "!Project_library.h"

void DMA_USART1outInit(uint32_t* gps_buf_ptr);
void DMA_USART1inInit(uint32_t* gps_buf_ptr);
void DMA_USART1outTransfer(uint8_t size);
void DMA_USART1inTransferStart(uint16_t size);
void DMA_USART1inTransferStop(void);
uint16_t DMA_USART1inGetRemainingDataCounter(void);


