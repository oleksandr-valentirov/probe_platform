/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_H
#define __GPS_H


#include "!Project_library.h"

#define GPS_RESP_BUF_SIZE       128
#define GPS_RESP_BUF_MASK       127
#define GPS_RD_BUF_SIZE         64

#define GPS_FLAG_NL     1

typedef struct 
{
    int32_t lat_m;
    int32_t lon_m;

    union
    {
        uint32_t _;
        int16_t d_arr[2];       /* 0 - lat; 1 - lon degrees */
    };

    uint8_t status;             /* 1 - valid, 0 - not valid */
} GPS_DataT;

#endif