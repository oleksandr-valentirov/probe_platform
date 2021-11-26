#include "onewire.h"


#define SENSORS_CNT     1


typedef struct {
    uint8_t tx_buf[10];
    uint8_t bits_counter;
} TxStruct;


typedef struct {
    uint8_t data[9];
    uint8_t bits_counter;
} RxStruct;


typedef struct {
    uint8_t address[8];
    uint16_t last_temp;
    bool is_available;
    uint8_t scratchpad[9];
} DS18x20;


/* private variables ---------------------------------------------------------*/
static TxStruct tx_struct;
static RxStruct rx_struct;
static StateType state = INIT;
static uint8_t status_flags = 0;

static uint8_t cur_sensor = 0;
static uint8_t *cur_dest = NULL;   // временный поинтер для упрощения разработки
static DS18x20 sensors[SENSORS_CNT];

static uint16_t values[16] = {0};
static uint8_t counter = 0;

/* private functions ---------------------------------------------------------*/
static bool OneWire_Is_Busy(void);


/* Read functions ------------------------------------------------------------*/
/**
  * @brief читаем все 9 байт
  */
void Read_ScratchPad(void)
{
    cur_dest = sensors[cur_sensor].scratchpad;
    rx_struct.bits_counter = 72;
}

/**
  * @brief читаем только 2 первых байта
  */
