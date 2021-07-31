#include "!Project_library.h"



void main(void)
{   
    // настройка тактирования
    uint8_t hse_res = HSE_Init();
    uint8_t lse_res = LSE_Init();  // сбрасывает Backup регистр
//    uint8_t rtc_res = RTC_Init();
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    
    // инициализация переферии
    ADC1_Init();
    
    int8_t temp = 0;
    uint8_t res = 0;
    while(1)
    {
        res = GetMCU_Temp(&temp);
    }
}
