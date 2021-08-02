#include "sx126x.h"


/**
 * @brief сброс настроек модуля
 * @retval None
 */
void reset(void)
{
//  delay(10);
    CLEAR_BIT(LORA_NRESET_PORT->ODR, LORA_NRESET_PIN);
//  delay(20);
    SET_BIT(LORA_NRESET_PORT->ODR, LORA_NRESET_PIN);
//  delay(10);
    while(READ_BIT(LORA_BUSY_PORT->IDR, LORA_BUSY_PIN));
}


/**
 * @brief Инициализация модуля sx1268
 * @retval
 *              @arg 0: - инициализация успешна
 */
uint8_t LoRa_Init(void)
{
    reset();
    return 0;
}


void LoRa_PinsInit(void)
{
    GPIO_InitTypeDef pin;
    
    /* Output pins ---------------------------------------------------------- */ 
    pin.GPIO_Pin = LORA_NRESET_PIN;
    pin.GPIO_Speed = GPIO_Low_Speed;
    pin.GPIO_OType = GPIO_OType_PP;
    pin.GPIO_PuPd = GPIO_PuPd_UP;       // NRESET valid on low state
    pin.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(LORA_NRESET_PORT, &pin);
    
    pin.GPIO_PuPd = GPIO_PuPd_DOWN;     // TXEN valid on high state
    pin.GPIO_Pin = LORA_TXEN_PIN;
    GPIO_Init(LORA_TXEN_PORT, &pin);
    
    pin.GPIO_PuPd = GPIO_PuPd_DOWN;
    pin.GPIO_Pin = LORA_RXEN_PIN;       // RXEN valid on high state
    GPIO_Init(LORA_RXEN_PORT, &pin);

    /* Input pins ----------------------------------------------------------- */ 
    pin.GPIO_Pin = LORA_BUSY_PIN;
    pin.GPIO_Mode = GPIO_Mode_IN;
    pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LORA_BUSY_PORT, &pin);
    
}



