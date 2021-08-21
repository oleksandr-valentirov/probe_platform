/*
 * spi.c
 *
 *  Created on: Apr 17, 2021
 *      Author: user
 */

#ifndef INC_SPI_C_
#define INC_SPI_C_

#include "stdint.h"

	void spi1_init(uint8_t crc_polinomial);
	void spi1_send_data(uint8_t* data, uint32_t len);

#endif /* INC_SPI_C_ */
