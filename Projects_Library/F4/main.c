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
    EXTI_DeInit();
    
    // прерывания
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     __enable_irq(); 
    
    /* modules init */
    Sim_StatusEXTI_Enable();
    Sim_CMD(ENABLE);

    while(1)
    {
        if (EXTI_GetFlagStatus(SIM_STATUS_EXTI))
        {
            EXTI_ClearFlag(SIM_STATUS_EXTI);
            /* repot SIM module status change */
        }

        if (Sim_GetNLFlag())
        {
            if (Sim_GetRIFlag() && !SysTick_GetSimTime())
            {
                if (READ_BIT(SIM_RI_PORT->IDR, SIM_RI_PIN))
                {/* process call */
                    Sim_ReceiveCall();
                }
                else
                {/* process URC/SMS */
                    Sim_ProcessLine();
                }
            }
            else if (!Sim_GetRIFlag())
            {/* process regular CMD response */
                Sim_ProcessLine();
            }
        }
    }
}
