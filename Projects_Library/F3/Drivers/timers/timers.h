/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H

#include "!Project_library.h"

/* TIM9 general functions --------------------------------------------------- */
void TIM9_Init(void);
void TIM9_DeInit(void);
void TIM9_Enable(void);
void TIM9_Disable(void);

/* TIM9 CH 1 functions ------------------------------------------------------ */
void TIM9_CH_1_Start(uint16_t period);
void TIM9_CH_1_Restart(void);

/* TIM9 CH 1 functions ------------------------------------------------------ */
void TIM9_CH_2_Start(uint16_t period);
void TIM9_CH_2_Restart(void);

#endif