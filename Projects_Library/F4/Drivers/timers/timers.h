/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H

#include "!Project_library.h"


/* TIM9 general functions --------------------------------------------------- */
void TIM9_Init(void);
void TIM9_DeInit(void);
void TIM9_Disable(void);
void TIM9_Start(uint16_t period);
void TIM9_Restart(void);

/* TIM9 CH 1 functions ------------------------------------------------------ */
void TIM9_CH_1_Start(uint16_t period);
void TIM9_CH_1_Restart(void);
void TIM9_CH_1_Set_Mode(uint8_t mode);


#endif