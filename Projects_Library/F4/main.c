#include "!Project_library.h"



void main(void)
{   
    // настройка тактирования и пинов
    uint8_t hse_res = HSE_Init();
    MyGPIO_Init();
    uint8_t lse_res = LSE_Init();
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
//    uint8_t rtc_res = RTC_Init();
    
    // инициализация переферии
    TIM9_Init();
//    ADC1_Init();
//    USART1_Init();
//    SPI3_Init();
    
    // прерывания
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     __enable_irq(); 
    
    // инициализация модулей
     OneWire_Init();
//    LoRa_Init();

    while(1)
    {
        OneWire_Main();
    }
}
