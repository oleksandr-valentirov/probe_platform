#include "max_m8.h"


/* response buffer */
static char buffer[MAX_M8_BUF_SIZE] = {0};
static uint8_t buf_pos = 0;

/* static functions */
static void GPS_gets(void);
static void GPS_ParseRMS(void);
static void GPS_ParseGGA(void);

static uint8_t flags = 0;
static GPS_DataT gps_data;

#define SetNLFlag       SET_BIT(flags, GPS_FLAG_NL)
#define GetNLFlag       READ_BIT(flags, GPS_FLAG_NL)
#define ClearNLFlag     CLEAR_BIT(flags, GPS_FLAG_NL)
#define GetOPFlag       READ_BIT(flags, GPS_FLAG_OP)
#define SetOPFlag       SET_BIT(flags, GPS_FLAG_OP)
//#define GetPackFlag     READ_BIT(flags, GPS_FLAG_PACK)
//#define SetPackFlag     SET_BIT(flags, GPS_FLAG_PACK)


void GPS_EndOfTransaction(void)
{
    SetOPFlag;
}


void GPS_main(void)
{
    GPS_gets();
    
    if (GetNLFlag)
    {
        ClearNLFlag;
        if (buffer[1] == 'P')
        {   /* proprietary message start */
        }
        else
        {   /* standard message start */
            switch (buffer[3])
            {
            case 'G':
                if (buffer[4] == 'G')
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
    
    if (buf_pos == 0)
    {   /* new line was before this call, so we need to clean buffer */
        while(buf_pos < MAX_M8_BUF_SIZE)
        {
            buffer[buf_pos++] = 0;
        }
        buf_pos = 0;
    }

    while(buf_pos < MAX_M8_BUF_SIZE && USART2_getc(&c))
    {
        buffer[buf_pos++] = c;
        buf_pos &= MAX_M8_BUF_MASK;
        if (c == '\n')
        {
            SetNLFlag;
            buf_pos = 0;
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
    char *segment = strtok(buffer, ",");
    
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
            gps_data.lat_m = (segment[0] - 48) * 10 + (segment[1] - 48);
            break;
        case 4: /* N/S hemisphere */
            break;
        case 5: /* lon */
            gps_data.lon_m = (segment[0] - 48) * 10 + (segment[1] - 48);
            break;
        case 6: /* E/W hemisphere */
            break;
        }
        segment = strtok(NULL, ",");
        i++;
    }
    
    gps_data.status = status;
}

static void GPS_ParseGGA(void)
{
    uint8_t i = 0, temp = 0;
    char *segment = strtok(buffer, ",");
    
    while (segment != NULL)
    {
        switch (i)
        {
        case 1: /* time */
            break;
        case 2: /* lat */
            gps_data.lat_m = (segment[0] - 48) * 10 + (segment[1] - 48);
            break;
        case 3: /* N/S hemisphere */
            break;
        case 4: /* lon */
            gps_data.lon_m = (segment[0] - 48) * 10 + (segment[1] - 48);
            break;
        case 5: /* E/W hemisphere */
            break;
        case 6: /* quality */
            gps_data.quality = (*segment) - 48;
            break;
        case 7: /* numbers of satelites used */
            break;
        case 9: /* altitude */
            temp = strlen(segment) - 2;
            uint8_t m = 0;
            uint16_t temp_alt = 0;
            gps_data.alt = 0;
            while (temp > 0)
            {
                temp_alt = segment[m] - 48;
                for (uint8_t n = 0; n < temp; n++)
                {
                    temp_alt *= 10;
                }
                temp--; m++;
                gps_data.alt += temp_alt;
            }
            break;
        }
        segment = strtok(NULL, ",");
        i++;
    }
}


GPS_DataT GPS_GetData(void)
{
    return gps_data;
}