/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART1_H
#define __USART1_H

#include "!Project_library.h"


#define USART1_BUF_SIZE         128
#define USART1_BUF_MASK         127


void USART1_Init(void);
void USART1_Set_EndOfTransactionCallback(void(*f)(void));
uint8_t USART1_Get_Busy_Flag(void);

uint8_t USART1_getc(uint8_t *ptr);
void USART1_putc(uint8_t c);

uint8_t USART1_Start_Transmission(void* source, size_t counter);
void USART1_Transmit_Next_Byte(void);
void USART1_Receive_Next_Byte(void);


void USART1_test_tx(void);


#endif