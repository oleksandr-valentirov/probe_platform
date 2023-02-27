#include "dma.h"

static void DMA_USART1inClearStatus(void);
static void DMA_USART1outClearStatus(void);

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
    
    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
    DMA_Init(DMA2_Stream7, &ch);
}


void DMA_USART1outTransfer(uint8_t size)
{
    DMA_USART1outClearStatus();
    
    DMA_SetCurrDataCounter(DMA2_Stream7, size);
    DMA_Cmd(DMA2_Stream7, ENABLE);
    
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
    
    DMA_Init(DMA2_Stream2, &ch);
}


static void DMA_USART1inClearStatus(void)
{
    DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_FEIF2);
    DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2 | DMA_IT_HTIF2 | DMA_IT_TEIF2 | DMA_IT_DMEIF2 | DMA_IT_FEIF2);
}

static void DMA_USART1outClearStatus(void)
{
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7);
    DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7 | DMA_IT_HTIF7 | DMA_IT_TEIF7 | DMA_IT_DMEIF7 | DMA_IT_FEIF7);
}


void DMA_USART1inTransferStart(uint16_t size)
{
    DMA_USART1inClearStatus();    
    
    DMA2_Stream2->NDTR = size;
    DMA_Cmd(DMA2_Stream2, ENABLE);

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}


void DMA_USART1inTransferStop(void)
{
    USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE);
    DMA_Cmd(DMA2_Stream2, DISABLE);
}


uint16_t DMA_USART1inGetRemainingDataCounter(void)
{
    return DMA_GetCurrDataCounter(DMA2_Stream2);
}
