/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __M8_H
#define __M8_H

#include "!Project_library.h"
#include "ubx.h"

#define GPS_BUF_SIZE            128
#define GPS_BUF_MASK            127

/* external functions */
void GPS_Init(void);
void GPS_main(void);
void GPS_EndOfTransaction(void);

#endif