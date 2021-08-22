/*
 * clock.c
 *
 *  Created on: Apr 18, 2021
 *      Author: Aleks Valentirov
 */

#include "clock.h"
// ------------------------------------


uint8_t HSE_Init(void){
    RCC_DeInit();  // тактируемся от HSI
    uint16_t i = 0;
        
    // запускаем HSE
    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while (i < 0xFFFF && (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0))
        i++;
    // если не запустился
    if(!READ_BIT(RCC->CR, RCC_CR_HSERDY))
        return 1;
    
    // настраиваем PLL
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
    
    // запускаем PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON); i = 0;
    while (i < 0xFFFF && (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0))
        i++;
    // если не запустился
    if(!READ_BIT(RCC->CR, RCC_CR_PLLRDY))
        return 2;
    
    // настраиваем скорость работы с памятью и тактирование шин
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_1);
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB
    FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Disable);
    /** @attention - здесь можно настроить AHB */
//    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /* ---------------------------------- */
    
    RCC_PCLK1Config(RCC_HCLK_Div2);     // low-speed bus APB1
    RCC_PCLK2Config(RCC_HCLK_Div1);     // fast-speed bus APB2
    
    // переключаемся на HSE
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    SystemCoreClockUpdate();
    return 0;
}
