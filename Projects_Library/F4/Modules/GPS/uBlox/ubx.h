#pragma once

#include "!Project_library.h"

#define GPS_FLAG_MSG_TX 0x01
#define GPS_FLAF_MSG_RX 0x02

#define GPS_BUF_SIZE    128
#define GPS_BUF_MASK    127

#define UBX_SYNC_CH_0           0xB5
#define UBX_SYNC_CH_1           0x62
#define UBX_SYNC_LEN            2
#define UBX_CK_LEN              2
#define UBX_MAX_PAYLOAD_LEN     sizeof(UBX_NAV_POSLLH)
#define UBX_MAX_PACK_LEN        ((sizeof(UBX_HEADER)) + (UBX_MAX_PAYLOAD_LEN) + (UBX_CK_LEN))

/* UBX msg class ------------ */
#define UBX_CLASS_NAV   0x01
#define UBX_CLASS_RXM   0x02
#define UBX_CLASS_INF   0x04
#define UBX_CLASS_ACK   0x05
#define UBX_CLASS_CFG   0x06

/* ACK msg ID */
#define UBX_ID_ACK      0x01
#define UBX_ID_NACK     0x00

/* CFG msg ID */
#define UBX_ID_MSG      0x01
#define UBX_ID_HNR      0x5C

/* NAV msg ID */
#define UBX_ID_POSLLH   0x02
/* -------------------------- */


/* NMEA --------------------- */
#define NMEA_CLASS      0xF0
#define NMEA_UBX_CLASS  0xF1

#define NMEA_DTM_ID     0xA0
#define NMEA_GBQ_ID     0x44
#define NMEA_GBS_ID     0x09
#define NMEA_GGA_ID     0x00
#define NMEA_GLL_ID     0x01
#define NMEA_GLQ_ID     0x43
#define NMEA_GNQ_ID     0x42
#define NMEA_GNS_ID     0x0D
#define NMEA_GPQ_ID     0x40
#define NMEA_GRS_ID     0x06
#define NMEA_GSA_ID     0x02
#define NMEA_GST_ID     0x07
#define NMEA_GSV_ID     0x03
#define NMEA_RMC_ID     0x04
#define NMEA_THS_ID     0x0E
#define NMEA_TXT_ID     0x41
#define NMEA_VLW_ID     0x0F
#define NMEA_VTG_ID     0x05
#define NMEA_ZDA_ID     0x08

/* PUBX */
#define NMEA_UBX_CONF   0x41
#define NMEA_UBX_POS    0x00
#define NMEA_UBX_RATE   0x40
#define NMEA_UBX_SVSTAT 0x03
#define NMEA_UBX_TIME   0x04
/* -------------------------- */


typedef struct
{
    uint8_t sync_chr_0;
    uint8_t sync_chr_1;
    uint8_t cls;
    uint8_t id;
    uint16_t length;
} UBX_HEADER;

typedef struct
{
    uint8_t highNavRate;
    uint8_t reserved[3];
} UBX_CFG_HNR;

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


void UBX_Init(void);
void UBX_main(void);
void UBX_ResetFlagMsgTx(void);
void UBX_ResetFlagMsgRx(void);
void UBX_SetFlagMsgTx(void);
void UBX_SetFlagMsgRx(void);
uint8_t UBX_GetFlagMsgRx(void);