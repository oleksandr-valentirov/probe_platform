#include "SPI.h"


/**
 * @brief  SPI общего назначения
 */
void SPI1_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_I2S_DeInit(SPI1);
    
    SPI_InitTypeDef init_struct;
}


/* -------------------------------------------------------------------------- */
/* ------------------------------- SPI4 ------------------------------------- */
/* -------------------------------------------------------------------------- */
/**
 * @brief  LoRa 1
 * CPOL = 0; CPHA = 0; Max speed 16 MHz. 
 * По умолчанию размер буфера 8 бит.
 */
void SPI3_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    SPI_I2S_DeInit(SPI3);
    
    SPI_InitTypeDef init_struct;
    /* Initialize the SPI_Direction member */
    init_struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    /* initialize the SPI_Mode member */
    init_struct.SPI_Mode = SPI_Mode_Master;
    /* initialize the SPI_DataSize member */
    init_struct.SPI_DataSize = SPI_DataSize_8b;
    /* Initialize the SPI_CPOL member */
    init_struct.SPI_CPOL = SPI_CPOL_Low;
    /* Initialize the SPI_CPHA member */
    init_struct.SPI_CPHA = SPI_CPHA_1Edge;
    /* Initialize the SPI_NSS member */
    init_struct.SPI_NSS = SPI_NSS_Hard;
    /* Initialize the SPI_BaudRatePrescaler member */
    init_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  // 48 Mhz APB1 -> 12 MHz LoRa
    /* Initialize the SPI_FirstBit member */
    init_struct.SPI_FirstBit = SPI_FirstBit_MSB;
    /* Initialize the SPI_CRCPolynomial member */
    init_struct.SPI_CRCPolynomial = 7;
    
    SPI_Init(SPI3, &init_struct);
}

void SPI3_send_data(uint16_t data)
{
    SPI_I2S_SendData(SPI3, data);
}

uint16_t SPI3_receive_data(void)
{
    return SPI_I2S_ReceiveData(SPI3);
}


/**
 * @brief  функция для настройки нужного размера буфера
 * @param  size: задаёт размер буфера
 *              @arg 0: размер буфера 8 бит
 *              @arg 1: размер буфера 16 бит
 */
void SPI3_data_size_config(uint8_t size)
{
    if (size == 0)
        SPI_DataSizeConfig(SPI3, SPI_DataSize_8b);
    else
        SPI_DataSizeConfig(SPI3, SPI_DataSize_16b);
}

/* -------------------------------------------------------------------------- */
/* --------------------------- SPI4 end ------------------------------------- */
/* -------------------------------------------------------------------------- */