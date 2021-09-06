#include "!Project_library.h"



void main(void)
{   
    // настройка тактирования и пинов
    uint8_t hse_res = HSE_Init();
    MyGPIO_Init();
    uint8_t lse_res = LSE_Init();
//    uint8_t rtc_res = RTC_Init();
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    
    // прерывания
     __enable_irq();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    TIM9_Init();
    TIM9_CH_1_Set_Mode(0);
    TIM9_CH_1_Restart();
    TIM9_Start(60);
    
    // инициализация переферии
//    ADC1_Init();
//    USART1_Init();
//    SPI3_Init();
    
    // инициализация модулей
//    LoRa_Init();
    
//    int8_t temp = 0;
//    uint8_t res = 0;
    while(1)
    {
//        res = GetMCU_Temp(&temp);
//        while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)){}
//        CLEAR_BIT(LORA_SPI_CS_PORT->ODR, LORA_SPI_CS_PIN);
//        SPI3_send_data(temp);
    }
}
