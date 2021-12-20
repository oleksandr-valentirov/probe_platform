/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

#include "!Project_library.h"

#define SPI_BUF_SIZE            128
#define SPI_BUF_MASK            127

#define SPI_FLAG_BUSY           1


void SPI3_Init(void);
void SPI3_ExchangeBytes(void);
void SPI3_gets(uint8_t *dst, uint8_t max_size);


#endif