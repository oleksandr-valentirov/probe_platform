#include "timers.h"


/* TIM9 general functions --------------------------------------------------- */
void TIM9_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
    
    // basic init
    TIM_TimeBaseInitTypeDef tim;
    tim.TIM_Prescaler = SystemCoreClock / 1000000;      // 1 us == 1 count
    tim.TIM_ClockDivision = TIM_CKD_DIV1;               // relates to filters
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_RepetitionCounter = 0;                      // is used only in TIM1 and TIM 8
    tim.TIM_Period = 0;                                 // measured in us.
    TIM_TimeBaseInit(TIM9, &tim);

    // CH 1 init to be 1-wire time-slot keeper
    CLEAR_BIT(TIM9->CCER, TIM_CCER_CC1E);
    TIM9->CCR1 = 0xFFFF;

    // CH 2 init to be transmitter/receiver
    SET_BIT(TIM9->CCER, TIM_CCER_CC2E);                 // enables capture/compare mode
    
    // interrupts
    SET_BIT(TIM9->CR1, TIM_CR1_URS);                    // запрет на генерацию ивента обновления
    //                  * update IT    * capture/compare IT
    SET_BIT(TIM9->DIER, TIM_DIER_UIE | TIM_DIER_CC2IE);
    NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
}

void TIM9_DeInit(void)
{
    TIM_DeInit(TIM9);
}

void TIM9_Enable(void)
{
    TIM_Cmd(TIM9, ENABLE);
}


void TIM9_Disable(void)
{
    TIM_Cmd(TIM9, DISABLE);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
    // CH 2 CC interrupt - data line state change
    if(READ_BIT(TIM9->SR, TIM_SR_CC2IF))
    {
        // input mode
        if(READ_BIT(TIM9->CCMR1, TIM_CCMR1_CC2S))
        {
        }
        else  // output mode
        {
        }
        CLEAR_BIT(TIM9->SR, TIM_SR_CC2IF);
    }
    
    // CH 1 update interrupt - next bit transmittion
    if(READ_BIT(TIM9->SR, TIM_SR_UIF))
    {  /** @todo - decide how to stop timer */
        CLEAR_BIT(TIM9->SR, TIM_SR_UIF);
    }
}


/* TIM9 CH 1 functions ------------------------------------------------------ */
void TIM9_CH_1_Start(uint16_t period)
{
    TIM9->ARR = period;
}

void TIM9_CH_1_Restart(void)
{
}


/* TIM9 CH 2 functions ------------------------------------------------------ */

/**
 * @brief - used only in output compare mode 
 *          to generate a signal of specific length
 */
void TIM9_CH_2_Start(uint16_t period)
{
    TIM9->CCR2 = period;
}

void TIM9_CH_2_Restart(void)
{
}

void TIM9_CH_2_Set_Mode(uint8_t mode)
{
    switch(mode){
    case 0:  // set input capture
        SET_BIT(TIM9->CCMR1, TIM_CCMR1_CC2S_0);  // CH2 to input mode from TI2
        break;
    case 1:  // set output compare
        CLEAR_BIT(TIM9->CCMR1, TIM_CCMR1_CC2S);
    }
}