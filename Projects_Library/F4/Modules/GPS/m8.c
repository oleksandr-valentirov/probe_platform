#include "m8.h"


/* response buffer */
static char buffer[GPS_BUF_SIZE] = {0};
static uint8_t buf_pos = 0;

/* static functions */
static void GPS_gets(void);

static uint8_t flags = 0;


void GPS_EndOfTransaction(void)
{
}

void GPS_Init(void)
{
    UBX_CFG_PRT port_cfg;
    memset(&port_cfg, 0, sizeof(port_cfg));
    
    /* port configuration */
    port_cfg.h1 = 0xB5;
    port_cfg.h2 = 0x62;
    port_cfg.class = 0x06;
    port_cfg.id = 0x00;
    port_cfg.length = 20;
    port_cfg.port_num = 1;      // UART 1
    port_cfg.txReady = 0;       // txReady interrupt turned off
    port_cfg.mode = 0x8C0;      // 1 stop bit, no parity, 8 bit data length
    port_cfg.baudRate = 9600;
    port_cfg.inProtoMask = 3;   // NMEA and UBX
    port_cfg.outProtoMask = 1;  // UBX
    port_cfg.flags = 0;
    
    UBX_CalcChecksum((uint8_t*)(&port_cfg.checksum), &port_cfg, sizeof(UBX_CFG_PRT));
    USART1_Start_Transmission(&port_cfg, sizeof(UBX_CFG_PRT));
}

void GPS_main(void)
{
    GPS_gets();
}


static void GPS_gets(void)
{
    uint8_t c = 0;
    
    if (buf_pos == 0)
    {   /* new line was before this call, so we need to clean buffer */
        while(buf_pos < GPS_BUF_SIZE)
        {
            buffer[buf_pos++] = 0;
        }
        buf_pos = 0;
    }

    while(buf_pos < GPS_BUF_SIZE && USART1_getc(&c))
    {
        buffer[buf_pos++] = c;
        buf_pos &= GPS_BUF_MASK;
    }
}