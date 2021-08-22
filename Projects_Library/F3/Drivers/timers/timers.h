/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H

#include "!Project_library.h"

/** @todo - перенести в L1 */
//void Tim6_Init(uint16_t period, bool is_one_pulse, void *callback_func);

void turn_off_systick(void);
void SysTick_Init(uint32_t period, bool is_one_pulse, void (*callback_func)(void *data), void *callback_params);


#endif