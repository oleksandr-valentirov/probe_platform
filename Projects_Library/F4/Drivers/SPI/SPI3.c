#include "SPI3.h"


//static uint8_t tx_buffer[256];
//static uint8_t rx_buffer[256];


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
    init_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    
    SPI_Init(SPI3, &init_struct);
//    SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);
        
//    NVIC_EnableIRQ(SPI3_IRQn);
}