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
static char buffer[SIM_BUF_SIZE] = {0};
static uint8_t buf_pos = 0;
static char number[15];

/* static functions */
static void Sim_gets(void);
static void Sim_ATHEventStart(void);
static void Sim_ProcessLine(void);
static void Sim_SendSMSCmd(void);
static void Sim_ReceiveCall(void);
static void Sim_SendMsg(void);
static void Sim_StatusEXTI_Enable(void);

static Sim_state_t state;


/* Flags -------------------------------------------------------------------- */
static uint8_t flags = 0;

#define SetReadyFlag            SET_BIT(flags, SIM_FLAG_READY)
#define GetReadyFlag            READ_BIT(flags, SIM_FLAG_READY)
#define ClearReadyFlag          CLEAR_BIT(flags, SIM_FLAG_READY)
#define GetNLFlag               READ_BIT(flags, SIM_FLAG_NL)
#define SetNLFlag               SET_BIT(flags, SIM_FLAG_NL)
#define ClearNLFlag             CLEAR_BIT(flags, SIM_FLAG_NL)
#define GetCallFlag             READ_BIT(flags, SIM_FLAG_CALL)
#define ClearCallFlag           CLEAR_BIT(flags, SIM_FLAG_CALL)
#define GetMsgTxtInFlag         READ_BIT(flags, SIM_FLAG_TXT_IN)
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
    return (READ_BIT(flags, SIM_FLAG_CALL_READY) & READ_BIT(flags, SIM_FLAG_SMS_READY));
}

void Sim_ClearRIFlag(void)
{
    CLEAR_BIT(flags, SIM_FLAG_RI);
}

