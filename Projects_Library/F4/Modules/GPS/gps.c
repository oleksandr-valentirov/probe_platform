#include "gps.h"

#define SPI_DEV_ID      SPI_GPS

static char resp_buffer[GPS_RESP_BUF_SIZE] = {0};
static uint8_t rd_pos = 0;
static uint16_t transaction_data_counter = SPI_DATA_TR_LEN;

static GPS_DataT gps_data;


static void GPS_gets(void);
static void GPS_ParseRMS(void);
static void GPS_ParseGGA(void);


/* Flags ---------------------------------------------------------------------*/
static uint8_t flags;

#define SetNLFlag       SET_BIT(flags, GPS_FLAG_NL)
#define GetNLFlag       READ_BIT(flags, GPS_FLAG_NL)
#define SetOPFlag       SET_BIT(flags, GPS_FLAG_OP)
#define GetOPFlag       READ_BIT(flags, GPS_FLAG_OP)


static void GPS_SpiOpCallback(void)
{
    CLEAR_BIT(flags, GPS_FLAG_OP);
}
/*----------------------------------------------------------------------------*/


void GPS_InitSPI(void)
{
    SET_BIT(GPS_DSEL_PORT->ODR, GPS_DSEL_PIN);
}

void GPS_InitUART(void)
{
    CLEAR_BIT(GPS_DSEL_PORT->ODR, GPS_DSEL_PIN);
}


void GPS_main(void)
{
    if (SPI3_IsMine(SPI_DEV_ID)) 
    {   /* read SPI buffer and start reception operation */
        GPS_gets();

        if (!GetOPFlag && !transaction_data_counter)
        {   /* check that all data was obtained */
            SPI3_FreeMutex(GPS_CS_PORT, GPS_CS_PIN);
        }
    }
    if (SPI3_IsFree() && !SysTick_GetGPSClock())
    {   /* check that SPI is free and it is time to perform IO */
        SysTick_UpdateGPSClock();

        SPI3_SetMutex(GPS_CS_PORT, GPS_CS_PIN, SPI_DEV_ID);
        SPI3_RegisterCallback(&GPS_SpiOpCallback, SPI_DEV_ID);
        transaction_data_counter = SPI_DATA_TR_LEN;
        SPI3_StartReading(transaction_data_counter, SPI_DEV_ID, 0xFF);
        SetOPFlag;
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
                if (resp_buffer[4] == 'G')
                {
                    GPS_ParseGGA();
                }
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

    while(rd_pos < GPS_RESP_BUF_SIZE && !SPI3_getc(&c) && transaction_data_counter--)
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

static void GPS_ParseGGA(void)
{
}


GPS_DataT GPS_GetData(void)
{
    return gps_data;
}