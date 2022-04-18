#include "ubx.h"

void UBX_CalcChecksum(unsigned char* CK)
{
    memset(CK, 0, 2);
    for (int i = 0; i < (int)sizeof(NAV_POSLLH); i++)
    {
        CK[0] += ((unsigned char*)(&posllh))[i];
        CK[1] += CK[0];
    }
}