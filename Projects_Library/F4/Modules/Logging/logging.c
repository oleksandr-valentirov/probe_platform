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
        memcpy(log_buf, "\tAlt -\t", 7);
        sprintf(log_buf + 7, "%f m\n", pos.hMSL / 1000.0f);
        while(CDC_Transmit_FS((uint8_t*)log_buf, strlen(log_buf)) == USBD_BUSY){}
    }
}