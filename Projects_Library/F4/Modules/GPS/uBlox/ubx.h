#pragma once

#include <stdint.h>
#include <stddef.h>


#define UBX_SYNC_CH_1   0xB5
#define UBX_SYNC_CH_2   0x62


#define UBX_CLASS_NAV   0x01
#define UBX_CLASS_RXM   0x02
#define UBX_CLASS_INF   0x04
#define UBX_CLASS_ACK   0x05
#define UBX_CLASS_CFG   0x06

/* ACK msg class */
#define UBX_ID_ACK      0x01
#define UBX_ID_NACK     0x00

/* CFG msg class */
#define UBX_ID_MSG      0x01
#define UBX_ID_HNR      0x5C


typedef struct
{
    uint8_t cls;
    uint8_t id;
    uint16_t length;
} UBX_HEADER;

typedef struct
{
    uint8_t highNavRate;
    uint8_t[3] reserved;
}

typedef struct
{
    uint8_t clsID;
    uint8_t msgID;
} UBX_ACK;

typedef struct
{
    uint8_t msgCls;
    uint8_t msgID;
    uint8_t rate;
} UBX_CFG_MSG;

typedef struct
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
} UBX_CFG_PRT;

typedef struct {
    uint32_t iTOW;
    int32_t lon;
    int32_t lat;
    int32_t height;
    int32_t hMSL;
    uint32_t hAcc;
    uint32_t vAcc;
} UBX_NAV_POSLLH;



void UBX_CalcChecksum(uint8_t* CK, void* _data, size_t size);