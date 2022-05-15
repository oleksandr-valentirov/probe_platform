/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART1_H
#define __USART1_H

#include "!Project_library.h"


#define USART1_BUF_SIZE         128
#define USART1_BUF_MASK         127


void USART1_Init(void);
void USART1_IdleCmd(FunctionalState NewState);

void USART1_test_tx(void);
void USART1_ClearStatusReg(void);

#endif