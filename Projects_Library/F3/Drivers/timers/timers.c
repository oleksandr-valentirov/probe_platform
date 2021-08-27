#include "timers.h"


typedef struct{
    void (*systick_callback)(void *params);
    void *params;
} Callback;


/* TIM9 general functions --------------------------------------------------- */
void TIM9_Init(void)
{
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