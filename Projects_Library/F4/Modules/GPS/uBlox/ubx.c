#include "ubx.h"

void UBX_CalcChecksum(uint8_t* CK, void* _data, size_t size)
{
    CK[0] = 0;
    CK[1] = 0;
    uint8_t* data = (uint8_t*)_data;
    for (size_t i = 0; i < size; i++)
    {
        CK[0] += data[i];
        CK[1] += CK[0];
    }
}

void UBX_SetHeader(UBX_PACK* pack)
{
    pack->h1 = 0xb5;
    pack->h2 = 0x62;
}