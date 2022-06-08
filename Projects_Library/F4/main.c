#include "!Project_library.h"


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
//    USART2_Init();
    SPI3_Init();
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
//    MX_USB_DEVICE_Init();
    
    /* exti init */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);
    EXTI_DeInit();
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    
#ifdef TEST_HW
    USART1_test_tx();
#endif
    
#ifdef TEST_FW
    while(1){}
#endif
    
    /* interrupts */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    __enable_irq();
    
    /* modules init */
//    Sim_init();
    UBX_Init();

    while(1)
    {   
//        Sim_main();
        if(!UBX_GetFlagMsgRx())
        {
            UBX_main();
        }
    }
}

void Error_Handler(void)
{
    while(1)
    {
    }
}