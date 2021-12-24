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
    /* clocks */
    uint8_t hse_res = HSE_Init();
    MyGPIO_Init();
    uint8_t lse_res = LSE_Init();
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
//    uint8_t rtc_res = RTC_Init();
    
    /* periph init */
    SysTick_Init();
//    ADC1_Init();
    USART1_Init();
//    SPI3_Init();
    
    /* exti init */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_DeInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);
    EXTI_DeInit();
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    /* interrupts */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     __enable_irq();
    
    /* modules init */
    Sim_init();
    
    while(1)
    {   
        Sim_main();
    }
}
