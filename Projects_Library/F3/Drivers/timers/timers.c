#include "timers.h"


typedef struct{
    void (*systick_callback)(void *params);
    void *params;
} Callback;


/* TIM9 general functions --------------------------------------------------- */
void TIM9_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
    
    // basic init
    TIM_TimeBaseInitTypeDef tim;
    tim.TIM_Prescaler = SystemCoreClock / 1000000;  // 1 us == 1 count
    tim.TIM_ClockDivision = TIM_CKD_DIV1;  // relates to filters
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_RepetitionCounter = 0;  // is used only in TIM1 and TIM 8
    tim.TIM_Period = 0;
    TIM_TimeBaseInit(TIM9, &tim);
    
    // CH 1 init to be 1-wire time-slot keeper
    
    
    // CH 2 init to be transmitter/receiver
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


/* TIM9 CH 1 functions ------------------------------------------------------ */
void TIM9_CH_1_Start(uint16_t period)
{
}

void TIM9_CH_1_Restart(void)
{
}


/* TIM9 CH 2 functions ------------------------------------------------------ */
void TIM9_CH_2_Start(uint16_t period)
{
}

void TIM9_CH_2_Restart(void)
{
}

void TIM9_CH_2_Set_Mode(uint8_t mode)
{
    switch(mode){
    case 0:
        // set capture
        break;
    case 1:
        // set compare
    }
}