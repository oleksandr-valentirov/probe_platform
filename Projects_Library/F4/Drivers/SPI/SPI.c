#include "SPI.h"


static uint8_t rd_pos = 0;
static uint8_t wr_pos = 0;
static uint8_t buffer[SPI_BUF_SIZE] = {0};

/* Flags */
static uint8_t flags = 0;

#define SetBusyFlag           SET_BIT(flags, SPI_FLAG_BUSY)
#define ClearBusyFlag         CLEAR_BIT(flags, SPI_FLAG_BUSY)
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
    
    SPI_Cmd(SPI3, ENABLE);
//    NVIC_EnableIRQ(SPI3_IRQn);
}


void SPI3_StartTransaction(uint8_t *src, uint8_t *dst)
{
}


void SPI3_ExchangeBytes(void)
{
    SPI3->DR = buffer[wr_pos];
    buffer[wr_pos++] = SPI3->DR;
    wr_pos &= SPI_BUF_MASK;
}


void SPI3_gets(uint8_t *dst, uint8_t max_size)
{
    while(rd_pos != wr_pos && max_size--)
    {
        *(dst++) = buffer[rd_pos];
        buffer[rd_pos++] = 0;
        rd_pos &= SPI_BUF_MASK;
    }
}


/* Mutex -------------------------------------------------------------------- */
void SPI3_SetMutex(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    SET_BIT(GPIOx->ODR, GPIO_Pin);
    SetBusyFlag;
}

void SPI3_FreeMutex(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    CLEAR_BIT(GPIOx->ODR, GPIO_Pin);
    ClearBusyFlag;
}
/* -------------------------------------------------------------------------- */