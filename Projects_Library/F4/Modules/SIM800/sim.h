/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM_H
#define __SIM_H

#include "!Project_library.h"


#define FLAG_READY              1
#define FLAG_INIT               2
#define FLAG_ALIVE              4

#define SIM_BUF_MASK            127

/* commands ----------------------------------------------------------------- */
#define SIM_CMD_NULL            -1
#define SIM_CMD_INIT             0
#define SIM_CMD_AT               1


/* states ------------------------------------------------------------------- */
#define SIM_ST_POWER_ON         0
#define SIM_ST_I_CONFIG         1  /* interface config */


/* SIM module IO functions -------------------------------------------------- */
void Sim_EndOfTransaction(void);
void Sim_putc(uint8_t c);
/* -------------------------------------------------------------------------- */


/* SIM module status functions ---------------------------------------------- */
void Sim_Init(void);
void Sim_CMD(FunctionalState state);
/* -------------------------------------------------------------------------- */

#endif