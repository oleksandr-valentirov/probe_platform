#include "USART.h"


/**
  * @brief      инициалиазция многофункционального USART
  * @retval     None
  */
void USART1_Init(void)
{
    USART_DeInit(USART1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);        
    USART_Cmd(USART1, ENABLE);
    
    USART_InitTypeDef init;
    USART_StructInit(&init);
    USART_Init(USART1, &init);   
}


/**
 * @brief       отправляет байт
 *
 * @param dest: указатель на переменную с байтом для передачи
 *
 * @retval      результат попытки передачи байта
 *              @arg 0: OK
 *              @arg 1: байт не был отправлен
 */
uint8_t USART1_SendByte(uint8_t data)
{
    if(!USART_GetFlagStatus(USART1, USART_FLAG_TXE))
        return 1;
    
    USART1->DR = data;
    return 0;
}


/**
 * @brief       вычитывает байт
 *
 * @param dest: указатель на переменную для сохранения вычитанного байта
 *
 * @retval      результат попытки чтения байта
 *              @arg 0: OK
 *              @arg 1: байт не был прочитан
 */
uint8_t USART1_ReceiveByte(uint8_t *dest)
{
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
        return 1;
    
    *dest = USART1->DR;
    return 0;
}


/**
  * @brief      UART to RS422 interface
  * @retval     None
  */
void USART2_Init(void)
{
    USART_DeInit(USART2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_Cmd(USART2, ENABLE);
    
    USART_InitTypeDef init;
    USART_StructInit(&init);
    USART_Init(USART2, &init);   
}


//void USART1_IRQHandler(void)
//{
//}
//
//
//void USART2_IRQHandler(void)
//{
//}