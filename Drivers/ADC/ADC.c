#include "!Project_library.h"


/*
    ADC2 and ADC3 are not available in STM32F411xC/E.
*/


static void ADC1_Init(void);


void ADC_SubsystemInit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // включаем тактирование нужных АЦП
    
    ADC_DeInit();                                         // сбрасываем регистры всех АЦП
    
    ADC1_Init();
}


static void ADC1_Init(void)
{
    ADC_InitTypeDef adc;
    ADC_StructInit(&adc);
    
    // эти параметры дефолтные для ADC_StructInit
//    adc.ADC_Resolution = ADC_Resolution_12b;
//    adc.ADC_ContinuousConvMode = DISABLE;
//    adc.ADC_DataAlign = ADC_DataAlign_Right;
//    adc.ADC_NbrOfConversion = 1;
    
    ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);  // без прерыванию по окончанию преобразования
    ADC_Init(ADC1, &adc);
}


uint8_t ADC_VrefMeasure(void)
{
    ADC_Cmd(ADC1, ENABLE);
    return 0;
}

bool ADC1_Busy (void)
{
    return (bool)READ_BIT(ADC1->CR2, ADC_CR2_ADON);
}