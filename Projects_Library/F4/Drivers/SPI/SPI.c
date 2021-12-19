#include "SPI.h"


/* -------------------------------------------------------------------------- */
/* ------------------------------- SPI3 ------------------------------------- */
/* -------------------------------------------------------------------------- */
/**
 * CPOL = 0; CPHA = 0; Max speed 16 MHz. 
 * По умолчанию размер буфера 8 бит.
 */
void SPI3_Init(void)
{
    SPI_I2S_DeInit(SPI3);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    
    SPI_InitTypeDef init_struct;
    SPI_StructInit(&init_struct);
    
    /* initialize the SPI_Mode member */
    init_struct.SPI_Mode = SPI_Mode_Master;
    
    /* Initialize the SPI_NSS member */
    init_struct.SPI_NSS = SPI_NSS_Soft;
    
    /* Initialize the SPI_BaudRatePrescaler member */
    init_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;  // 48 Mhz APB1 -> 3 MHz GPS
    
    SPI_Init(SPI3, &init_struct);
    SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);
    
    SPI_Cmd(SPI3, ENABLE);
//    NVIC_EnableIRQ(SPI3_IRQn);
}


void SPI3_StartTransaction(uint8_t *src, uint8_t *dst)
{
}


void SPI3_ExchangeBytes(void)
{
}