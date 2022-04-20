#include "dma.h"


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
    /*------------------------------------------------------------------------*/
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_Init(DMA2_Stream7, &ch);
}


void DMA_GPSoutTransfer(uint8_t size)
{
    DMA_SetCurrDataCounter(DMA2_Stream7, size);
    DMA_Cmd(DMA2_Stream7, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_TC);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

}
