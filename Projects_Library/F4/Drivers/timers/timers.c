#include "timers.h"




/* TIM9 general functions --------------------------------------------------- */
void TIM9_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
    
    // basic init
    TIM_TimeBaseInitTypeDef tim;
    tim.TIM_Prescaler = 95;        // 1 us == 1 count
    tim.TIM_ClockDivision = TIM_CKD_DIV1;                       // relates to filters
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_RepetitionCounter = 0;                      // is used only in TIM1 and TIM 8
    tim.TIM_Period = 0;                                 // measured in us.
    TIM_TimeBaseInit(TIM9, &tim);
    
    TIM_SelectOnePulseMode(TIM9, TIM_OPMode_Repetitive);

    // CH 1 init to be transmitter/receiver
//    SET_BIT(TIM9->CCER, TIM_CCER_CC1E);                 // enables capture/compare mode
    
    // interrupts
    SET_BIT(TIM9->CR1, TIM_CR1_ARPE); 
    //SET_BIT(TIM9->CR1, TIM_CR1_URS);                    // запрет на генерацию ивента обновления
    //                  * update IT    * capture/compare IT
    SET_BIT(TIM9->DIER, TIM_DIER_UIE | TIM_DIER_CC1IE);
    NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
}

void TIM9_DeInit(void)
{
    TIM_DeInit(TIM9);
}

void TIM9_Start(uint16_t period)
{
    TIM9->ARR = period;
    TIM_Cmd(TIM9, ENABLE);
}

bool TIM9_Is_Busy(void)
{
    return READ_BIT(TIM9->CR1, TIM_CR1_CEN);
}


/* TIM9 CH 1 functions ------------------------------------------------------ */

/**
 * @brief - used only in output compare mode 
 *          to generate a signal of specific length
 */
void TIM9_CH_1_Start(uint16_t period)
{
    TIM9->CCR1 = period;
    Turn_CH_1_On();
}

void TIM9_CH_1_Restart(void)
{
    Turn_CH_1_On();
}

void TIM9_CH_1_Stop(void)
{
    Turn_CH_1_Off();
}

/* 
функции для изменения состояния выхода при передаче.
форсить состояние - один из возможных вариантов, как работать с СС в режиме выхода
*/
/**
  * @brief - forces CH 1 output to be high
  */
void TIM9_CH_1_Set_High(void)
{
}

/**
  * @brief - forces CH 1 output to be low
  */
void TIM9_CH_1_Set_Low(void)
{
}
/*----------------------------------------------------------------------------*/

void TIM9_CH_1_Set_Mode(uint8_t mode)
{
}