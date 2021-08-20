/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

#include "!Project_library.h"


void USART1_Init(void);
void USART2_Init(void);


uint8_t USART1_SendByte(uint8_t data);
uint8_t USART1_ReceiveByte(uint8_t* dest);


#endif