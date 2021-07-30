#include "!Project_library.h"


int8_t TempMeasure(void);


void main(void)
{
    // настройка тактирования
    uint8_t hse_res = HSE_Init();
//    uint8_t lse_res = LSE_Init();
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    
    // инициализация переферии
    ADC1_Init();
    
    int8_t temp = 0;
    while(1)
    {
        temp = TempMeasure();
    }
}


int8_t TempMeasure(void)
{
    uint32_t res = 0;
    uint16_t vref = 0, adc = 0;
    int16_t accumulator = 0;
    int16_t temp = 0;
    for (uint8_t i = 0; i < 10; i++)
    {
        ADC_VrefMeasure(&vref);
        ADC_TempMeasure(&adc);
        
        res = (uint32_t)(adc * 1210 / vref);
        // mV to degree
        temp = (res - 760) * 2;
        temp = temp / 5 + 25;

        accumulator += temp;
    }
    return (int8_t)(accumulator / 10);
}