#include "logging.h"
#include "usbd_cdc_if.h"


void Log_main(void)
{
    char log_buf[128];
    UBX_NAV_POSLLH pos;
    if(!UBX_GetCurPos(&pos))
    {   
        memcpy(log_buf, "\tAlt -\t", 7);
        sprintf(log_buf + 7, "%f\n", pos.alt / 1000.0f);
    }
}