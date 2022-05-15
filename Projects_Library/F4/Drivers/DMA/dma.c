#include "dma.h"

void DMA_ClearStatus(DMA_Stream_TypeDef* DMAy_Streamx)
{
    DMA_ClearFlag(DMAy_Streamx, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_FEIF2);
    DMA_ClearITPendingBit(DMAy_Streamx, DMA_IT_TCIF2 | DMA_IT_HTIF2 | DMA_IT_TEIF2 | DMA_IT_DMEIF2 | DMA_IT_FEIF2);
}