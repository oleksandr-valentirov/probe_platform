#include  "sim.h"


char CMD_BASIC_CONF[] = "ATE0V0+CMEE=1;&W\n";
const char *CMD_TXT_MSG_FMT = "AT+CMGF=1\n";
const char *CMD_DST_NUM = "AT+CMGS=\"+380981153182\"\n";
const char *CMD_SEND_MSG = "+CMGS: 37";


typedef enum {
    AUTO_T,
    AWAITED_T,
    RESP_CODE_T
} CMD_TYPE;

typedef enum {
    OK,
    RING,
    NO_CARRIER,
    SIM_ERROR
} RESP_CODE;


const char *auto_responses[12] = {
    "RDY",                      /*     Startup messages     */
    "+CFUN",                    /* functionality indication */
    "+CPIN",                    /*                          */
    "Call Ready",               /*                          */
    "SMS Ready",                /* ------------------------ */
    

    "+CLIP",                    /* number checker */
    "+CMTE",                    /* incorrect temperature */
    
    "UNDER-VOLTAGE POWER DOWN", /* Power-down messages */
    "NORMAL POWER DOWN",        /*                     */
    "OVER-VOLTAGE POWER DOWN",  /* ------------------- */
    
    "UNDER-VOLTAGE WARNNING",   /* Power warnings */
    "OVER-VOLTAGE WARNNING"
};

const char *resp_codes[4] = {
    "0\r\n",            /* OK */
    "2\r\n",            /* ring */
    "3\r\n",            /* no carrier (ex. - call was canceled) */
    "4\r\n"             /* error */
};

/* response buffer */
static char resp_buffer[128];
static char number[15];
static uint8_t pos = 0;
static uint8_t cmd_led = 0;


/* state machine */
static uint8_t state = SIM_ST_POWER_ON;
static int8_t cur_cmd = SIM_CMD_INIT;

/* not needed when parsing 'shutdown' msgs*/
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

uint8_t Sim_GetStatusVal(void)
{
    return READ_BIT(SIM_STATUS_PORT->IDR, SIM_STATUS_PIN);
}

void Sim_EndOfTransaction(void)
{
    SetReadyFlag;
    cmd_led = 0;
}

void FlyMode(FunctionalState state)
{
    assert_param(IS_FUNCTIONAL_STATE(state));
    
    /* default mode */
    char cmd[11] = "AT+CFUN=1\r\n";
    if(state)
    {
        cmd[8] = '4';
    }
    ClearReadyFlag;
    USART1_Start_Transmission(cmd, 11);
}


void Sim_SendAT(void)
{
    ClearReadyFlag;
    cur_cmd = SIM_CMD_AT;
    USART1_Start_Transmission("AT\n", 3);
}


void Sim_ToggleState(void)
{
    SET_BIT(SIM_PWRKEY_PORT->BSRRL, SIM_PWRKEY_PIN);
    SysTick_WaitTill(SysTick_GetCurrentClock() + 1000);
    SET_BIT(SIM_PWRKEY_PORT->BSRRH, SIM_PWRKEY_PIN);
}


void Sim_CMD(FunctionalState state)
{
    uint8_t status_val = Sim_GetStatusVal();
    if((state && !status_val) || (!state && status_val))
    {
        Sim_ToggleState();
    }
}


void Sim_SendMsg(void)
{
    ClearReadyFlag;
    cur_cmd = SIM_CMD_SMS;
}


void Sim_ReceiveCall(void)
{
}


CMD_TYPE Sim_ClassifyResponse(char *cmd)
{
    uint8_t i;
    for (i = 0; i < 12; i++)
    {
        if (strstr(cmd, auto_responses[i]) != NULL)
        {
            return AUTO_T;
        }
    }
    for (i = 0; i < 3; i++)
    {
        if (strstr(cmd, resp_codes[i]) != NULL)
        {
            return RESP_CODE_T;
        }
    }
    return AWAITED_T;
}


/**
  * @brief - handles initialization and parses responses.
  *          Should be called after each CMD response received.
  */
void Sim_StateMachine(void)
{
    if(!READ_BIT(flags, FLAG_READY))
    {
        return;
    }
    
    switch(state)
    {
    case SIM_ST_POWER_ON:
        Sim_CMD(ENABLE);
        if(Sim_GetStatusVal())
        {
            state = SIM_ST_I_CONFIG;
        }
        else
        {
            return;
        }
        SysTick_WaitTill(SysTick_GetCurrentClock() + 1);
        if(strncmp("RDY", resp_buffer, 3) == 0)
        {/* skip state 1 - no need to config SIM baud rate if RDY received*/
            state++;
            break;
        }
    case SIM_ST_I_CONFIG:
        if(cur_cmd == SIM_CMD_AT)
        {/* process response to AT cmd; repeat if needed */
            if((strncmp("OK", resp_buffer, 2) == 0) || (strncmp("0", resp_buffer, 1) == 0))
            {
                /* interface is OK */
                state = 2;
                cur_cmd = SIM_CMD_NULL;
                last_heart_bit = SysTick_GetCurrentClock();
            }
            else
            {
                /* keep state, reset cmd */
                cur_cmd = SIM_CMD_REP;
            }
        }
        else
        {
            Sim_SendAT();
        }
        break;
    case 2:
        break;
    default:
        /* response parser */
        switch(cur_cmd)
        {
        case SIM_CMD_AT:
            if(strncmp("0", resp_buffer, 1) == 0)
            {
                last_heart_bit = SysTick_GetCurrentClock();
            }
            else
            {
            }
            break;
        case SIM_CMD_SMS:  /* check that SMS was sent */
            if(strncmp("0", resp_buffer, 1) == 0)
            {
                cur_cmd = SIM_CMD_NULL;
            }
            else
            {
            }
            break;
        case SIM_CMD_FLY:
            break;
        case SIM_CMD_NULL:
            break;
        default:
            /* parse +CLIP and send SMS */
            if(strncmp("+CLIP", resp_buffer, 5) == 0)
            {
                strncpy(number, resp_buffer+7, 15);
            }
            return;
        }
    }
}

void Sim_putc(uint8_t c)
{
    resp_buffer[pos++] = c;
    pos &= SIM_BUF_MASK;
}