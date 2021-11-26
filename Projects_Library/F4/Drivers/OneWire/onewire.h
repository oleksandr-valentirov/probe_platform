/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ONEWIRE_F4_H
#define __ONEWIRE_F4_H

#include "!Project_library.h"


typedef enum{
    Search_ROM = 0xF0,  // command is used to detect addresses of multiple devices on startup.
    Read_ROM = 0x33,    // command is used to detect address of device on startup, when there is only 1 device on bus.
    Match_ROM = 0x55,   // command is used to address a specific device.
    Skip_ROM = 0xCC,    // command is used for multicast messages.
} ROM_Cmd;


typedef enum{
    Conv_T = 0x44,
    Read_SCR = 0xBE,
} FUNC_Cmd;


typedef enum {
    INIT,
    READ,
    ROMCOM,
    FUNCCOM
}StateType;
        

#define         ONEWIRE_RESET_STATUS            0x01
#define         ONEWIRE_BUS_STATUS              0x02
#define         ONEWIRE_SEARCH_ROM_STATUS       0x04
#define         ONEWIRE_CONVERT_T_STATUS        0x08
#define         ONEWIRE_READ_SCR_STATUS         0x10
#define         ONEWIRE_DESCREP_STATUS          0x20
#define         ONEWIRE_BUSY_STATUS             0x80


void Transmit_Bit(void);
void Receive_Bit(void);
void OneWire_ReadWrite(void);
void Reset(void);

StateType Get_Current_State(void);
void Set_OneWire_Init_State(void);

void Set_OneWire_Status(uint8_t mask);
void Reset_OneWire_Status(uint8_t mask);
uint8_t Get_OneWire_Status(uint8_t mask);

void OneWire_Init(void);
void OneWire_Main(void);
void Data_Line_Up(void);
void Data_Line_Down(void);
void Data_Line_Float(void);
void Data_Line_Set_Out(void);
void Data_Line_Set_AF(void);

#endif