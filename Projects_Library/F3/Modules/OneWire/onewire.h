/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include "!Project_library.h"
#include "timers.h"


typedef struct {
    void* data;
    uint8_t size;
} OneWireData;



void reset(void);
void OneWire_Write_Pin_Init(void);
void OneWire_Read_Pin_Init(void);
void OneWireSendData(void* data);

#endif