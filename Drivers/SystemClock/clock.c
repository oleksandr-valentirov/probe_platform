#include "clock.h"


/**
 *  @brief      Инициализируем кварц 32 кГц
 *  @retval     0 - всё ОК; 1 - кварц не запустился
 */
uint8_t LSE_Init(void)
{
    // Bypass нужен только если у нас кварц больше 32к - до 1 МГц
    SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
    uint16_t i = 0;
    while (i < 0xFFFF || !READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY))
        i++;
    if (!READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY))
        return 1;
    return 0;
}


/**
 *  @brief      Инициализируем кварц 32 кГц
 *  @retval     0 - всё ОК; 1 - кварц не запустился
 */
uint8_t HSE_Init(void)
{
    RCC_DeInit();  // тактируемся от HSI
    uint32_t i = 0;
    
    // запускаем HSE
    SET_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_HSEBYP);
    while (i < 0xFFFFFFFF || !READ_BIT(RCC->CR, RCC_CR_HSERDY))
        i++;
    // если не запустился
    if(!READ_BIT(RCC->CR, RCC_CR_HSERDY))
        return 1;
    
    // настраиваем PLL
//    RCC_PLLConfig(RCC_PLLSource_HSE, 25, 200, 2);
    
    // запускаем PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON); i = 0;
    while (i < 0xFFFFFFFF || !READ_BIT(RCC->CR, RCC_CR_PLLRDY))
        i++;
    // если не запустился
    if(!READ_BIT(RCC->CR, RCC_CR_HSERDY))
        return 2;
    
    // настраиваем скорость работы с памятью, питание и тактирование шин
    PWR->CR |= PWR_CR_VOS;  // второй режим питания (100 MHz)
    
    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN |     // предварительное извлечение инструкций из памяти
        FLASH_ACR_ICEN |                // кеширование инструкций
            FLASH_ACR_DCEN |            // кеширование памяти
                FLASH_ACR_LATENCY_3WS);  // задержка в 4 цикла CPU
    
    // переключаемся на HSE
    
    
    return 0;
}






