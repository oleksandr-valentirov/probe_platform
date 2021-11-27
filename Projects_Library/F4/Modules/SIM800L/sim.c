#include  "sim.h"

#define FLAG_READY              1
#define FLAG_MSG_SENT           2
#define FLAG_CALL_RECEIVED      4

const char *CMD_TXT_MSG_FMT = "AT+CMGF=1\r\n";
const char *CMD_DST_NUM = "AT+CMGS=\"+380981153182\"\r\n";
const char *CMD_SEND_MSG = "+CMGS: 37";

static char buffer[128];

/* Flags -------------------------------------------------------------------- */
static uint8_t flags;

#define SetReadyFlag()          SET_BIT(flags, FLAG_READY)
#define GetReadyFlag()          READ_BIT(flags, FLAG_READY)

uint8_t Sim_GetReadyFlag(void)
{
    return GetReadyFlag();
}

/**
  * @brief - only for UART callback
  */
void Sim_SetReadyFlag(void)
{
    SetReadyFlag();
}
/* -------------------------------------------------------------------------- */


void Sim_PolInit(void)
{
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