#include  "sim.h"

#define FLAG_READY              1
#define FLAG_MSG_SENT           2
#define FLAG_CALL_RECEIVED      4

const char CMD_TXT_MSG_FMT = "AT+CMGF=1\r\n";
const char CMD_DST_NUM = "AT+CMGS=\"+380981153182\"\r\n";
const char CMD_SEND_MSG = "+CMGS: 37";

static char buffer[128];
static uint8_t flags;


void Sim_PolInit(void)
{
    uint8_t i;
    uint8_t res = 0;
    
    uint8_t msg_len = 11;
    for(i = 0; i < msg_len; i++)
    {
        while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE)){}
        USART1_SendByte(CMD_TXT_MSG_FMT[i]);
    }
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)){}
    USART1_ReceiveByte(&res);
    if (res != 0)
    {
        return;
    }

    
    /** @todo - read num from call */
    msg_len = 25;
    for(i = 0; i < msg_len; i++)
    {
        while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE)){}
        USART1_SendByte(CMD_DST_NUM[i]);
    }
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)){}
    USART1_ReceiveByte(&res);
    if (res != 0)
    {
        return;
    }
}


void Sim_SendMsg(void)
{
}


void Sim_ReceiveCall(void)
{
}

void Sim_StateMachine(void)
{
    if(!READ_BIT(flags, FLAG_READY))
    {
        return;
    }
}