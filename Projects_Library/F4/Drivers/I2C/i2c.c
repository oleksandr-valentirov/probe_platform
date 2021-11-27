#include "i2c.h"

#define FLAG_BUSY               1


static struct i2c_state {
    void(*end_of_trancsaction_callback)(void);
    uint8_t* ptr;
    size_t counter;
} state;

/* Flags -------------------------------------------------------------------- */
static uint8_t flags;

#define Set_Busy_Flag()         SET_BIT(flags, FLAG_BUSY)
#define Reset_Busy_Flag()       CLEAR_BIT(flags, FLAG_BUSY)
#define Get_Busy_Flag()         READ_BIT(flags, FLAG_BUSY)

uint8_t I2C_Get_Busy_Flag(void)
{
    return Get_Busy_Flag();
}
/* -------------------------------------------------------------------------- */


void I2C_Reset(void)
{
    state.ptr = NULL;
    state.counter = 0;
    state.end_of_trancsaction_callback = NULL;
    Reset_Busy_Flag();
}


void I2C_Driver_Init(void)
{
    I2C_DeInit(I2C1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
}


void I2C_Set_EndOfTransactionCallback(void(*f)(void))
{
    state.end_of_trancsaction_callback = f;
}


void I2C_Call_EnfOfTransactionCallback(void)
{
    (*state.end_of_trancsaction_callback)();
}


void I2C_Start_Transmission(void* source, size_t counter)
{
    if (Get_Busy_Flag() || source == NULL || counter == 0)
    {
        return;
    }
    state.ptr = (uint8_t*) source;
    state.counter = counter;
    
    Set_Busy_Flag();
    
    /* platform logic */
}

void I2C_Start_Reception(void* destination, size_t counter)
{
    if (Get_Busy_Flag() || destination == NULL || counter == 0)
    {
        return;
    }
    state.ptr = (uint8_t*) destination;
    state.counter = counter;
}


/**
 * @brief       Returns next byte to interrupt routine
 */
void I2C_Transmit_Next_Byte(void)
{
    if (state.ptr == NULL || state.counter == 0)
    {
        return;
    }
    if (--state.counter == 0)
    {
        Reset_Busy_Flag();
    }

    I2C1->DR = *(++state.ptr);
}


/**
 * @brief       Gets next byte from interrupt routine
 */
void I2C_Receive_Next_Byte(void)
{
    if (state.ptr == NULL || state.counter == 0)
    {
        return;
    }
    if (--state.counter == 0)
    {
        Reset_Busy_Flag();
    }
    *(++state.ptr) = I2C1->DR;
}