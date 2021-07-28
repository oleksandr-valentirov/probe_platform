#include "USART.h"


/**
  * @brief      USART to USB interface
  * @retval     None
  */
void USART1_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    USART_DeInit(USART1);
    USART_InitTypeDef init;
    USART_StructInit(&init);
    init.USART_BaudRate = 115200;
    USART_Init(USART1, &init);
}


void USART1_SendByte(uint8_t data)
{
    USART1->DR = data;
}

void USART1_ReceiveData(uint8_t* dest)
{
    *dest = USART1->DR;
}


/**
  * @brief      UART to RS422 interface
  * @retval     None
  */
void USART2_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    USART_DeInit(USART1);
    USART_InitTypeDef init;
    USART_StructInit(&init);
    USART_Init(USART2, &init);
}


void USART1_IRQHandler(void)
{
}


void USART2_IRQHandler(void)
{
}