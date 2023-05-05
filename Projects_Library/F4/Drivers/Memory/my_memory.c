#include "my_memory.h"


void MEM_Write(void* src, void* dst, size_t size)
{
    uint8_t* a = (uint8_t*)src;
    uint8_t* b = (uint8_t*)dst;
//    if(a >= (uint8_t*)SECTOR_7 && (a + size) > (SECTOR_7 + SIZE)) {return;}
    while(size--)
    {
        *(a++) = *(b++);
    }
}

void MEM_CalcCRC(void)
{
}

void MEM_CheckCRC(void)
{
}
