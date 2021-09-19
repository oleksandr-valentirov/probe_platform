/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H

#include "!Project_library.h"

/* user-defined macros ------------------------------------------------------ */
#define TIM9_Restart()          TIM_Cmd(TIM9, ENABLE)
#define TIM9_Disable()          TIM_Cmd(TIM9, DISABLE)
#define Turn_CH_1_On()          SET_BIT(TIM9->CCER, TIM_CCER_CC1E)
#define Turn_CH_1_Off()         CLEAR_BIT(TIM9->CCER, TIM_CCER_CC1E)


/* TIM9 general functions --------------------------------------------------- */
void TIM9_Init(void);
void TIM9_DeInit(void);
void TIM9_Start(uint16_t period);
bool TIM9_Is_Busy(void);

/* TIM9 CH 1 functions ------------------------------------------------------ */
void TIM9_CH_1_Start(uint16_t period);
void TIM9_CH_1_Restart(void);
void TIM9_CH_1_Set_Mode(uint8_t mode);

#endif