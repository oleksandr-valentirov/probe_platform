#include "onewire.h"


typedef struct {
    uint8_t tx_buf[10];
    uint8_t bits_coutner;
} TxStruct;
    
typedef enum {
    INIT = 0,
}StateType;


/* private variables ---------------------------------------------------------*/
static TxStruct tx_struct;

/* private functions ---------------------------------------------------------*/



/* public functions ----------------------------------------------------------*/

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
}

/**
  * @brief - transmits next bit of Tx buffer (current LSB) and shifts buffer
  */
void Transmit_Bit(void)
{
    if(tx_struct.bits_coutner == 0)
        return;
    
    // starting Tx CH to send current 1st bit;
    uint8_t curr_buf_index = tx_struct.bits_coutner / 8;
    (tx_struct.tx_buf[curr_buf_index] & 1) ? TIM9_CH_1_Start(10) : TIM9_CH_1_Start(100);  /** @todo - заменить цифры */
    
    // shifting buffer
    tx_struct.tx_buf[curr_buf_index] = tx_struct.tx_buf[curr_buf_index] >> 1;
    tx_struct.bits_coutner--;
}
    
/**
  * @brief - receives next bit to Rx buffer, LSB first and shifts it
  */
void Receive_Bit(void)
{
}


void Start_Transaction(uint8_t *data_ptr, uint8_t size)
{
    memcpy(tx_struct.tx_buf, data_ptr, size);
    tx_struct.bits_coutner = size * 8;
    
    // timer setup
    TIM9_CH_1_Set_Mode(1);
}

void OneWire_Main(void)
{
}
