#include "systick.h"


static unsigned int clock = 0;
static unsigned int sim_ri_clock = 0;
static unsigned int sim_state_clock = 20000;


/* SIM */
void SysTick_SetSimTimeMs(unsigned int time)
{
    sim_ri_clock = time;
}

unsigned int SysTick_GetSimTime(void)
{
    return sim_ri_clock;
}

void SysTick_SimStateClockUpdate(void)
{
    sim_state_clock = 180000;
}

unsigned short SysTick_GetSimStateClock(void)
{
    return sim_state_clock;
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
    
    /* SIM */
    if(sim_ri_clock > 0) 
    {
        sim_ri_clock--;
    }
    if(sim_state_clock > 0) 
    {
        sim_state_clock--;
    }
}


