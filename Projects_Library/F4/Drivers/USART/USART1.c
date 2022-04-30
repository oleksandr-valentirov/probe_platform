#include "USART1.h"

#define FLAG_BUSY               1


static struct usart_state {
    void(*end_of_trancsaction_callback)(void);
    uint8_t* ptr;
    size_t counter;
} state;

static uint8_t resp_buffer[USART1_BUF_SIZE] = {0};
static uint8_t rd_pos = 0;
static uint8_t wr_pos = 0;


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

void USART1_Init(void)
{
    USART_DeInit(USART1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    USART_InitTypeDef init;
    USART_StructInit(&init);
    USART_Init(USART1, &init);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    
    NVIC_EnableIRQ(USART1_IRQn);
    USART_Cmd(USART1, ENABLE);
}


uint8_t USART1_Start_Transmission(void* source, size_t counter)
{
    if (Get_Busy_Flag || source == NULL || counter == 0)
    {
        return 1;
    }
    state.ptr = (uint8_t*) source;
    state.counter = counter;
    Set_Busy_Flag;
    
    /* platform logic */
    USART1_Transmit_Next_Byte();
    return 0;
}


/**
 * @brief       Returns next byte to interrupt routine
 */
void USART1_Transmit_Next_Byte(void)
{
    if (state.ptr == NULL)
    {
        return;
    }
    
    if (state.counter > 0)
    {
        state.counter--;
        USART1->DR = *(state.ptr++);
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
void USART1_putc(uint8_t c)
{
    resp_buffer[wr_pos++] = c;
    wr_pos &= USART1_BUF_MASK;
}

uint8_t USART1_getc(uint8_t *ptr)
{
    if (wr_pos == rd_pos)
    {
        return 0;
    }
    *ptr = resp_buffer[rd_pos++];
    rd_pos &= USART1_BUF_MASK;
    return 1;
}
/* ---------------------------- */


void USART1_test_tx(void)
{
    for(uint8_t i = 0; i < 10; i++)
    {
        while(!READ_BIT(USART1->SR, USART_SR_TXE)){}
        USART1->DR = i + 48;
    }
}