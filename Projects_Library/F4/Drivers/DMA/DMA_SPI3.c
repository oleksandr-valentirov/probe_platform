#include "dma.h"


#define OUT     DMA1_Stream5
#define IN      DMA1_Stream0

void (*dma_op_end_callback)(void);

void DMA_op_end_callback_decorator(void)
{
    if(dma_op_end_callback != NULL)
    {
        dma_op_end_callback();
        dma_op_end_callback = NULL;
    }
}

void DMA_SetCallback(void (*f)(void))
{
    if(f != NULL)
    {
        dma_op_end_callback = f;
    }
}


void DMA_SPI3outInit(uint8_t* data_ptr, uint32_t size)
{
    DMA_InitTypeDef ch;
    DMA_StructInit(&ch);
    
    /* check flag and params */
    if(DMA_GetFlagStatus(OUT, DMA_FLAG_TCIF0) || data_ptr == NULL || size == 0)
    {
        return;
    }
    
    /* my params */
    ch.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    ch.DMA_Channel = DMA_Channel_0;
    ch.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI3->DR));
    ch.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*------------------------------------------------------------------------*/
    
    /* interrupt */
    DMA_ITConfig(OUT, DMA_IT_TC, ENABLE);
    DMA_Init(OUT, &ch);
    
    /* start transfer */
    DMA_ClearStatus(OUT);
    OUT->M0AR = (uint32_t)data_ptr;
    DMA_SetCurrDataCounter(OUT, size);
    DMA_Cmd(OUT, ENABLE);
    SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);
}


void DMA_SPI3inInit(uint8_t* data_ptr, uint32_t size)
{
    DMA_InitTypeDef ch;
    DMA_StructInit(&ch);
    
    /* check flag and params */
    if(DMA_GetFlagStatus(IN, DMA_FLAG_TCIF0) || data_ptr == NULL || size == 0)
    {
        return;
    }
    
    /* my params */
    ch.DMA_DIR = DMA_DIR_PeripheralToMemory;
    ch.DMA_Channel = DMA_Channel_0;
    ch.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI3->DR));
    ch.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*------------------------------------------------------------------------*/
    
    /* interrupt */
    DMA_ITConfig(IN, DMA_IT_TC, ENABLE);
    DMA_Init(IN, &ch);
    
    /* start transfer */
    DMA_ClearStatus(IN);
    IN->M0AR = (uint32_t)data_ptr;
    DMA_SetCurrDataCounter(IN, size);
    DMA_Cmd(IN, ENABLE);
    SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
}