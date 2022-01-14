/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART6_H
#define __USART6_H

#include "!Project_library.h"


#define USART6_BUF_SIZE         128
#define USART6_BUF_MASK         127


void USART6_Init(void);
void USART6_Set_EndOfTransactionCallback(void(*f)(void));
uint8_t USART6_Get_Busy_Flag(void);

uint8_t USART6_getc(uint8_t *ptr);
void USART6_putc(uint8_t c);

uint8_t USART6_Start_Transmission(void* source, size_t counter);
void USART6_Transmit_Next_Byte(void);
void USART6_Receive_Next_Byte(void);


#endif