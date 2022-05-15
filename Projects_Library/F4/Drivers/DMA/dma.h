#pragma once

#include "!Project_library.h"

void DMA_MyInit(void);
void DMA_ClearStatus(DMA_Stream_TypeDef* DMAy_Streamx);

void DMA_USART1outInit(uint32_t* gps_buf_ptr);
void DMA_USART1inInit(uint32_t* gps_buf_ptr);
void DMA_USART1outTransfer(uint8_t size);
void DMA_USART1inTransferStart(uint16_t size);
void DMA_USART1inTransferStop(void);
uint16_t DMA_USART1inGetRemainingDataCounter(void);


void DMA_SPI3outInit(void);
void DMA_SPI3inInit(void);
uint8_t DMA_SPI3outTransfer(uint8_t* data_ptr, uint8_t size);
uint8_t DMA_SPI3inTransfer(uint8_t* dst_ptr, uint8_t size);