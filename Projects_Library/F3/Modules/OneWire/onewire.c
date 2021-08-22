#include "onewire.h"


static void reset_callback(void);


/**
  * @brief - опускает линию, ждёт 480 мкс и поднимает линию
  */
void reset(void)
{
    CLEAR_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);
    SysTick_Init(24000, true, &reset_callback);  // задержка в 500 мкс
}


/**
  * @brief - поднимает линию после задержки
  */
static void reset_callback(void)
{
    SET_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);  // поднять линию
    SysTick_Init(720, true, NULL);  // задержка в 15 мкс
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