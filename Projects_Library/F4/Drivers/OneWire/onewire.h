/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include "!Project_library.h"


typedef enum{
    Search_ROM = 0xF0,  // command is used to detect addresses of multiple devices on startup.
    Read_ROM = 0x33,    // command is used to detect address of device on startup, when there is only 1 device on bus.
    Match_ROM = 0x55,   // command is used to address a specific device.
    Skip_ROM = 0xCC,    // command is used for multicast messages.
} ROM_Cmd;


void Transmit_Bit(void);
void Receive_Bit(void);
void Start_Transaction(uint8_t *data_ptr, uint8_t size);
void Reset(void);
void Send_Cmd(ROM_Cmd cmd);
void OneWire_Main(void);

#endif