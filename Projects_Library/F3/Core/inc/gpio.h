/*
 * gpio.h
 *
 *  Created on: Apr 15, 2021
 *      Author: user
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <stdint.h>

	extern volatile uint8_t port_status;
	void gpio_init(void);

#endif /* INC_GPIO_H_ */
