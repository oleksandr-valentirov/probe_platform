#include "!Project_library.h"


void main(void)
{
    uint8_t hse_res = HSE_Init();
//    uint8_t lse_res = LSE_Init();
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    
    ADC1_Init();
    
    uint16_t vref = 0;
    int16_t temp = 0;
    uint16_t adc = 0;
    int32_t res = 0;
    while(1)
    {
        ADC_VrefMeasure(&vref);
        ADC_TempMeasure(&adc);
        
        res = (int32_t)(adc * 1210 / vref);
        
        // mV to degree
        temp = (res - 760) * 2;
        temp = temp / 5 + 25;
    }
}
