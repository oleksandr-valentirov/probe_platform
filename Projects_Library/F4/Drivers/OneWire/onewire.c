#include "onewire.h"


#define SENSORS_CNT     2


typedef struct {
    uint8_t tx_buf[10];
    uint8_t bits_counter;
} TxStruct;


typedef struct {
    uint16_t data;
    uint8_t bits_counter;
} RxStruct;


/* private variables ---------------------------------------------------------*/
static TxStruct tx_struct;
static RxStruct rx_struct;
static StateType state = INIT;
static uint8_t status_flags = 0;

static uint8_t cur_sensor = 0;
static uint8_t sensors_addresses[8][SENSORS_CNT];
static uint16_t sensors_data[SENSORS_CNT];

/* private functions ---------------------------------------------------------*/
static uint8_t Invert(uint8_t x);




// ---------------------------------- SysTick timer

static uint32_t ticks_delay = 0;
void turn_off_systick(void);

void delay_ms(uint32_t milliseconds) {
	SysTick_Config(96000);

	uint32_t start = ticks_delay;
	while((ticks_delay - start) < milliseconds);
	ticks_delay = 0;
	turn_off_systick();
}

void delay_us(uint32_t microseconds) {
	SysTick_Config(96);

	uint32_t start = ticks_delay;
	while((ticks_delay - start) < microseconds);
	ticks_delay = 0;
	turn_off_systick();
}

//void SysTick_Handler(void) {
//	ticks_delay++;
//}

void turn_off_systick(void) {
	CLEAR_BIT(SysTick->CTRL, (SysTick_CTRL_CLKSOURCE_Msk |
                   	   	   	  SysTick_CTRL_TICKINT_Msk   |
							  SysTick_CTRL_ENABLE_Msk)
			);
}
// -------------------------------------------------------




/**
  * @brief - func provides current state of state machine to external modules.
  */
StateType Get_Current_State(void)
{
    return state;
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

/**
  * @brief - appends CMD to Tx buffer
  */
void Send_Cmd(ROM_Cmd cmd)
{   
    switch(cmd)
    {
    case Search_ROM:
        break;
    case Read_ROM:
        break;
    case Match_ROM:
        break;
    case Skip_ROM:
        break;
    default:
        return;
    }
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
        for (uint8_t i = 0; i < 2; i++)
        {
            Data_Line_Set_Out();
            Data_Line_Down();
            for(uint8_t i = 0; i < 96; i++){}
            Data_Line_Float();
            for(uint16_t i = 0; i < (96 * 15); i++){}
        }
        return;
    }
    
    TIM9_CH_1_Set_Mode(1);
    Data_Line_Down();

    // starting Tx CH to send current 1st bit;
    uint8_t curr_buf_index = (--tx_struct.bits_counter) / 8;
    (tx_struct.tx_buf[curr_buf_index] & 1) ? TIM9_CH_1_Start(10) : TIM9_CH_1_Start(30);  // 10 us to transmit 1 and 30 us to transmit 0
    TIM9_Start(60);

    // shifting buffer
    tx_struct.tx_buf[curr_buf_index] = tx_struct.tx_buf[curr_buf_index] >> 1;
}

/**
  * @brief - receives next bit to Rx buffer, LSB first and shifts it
  */
void Receive_Bit(void)
{
    if(TIM9_Is_Busy())  // сохранить текущий бит
    {
        // сохраняем, чтобы не похерить значение таймера, пока будем сравнивать
        uint16_t temp = TIM9->CNT;
        temp = (((1 < temp < 15) ? 128 : 0) << 8) + 128;
        rx_struct.data = (rx_struct.data >> 1) | temp;
    }
    else                // начать приём следующего бита
    {
        TIM9_CH_1_Set_Mode(1);
        Data_Line_Down();
        
        TIM9_CH_1_Start(2);
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
    rx_struct.data = 0;
    
    state = INIT;
}

void OneWire_Main(void)
{
    if (TIM9_Is_Busy())
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
    }
    else if (state == ROMCOM)
    {
#if SENSORS_CNT > 1
        if (!Get_OneWire_Status(ONEWIRE_SEARCH_ROM_STATUS))
        {   // читаем адреса на старте девайса
            tx_struct.tx_buf[0] = Search_ROM;
            tx_struct.bits_counter = 8;
//            state = READ;
        }
        else
        {
            tx_struct.tx_buf[8] = Match_ROM;
            for(uint8_t i = 0; i < 8; i++)
            {
                tx_struct.tx_buf[7-i] = sensors_addresses[i][cur_sensor];
            }
            tx_struct.bits_counter = 72;
//            state = FUNCCOM;
        }
#else
        tx_struct.tx_buf[0] = Skip_ROM;
        state = FUNCCOM;
#endif
        Transmit_Bit();
    }
    else if (state == FUNCCOM)
    {
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


static uint8_t Invert(uint8_t x)
{        
    int base = 256;

    uint8_t  res = 0;
    while (x != 0) 
    {
        res += (x & 1) * (base >>= 1);
        x >>= 1;
    }

    return res;
}