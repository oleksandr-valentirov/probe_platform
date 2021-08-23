#include "!Project_library.h"


void main (void)
{
    uint8_t clock_status = HSE_Init();
//    RCC_ClocksTypeDef clocks;
//    RCC_GetClocksFreq(&clocks);
    if (clock_status){}
    MyGPIO_Init();
    
//    reset();
    
    uint8_t test_byte = 170;
    OneWireData data;
    data.size = 1;
    data.data = &test_byte;
    OneWireSendData(&data);
    
    while(1)
    {
    }
}