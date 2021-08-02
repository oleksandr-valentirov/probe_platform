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
    
}