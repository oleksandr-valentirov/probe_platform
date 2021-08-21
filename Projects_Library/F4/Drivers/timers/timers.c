#include "timers.h"


/**
  * @brief - таймер общего назначения на 1 канал.
  *          используется для замера интервалов для 1-wire
  */
void Tim10_Init(uint16_t period, bool is_one_pulse)
{
    // тактирование
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
    
    TIM_DeInit(TIM10);
    TIM_TimeBaseInitTypeDef timer;
    TIM_TimeBaseStructInit(&timer);
    timer.TIM_Period = period;
    TIM_TimeBaseInit(TIM10, &timer);
    
    if (is_one_pulse)
        TIM_SelectOnePulseMode(TIM10, TIM_OPMode_Single);
    
    TIM_Cmd(TIM10, ENABLE);
}