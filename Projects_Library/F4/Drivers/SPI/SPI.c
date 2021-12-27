#include "SPI.h"


static uint8_t rd_pos = 0;
static uint8_t wr_pos = 0;
static uint8_t resp_buffer[SPI_BUF_SIZE] = {0};

static struct
{
    void(*end_of_trancsaction_callback)(void);
    uint8_t *src;
    size_t size;
    SPIDev_t current_device;
    uint8_t flags;
    uint8_t dummy_byte;
} state;

#define IsBusy          state.current_device
#define SetRO           SET_BIT(state.flags, SPI_FLAG_RO)
#define ClearRO         CLEAR_BIT(state.flags, SPI_FLAG_RO)
#define GetRO           READ_BIT(state.flags, SPI_FLAG_RO)
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/* ------------------------------- SPI3 ------------------------------------- */
/* -------------------------------------------------------------------------- */
/**
 * CPOL = 0; CPHA = 0; Max speed 16 MHz. 
 * По умолчанию размер буфера 8 бит.
 */
void SPI3_Init(void)
{
    SPI_I2S_DeInit(SPI3);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    
    SPI_InitTypeDef init_struct;
    SPI_StructInit(&init_struct);
    
    /* initialize the SPI_Mode member */
    init_struct.SPI_Mode = SPI_Mode_Master;
    
    /* Initialize the SPI_NSS member */
    init_struct.SPI_NSS = SPI_NSS_Soft;
    
    /* Initialize the SPI_BaudRatePrescaler member */
    init_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;  /* 48 Mhz APB1 -> 3 MHz GPS */
    
    SPI_Init(SPI3, &init_struct);
    SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);
    
    state.flags = 0;
    state.current_device = SPI_None;
    
    SPI_Cmd(SPI3, ENABLE);
//    NVIC_EnableIRQ(SPI3_IRQn);
}


uint8_t SPI3_StartReading(size_t size, SPIDev_t device, uint8_t dummy_byte)
{
    if (device != state.current_device) {return 1;}
    state.dummy_byte = dummy_byte;
    state.size = size;
    SPI3_ExchangeBytes();
    return 0;
}

uint8_t SPI3_StartWriting(uint8_t *src, size_t size, SPIDev_t device)
{
    if (device != state.current_device || src == NULL) {return 1;}
    state.src = src;
    state.size = size;
    SPI3_ExchangeBytes();
    return 0;
}


void SPI3_ExchangeBytes(void)
{
    if (state.src == NULL)
    {
        return;
    }
    
    if (state.size > 0)
    {
        state.size--;
        if (GetRO)
        {
            SPI3->DR = state.dummy_byte;
        }
        else
        {
            SPI3->DR = *(state.src++);
        }
        resp_buffer[wr_pos++] = SPI3->DR;
        wr_pos &= SPI_BUF_MASK;
    }
    else
    {
        state.src = NULL;
        if (state.end_of_trancsaction_callback != NULL)
        {
            state.end_of_trancsaction_callback();
        }
    }
}


uint8_t SPI3_getc(uint8_t *dst)
{
    if (wr_pos == rd_pos) {return 1;}

    *dst = resp_buffer[rd_pos];
    resp_buffer[rd_pos++] = 0;
    rd_pos &= SPI_BUF_MASK;
    return 0;
}


/* Mutex -------------------------------------------------------------------- */
uint8_t SPI3_SetMutex(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, SPIDev_t device)
{
    if (IsBusy) {return 1;}

    SET_BIT(GPIOx->ODR, GPIO_Pin);
    state.current_device = device;
    return 0;
}

void SPI3_FreeMutex(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    CLEAR_BIT(GPIOx->ODR, GPIO_Pin);
    state.current_device = SPI_None;
}

uint8_t SPI3_IsMine(SPIDev_t device)
{
    return state.current_device == device ? 1 : 0;
}

uint8_t SPI3_IsFree(void)
{
    return state.current_device == SPI_None ? 1 : 0;
}
/* -------------------------------------------------------------------------- */