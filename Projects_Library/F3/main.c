#include "!Project_library.h"


void main (void)
{
    uint8_t clock_status = HSE_Init();
//    RCC_ClocksTypeDef clocks;
//    RCC_GetClocksFreq(&clocks);
    if (clock_status){}
    MyGPIO_Init();
    
    while(1)
    {
    }
}