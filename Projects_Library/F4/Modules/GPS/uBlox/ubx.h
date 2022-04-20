#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t cls;
    uint8_t id;
    uint16_t len;
    uint32_t iTOW;
    int32_t lon;
    int32_t lat;
    int32_t height;
    int32_t hMSL;
    uint32_t hAcc;
    uint32_t vAcc;
} NAV_POSLLH;


typedef struct
{
    uint8_t h1;
    uint8_t h2;
    uint8_t class;
    uint8_t id;
    uint16_t length;
    uint8_t* payload;
} UBX_PACK;

typedef union
{   
    uint8_t array[20];
    struct
    {
        uint8_t port_num;
        uint8_t reserved1;
        uint16_t txReady;
        uint32_t mode;
        uint32_t baudRate;
        uint16_t inProtoMask;
        uint16_t outProtoMask;
        uint16_t flags;
        uint8_t reserved2;
        uint8_t reserved3;
    } fields;
} UBX_CFG_PRT;


void UBX_CalcChecksum(uint8_t* CK, void* _data, size_t size);
void UBX_SetHeader(UBX_PACK* pack);