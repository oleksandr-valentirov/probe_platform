/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IIC_H
#define __IIC_H

#include "!Project_library.h"


/* Init ------------------- */
void I2C_Reset(void);
void I2C_Driver_Init(void);
void I2C_Set_EndOfTransactionCallback(void(*f)(void));

/* Flags ------------------ */
uint8_t I2C_Get_Busy_Flag(void);

/* Communication ---------- */
void I2C_Start_Transmission(void* source, size_t counter);
void I2C_Start_Reception(void* destination, size_t counter);
void I2C_Transmit_Next_Byte(void);
void I2C_Receive_Next_Byte(void);
void I2C_Call_EnfOfTransactionCallback(void);

#endif