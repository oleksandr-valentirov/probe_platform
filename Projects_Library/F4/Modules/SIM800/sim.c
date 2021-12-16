#include  "sim.h"



//const char *auto_responses[7] = {
//    "+CFUN",                    /* functionality indication */    
//
//    "+CMTE",                    /* incorrect temperature */
//    
//    "UNDER-VOLTAGE POWER DOWN", /* Power-down messages */
//    "NORMAL POWER DOWN",        /*                     */
//    "OVER-VOLTAGE POWER DOWN",  /* ------------------- */
//    
//    "UNDER-VOLTAGE WARNNING",   /* Power warnings */
//    "OVER-VOLTAGE WARNNING"
//};


/* response buffer */
static char resp_buffer[SIM_RESP_BUF_SIZE] = {0};
static char rd_buffer[SIM_RD_BUF_SIZE] = {0};
static char number[15];
static uint8_t wr_pos = 0;
static uint8_t rd_pos = 0;

static void Sim_gets(void);

static uint8_t cur_cmd_num = 0;
static Sim_state_t state;

/* not needed when parsing 'shutdown' msgs*/
static unsigned int last_heart_bit = 0;


/* Flags -------------------------------------------------------------------- */
static uint8_t flags = 0;

#define SetReadyFlag            SET_BIT(flags, SIM_FLAG_READY)
#define GetReadyFlag            READ_BIT(flags, SIM_FLAG_READY)
#define ClearReadyFlag          CLEAR_BIT(flags, SIM_FLAG_READY)
#define GetNLFlag               READ_BIT(flags, SIM_FLAG_NL)
#define GetCallFlag             READ_BIT(flags, SIM_FLAG_CALL)
#define SetMsgTxtInFlag         SET_BIT(flags, SIM_FLAG_TXT_IN)
#define ClearMsgTxtInFlag       CLEAR_BIT(flags, SIM_FLAG_TXT_IN)

uint8_t Sim_GetReadyFlag(void)
{
    return GetReadyFlag;
}

uint8_t Sim_GetNLFlag(void)
{
    return GetNLFlag;
}

uint8_t Sim_OperationReady(void)
{
    return (READ_BIT(flags, SIM_FLAG_CALL_READY) && READ_BIT(flags, SIM_FLAG_SMS_READY));
}

void Sim_ClearRIFlag(void)
{
    CLEAR_BIT(flags, SIM_FLAG_RI);
}

uint8_t Sim_GetCallFlag(void)
{
    return GetCallFlag;
}

void Sim_ClearCallFlag(void)
{
    CLEAR_BIT(flags, SIM_FLAG_CALL);
}

uint8_t Sim_GetTxtInFlag(void)
{
    return READ_BIT(flags, SIM_FLAG_TXT_IN);
}

void Sim_ClearTxtInFlag(void)
{
    CLEAR_BIT(flags, SIM_FLAG_TXT_IN);
}
/* -------------------------------------------------------------------------- */

/**/
void Sim_StateInit(void)
{
    for (uint8_t i = 0; i < 15; i++)
    {
        state.my_num[i] = 0;
    }
    state.rssi = 0;
}

void Sim_StateUpdateRSSI(void)
{
    ClearReadyFlag;
    USART1_Start_Transmission("AT+CSQ\r\n", 8);
}

/**/


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
    SET_BIT(flags, SIM_FLAG_RI);
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

void Sim_ResetCurCmd(void)
{
    cur_cmd_num = 0;
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
    SysTick_WaitTill(SysTick_GetCurrentClock() + 3000);
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


void Sim_SendSMSCmd(void)
{
    ClearReadyFlag;
    
    char cmd[24] = {0};
    strncpy(cmd, "AT+CMGS=", 8);
    strncat(cmd, number, 15);
    strncat(cmd, "\r", 1);

    USART1_Start_Transmission(cmd, 24);
}

void Sim_SendMsg(void)
{
    ClearReadyFlag;

    char msg[46] = {0};
    strncpy(msg, "Programming, motherfucker! Do you speak it ?!", 45);
    msg[45] = 26;
    
    USART1_Start_Transmission(msg, 46);
}


void Sim_ReceiveCall(void)
{
    Sim_gets();
    if (rd_buffer[0] == '2') 
    { /* skip RING msg and wait for +CLIP msg */
        while(Sim_GetNLFlag())
        Sim_gets();
    }

    char *clip_ptr = strstr(rd_buffer, "+CLIP");
    for(uint8_t i = 0; i < 15; i++)
    {
        number[i] = clip_ptr[i + 7];
    }
    USART1_Start_Transmission("ATH\r\n", 5);
    SET_BIT(flags, SIM_FLAG_CALL);
}


void Sim_ProcessLine(void)
{
    Sim_gets();
    char *temp_str = NULL;
    
    
    /* process response codes */
    if (rd_buffer[0] == '0')
    {/* OK */
    }
    else if(rd_buffer[0] == '2')
    {/* RING */
    }
    else if (strncmp("RDY", rd_buffer, 3) == 0)
    {
    }
    else if (strncmp("Call Ready", rd_buffer, 10) == 0)
    {
        SET_BIT(flags, SIM_FLAG_CALL_READY);
        if (Sim_OperationReady())
            SetReadyFlag;
    }
    else if (strncmp("SMS Ready", rd_buffer, 9) == 0)
    {
        SET_BIT(flags, SIM_FLAG_SMS_READY);
        if (Sim_OperationReady())
            SetReadyFlag;
    }
    else if ((temp_str = strstr(rd_buffer, "+CSQ")) != NULL)
    {
        state.rssi = (temp_str[6] - 48) * 10;
        state.rssi = state.rssi + temp_str[7] - 48;
    }
}


/* Circular buffer simple IO functions */
void Sim_putc(uint8_t c)
{
    if (c == '\n')
    {
        SET_BIT(flags, SIM_FLAG_NL);
    }
    else if (c == '>')
    {
        SetMsgTxtInFlag;
    }
    resp_buffer[wr_pos++] = c;
    wr_pos &= SIM_RESP_BUF_MASK;
}

uint8_t Sim_getc(void)
{
    uint8_t c = resp_buffer[rd_pos];
    resp_buffer[rd_pos++] = 0;
    rd_pos &= SIM_RESP_BUF_MASK;
    return c;
}

static void Sim_gets(void)
{
    uint8_t c = 0, i = 0;
    for(i = 0; i < SIM_RD_BUF_SIZE; i++)
    {
        c = Sim_getc();
        rd_buffer[i] = c;
        if (c == '\n') {break;}
    }
    for(; i < SIM_RD_BUF_SIZE; i++)
    {
        rd_buffer[i] = 0;
    }
    CLEAR_BIT(flags, SIM_FLAG_NL);
}
/* -------------------------------------- */
