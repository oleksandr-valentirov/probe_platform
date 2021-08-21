#include "clock.h"


/**
 *  @brief      Инициализируем кварц 32 кГц. 
 *              Сбрасывает Backup регистр, т.е. и LSE и RTC нужно настроить заново.
 *  @retval     
 *              @arg 0 - всё ОК
 *              @arg 1 - кварц не запустился
 */
uint8_t LSE_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    SET_BIT(PWR->CR, PWR_CR_DBP);  // disable BDCR protection
    SET_BIT(RCC->BDCR, RCC_BDCR_BDRST);  // reset BDCR
    CLEAR_BIT(RCC->BDCR, RCC_BDCR_BDRST);
    SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
    CLEAR_BIT(PWR->CR, PWR_CR_DBP);
    
    uint16_t i = 0;
    while (i < 0xFFFF && (READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == 0))
        i++;
    if (!READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY))
        return 1;
    
    SET_BIT(RCC->CFGR, RCC_CFGR_MCO1_0);  // provide LSE to MCO1
    return 0;
}


uint8_t MyRTC_Init(void)
{
    if (!READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY))
       return 1;
    
    RTC_DeInit();
//    RTC_Init();
    return 0;
}



/**
 *  @brief      Инициализируем кварц 25 МГц
 *  @retval     
 *              @arg 0 - всё ОК
 *              @arg 1 - кварц не запустился
 *              @arg 2 - PLL не запустился
 */
uint8_t HSE_Init(void)
{
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
    RCC_PLLConfig(RCC_PLLSource_HSE, 25, 192, 2, 4);
    
    // запускаем PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON); i = 0;
    while (i < 0xFFFF && (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0))
        i++;
    // если не запустился
    if(!READ_BIT(RCC->CR, RCC_CR_PLLRDY))
        return 2;
    
    // настраиваем скорость работы с памятью, питание и тактирование шин
    PWR->CR |= PWR_CR_VOS;  // второй режим питания (100 MHz)

    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN |     // предварительное извлечение инструкций из памяти
        FLASH_ACR_ICEN |                // кеширование инструкций
            FLASH_ACR_DCEN |            // кеширование памяти
                FLASH_ACR_LATENCY_3WS);  // задержка в 4 цикла CPU
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB
    RCC_PCLK1Config(RCC_HCLK_Div2);     // low-speed bus APB1
    RCC_PCLK2Config(RCC_HCLK_Div2);     // fast-speed bus APB2
    
    // переключаемся на HSE
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    SystemCoreClockUpdate();
    return 0;
}