uint8_t Sim_GetCallFlag(void)
{
    return GetCallFlag;
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
void Sim_SetStatus(uint8_t status)
{
    state.is_enabled = status;
}

void Sim_init(void)
{
//    Sim_StatusEXTI_Enable();
    if (!READ_BIT(SIM_STATUS_PORT->IDR, SIM_STATUS_PIN))
    {
        Sim_CMD(ENABLE);
    }
    
    /* SIM state init */
    for (uint8_t i = 0; i < 15; i++)
    {
        state.my_num[i] = 0;
    }
    state.rssi = 0;
    state.aw_cmd = 0;
}


void Sim_StateUpdateRSSI(void)
{
    ClearReadyFlag;
    USART6_Start_Transmission("AT+CSQ\r\n", 8);
}

/**/


static void Sim_StatusEXTI_Enable(void)
{
//    SYSCFG_EXTILineConfig(SIM_STATUS_EXTI_PORT, SIM_STATUS_EXTI_SRC);
    
    EXTI_InitTypeDef exti;
    exti.EXTI_Line = SIM_STATUS_EXTI_LINE;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
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
    USART6_Start_Transmission(cmd, 11);
}


void Sim_SendAT(void)
{
    ClearReadyFlag;
    USART6_Start_Transmission("AT\r\n", 4);
}


void Sim_ToggleState(void)
{
    SET_BIT(SIM_PWRKEY_PORT->BSRRL, SIM_PWRKEY_PIN);
    SysTick_WaitTill(SysTick_GetCurrentClock() + 1500);
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


static void Sim_SendSMSCmd(void)
{
    ClearReadyFlag;
    
    char cmd[24] = {0};
    strncpy(cmd, "AT+CMGS=", 8);
    strncat(cmd, number, 15);
    strncat(cmd, "\r", 1);

    if(USART6_Start_Transmission(cmd, 24) == 0)
    {
        ClearCallFlag;
    }
}

static void Sim_SendMsg(void)
{
    ClearReadyFlag;

    char msg[46] = {0};
    strncpy(msg, "Programming, motherfucker! Do you speak it ?!", 45);
    msg[45] = 26;
    
    if(USART6_Start_Transmission(msg, 46) == 0)
    {
        ClearMsgTxtInFlag;
    }
}


static void Sim_ReceiveCall(void)
{
    if (buffer[0] == '2') 
    { /* skip RING msg and wait for +CLIP msg */
        return;  /** @todo - need to test this */
    }

    char *clip_ptr = strstr(buffer, "+CLIP");
    if (clip_ptr == NULL) return;

    for(uint8_t i = 0; i < 15; i++)
    {
        number[i] = clip_ptr[i + 7];
    }
    if(USART6_Start_Transmission("ATH\r\n", 5) == 0)
    {
        Sim_ATHEventStart();
        SET_BIT(flags, SIM_FLAG_CALL);
    }
}


static void Sim_ProcessLine(void)
{
    char *temp_str = NULL;
    
    
    /* process response codes */
    if (buffer[0] == '0')
    {/* OK */
        state.aw_cmd = 0;
        SysTick_SetSimTimeMs(0);
    }
    else if(buffer[0] == '2')
    {/* RING */
    }
    else if (strncmp("RDY", buffer, 3) == 0)
    {
    }
    else if (strncmp("Call Ready", buffer, 10) == 0)
    {
        SET_BIT(flags, SIM_FLAG_CALL_READY);
        if (Sim_OperationReady())
            SetReadyFlag;
    }
    else if (strncmp("SMS Ready", buffer, 9) == 0)
    {
        SET_BIT(flags, SIM_FLAG_SMS_READY);
        if (Sim_OperationReady())
            SetReadyFlag;
    }
    else if ((temp_str = strstr(buffer, "+CSQ")) != NULL)
    {
        state.rssi = (temp_str[6] - 48) * 10;
        state.rssi = state.rssi + temp_str[7] - 48;
    }
}


static void Sim_gets(void)
{
    uint8_t c = 0;
    
    if (buf_pos == 0)
    {   /* new line was before this call, so we need to clean buffer */
        while(buf_pos < SIM_BUF_SIZE)
        {
            buffer[buf_pos++] = 0;
        }
        buf_pos = 0;
    }
    
    while(buf_pos < SIM_BUF_SIZE && USART6_getc(&c))
    {   /* read to buffer from UART buffer before NL symbol */
        buffer[buf_pos++] = c;
        if (c == '\n')
        {
            SetNLFlag;
            buf_pos = 0;
            break;
        }
        else if (c == '>')
        {
            SetMsgTxtInFlag;
        }
    }
}
/* -------------------------------------- */

static void Sim_ATHEventStart(void)
{
    state.aw_cmd = ATH;
    SysTick_SetSimTimeMs(10000);
}


void Sim_main(void)
{
    uint8_t status_pin = READ_BIT(SIM_STATUS_PORT->IDR, SIM_STATUS_PIN);
    if (state.is_enabled != status_pin)
    {
        state.is_enabled = status_pin;
        /** @todo - report status change */
    }
//    if (!status_pin) {return;}
    
    Sim_gets();
    
    if (GetNLFlag)
    {
        if (Sim_GetRIFlag() && !SysTick_GetSimTime() && !READ_BIT(SIM_RI_PORT->IDR, SIM_RI_PIN))
        {/* process call */
            Sim_ReceiveCall();
            Sim_ClearRIFlag();
        }
        else if (Sim_GetRIFlag() && !SysTick_GetSimTime() && READ_BIT(SIM_RI_PORT->IDR, SIM_RI_PIN))
        {/* process URS/SMS */
            Sim_ProcessLine();
            Sim_ClearRIFlag();
        }
        else
        {/* process all other messages */
            Sim_ProcessLine();
        }
        ClearNLFlag;
    }

    if (GetReadyFlag && !SysTick_GetSimTime())
    {
        if (!SysTick_GetSimStateClock() && !GetMsgTxtInFlag)
        {
            Sim_StateUpdateRSSI();
            SysTick_SimStateClockUpdate();
        }
        else if (GetCallFlag)
        {
            Sim_SendSMSCmd();
        }
        else if (GetMsgTxtInFlag)
        {
            Sim_SendMsg();
        }
    }
}