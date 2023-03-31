#pragma once

#include "!Project_library.h"


void SPI3_Test(void);
void SPI_StartOp_DMA(uint8_t *src, uint32_t tx_size, 
                     uint8_t *dst, uint32_t rx_size,
                     void (*callback)(void));
void SPI3_ReadData_Pol(uint8_t* dst, uint32_t len);
void SPI3_WriteData_Pol(uint8_t* src, uint32_t len);
void SPI3_Init(void);