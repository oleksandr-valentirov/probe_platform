#include "gps.h"

#define SPI_DEV_ID      SPI_GPS

static char resp_buffer[GPS_RESP_BUF_SIZE] = {0};
static uint8_t wr_pos = 0;
static uint8_t rd_pos = 0;

static GPS_DataT gps_data;


static void GPS_gets(void);
static void GPS_ParseRMS(void);


/* Flags ---------------------------------------------------------------------*/
static uint8_t flags;

#define SetNLFlag       SET_BIT(flags, GPS_FLAG_NL)
#define GetNLFlag       READ_BIT(flags, GPS_FLAG_NL)

/*----------------------------------------------------------------------------*/


void GPS_main(void)
{
    if (SPI3_IsMine(SPI_DEV_ID)) 
    {   /* read SPI buffer and start reception operation */
        GPS_gets();
    }
    if (SPI3_IsFree() && 0)
    {   /* check that SPI is free and it is time to perform IO */
    }
    
    if (GetNLFlag)
    {
        if (resp_buffer[1] == 'P')
        {   /* proprietary message start */
        }
        else
        {   /* standard message start */
            switch (resp_buffer[3])
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
}


static void GPS_gets(void)
{
    uint8_t c = 0;

    while(rd_pos < GPS_RESP_BUF_SIZE && SPI3_getc(&c))
    {
        resp_buffer[rd_pos++] = c;
        rd_pos &= GPS_RESP_BUF_MASK;
        if (c == '\n')
        {
            SetNLFlag;
            rd_pos = 0;
            break;
        }
         else if (c == '*')
        {   /* checksum */
        }
        else if (c == '$')
        {   /* packet data start */
        }
    }
}


static void GPS_ParseRMS(void)
{
    uint8_t i = 0, status = 1;
    char *segment = strtok(resp_buffer, ",");
    
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