/*
 * my_system.h
 *
 *  Created on: Apr 15, 2021
 *      Author: user
 */

#ifndef INC_MY_SYSTEM_H_
#define INC_MY_SYSTEM_H_

#include "stdint.h"

#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (BIT))
//#define LOGGING


	void delay_ms(uint32_t milliseconds);
	void delay_us(uint32_t microseconds);
	void turn_off_systick(void);
	void my_system_init(void);


#endif /* INC_MY_SYSTEM_H_ */
