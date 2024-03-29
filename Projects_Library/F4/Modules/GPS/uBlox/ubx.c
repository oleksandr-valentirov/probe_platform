#include "ubx.h"
#include "usbd_cdc_if.h"

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
static uint8_t UBX_CheckCkecksum(UBX_HEADER* header);
static uint8_t* UBX_FindSynchChars(uint8_t* start_ptr, uint8_t msg_len);

#if FULL_NAV_MSG_ENABLED
static UBX_NAV_PVT cur_pos;
#else
static UBX_NAV_POSLLH cur_pos;
#endif

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
    SET_FLAG_MSG_RX;
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

static uint8_t UBX_CheckCkecksum(UBX_HEADER* header)
{
    const uint8_t* rx_buf_ptr = (uint8_t*)header;
    
    size_t crc_size = 4 + header->length;
    uint8_t* CK_0 = (uint8_t*)header + sizeof(UBX_HEADER) + header->length;
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

static uint8_t UBX_SetMsgRate(uint8_t cls, uint8_t id, uint8_t rate)
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

void* UBX_Init(void)
{    
    tx_buffer[0] = UBX_SYNC_CH_0;
    tx_buffer[1] = UBX_SYNC_CH_1;
    DMA_USART1outInit((uint32_t*)tx_buffer);
    DMA_USART1inInit((uint32_t*)rx_buffer);
    unsigned int clock;

    const uint8_t nmea_msg_id[19] = {NMEA_DTM_ID, NMEA_GBQ_ID, NMEA_GBS_ID,
        NMEA_GGA_ID, NMEA_GLL_ID, NMEA_GLQ_ID, NMEA_GNQ_ID, NMEA_GNS_ID,
        NMEA_GPQ_ID, NMEA_GRS_ID, NMEA_GSA_ID, NMEA_GST_ID, NMEA_GSV_ID,
        NMEA_RMC_ID, NMEA_THS_ID, NMEA_VLW_ID, NMEA_VTG_ID, NMEA_TXT_ID,
        NMEA_ZDA_ID
    };
    const uint8_t ubx_msg_id[3] = {UBX_CLASS_NAV, 
#if FULL_NAV_MSG_ENABLED
    UBX_ID_PVT,
#else
    UBX_ID_POSLLH,
#endif
    1};
    
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
    
    clock = SysTick_GetCurrentClock();
    for(int i = 0; i < sizeof(ubx_msg_id[2]); i++)
    {
        while(!READ_BIT(USART1->SR, USART_SR_RXNE) && (SysTick_GetCurrentClock() - clock) < 1500){}
        
        if(READ_BIT(USART1->SR, USART_SR_RXNE))
        {
            rx_buffer[i] = USART1->DR;
        }
        else
        {
            return NULL;
        }
    }
    
    if(UBX_FindSynchChars(rx_buffer, sizeof(ubx_msg_id[2])) != NULL)
    {
        DMA_USART1inTransferStart(GPS_BUF_SIZE);
        return (void*)UBX_main;
    }
    else
    {
        return NULL;
    }
}


void UBX_main(void)
{
    if(!READ_FLAG_MSG_RX)
    {
        SET_FLAG_MSG_RX;
//    if(sizeof(UBX_HEADER) < (UBX_MAX_PACK_LEN - DMA_USART1inGetRemainingDataCounter()))
        {
            UBX_ProcessResponce();
        }
        for(uint8_t i = 0; i < GPS_BUF_SIZE - 1; i++)
        {
            rx_buffer[i] = 0;
        }
        DMA_USART1inTransferStart(GPS_BUF_SIZE);
    }
}


static uint8_t* UBX_FindSynchChars(uint8_t* start_ptr, uint8_t msg_len)
{
    while(msg_len)
    {
        if(*start_ptr == UBX_SYNC_CH_0 && *(start_ptr + 1) == UBX_SYNC_CH_1)
        {
            return start_ptr;
        }
        msg_len -= 2;
        start_ptr += 2;
    }
    return NULL;
}


static void UBX_ProcessResponce(void)
{
    uint16_t data_len = GPS_BUF_SIZE - DMA_USART1inGetRemainingDataCounter();
//    uint8_t* rx_buf_ptr = rx_buffer;    
//    uint8_t* dst = NULL;
//    UBX_HEADER* rx_header = NULL;
    
    
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
        case UBX_ID_PVT:
            for(uint8_t i = 0; i < sizeof(UBX_NAV_PVT); i++)
            {
                cur_pos_ptr[i] = rx_buf_ptr[sizeof(UBX_HEADER) + i];
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
    default:
        CDC_Transmit_FS((uint8_t*)"UBX class err\r\n", 15);
    }
    data_len = data_len - sizeof(UBX_HEADER) - rx_header->length - 2;
}

static void UBX_ProcessResponceNew(void)
{
    uint8_t data_len = GPS_BUF_SIZE - DMA_USART1inGetRemainingDataCounter();
    uint8_t* rx_buf_ptr = rx_buffer;    
    uint8_t* dst = NULL;
    UBX_HEADER* rx_header = NULL;
    
    while (data_len > 0)
    {
        rx_buf_ptr = UBX_FindSynchChars(rx_buf_ptr, data_len);
        if(!rx_buf_ptr)
        {
            return;
        }
        
        rx_header = (UBX_HEADER*)rx_buf_ptr;
        if(!UBX_CheckCkecksum(rx_header))
        {
            switch (rx_header->cls)
            {
            case UBX_CLASS_NAV:
                switch (rx_header->id)
                {
                case UBX_ID_POSLLH:
                    dst = (uint8_t*)(&cur_pos);
                    for(uint8_t i = 0; i < sizeof(UBX_NAV_POSLLH); i++)
                    {
                        dst[i] = rx_buf_ptr[sizeof(UBX_HEADER) + i];
                    }
                    RESET_FLAG_POS_UPD;
                    break;
                case UBX_ID_PVT:
                    dst = (uint8_t*)(&cur_pos);
                    for(uint8_t i = 0; i < sizeof(UBX_NAV_PVT); i++)
                    {
                        dst[i] = rx_buf_ptr[sizeof(UBX_HEADER) + i];
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
            default:
                CDC_Transmit_FS((uint8_t*)"UBX class err\r\n", 15);
            }
        }
        else
        {
            data_len -= 2;
            rx_buf_ptr += 2;
        }
        
        data_len = data_len - sizeof(UBX_HEADER) - rx_header->length - 2;
        rx_buf_ptr = rx_buf_ptr + sizeof(UBX_HEADER) + rx_header->length + 2;
    }
}
