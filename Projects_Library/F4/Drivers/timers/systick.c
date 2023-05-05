#include "systick.h"


static unsigned int clock = 0;
static unsigned int sim_downcounter = 0;
static unsigned int sim_gen_clock = 0;
static unsigned int gps_clock = 0;
static uint8_t IMU_flag = 0;

/* GPS */
void SysTick_UpdateGPSClock(void)
{
    gps_clock = 200; /* poll GPS each second */
}

unsigned int SysTick_GetGPSClock(void)
{
    return gps_clock;
}
/* ------------------------------------------ */


/* SIM */
void SysTick_SetSimTimeMs(unsigned int time)
{
    sim_downcounter = time;
}

unsigned int SysTick_GetSimTime(void)
{
    return sim_downcounter;
}

unsigned short SysTick_GetSimGenClock(void)
{
    return sim_gen_clock;
}
/* ------------------------------------------- */


/* IMU */
uint8_t SysTick_GetIMUFlag(void)
{
    if(IMU_flag)
    {
        IMU_flag = 0;
        return 1;
    }
    return 0;
}


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
    
    /* read IMU with 100 Hz freq */
    if (!(clock % 100))
    {
        IMU_flag = 1;
    }
    
    /* SIM */
    if (sim_downcounter > 0) 
    {
        sim_downcounter--;
    }
    sim_gen_clock++;
    
    /* GPS */
    if (gps_clock > 0)
    {
        gps_clock--;
    }
}


