#include "sx126x.h"


/**
 * @brief сброс настроек модуля
 * @retval None
 */
void reset(void)
{
//  delay(10);
//  CLEAR_BIT(NRESET_PORT, NRESET_PIN);
//  delay(20);
//  SET_BIT(NRESET_PORT, NRESET_PIN);
//  delay(10);
//  while(READ_BIT(BUSY_PORT, BUSY_PIN));
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