void Read_Temperature(void)
{
    cur_dest = sensors[cur_sensor].scratchpad;
    rx_struct.bits_counter = 16;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


/**
  * @brief - func provides current state of state machine to external modules.
  */
StateType Get_Current_State(void)
{
    return state;
}

void Set_OneWire_Init_State(void)
{
    state = INIT;
}

void Set_OneWire_Status(uint8_t mask)
{
    status_flags |= mask;
}

void Reset_OneWire_Status(uint8_t mask)
{
    CLEAR_BIT(status_flags, mask);
}

uint8_t Get_OneWire_Status(uint8_t mask)
{
    return status_flags & mask;
}

void Reset(void)
{
    TIM9_CH_1_Set_Mode(1);      // output-compare
    Data_Line_Down();
    TIM9_Start(920);            // 920 us 
    TIM9_CH_1_Start(480);       // 480 us
    Set_OneWire_Status(ONEWIRE_RESET_STATUS);
}

/**
  * @brief - transmits next bit of Tx buffer (current LSB) and shifts buffer
  */
void Transmit_Bit(void)
{
    if(tx_struct.bits_counter == 0)
    {
        Reset_OneWire_Status(ONEWIRE_BUSY_STATUS);
        switch (state)
        {
        case ROMCOM:
            state = FUNCCOM;
            break;
        case FUNCCOM:
            if (status_flags & ONEWIRE_READ_SCR_STATUS)
                state = READ;
            break;
        default:
            state = INIT;
        }
        return;
    }
    
    TIM9_CH_1_Set_Mode(1);
    Data_Line_Down();

    // starting Tx CH to send current 1st bit;
    uint8_t curr_buf_index = (--tx_struct.bits_counter) / 8;
    (tx_struct.tx_buf[curr_buf_index] & 1) ? TIM9_CH_1_Start(7) : TIM9_CH_1_Start(30);  // 10 us to transmit 1 and 30 us to transmit 0
    // shifting buffer
    tx_struct.tx_buf[curr_buf_index] = tx_struct.tx_buf[curr_buf_index] >> 1;
    TIM9_Start(60);
}


/**
  * @brief - receives next bit to Rx buffer, LSB first and shifts it
  */
void Receive_Bit(void)
{
    if(READ_BIT(TIM9->SR, TIM_SR_CC1IF))  // сохранить текущий бит
    {
        values[counter++] = TIM9->CNT;
        
        uint8_t curr_buf_index = (rx_struct.bits_counter) / 8;
        uint8_t temp = (TIM9->CCR1 <= 15) ? 128 : 0;
        rx_struct.data[curr_buf_index] = (rx_struct.data[curr_buf_index] >> 1) | temp;
    }
    else if (rx_struct.bits_counter == 0)
    {
        counter = 0;
        memset(values, 0, 32);
        
        Reset_OneWire_Status(ONEWIRE_BUSY_STATUS | ONEWIRE_RESET_STATUS | ONEWIRE_BUS_STATUS | ONEWIRE_READ_SCR_STATUS);
        state = INIT;
        cur_dest = rx_struct.data;
    }
    else                // начать приём следующего бита
    {
        rx_struct.bits_counter--;

        TIM9_CH_1_Set_Mode(1);
        Data_Line_Down();

        TIM9_CH_1_Start(1);
        TIM9_Start(60);
    }
}


void OneWire_Init(void)
{
    // Tx struct init
    tx_struct.bits_counter = 0;
    memset(tx_struct.tx_buf, 0, 10);
    // Rx struct init
    rx_struct.bits_counter = 0;
    memset(rx_struct.data, 0, 10);
    
    state = INIT;
}

void OneWire_Main(void)
{
    if (OneWire_Is_Busy())
        return;

    if (state == INIT)
    {
        if(!Get_OneWire_Status(ONEWIRE_RESET_STATUS))
        {     // запускаем Reset
            Reset();
            return;
        }
        else if (Get_OneWire_Status(ONEWIRE_BUS_STATUS))  
        {     // если есть Presence Pulse
            state = ROMCOM;
        }
        Reset_OneWire_Status(ONEWIRE_RESET_STATUS);
    }
    else if (state == READ)
    {
        Receive_Bit();
    }
    else if (state == ROMCOM)
    {
#if SENSORS_CNT > 1
        if (Get_OneWire_Status(ONEWIRE_SEARCH_ROM_STATUS))
        {
            tx_struct.tx_buf[8] = Match_ROM;
            for(uint8_t i = 0; i < 8; i++)
            {
                tx_struct.tx_buf[7-i] = sensors[cur_sensor].address[i];
            }
            tx_struct.bits_counter = 72;
            state = FUNCCOM;
        }
        else
        {   // читаем адреса на старте девайса
            tx_struct.tx_buf[0] = Search_ROM;
            tx_struct.bits_counter = 8;
            rx_struct.bits_counter = 8;
            state = READ;
        }
#else
        tx_struct.tx_buf[0] = Skip_ROM;
        tx_struct.bits_counter = 8;
#endif
        Set_OneWire_Status(ONEWIRE_BUSY_STATUS);
        Transmit_Bit();
    }
    else if (state == FUNCCOM)
    {
        if (Get_OneWire_Status(ONEWIRE_CONVERT_T_STATUS))
        {
            tx_struct.tx_buf[0] = Read_SCR;
            tx_struct.bits_counter = 8;
            Read_Temperature();
            Set_OneWire_Status(ONEWIRE_READ_SCR_STATUS);
        }
        else
        {
            tx_struct.tx_buf[0] = Conv_T;
            tx_struct.bits_counter = 8;
            Set_OneWire_Status(ONEWIRE_CONVERT_T_STATUS);
        }
        Set_OneWire_Status(ONEWIRE_BUSY_STATUS);
        Transmit_Bit();
    }
}


void Data_Line_Up(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

void Data_Line_Down(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void Data_Line_Float(void)
{
    CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER2);
}

void Data_Line_Set_Out(void)
{
    CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER2);
    SET_BIT(GPIOA->MODER, GPIO_MODER_MODER2_0);
}

void Data_Line_Set_AF(void)
{
    CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER2);
    SET_BIT(GPIOA->MODER, GPIO_MODER_MODER2_1);
}


static bool OneWire_Is_Busy(void)
{
    return Get_OneWire_Status(ONEWIRE_BUSY_STATUS);
}