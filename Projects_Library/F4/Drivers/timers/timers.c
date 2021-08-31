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

    // CH 1 init to be transmitter/receiver
    SET_BIT(TIM9->CCER, TIM_CCER_CC1E);                 // enables capture/compare mode
    
    // interrupts
    SET_BIT(TIM9->CR1, TIM_CR1_URS);                    // запрет на генерацию ивента обновления
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

void TIM9_Restart(void)
{
}


void TIM9_Disable(void)
{
    TIM_Cmd(TIM9, DISABLE);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
    // CH 1 CC interrupt - data line state change
    if(READ_BIT(TIM9->SR, TIM_SR_CC1IF))
    {
        // input mode
        if(READ_BIT(TIM9->CCMR1, TIM_CCMR1_CC2S))
        {
        }
        else  // output mode
        {
        }
    }
    
    // update interrupt - next bit transmittion
    if(READ_BIT(TIM9->SR, TIM_SR_UIF))
    {  /** @todo - decide how to stop timer */
    }
    TIM9->SR = 0;
}


/* TIM9 CH 1 functions ------------------------------------------------------ */

/**
 * @brief - used only in output compare mode 
 *          to generate a signal of specific length
 */
void TIM9_CH_1_Start(uint16_t period)
{
}

void TIM9_CH_1_Restart(void)
{
}



void TIM9_CH_1_Set_Mode(uint8_t mode)
{
    CLEAR_BIT(TIM9->CCER, TIM_CCER_CC1E);       // turn CH off
    
    /* change mode ---------------------------------------------------------- */
    switch(mode){
    case 0:  // set input capture
        SET_BIT(TIM9->CCMR1, TIM_CCMR1_CC1S_0);  // CH1 to input mode from TI1
        CLEAR_BIT(TIM9->CCER, TIM_CCER_CC1P);    // polarity high
        break;
    case 1:  // set output compare
        CLEAR_BIT(TIM9->CCMR1, TIM_CCMR1_CC1S);
    }
    /* ---------------------------------------------------------------------- */
    
    SET_BIT(TIM9->CCER, TIM_CCER_CC1E);         // turn CH on
}