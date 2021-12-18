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
    uint8_t call_flag = 0, sms_txt_in_flag = 0;
    
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);
    EXTI_DeInit();
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    // прерывания
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     __enable_irq();
    
    /* modules init */
    Sim_StatusEXTI_Enable();
//    if (!READ_BIT(SIM_STATUS_PORT->IDR, SIM_STATUS_PIN))
    {
        Sim_CMD(ENABLE);
    }
    Sim_StateInit();

    while(1)
    {
        if (EXTI_GetFlagStatus(SIM_STATUS_EXTI))
        {
            EXTI_ClearFlag(SIM_STATUS_EXTI);
            /* repot SIM module status change */
        }

        if (Sim_GetNLFlag())
        {
            if (Sim_GetRIFlag() && !SysTick_GetSimTime() && !READ_BIT(SIM_RI_PORT->IDR, SIM_RI_PIN))
            {/* process call */
                Sim_ReceiveCall();
                Sim_ClearRIFlag();
            }
            else if (Sim_GetRIFlag() && !SysTick_GetSimTime() && READ_BIT(SIM_RI_PORT->IDR, SIM_RI_PIN))
            {/* process URS/SMS */
                Sim_ProcessLine();
                Sim_ClearRIFlag();
            }
            else
            {/* process all other messages */
                Sim_ProcessLine();
            }
        }
        
        if (Sim_GetReadyFlag() && !SysTick_GetSimTime())
        {
            call_flag = Sim_GetCallFlag();
            sms_txt_in_flag = Sim_GetTxtInFlag();

            if (!SysTick_GetSimStateClock() && !sms_txt_in_flag)
            {
                Sim_StateUpdateRSSI();
                SysTick_SimStateClockUpdate();
            }
            else if (call_flag)
            {
                Sim_SendSMSCmd();
            }
            else if (sms_txt_in_flag)
            {
                Sim_SendMsg();
            }
        }
    }
}
