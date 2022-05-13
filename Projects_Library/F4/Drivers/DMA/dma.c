#include "dma.h"

static void DMA_GPSinClearStatus(void);
static void DMA_GPSoutClearStatus(void);

/**
  * @desc - GPS circular buffer to UART1
  */
void DMA_GPSoutInit(uint32_t* gps_buf_ptr)
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


void DMA_GPSoutTransfer(uint8_t size)
{
    DMA_GPSoutClearStatus();
    
    DMA_SetCurrDataCounter(DMA2_Stream7, size);
    DMA_Cmd(DMA2_Stream7, ENABLE);
    
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_TC);
}


void DMA_GPSinInit(uint32_t* gps_buf_ptr)
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


static void DMA_GPSinClearStatus(void)
{
    DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_FEIF2);
    DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2 | DMA_IT_HTIF2 | DMA_IT_TEIF2 | DMA_IT_DMEIF2 | DMA_IT_FEIF2);
}

static void DMA_GPSoutClearStatus(void)
{
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7);
    DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7 | DMA_IT_HTIF7 | DMA_IT_TEIF7 | DMA_IT_DMEIF7 | DMA_IT_FEIF7);
}


void DMA_GPSinTransferStart(uint16_t size)
{
    DMA_GPSinClearStatus();    
    
    DMA2_Stream2->NDTR = size * 2;
    DMA_Cmd(DMA2_Stream2, ENABLE);

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    
    USART1_IdleCmd(ENABLE);
}


void DMA_GPSinTransferStop(void)
{
    USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE);
    DMA_Cmd(DMA2_Stream2, DISABLE);
}


uint16_t DMA_GPSinGetRemainingDataCounter(void)
{
    return DMA_GetCurrDataCounter(DMA2_Stream5);
}
