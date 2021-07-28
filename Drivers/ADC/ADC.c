#include "ADC.h"


/*
    ADC2 and ADC3 are not available in STM32F411xC/E.
*/


#define VREFINT_CAL_ADDR        0x1FFF7A2A


uint16_t get_calib_vref(void)
{
    return *((uint16_t*) VREFINT_CAL_ADDR);
}


void ADC1_Init(void)
{    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_DeInit();
    
    ADC_InitTypeDef adc;
    ADC_StructInit(&adc);
    ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);  // без прерыванию по окончанию преобразования
    ADC_Init(ADC1, &adc);
    
    ADC_CommonInitTypeDef adc_common;
    ADC_CommonStructInit(&adc_common);
    // при тактировании APB2 в 48 МГц нужно минимум 10 мкс чтобы успеть померять
    adc_common.ADC_Prescaler = ADC_Prescaler_Div8;
    ADC_CommonInit(&adc_common);
}


uint8_t ADC_VrefMeasure(uint16_t* res)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_56Cycles);
    
    ADC_Cmd(ADC1, ENABLE);
    ADC_TempSensorVrefintCmd(ENABLE);
    SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
    
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
        // добавить таймаут и возвращать 1
        // или семафору
    }
    
    *res = ADC1->DR;
    ADC_TempSensorVrefintCmd(DISABLE);
    ADC_Cmd(ADC1, DISABLE);
    return 0;
}


uint8_t ADC_TempMeasure(uint16_t* res)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_18, 1, ADC_SampleTime_56Cycles);
    
    ADC_Cmd(ADC1, ENABLE);
    ADC_TempSensorVrefintCmd(ENABLE);
    SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
    
    while(!READ_BIT(ADC1->SR, ADC_FLAG_EOC))
    {
        // добавить таймаут и возвращать 1
        // или семафору
    }
    
    *res = ADC1->DR;
    ADC_TempSensorVrefintCmd(DISABLE);
    ADC_Cmd(ADC1, DISABLE);
    return 0;
}


bool ADC1_Busy (void)
{
    return (bool)READ_BIT(ADC1->CR2, ADC_CR2_ADON);
}