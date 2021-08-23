/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H

#include "!Project_library.h"

/** @todo - перенести в L1 */
//void Tim6_Init(uint16_t period, bool is_one_pulse, void *callback_func);

void toggle_systick(bool state);
void SysTick_Init(uint32_t period, bool is_one_pulse, void (*callback_func)(void *data), void *callback_params);
void SysTick_Value_Update(uint32_t ticks);
void SysTick_Callback_DeInit(void);


#endif