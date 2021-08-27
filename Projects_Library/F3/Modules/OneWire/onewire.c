#include "onewire.h"

/* Private functions ---------------------------------------------------------*/


/**
 * @brief - передаёт данные побитово
 */
void OneWireSendData(void)
{
}


/**
  * @brief - опускает линию, ждёт 480 мкс и поднимает линию
  */
void reset(void)
{
    CLEAR_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);
    // 500 us delay
}


void OneWire_Write_Pin_Init(void)
{
    GPIO_InitTypeDef pin;
    pin.GPIO_Speed = GPIO_Speed_2MHz;
    pin.GPIO_Mode = GPIO_Mode_Out_OD;
    pin.GPIO_Pin = ONEWIRE_PIN;
    GPIO_Init(ONEWIRE_PORT, &pin);
    SET_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);
}


void OneWrie_Read_Pin_Init(void)
{
    GPIO_InitTypeDef pin;
    pin.GPIO_Speed = GPIO_Speed_2MHz;
    pin.GPIO_Mode = GPIO_Mode_AF_OD;
    pin.GPIO_Pin = ONEWIRE_PIN;
    GPIO_Init(ONEWIRE_PORT, &pin);
    SET_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);
}