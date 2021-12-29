/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART2_H
#define __USART2_H

#include "!Project_library.h"


#define USART2_BUF_SIZE         128
#define USART2_BUF_MASK         127


void USART2_Init(void);
void USART2_Set_EndOfTransactionCallback(void(*f)(void));
uint8_t USART2_Get_Busy_Flag(void);

uint8_t USART2_getc(uint8_t *ptr);
void USART2_putc(uint8_t c);

uint8_t USART2_Start_Transmission(void* source, size_t counter);
void USART2_Transmit_Next_Byte(void);
void USART2_Receive_Next_Byte(void);


#endif