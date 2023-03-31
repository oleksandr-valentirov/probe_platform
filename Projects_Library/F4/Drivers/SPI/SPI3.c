#include "SPI3.h"


static uint8_t is_busy = 0;

/* 
    callback will be called in the decorator, from the ISR.
    callback function must be as short and fast as possible
*/
static void (*spi_op_end_callback)(void) = NULL;


uint8_t get_busy_flag(void)
{
    return is_busy;
}

/**
  * @brief  dma_op_end_callback_decorator is used in DMA ISR to turn off DMA
  *         and interrupt
  */
static void spi_op_end_callback_decorator(void)
{
    if(spi_op_end_callback != NULL)
    {
        spi_op_end_callback();
        spi_op_end_callback = NULL;
    }
    is_busy = 0;
}


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
    SPI_Cmd(SPI3, ENABLE);
    SPI3->DR;
}


void SPI3_WriteData_Pol(uint8_t* src, uint32_t len)
{
    while(len--)
    {
        while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)){}
        SPI3->DR = *(src++);
    }
}

void SPI3_ReadData_Pol(uint8_t* dst, uint32_t len)
{
    while(len--)
    {
        while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE)){}
        *(dst++) = SPI3->DR;
    }
}

void SPI_StartOp_DMA(uint8_t *src, uint32_t tx_size, 
                     uint8_t *dst, uint32_t rx_size,
                     void (*callback)(void))
{
    if(src == NULL || !tx_size)
    {
        return;
    }
    
    /* start DMA Tx operation */
    DMA_SPI3outInit(src, tx_size);
    
    if(dst != NULL && rx_size)
    {   /* start DMA Rx operation */
        DMA_SPI3inInit(dst, rx_size);
    }
    
    if(callback != NULL)
    {
        spi_op_end_callback = callback;
    }
    DMA_SetCallback(spi_op_end_callback_decorator);
    is_busy = 1;
}

void SPI3_Test(void)
{
    uint8_t tx[5] = {'a', 'b', 'c', 'd', 'e'};
    uint8_t rx[5] = {0};
    for(uint8_t i = 0; i < 5; i++)
    {   /* poling test */
        while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)){}
        SPI3->DR = tx[i];
        while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE)){}
        rx[i] = SPI3->DR;
        
        /* update TX data to see that rx array was actualy been updated 
           during DMA test*/
        tx[i] = '1' + i;
    }
    
    /* DMA test */
    SPI_StartOp_DMA(tx, 5, rx, 5, NULL);
    SysTick_WaitTill(SysTick_GetCurrentClock() + 1000);
    (void)rx;
}