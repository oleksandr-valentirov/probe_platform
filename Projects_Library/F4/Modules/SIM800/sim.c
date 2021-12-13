#include  "sim.h"


char CMD_BASIC_CONF[] = "ATE0V0+CMEE=1;&W\n";
const char *CMD_TXT_MSG_FMT = "AT+CMGF=1\n";
const char *CMD_DST_NUM = "AT+CMGS=\"+380981153182\"\n";
const char *CMD_SEND_MSG = "+CMGS: 37";


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
static char resp_buffer[SIM_RESP_BUF_SIZE];
static char rd_buffer[SIM_RD_BUF_SIZE];
static char number[15];
static uint8_t wr_pos = 0;
static uint8_t rd_pos = 0;

static void Sim_gets(void);

/* not needed when parsing 'shutdown' msgs*/
static unsigned int last_heart_bit = 0;
static unsigned int ri_low_start = 0;


/* Flags -------------------------------------------------------------------- */
static uint8_t flags;

#define SetReadyFlag            SET_BIT(flags, SIM_FLAG_READY)
#define GetReadyFlag            READ_BIT(flags, SIM_FLAG_READY)
#define ClearReadyFlag          CLEAR_BIT(flags, SIM_FLAG_READY)
#define GetNLFlag               READ_BIT(flags, SIM_FLAG_NL)

uint8_t Sim_GetReadyFlag(void)
{
    return GetReadyFlag;
}

uint8_t Sim_GetNLFlag(void)
{
    return GetNLFlag;
}
/* -------------------------------------------------------------------------- */


void Sim_StatusEXTI_Enable(void)
{
    EXTI_InitTypeDef exti;
    exti.EXTI_Line = SIM_STATUS_EXTI;
    exti.EXTI_Mode = EXTI_Mode_Event;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
}

void Sim_RI_EXTICmd(FunctionalState state)
{
    EXTI_InitTypeDef exti;
    exti.EXTI_Line = SIM_RI_EXTI;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = state;
    EXTI_Init(&exti);
}

void Sim_RIEventStart(void)
{
    SysTick_SetSimTimeMs(120);
}

uint8_t Sim_GetRIFlag(void)
{
    return READ_BIT(flags, SIM_FLAG_RI);
}

uint8_t Sim_GetStatusVal(void)
{
    return READ_BIT(SIM_STATUS_PORT->IDR, SIM_STATUS_PIN);
}

void Sim_EndOfTransaction(void)
{
    SetReadyFlag;
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
    USART1_Start_Transmission("AT\r\n", 4);
}


void Sim_ToggleState(void)
{
    SET_BIT(SIM_PWRKEY_PORT->BSRRL, SIM_PWRKEY_PIN);
    SysTick_WaitTill(SysTick_GetCurrentClock() + 1000);
    SET_BIT(SIM_PWRKEY_PORT->BSRRH, SIM_PWRKEY_PIN);
}


void Sim_CMD(FunctionalState state)
{
    /* should be called before module shut-down
       because line fall will trigger an interrupt*/
    Sim_RI_EXTICmd(state);
    
    uint8_t status_val = Sim_GetStatusVal();
    if((state && !status_val) || (!state && status_val))
    {
        Sim_ToggleState();
    }
}


void Sim_SendMsg(void)
{
    ClearReadyFlag;
}


void Sim_ReceiveCall(void)
{
    Sim_gets();
    if (rd_buffer[0] == '2') {Sim_gets();} /* skip RING msg */

    char *clip_ptr = strstr(rd_buffer, auto_responses[5]);
    for(uint8_t i = 0; i < 15; i++)
    {
        number[i] = clip_ptr[i + 7];
    }
    USART1_Start_Transmission("ATH\r\n", 5);
//    SET_BIT(flags, SIM_FLAG_CALL);
}


void Sim_ProcessLine(void)
{
    Sim_gets();
    
    /* process response codes */
    if (rd_buffer[0] == '0')
    {/* OK */
    }
    else if(rd_buffer[0] == '2')
    {/* RING */
    }
    else if (strcmp("RDY", rd_buffer) == 0)
    {
    }
    else if (strcmp("Call Ready", rd_buffer) == 0)
    {
        SET_BIT(flags, SIM_FLAG_CALL_READY);
    }
    else if (strcmp("SMS Ready", rd_buffer) == 0)
    {
        SET_BIT(flags, SIM_FLAG_SMS_READY);
    }
}


/* Circular buffer simple IO functions */
void Sim_putc(uint8_t c)
{
    if (c == '\n')
    {
        SET_BIT(flags, SIM_FLAG_NL);
    }
    resp_buffer[wr_pos++] = c;
    wr_pos &= SIM_RESP_BUF_MASK;
}

uint8_t Sim_getc(void)
{
    uint8_t c = resp_buffer[rd_pos++];
    rd_pos &= SIM_RESP_BUF_MASK;
    return c;
}

static void Sim_gets(void)
{
    uint8_t c = 0, i = 0;
    for(i = 0; i < SIM_RD_BUF_SIZE; i++)
    {
        rd_buffer[i] = 0;
    }
    i = 0;
    while ((c = Sim_getc() != '\n') || i < SIM_RD_BUF_SIZE)
    {
        rd_buffer[i++] = c;
    }
}
/* -------------------------------------- */
