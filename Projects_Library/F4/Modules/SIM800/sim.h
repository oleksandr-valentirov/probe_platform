/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM_H
#define __SIM_H

#include "!Project_library.h"


typedef struct
{
    int8_t rssi;        /* AT+CSQ */
    char my_num[15];    /* AT+CNUM */
} Sim_state_t;


#define SIM_FLAG_READY          1  /* transmittion finished */
#define SIM_FLAG_NL             2  /* new line received */
#define SIM_FLAG_ALIVE          4
#define SIM_FLAG_RI             8  /* call was received */
#define SIM_FLAG_CALL_READY     16
#define SIM_FLAG_SMS_READY      32
#define SIM_FLAG_CALL           64 /* number was fetched, ready to send SMS */
#define SIM_FLAG_TXT_IN         128


#define SIM_RESP_BUF_SIZE       128
#define SIM_RESP_BUF_MASK       127
#define SIM_RD_BUF_SIZE         64
#define SUN_RD_BUF_MASK         63

/* commands ----------------------------------------------------------------- */
/* 2 non-processible CMDs */
#define SIM_CMD_NULL            -2  /* there is no current CMD */
#define SIM_CMD_REP             -1  /* repeat previous CMD     */
/*----------------------------- */
#define SIM_CMD_INIT            0
#define SIM_CMD_AT              1
#define SIM_CMD_SMS             2
#define SIM_CMD_FLY             3


typedef enum {
    SIM_RI_SHORT,
    SIM_RI_LONG
} SIM_RI_TYPE;


/* states ------------------------------------------------------------------- */
#define SIM_ST_DEFAULT          -1
#define SIM_ST_POWER_ON         0
#define SIM_ST_I_CONFIG         1  /* interface config */


/* SIM module IO functions -------------------------------------------------- */
void Sim_EndOfTransaction(void);
void Sim_putc(uint8_t c);
uint8_t Sim_getc(void);
/* -------------------------------------------------------------------------- */


/* SIM module status functions ---------------------------------------------- */
void Sim_CMD(FunctionalState state);
void Sim_StatusEXTI_Enable(void);
void Sim_RI_EXTICmd(FunctionalState state);
/* -------------------------------------------------------------------------- */


/* SIM module logic functions ----------------------------------------------- */
void Sim_ReceiveCall(void);
void Sim_SendSMSCmd(void);
void Sim_SendMsg(void);
void Sim_ProcessLine(void);
void Sim_StateInit(void);
void Sim_ResetCurCmd(void);


/* RI */
void Sim_RIEventStart(void);


/* Flags */
uint8_t Sim_GetReadyFlag(void);
uint8_t Sim_GetNLFlag(void);
uint8_t Sim_GetRIFlag(void);
void Sim_ClearRIFlag(void);
uint8_t Sim_OperationReady(void);
uint8_t Sim_GetCallFlag(void);
void Sim_ClearCallFlag(void);
uint8_t Sim_GetTxtInFlag(void);
void Sim_ClearTxtInFlag(void);

#endif