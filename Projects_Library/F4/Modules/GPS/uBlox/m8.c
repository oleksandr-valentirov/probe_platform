#include "m8.h"


/* response buffer */
static char buffer[GPS_BUF_SIZE] = {0};
static uint8_t buf_pos = 0;

/* static functions */
static void GPS_gets(void);

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

void GPS_Init(void)
{

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

GPS_DataT GPS_GetData(void)
{
    return gps_data;
}