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
    UBX_PACK pack;
    UBX_SetHeader(&pack);
    
    UBX_CFG_PRT port_cfg;
    memset(&port_cfg.array, 0, sizeof(port_cfg));
    
    /* port configuration */
    pack.class = 0x06;
    pack.id = 0x00;
    pack.length = 20;
    pack.payload = port_cfg.array;
    
    port_cfg.fields.port_num = 1;      // UART 1
    port_cfg.fields.txReady = 0;       // txReady interrupt turned off
    port_cfg.fields.mode = 0x8C0;      // 1 stop bit, no parity, 8 bit data length
    port_cfg.fields.baudRate = 9600;
    port_cfg.fields.inProtoMask = 3;   // NMEA and UBX
    port_cfg.fields.outProtoMask = 1;  // UBX
    port_cfg.fields.flags = 0;

//    UBX_CalcChecksum((uint8_t*)(&port_cfg.checksum), &port_cfg, sizeof(UBX_CFG_PRT));
//    USART1_Start_Transmission(&pack, sizeof(UBX_CFG_PRT));
    
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