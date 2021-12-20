#include "gps.h"

static char resp_buffer[GPS_RESP_BUF_SIZE] = {0};
static char rd_buffer[GPS_RD_BUF_SIZE] = {0};
static uint8_t wr_pos = 0;
static uint8_t rd_pos = 0;

static GPS_DataT gps_data;


static void GPS_gets(void);
static void GPS_ParseRMS(void);


/* Flags */
static uint8_t flags;


void GPS_DataParser(void)
{
    GPS_gets();
    
    if (rd_buffer[1] == 'P')
    {   /* proprietary message start */
    }
    else
    {   /* standard message start */
        switch (rd_buffer[3])
        {
        case 'G':
            break;
        case 'V':
            break;
        case 'T':
            break;
        case 'D':
            break;
        case 'R':
            GPS_ParseRMS();
            break;
        case 'Z':
            break;
        }
    }
}


void GPS_putc(uint8_t c)
{
    if (c == '\n')
    {   /* packet data end */
        SET_BIT(flags, GPS_FLAG_NL);
    }
    else if (c == '*')
    {   /* checksum */
    }
    else if (c == '$')
    {   /* packet data start */
    }
    resp_buffer[wr_pos++] = c;
    wr_pos &= SIM_RESP_BUF_MASK;
}


uint8_t GPS_getc(void)
{
    uint8_t c = resp_buffer[rd_pos];
    resp_buffer[rd_pos++] = 0;
    rd_pos &= GPS_RESP_BUF_MASK;
    return c;
}


static void GPS_gets(void)
{
    uint8_t i = 0;
    for(i = 0; i < GPS_RD_BUF_SIZE; i++)
    {
        rd_buffer[i] = Sim_getc();
        if (rd_buffer[i] == '\n') {break;}
    }
    for(; i < GPS_RD_BUF_SIZE; i++)
    {
        rd_buffer[i] = 0;
    }
    CLEAR_BIT(flags, GPS_FLAG_NL);
}


static void GPS_ParseRMS(void)
{
    uint8_t i = 0, status = 1;
    char *segment = strtok(rd_buffer, ",");
    
    while (segment != NULL && status)
    {
        switch (i)
        {
        case 1: /* time */
            break;
        case 2: /* status */
            if (segment[0] == 'V') status = 0;
            break;
        case 3: /* lat */
            break;
        case 4: /* N/S hemisphere */
            break;
        case 5: /* lon */
            break;
        case 6: /* E/W hemisphere */
            break;
        }
        strtok(NULL, ",");
        i++;
    }
    
    gps_data.status = status;
}