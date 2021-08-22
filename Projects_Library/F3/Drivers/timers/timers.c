#include "timers.h"


typedef struct{
    void (*systick_callback)(void *params);
    void *params;
} Callback;

/** @todo - перенести в L1 */
//static void (*callback)(void) = NULL;
///**
//  * @brief - базовый таймер.
//  *          используется для замера интервалов для 1-wire
//  */
//void Tim6_Init(uint16_t period, bool is_one_pulse, void *callback_func)
//{
//    // тактирование
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//    
//    TIM_DeInit(TIM6);
//    TIM_TimeBaseInitTypeDef timer;
//    TIM_TimeBaseStructInit(&timer);
//    timer.TIM_Period = period;
//    TIM_TimeBaseInit(TIM6, &timer);
//    
//    if (is_one_pulse)
//        TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Single);
//    
//    /** @todo - возможно, нужно добавить контроль доступа к коллбекам */
//    callback = callback_func;
//    
//    TIM_Cmd(TIM6, ENABLE);
//}
//
//void TIM6_IRQHandler(void)
//{
//    if(callback != NULL)
//        callback();
//}


/* Private variables ---------------------------------------------------------*/
static Callback SysTickCallback; 
static bool systick_one_pulse = false;

void SysTick_Init(uint32_t period, bool is_one_pulse, void (*callback_func)(void *params), void *callback_params)
{
    systick_one_pulse = is_one_pulse;
   
    SysTickCallback.systick_callback = callback_func;
    SysTickCallback.params = callback_params;
    SysTick_Config(period);
}


void SysTick_Handler(void)
{
    if(systick_one_pulse)
        turn_off_systick();
    if(SysTickCallback.systick_callback != NULL)
    {
        SysTickCallback.systick_callback(SysTickCallback.params);
        SysTickCallback.systick_callback = NULL;
    }
}

void turn_off_systick(void) {
	CLEAR_BIT(SysTick->CTRL, (SysTick_CTRL_CLKSOURCE_Msk |
                                  SysTick_CTRL_TICKINT_Msk   |
                                  SysTick_CTRL_ENABLE_Msk)
			);
}