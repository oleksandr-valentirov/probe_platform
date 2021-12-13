#include "!Project_library.h"

#ifdef USE_FULL_ASSERT
void assert_failed(u8* file, u32 line)
{ 
//    printf("\r\nassert_failed(). file: %s, line: %ld\r\n", file, line );
    while (1){}
}
#endif


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
    SysTick_Init();
//    ADC1_Init();
    USART1_Init();
//    SPI3_Init();
    
    // прерывания
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     __enable_irq(); 
    
    // инициализация модулей
//    LoRa_Init();

    while(1)
    {
        if (Sim_GetRIFlag())
        {
            if (SysTick_GetSimTime() && !0)
            {/* process call */
            }
            else
            {/* process URC/SMS */            
            }
        }
//        Sim_StateMachine();
    }
}
