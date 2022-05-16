#include "SPI3.h"


//static uint8_t tx_buffer[256];
//static uint8_t rx_buffer[256];


void SPI3_Init(void)
{
    DMA_SPI3outInit();
    DMA_SPI3inInit();
    
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
    SPI_Cmd(SPI3, ENABLE);
    SPI3->DR;
//    SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);
        
//    NVIC_EnableIRQ(SPI3_IRQn);
}


void SPI3_Test(void)
{
    uint8_t tx[5] = {'a', 'b', 'c', 'd', 'e'};
    uint8_t rx[5] = {0};
    for(uint8_t i = 0; i < 5; i++)
    {
        while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)){}
        SPI3->DR = tx[i];
        while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE)){}
        rx[i] = SPI3->DR;
    }
}