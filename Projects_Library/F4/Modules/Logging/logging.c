#include "logging.h"
#include "usbd_cdc_if.h"


// This function can effectively check
// does 4-byte word contains zero byte or not
uint32_t haszero(uint32_t v)
{
    return (v - 0x01010101) & ~v & 0x80808080;
}


void Log_main(void)
{
    char log_buf[128];
    UBX_NAV_POSLLH pos;

    if(!UBX_GetCurPos(&pos))
    {   
        sprintf(log_buf, "\r\n\tAlt:\t%.3f m\r\n\tLat:\t%f\r\n\tLon:\t%f\r\n", pos.height / 1000.0f, pos.lat / 10000000.0f, pos.lon / 10000000.0f);
        while(CDC_Transmit_FS((uint8_t*)log_buf, 53) == USBD_BUSY){}
    }
    
    if(DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TEIF2))
    {
        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TEIF2);
        while(CDC_Transmit_FS((uint8_t*)"DMA2 Stream2 Tx err\r\n", 21) == USBD_BUSY){}
    }
    if(DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TEIF2))
    {
        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_DMEIF2);
        while(CDC_Transmit_FS((uint8_t*)"DMA2 Stream2 direct tx err\r\n", 28) == USBD_BUSY){}
    }
    if(DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TEIF2))
    {
        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_FEIF2);
        while(CDC_Transmit_FS((uint8_t*)"DMA2 Stream2 fifo tx err\r\n", 26) == USBD_BUSY){}
    }
}