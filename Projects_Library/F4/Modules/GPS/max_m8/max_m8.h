/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAX_M8_H
#define __MAX_M8_H

#include "!Project_library.h"

#define MAX_M8_BUF_SIZE         128
#define MAX_M8_BUF_MASK         127

/* external functions */
void GPS_main(void);
void GPS_EndOfTransaction(void);

#endif