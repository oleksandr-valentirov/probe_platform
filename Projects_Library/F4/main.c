#include "!Project_library.h"
#include "usbd_cdc_if.h"

struct routines_list {
    void (*f)(void);
    struct routines_list *next;
};


void main(void)
{
    struct routines_list routines;
    routines.f = NULL;
    routines.next = NULL;
    
    struct routines_list *routines_ptr = &routines;
    uint8_t routines_counter = 0;

    /* clocks */
    uint8_t hse_res = HSE_Init();
    MyGPIO_Init();
    uint8_t lse_res = LSE_Init();
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
//    uint8_t rtc_res = RTC_Init();
    
    EXTI_DeInit();
    
    /* periph init */
    SysTick_Init();
//    ADC1_Init();
    USART1_Init();
//    USART2_Init();
    SPI3_Init();
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    MX_USB_DEVICE_Init();
    DMA_MyInit();
    
    /* exti init */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);
    
    NVIC_EnableIRQ(EXTI9_5_IRQn);       // SIM
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);  // USART
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);  // SPI3
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);  // SPI3

//#define TEST_HW
#ifdef TEST_HW
    USART1_test_tx();
    SPI3_Test();
    while(1){}
#endif
    
    /* interrupts */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    __enable_irq();
    
    /* modules init */
    /* GPS */
    routines_ptr->f = (void (*)(void)) UBX_Init();
    
    /* SIM */
    if(routines_ptr->f != NULL)
    {   /* previos init succeeded */
        routines_counter++;
        routines_ptr->next = malloc(sizeof(struct routines_list));
        if(routines_ptr->next == NULL)
        {
            CDC_Transmit_FS((uint8_t*)"malloc err\r\n", 12);
            while(1){}
        }
        else
        {
            routines_ptr = routines_ptr->next;
            routines_ptr->f = (void (*)(void)) Sim_init();
            routines_ptr->next = NULL;
        }
    }
    else
    {   /* previous init failed */
        routines_ptr->f = (void (*)(void)) Sim_init();
    }

    /* IMU */
    if(routines_ptr->f != NULL)
    {   /* previos init succeeded */
        routines_counter++;
        routines_ptr->next = malloc(sizeof(struct routines_list));
        if(routines_ptr->next == NULL)
        {
            CDC_Transmit_FS((uint8_t*)"malloc err\r\n", 12);
            while(1){}
        }
        else
        {
            routines_ptr = routines_ptr->next;
            routines_ptr->f = (void (*)(void)) IMU_Init();
            routines_ptr->next = NULL;
        }
    }
    else
    {   /* previous init failed */
        routines_ptr->f = (void (*)(void)) IMU_Init();
    }

    while(1)
    {
        /* exxecutes if periph device is presented */
        routines_ptr = &routines;
        for(uint8_t i = 0; i < routines_counter; i++)
        {
            routines_ptr->f();
            routines_ptr = routines_ptr->next;
        }
        
        /* executes always */
        Log_main();
    }
}


void Error_Handler(void)
{
    while(1)
    {
    }
}