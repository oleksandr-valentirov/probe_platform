#include "temp_api.h"


/**
 * @brief функция замера температуры MCU
 * 
 * @param dest: указатель на результат
 *
 * @retval
 *              @arg 0: замер успешен
 *              @arg 1: замер не успешен
 */
uint8_t GetMCU_Temp(int8_t *dest)
{
    if (ADC1_Busy())
        return 1;
    
    // добавить какой-то контроль на уровне FreeRTOS
    
    uint32_t res = 0;
    uint16_t vref = 0, adc = 0;
    int16_t accumulator = 0;
    for (uint8_t i = 0; i < 10; i++)
    {
        /** @ToDo - добавить проверку успешности замеров */
        ADC_VrefMeasure(&vref);
        ADC_TempMeasure(&adc);
        
        res = (uint32_t)(adc * 1210 / vref);
        // mV to degree
        res = (res - 760) * 2;
        res = res / 5 + 25;

        accumulator += res;
    }
    
    *dest = (int8_t)(accumulator / 10);
    return 0;
}