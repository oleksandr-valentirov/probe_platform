#include "dma.h"

#define OUT     DMA2_Stream7
#define IN      DMA2_Stream2

/**
  * @desc - GPS circular buffer to UART1
  */
void DMA_USART1outInit(uint32_t* gps_buf_ptr)
{
    DMA_InitTypeDef ch;
    DMA_StructInit(&ch);
    
    /* my params */
    ch.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    ch.DMA_Channel = DMA_Channel_4;
    ch.DMA_Memory0BaseAddr = (uint32_t)gps_buf_ptr;
    ch.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
    ch.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*------------------------------------------------------------------------*/
    
    DMA_ITConfig(OUT, DMA_IT_TC, ENABLE);
    DMA_Init(OUT, &ch);
}


void DMA_USART1outTransfer(uint8_t size)
{
    DMA_ClearStatus(OUT);
    
    DMA_SetCurrDataCounter(OUT, size);
    DMA_Cmd(OUT, ENABLE);
    
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_TC);
}


void DMA_USART1inInit(uint32_t* gps_buf_ptr)
{
    DMA_InitTypeDef ch;
    DMA_StructInit(&ch);
    
    /* my params */
    ch.DMA_DIR = DMA_DIR_PeripheralToMemory;
    ch.DMA_Channel = DMA_Channel_4;
    ch.DMA_Memory0BaseAddr = (uint32_t)gps_buf_ptr;
    ch.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
    ch.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*------------------------------------------------------------------------*/
    
    DMA_Init(IN, &ch);
}


void DMA_USART1inTransferStart(uint16_t size)
{
    DMA_ClearStatus(IN);;    
    
    IN->NDTR = size * 2;
    DMA_Cmd(IN, ENABLE);

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}


void DMA_USART1inTransferStop(void)
{
    USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE);
    DMA_Cmd(IN, DISABLE);
}


uint16_t DMA_USART1inGetRemainingDataCounter(void)
{
    return DMA_GetCurrDataCounter(DMA2_Stream2);
}
