#include "systick.h"


static unsigned int clock = 0;
static unsigned int sim_ri_clock = 0;
static unsigned int sim_state_clock = 20000;
static unsigned int gps_clock = 0;

/* GPS */
void SysTick_UpdateGPSClock(void)
{
    gps_clock = 3000; /* poll GPS each second */
}

unsigned int SysTick_GetGPSClock(void)
{
    return gps_clock;
}
/* ------------------------------------------ */


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
    SysTick_Config(96000);  // milisecond timer
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
    if (sim_ri_clock > 0) 
    {
        sim_ri_clock--;
    }
    if (sim_state_clock > 0) 
    {
        sim_state_clock--;
    }
    
    /* GPS */
    if (gps_clock > 0)
    {
        gps_clock--;
    }
}


