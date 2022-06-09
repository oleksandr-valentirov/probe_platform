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
}