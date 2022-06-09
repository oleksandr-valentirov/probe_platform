#include "ubx.h"

/* Rx buffer */
static uint8_t rx_buffer[GPS_BUF_SIZE] = {0};
static uint8_t* rx_buf_ptr = rx_buffer;
static UBX_HEADER* rx_header = (UBX_HEADER*)rx_buffer;

/* Tx buffer */
static uint8_t tx_buffer[GPS_BUF_SIZE] = {0};
static UBX_HEADER* tx_header = (UBX_HEADER*)tx_buffer;

/* flags ---------------------- */
static uint8_t flags = 0x7;

#define SET_FLAG_MSG_TX         SET_BIT(flags, GPS_FLAG_MSG_TX)
#define READ_FLAG_MSG_TX        READ_BIT(flags, GPS_FLAG_MSG_TX)
#define RESET_FLAG_MSG_TX       CLEAR_BIT(flags, GPS_FLAG_MSG_TX)

#define SET_FLAG_MSG_RX         SET_BIT(flags, GPS_FLAF_MSG_RX)
#define READ_FLAG_MSG_RX        READ_BIT(flags, GPS_FLAF_MSG_RX)
#define RESET_FLAG_MSG_RX       CLEAR_BIT(flags, GPS_FLAF_MSG_RX)

#define SET_FLAG_POS_UPD        SET_BIT(flags, GPS_FLAG_POS_UPD)
#define GET_FLAG_POS_UPD        READ_BIT(flags, GPS_FLAG_POS_UPD)
#define RESET_FLAG_POS_UPD      CLEAR_BIT(flags, GPS_FLAG_POS_UPD)


static void UBX_ProcessResponce(void);
static void UBX_CalcChecksum(size_t payload_size);
static uint8_t UBX_CheckCkecksum(size_t payload_size);

static UBX_NAV_POSLLH cur_pos;

uint8_t UBX_GetCurPos(UBX_NAV_POSLLH* dst)
{
    if(!GET_FLAG_POS_UPD)
    {   /** @todo - need to manage with flags */
        SET_FLAG_POS_UPD;
        dst->hAcc = cur_pos.hAcc;
        dst->height = cur_pos.height;
        dst->hMSL = cur_pos.hMSL;
        dst->iTOW = cur_pos.iTOW;
        dst->lat = cur_pos.lat;
        dst->lon = cur_pos.lon;
        dst->vAcc = cur_pos.vAcc;
        return 0;
    }
    return 1;
}

uint8_t UBX_GetFlagMsgRx(void)
{
    return READ_FLAG_MSG_RX;
}

void UBX_ResetFlagMsgTx(void)
{
    RESET_FLAG_MSG_TX;
}

void UBX_ResetFlagMsgRx(void)
{
    RESET_FLAG_MSG_RX;
}

void UBX_SetFlagMsgTx(void)
{
    SET_FLAG_MSG_TX;
}

void UBX_SetFlagMsgRx(void)
{
    RESET_FLAG_MSG_RX;
}
/* ---------------------------- */


static void UBX_CalcChecksum(size_t payload_size)
{
    size_t crc_size = payload_size + sizeof(UBX_HEADER) - UBX_SYNC_LEN;
    uint8_t* CK_0 = tx_buffer + crc_size + UBX_SYNC_LEN;
    uint8_t* CK_1 = CK_0 + 1;
    *CK_0 = 0;
    *CK_1 = 0;
    for (size_t i = 0; i < crc_size; i++)
    {
        *CK_0 += tx_buffer[i + UBX_SYNC_LEN];
        *CK_1 += *CK_0;
    }
}

static uint8_t UBX_CheckCkecksum(size_t payload_size)
{
    size_t crc_size = payload_size + sizeof(UBX_HEADER) - UBX_SYNC_LEN;
    uint8_t* CK_0 = rx_buf_ptr + crc_size + UBX_SYNC_LEN;
    uint8_t* CK_1 = CK_0 + 1;
    
    uint8_t CK_0_cur = 0;
    uint8_t CK_1_cur = 0;
    
    for (size_t i = 0; i < crc_size; i++)
    {
        CK_0_cur += rx_buf_ptr[i];
        CK_1_cur += CK_0_cur;
    }
    
    return (CK_0_cur == (*CK_0) && CK_1_cur == (*CK_1)) ? 0 : 1;
}

uint8_t UBX_SetMsgRate(uint8_t cls, uint8_t id, uint8_t rate)
{
    if(!READ_FLAG_MSG_TX)
    {
        return 1;
    }
    else
    {
        RESET_FLAG_MSG_TX;
    }

    tx_header->cls = UBX_CLASS_CFG;
    tx_header->id = UBX_ID_MSG;
    tx_header->length = 3;  /* 8 for port config */
    
    UBX_CFG_MSG payload;
    payload.msgCls = cls;
    payload.msgID = id;
    payload.rate = rate;
    
    memset(tx_buffer + sizeof(UBX_HEADER), 0, tx_header->length + UBX_CK_LEN);
    memcpy(tx_buffer + sizeof(UBX_HEADER),      /* dst  */
           &payload,                            /* src  */
           tx_header->length);                  /* size */

    UBX_CalcChecksum(tx_header->length);
    DMA_USART1outTransfer(tx_header->length + sizeof(UBX_HEADER) + UBX_CK_LEN);
    return 0;
}

void UBX_Init(void)
{    
    tx_buffer[0] = UBX_SYNC_CH_0;
    tx_buffer[1] = UBX_SYNC_CH_1;
    DMA_USART1outInit((uint32_t*)tx_buffer);
    DMA_USART1inInit((uint32_t*)rx_buffer);

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
        while(!READ_FLAG_MSG_TX) {}
        UBX_SetMsgRate(NMEA_CLASS, nmea_msg_id[i], 0);
    }
    
    /* enable UBX messages */
    for(uint8_t i = 0; i < 3; i =+ 3)
    {
        while(!READ_FLAG_MSG_TX) {}
        UBX_SetMsgRate(ubx_msg_id[i], ubx_msg_id[i + 1], ubx_msg_id[i + 2]);
    }
    
    DMA_USART1inTransferStart(UBX_MAX_PACK_LEN);
}


void UBX_main(void)
{
    if(sizeof(UBX_HEADER) < (UBX_MAX_PACK_LEN - DMA_USART1inGetRemainingDataCounter()))
    {
        UBX_ProcessResponce();
    }
    for(uint8_t i = 0; i < 40; i++)
    {
        rx_buffer[i] = 0;
    }
    DMA_USART1inTransferStart(UBX_MAX_PACK_LEN);
}


static void UBX_ProcessResponce(void)
{
    uint8_t* cur_pos_ptr = (uint8_t*)(&cur_pos);
    switch (rx_header->cls)
    {
    case UBX_CLASS_NAV:
        switch (rx_header->id)
        {
        case UBX_ID_POSLLH:
            for(uint8_t i = 0; i < sizeof(UBX_NAV_POSLLH); i++)
            {
                cur_pos_ptr[i] = rx_buffer[sizeof(UBX_HEADER) + i];
            }
            RESET_FLAG_POS_UPD;
            break;
        }
        break;
    case UBX_CLASS_RXM:
        break;
    case UBX_CLASS_ACK:
        if (!rx_header->id)
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
