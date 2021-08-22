/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include "!Project_library.h"
#include "timers.h"

void reset(void);
void OneWire_Write_Pin_Init(void);
void OneWire_Read_Pin_Init(void);

#endif