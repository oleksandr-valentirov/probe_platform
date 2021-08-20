/*
 * usart.h
 *
 *  Created on: Apr 15, 2021
 *      Author: user
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include <stdint.h>
#include <stdbool.h>

	extern volatile bool uart1_initialized;

	void usart1_init(uint32_t baud);
	void DBG_printf(const char *fmt, ...);

#endif /* INC_USART_H_ */
