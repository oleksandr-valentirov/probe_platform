#include "dma.h"


#define OUT     DMA1_Stream5
#define IN      DMA1_Stream0


void DMA_SPI3outInit(void)
{
    DMA_InitTypeDef ch;
    DMA_StructInit(&ch);
    
    /* my params */
    ch.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    ch.DMA_Channel = DMA_Channel_0;
    ch.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI3->DR));
    ch.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*------------------------------------------------------------------------*/
    
    DMA_ITConfig(OUT, DMA_IT_TC, ENABLE);
    DMA_Init(OUT, &ch);
}

uint8_t DMA_SPI3outTransfer(uint8_t* data_ptr, uint8_t size)
{
    if(DMA_GetFlagStatus(OUT, DMA_FLAG_TCIF0))
    {
        return 1;
    }
    DMA_ClearStatus(OUT);
    OUT->M0AR = (uint32_t)data_ptr;
    DMA_SetCurrDataCounter(OUT, size);
    DMA_Cmd(OUT, ENABLE);
    return 0;
}


void DMA_SPI3inInit(void)
{
    DMA_InitTypeDef ch;
    DMA_StructInit(&ch);
    
    /* my params */
    ch.DMA_DIR = DMA_DIR_PeripheralToMemory;
    ch.DMA_Channel = DMA_Channel_0;
    ch.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI3->DR));
    ch.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*------------------------------------------------------------------------*/
    
    DMA_ITConfig(IN, DMA_IT_TC, ENABLE);
    DMA_Init(IN, &ch);
}

uint8_t DMA_SPI3inTransfer(uint8_t* dst_ptr, uint8_t size)
{
    if(DMA_GetFlagStatus(IN, DMA_FLAG_TCIF0))
    {
        return 1;
    }
    DMA_ClearStatus(IN);
    IN->M0AR = (uint32_t)dst_ptr;
    DMA_SetCurrDataCounter(IN, size);
    DMA_Cmd(IN, ENABLE);
    return 0;
}