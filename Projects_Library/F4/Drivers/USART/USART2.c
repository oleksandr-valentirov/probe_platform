#include "USART2.h"

#define FLAG_BUSY               1

#define CALLBACK_FUNC           &GPS_EndOfTransaction


static struct usart_state {
    void(*end_of_trancsaction_callback)(void);
    uint8_t* ptr;
    size_t counter;
} state;

static uint8_t resp_buffer[USART2_BUF_SIZE] = {0};
static uint8_t rd_pos = 0;
static uint8_t wr_pos = 0;


/* Flags -------------------------------------------------------------------- */
static uint8_t flags;

#define Set_Busy_Flag         SET_BIT(flags, FLAG_BUSY)
#define Reset_Busy_Flag       CLEAR_BIT(flags, FLAG_BUSY)
#define Get_Busy_Flag         READ_BIT(flags, FLAG_BUSY)

uint8_t USART2_Get_Busy_Flag(void)
{
    return Get_Busy_Flag;
}
/* -------------------------------------------------------------------------- */

void USART2_Init(void)
{
    USART_DeInit(USART2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    USART_InitTypeDef init;
    USART_StructInit(&init);
    /* Rx only for now */
    init.USART_Mode = USART_Mode_Rx;
    USART_Init(USART2, &init);
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART2, USART_IT_TC, ENABLE);

    // set callback
#ifdef CALLBACK_FUNC
    state.end_of_trancsaction_callback = CALLBACK_FUNC;
#else
    state.end_of_trancsaction_callback = NULL;
#endif
    
    USART_Cmd(USART2, ENABLE);
    NVIC_EnableIRQ(USART2_IRQn);
}


uint8_t USART2_Start_Transmission(void* source, size_t counter)
{
    if (Get_Busy_Flag || source == NULL || counter == 0)
    {
        return 1;
    }
    state.ptr = (uint8_t*) source;
    state.counter = counter;
    Set_Busy_Flag;
    
    /* platform logic */
    USART2_Transmit_Next_Byte();
    return 0;
}


/**
 * @brief       Returns next byte to interrupt routine
 */
void USART2_Transmit_Next_Byte(void)
{
    if (state.ptr == NULL)
    {
        return;
    }
    
    if (state.counter > 0)
    {
        state.counter--;
        USART2->DR = *(state.ptr++);
    }
    else
    {
        Reset_Busy_Flag;
        state.ptr = NULL;
        if (state.end_of_trancsaction_callback != NULL)
        {
            state.end_of_trancsaction_callback();
        }
    }
}

/* IO -------------------------- */
void USART2_putc(uint8_t c)
{
    resp_buffer[wr_pos++] = c;
    wr_pos &= USART2_BUF_MASK;
}

uint8_t USART2_getc(uint8_t *ptr)
{
    if (wr_pos == rd_pos)
    {
        return 0;
    }
    *ptr = resp_buffer[rd_pos++];
    rd_pos &= USART2_BUF_MASK;
    return 1;
}
/* ---------------------------- */