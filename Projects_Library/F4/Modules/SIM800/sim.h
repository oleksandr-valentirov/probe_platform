/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM_H
#define __SIM_H

#include "!Project_library.h"


#define FLAG_READY              1
#define FLAG_INIT               2
#define FLAG_ALIVE              4

#define SIM_BUF_MASK            127

#define CMD_INIT                0
#define CMD_AT                  1


void Sim_EndOfTransaction(void);
void Sim_putc(uint8_t c);
void Sim_Init(void);

#endif