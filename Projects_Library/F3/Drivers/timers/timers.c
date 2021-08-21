#include "timers.h"


/**
  * @brief - базовый таймер.
  *          используется для замера интервалов для 1-wire
  */
void Tim6_Init(uint16_t period, bool is_one_pulse)
{
    // тактирование
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    TIM_DeInit(TIM6);
    TIM_TimeBaseInitTypeDef timer;
    TIM_TimeBaseStructInit(&timer);
    timer.TIM_Period = period;
    TIM_TimeBaseInit(TIM6, &timer);
    
    if (is_one_pulse)
        TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Single);
    
    TIM_Cmd(TIM6, ENABLE);
}