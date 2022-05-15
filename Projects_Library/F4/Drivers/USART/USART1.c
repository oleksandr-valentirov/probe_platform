#include "USART1.h"

#define FLAG_BUSY               1


void USART1_Init(void)
{
    USART_DeInit(USART1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    USART_InitTypeDef init;
    USART_StructInit(&init);
    USART_Init(USART1, &init);

    NVIC_EnableIRQ(USART1_IRQn);
    USART_Cmd(USART1, ENABLE);
    
    USART1->SR;
    USART1->DR;
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}


void USART1_ClearStatusReg(void)
{
    USART1->SR;
    USART1->DR;
}


void USART1_IdleCmd(FunctionalState NewState)
{
    USART1->SR;
    USART1->DR;
    USART_ITConfig(USART1, USART_IT_IDLE, NewState);
}


void USART1_test_tx(void)
{
    for(uint8_t i = 0; i < 10; i++)
    {
        while(!READ_BIT(USART1->SR, USART_SR_TXE)){}
        USART1->DR = i + 48;
    }
}