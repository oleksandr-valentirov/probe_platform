#include "m8.h"


/* Rx buffer */
static uint8_t rx_buffer[GPS_BUF_SIZE] = {0};
static uint8_t rx_buf_pos = 0;

static uint8_t tx_buffer[GPS_BUF_SIZE] = {0};
static uint8_t tx_buf_pos = 0;

/* static functions */
static void GPS_gets(void);

static uint8_t flags = 0;


void GPS_EndOfTransaction(void)
{
}


/**
  *  @desc - used to pass Tx buffer to HW interface
  */
uint8_t GPS_getc(void)
{
    uint8_t res = tx_buffer[tx_buf_pos++];
    tx_buf_pos &= GPS_BUF_MASK;
    return res;
}

void GPS_Init(void)
{    
    for(uint8_t i = 0; i < 10; i++)
    {
        tx_buffer[i] = 'a';
    }
    DMA_GPSoutInit((uint32_t*)tx_buffer);
    DMA_GPSoutTransfer(10);
}

void GPS_main(void)
{
    GPS_gets();
}


static void GPS_gets(void)
{
    uint8_t c = 0;
    
    if (rx_buf_pos == 0)
    {   /* new line was before this call, so we need to clean buffer */
        while(rx_buf_pos < GPS_BUF_SIZE)
        {
            rx_buffer[rx_buf_pos++] = 0;
        }
        rx_buf_pos = 0;
    }

    while(rx_buf_pos < GPS_BUF_SIZE && USART1_getc(&c))
    {
        rx_buffer[rx_buf_pos++] = c;
        rx_buf_pos &= GPS_BUF_MASK;
    }
}