/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM_H
#define __SIM_H

#include "!Project_library.h"


typedef enum
{
    ATH = 1,
    SAPBR,
} Sim_Awaitable_CMD_t;


typedef struct
{
    int8_t rssi;        /* AT+CSQ */
    uint8_t aw_cmd;
    char my_num[15];    /* AT+CNUM */
    uint8_t is_enabled  :1;
} Sim_state_t;


#define SIM_FLAG_READY          1  /* transmittion finished */
#define SIM_FLAG_NL             2  /* new line received */
#define SIM_FLAG_ALIVE          4
#define SIM_FLAG_RI             8  /* call was received */
#define SIM_FLAG_CALL_READY     16
#define SIM_FLAG_SMS_READY      32
#define SIM_FLAG_CALL           64 /* number was fetched, ready to send SMS */
#define SIM_FLAG_TXT_IN         128
#define SIM_FLAG_TOKEN          256  /* 0 - no token; 1 - token was received */

#define SIM_BUF_SIZE            64



/* SIM module status functions ---------------------------------------------- */
void Sim_CMD(FunctionalState state);
void Sim_RI_EXTICmd(FunctionalState state);
/* -------------------------------------------------------------------------- */


/* SIM module extern functions ---------------------------------------------- */
void Sim_main(void);
void Sim_EndOfTransaction(void);
void* Sim_init(void);
void Sim_SetStatus(uint8_t status);


/* RI */
void Sim_RIEventStart(void);


#endif