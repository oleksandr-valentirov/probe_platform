#include "ubx.h"

/* Rx buffer */
static uint8_t rx_buffer[GPS_BUF_SIZE] = {0};
static uint8_t rx_buf_pos = 0;
/* Tx buffer */
static uint8_t tx_buffer[GPS_BUF_SIZE] = {0};
static const uint8_t* tx_packet_start = tx_buffer + 2;

/* flags ---------------------- */
static uint8_t flags = 0;

#define SET_FLAG_MSG_TX         SET_BIT(flags, GPS_FLAG_MSG_TX)
#define READ_FLAG_MSG_TX        READ_BIT(flags, GPS_FLAG_MSG_TX)
#define RESET_FLAG_MSG_TX       CLEAR_BIT(flags, GPS_FLAG_MSG_TX)

#define SET_FLAG_MSG_RX         SET_BIT(flags, GPS_FLAF_MSG_RX)
#define READ_FLAG_MSG_RX        READ_BIT(flags, GPS_FLAF_MSG_RX)
#define RESET_FLAG_MSG_RX       CLEAR_BIT(flags, GPS_FLAF_MSG_RX)

void UBX_ResetFlagMsgTx(void)
{
    RESET_FLAG_MSG_TX;
}

void UBX_ResetFlagMsgRx(void)
{
    RESET_FLAG_MSG_RX;
}
/* ---------------------------- */


static void UBX_CalcChecksum(size_t payload_size)
{
    size_t crc_size = payload_size + sizeof(UBX_HEADER);
    uint8_t* CK_0 = ((uint8_t* )tx_packet_start) + crc_size;
    uint8_t* CK_1 = CK_0 + 1;
    *CK_0 = 0;
    *CK_1 = 0;
    for (size_t i = 0; i < crc_size; i++)
    {
        *CK_0 += tx_packet_start[i];
        *CK_1 += *CK_0;
    }
}

uint8_t UBX_SetMsgRate(uint8_t cls, uint8_t id, uint8_t rate)
{
    if(!READ_FLAG_MSG_TX)
    {
        SET_FLAG_MSG_TX;
    }
    else
    {
        return 1;
    }
    

    UBX_HEADER header;
    header.cls = UBX_CLASS_CFG;
    header.id = UBX_ID_MSG;
    header.length = 3;  /* 8 for port config */
    
    UBX_CFG_MSG payload;
    payload.msgCls = cls;
    payload.msgID = id;
    payload.rate = rate;
    
    memcpy((uint8_t*)tx_packet_start,   /* dst  */
           &header,                     /* src  */
           sizeof(UBX_HEADER));         /* size */
    memcpy((uint8_t*)tx_packet_start + sizeof(UBX_HEADER),      /* dst  */
           &payload,                                            /* src  */
           header.length);                                      /* size */

    UBX_CalcChecksum(header.length);
    DMA_GPSoutTransfer(header.length + sizeof(UBX_HEADER) + 2 + 2);
    return 0;
}

void UBX_Init(void)
{    
    tx_buffer[0] = UBX_SYNC_CH_1;
    tx_buffer[1] = UBX_SYNC_CH_2;
    DMA_GPSoutInit((uint32_t*)tx_buffer);

    const uint8_t nmea_msg_id[19] = {NMEA_DTM_ID, NMEA_GBQ_ID, NMEA_GBS_ID,
        NMEA_GGA_ID, NMEA_GLL_ID, NMEA_GLQ_ID, NMEA_GNQ_ID, NMEA_GNS_ID,
        NMEA_GPQ_ID, NMEA_GRS_ID, NMEA_GSA_ID, NMEA_GST_ID, NMEA_GSV_ID,
        NMEA_RMC_ID, NMEA_THS_ID, NMEA_VLW_ID, NMEA_VTG_ID, NMEA_TXT_ID,
        NMEA_ZDA_ID
    };
    const uint8_t ubx_msg_id[3] = {UBX_CLASS_NAV, UBX_ID_POSLLH, 1};
    
    /* disable all nmea messages */
    for(uint8_t i = 0; i < 19; i++)
    {
        while(READ_FLAG_MSG_TX) {}
        UBX_SetMsgRate(NMEA_CLASS, nmea_msg_id[i], 0);
    }
    
    /* enable UBX messages */
    for(uint8_t i = 0; i < 3; i =+ 3)
    {
        while(READ_FLAG_MSG_TX) {}
        UBX_SetMsgRate(ubx_msg_id[i], ubx_msg_id[i + 1], ubx_msg_id[i + 2]);
    }
}


void UBX_ProcessResponce(uint8_t* data)
{
    UBX_HEADER header;
//    memset(&header, 0, sizeof(header));
    memcpy(&header, data + 2, sizeof(header));
    switch (header.cls)
    {
    case UBX_CLASS_NAV:
        break;
    case UBX_CLASS_RXM:
        break;
    case UBX_CLASS_ACK:
        if (!header.id)
        {
        }
        else
        {
        }
        break;
    case UBX_CLASS_CFG:
        break;
    }
}