#include "systick.h"


static unsigned int clock = 0;
static unsigned int sim_clock = 0;


/* SIM */
void SysTick_SetSimTimeMs(unsigned int time)
{
    sim_clock = time;
}

unsigned int SysTick_GetSimTime(void)
{
    return sim_clock;
}
/* ------------------------------------------- */


void SysTick_Init(void)
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);  // milisecond timer
}


unsigned int SysTick_GetCurrentClock(void)
{
    return clock;
}


void SysTick_WaitTill(unsigned int val)
{
    while(clock != val){};
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    clock++;
    
    if(sim_clock > 0) sim_clock--;
}


