#include "USART1.h"

#define FLAG_BUSY               1

#define CALLBACK_FUNC           &Sim_EndOfTransaction


static struct usart_state {
    void(*end_of_trancsaction_callback)(void);
    uint8_t* ptr;
    size_t counter;
} state;

/* Flags -------------------------------------------------------------------- */
static uint8_t flags;

#define Set_Busy_Flag         SET_BIT(flags, FLAG_BUSY)
#define Reset_Busy_Flag       CLEAR_BIT(flags, FLAG_BUSY)
#define Get_Busy_Flag         READ_BIT(flags, FLAG_BUSY)

uint8_t USART1_Get_Busy_Flag(void)
{
    return Get_Busy_Flag;
}
/* -------------------------------------------------------------------------- */

/**
  * @brief      инициалиазция многофункционального USART
  * @retval     None
  */
void USART1_Init(void)
{
    USART_DeInit(USART1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    USART_InitTypeDef init;
    USART_StructInit(&init);
    USART_Init(USART1, &init);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);

    // set callback
#ifdef CALLBACK_FUNC
    state.end_of_trancsaction_callback = CALLBACK_FUNC;
#else
    state.end_of_trancsaction_callback = NULL;
#endif
    
    USART_Cmd(USART1, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);
}


void USART1_Start_Transmission(void* source, size_t counter)
{
    if (Get_Busy_Flag || source == NULL || counter == 0)
    {
        return;
    }
    state.ptr = (uint8_t*) source;
    state.counter = counter;
//    Set_Busy_Flag;
    
    /* platform logic */
    USART1_Transmit_Next_Byte();
}


//void USART1_Start_Reception(void)
//{
//    if (Get_Busy_Flag)
//    {
//        return;
//    }
//    Set_Busy_Flag;
//    
//    /* platform logic */
//    USART1_Receive_Next_Byte();
//}

/**
 * @brief       Returns next byte to interrupt routine
 */
void USART1_Transmit_Next_Byte(void)
{
    if (state.ptr == NULL || state.counter == 0)
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC);
        return;
    }
    if (state.counter-- == 0)
    {
        Reset_Busy_Flag;
        if (state.end_of_trancsaction_callback != NULL)
        {
            state.end_of_trancsaction_callback();
        }
    }
    
    USART1->DR = *(state.ptr++);
}


/**
 * @brief       Gets next byte from interrupt routine
 */
void USART1_Receive_Next_Byte(void)
{
    uint8_t data = USART1->DR;
    switch (data)
    {
    case '\r':
    case '\n':
        if (state.end_of_trancsaction_callback != NULL)
            state.end_of_trancsaction_callback();
    default:
        Sim_putc(data);
    }
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
