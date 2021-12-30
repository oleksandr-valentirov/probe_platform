/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_H
#define __GPS_H


#include "!Project_library.h"

#define GPS_FLAG_NL             1  /* new line flag */
#define GPS_FLAG_OP             2  /* performing IO operation right now */
#define GPS_FLAG_PACK           4  /* packet start byte was detected */

typedef struct 
{
    uint32_t lat_m;
    uint32_t lon_m;
    uint16_t alt;
    union
    {
        uint32_t _;
        int16_t d_arr[2];       /* 0 - lat; 1 - lon degrees */
    };
    uint8_t quality;
    uint8_t status;             /* 1 - valid, 0 - not valid */
} GPS_DataT;

#endif