#include "onewire.h"


static void reset_callback(void *data);


/**
 * @brief - передаёт данные побитово, передаёт текущий бит
 *          и запускает таймер с коллбеком на себя
 */
void OneWireSendData(void* data)
{
    SET_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);  // поднять линию
    
    static uint8_t size;
    static uint8_t *data_to_send;

    if (data != NULL && size == 0)
    {
        OneWireData casted_data = *((OneWireData*)data);
        
        data_to_send = (uint8_t*)casted_data.data;
        size = casted_data.size;
    } 
    else if (data != NULL && size != 0)
        return;

    // проверяем size
    if (size == 0)
        return;
    
    // передаём следующий бит и создаём коллбек на себя
    CLEAR_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);
    // ставим таймер с коллбеком на себя
    if ((*data_to_send) & 1)
        SysTick_Init(720, true, &OneWireSendData, NULL);  // задержка в 15 мкс
    else
        SysTick_Init(2880, true, &OneWireSendData, NULL);  // задержка в 60 мкс
    
    (*data_to_send) >>= 1;  size--;  // сдвигаем только-что переданный бит
}


/**
  * @brief - опускает линию, ждёт 480 мкс и поднимает линию
  */
void reset(void)
{
    CLEAR_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);
    SysTick_Init(24000, true, &reset_callback, NULL);  // задержка в 500 мкс
}


/**
  * @brief - поднимает линию после задержки
  */
static void reset_callback(void *data)
{
    SET_BIT(ONEWIRE_PORT->ODR, ONEWIRE_PIN);  // поднять линию
    SysTick_Init(720, true, NULL, NULL);  // задержка в 15 мкс
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