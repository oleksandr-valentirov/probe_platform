/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

#include "!Project_library.h"

#define SPI_BUF_SIZE            128
#define SPI_BUF_MASK            127
#define SPI_DATA_TR_LEN         512

#define SPI_FLAG_BUSY           1
#define SPI_FLAG_RO             2


typedef enum
{
    SPI_None = 0,
    SPI_Flash = 1,
    SPI_GPS = 2,
} SPIDev_t;


/*----------- Extern functions -----------*/
void SPI3_Init(void);
uint8_t SPI3_getc(uint8_t *dst);
uint8_t SPI3_StartWriting(uint8_t *src, size_t size, SPIDev_t device);
uint8_t SPI3_StartReading(size_t size, SPIDev_t device, uint8_t dummy_byte);
uint8_t SPI3_RegisterCallback(void(*callback)(void));
/* Mutexes */
uint8_t SPI3_SetMutex(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, SPIDev_t device);
void SPI3_FreeMutex(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
uint8_t SPI3_IsMine(SPIDev_t device);
uint8_t SPI3_IsFree(void);


/* Interrupt routine */
void SPI3_ExchangeBytes(void);
void SPI3_gets(uint8_t *dst, uint8_t max_size);

#endif