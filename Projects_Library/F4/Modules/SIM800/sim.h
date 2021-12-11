/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM_H
#define __SIM_H

#include "!Project_library.h"


#define FLAG_READY              1
#define FLAG_INIT               2
#define FLAG_ALIVE              4

#define SIM_BUF_MASK            127

/* commands ----------------------------------------------------------------- */
/* 2 non-processible CMDs */
#define SIM_CMD_NULL            -2  /* there is no current CMD */
#define SIM_CMD_REP             -1  /* repeat previous CMD     */
/*----------------------------- */
#define SIM_CMD_INIT            0
#define SIM_CMD_AT              1
#define SIM_CMD_SMS             2
#define SIM_CMD_FLY             3


/* states ------------------------------------------------------------------- */
#define SIM_ST_DEFAULT          -1
#define SIM_ST_POWER_ON         0
#define SIM_ST_I_CONFIG         1  /* interface config */


/* SIM module IO functions -------------------------------------------------- */
void Sim_EndOfTransaction(void);
void Sim_putc(uint8_t c);
/* -------------------------------------------------------------------------- */


/* SIM module status functions ---------------------------------------------- */
void Sim_StateMachine(void);
void Sim_CMD(FunctionalState state);
void Sim_StatusEXTI_Enable(void);
void Sim_RI_EXTICmd(FunctionalState state);
/* -------------------------------------------------------------------------- */

#endif