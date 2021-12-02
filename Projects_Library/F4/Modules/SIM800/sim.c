#include  "sim.h"


char CMD_BASIC_CONF[] = "ATE0V0+CMEE=1;&W\n";
const char *CMD_TXT_MSG_FMT = "AT+CMGF=1\n";
const char *CMD_DST_NUM = "AT+CMGS=\"+380981153182\"\n";
const char *CMD_SEND_MSG = "+CMGS: 37";

static char resp_buffer[128];
static uint8_t pos = 0;
static uint8_t cur_cmd = 0;
static uint32_t last_heart_bit = 0;


/* Flags -------------------------------------------------------------------- */
static uint8_t flags;

#define SetReadyFlag            SET_BIT(flags, FLAG_READY)
#define GetReadyFlag            READ_BIT(flags, FLAG_READY)
#define ClearReadyFlag          CLEAR_BIT(flags, FLAG_READY)

uint8_t Sim_GetReadyFlag(void)
{
    return GetReadyFlag;
}
/* -------------------------------------------------------------------------- */

void Sim_EndOfTransaction(void)
{
    SetReadyFlag;
    pos = 0;
}

static void FlyMode(FunctionalState state)
{
    assert_param(IS_FUNCTIONAL_STATE(state));
    
    /* default mode */
    char cmd[10] = "AT+CFUN=1\n";
    if(state)
    {
        cmd[8] = '4';
    }
    ClearReadyFlag;
    USART1_Start_Transmission(cmd, 10);
}


static void Sim_SendAT(void)
{
    ClearReadyFlag;
    cur_cmd = CMD_AT;
    USART1_Start_Transmission("AT\n", 3);
}


void Sim_ToggleState(void)
{
    SET_BIT(SIM_PWRKEY_PORT->BSRRL, SIM_PWRKEY_PIN);
    SysTick_WaitTill(SysTick_GetCurrentClock() + 1000);
    SET_BIT(SIM_PWRKEY_PORT->BSRRH, SIM_PWRKEY_PIN);
}


void Sim_Init(void)
{   
    ClearReadyFlag;
    USART1_Start_Transmission(CMD_BASIC_CONF, strlen(CMD_BASIC_CONF));
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
    
    if(!READ_BIT(flags, FLAG_READY))
    {/* process current cmd */
        if(!READ_BIT(flags, FLAG_ALIVE))
        {
            Sim_SendAT();
        }
        else if (!READ_BIT(flags, INIT_FLAG))
        {
        }
    }
}

void Sim_putc(uint8_t c)
{
    resp_buffer[pos++] = c;
    pos &= SIM_BUF_MASK;
}