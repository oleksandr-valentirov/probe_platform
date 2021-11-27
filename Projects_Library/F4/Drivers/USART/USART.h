/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

#include "!Project_library.h"


void USART1_Init(void);
void USART1_Set_EndOfTransactionCallback(void(*f)(void));
uint8_t USART1_Get_Busy_Flag(void);

uint8_t USART1_SendByte(uint8_t data);
uint8_t USART1_ReceiveByte(uint8_t* dest);

void USART1_Start_Transmission(void* source, size_t counter);
void USART1_Start_Reception(void* destination, size_t counter);
void USART1_Transmit_Next_Byte(void);
void USART1_Receive_Next_Byte(void);


#endif