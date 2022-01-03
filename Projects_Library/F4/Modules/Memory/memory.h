#ifndef _MEMORY_H
#define _MEMORY_H

#include "!Project_library.h"

typedef enum
{
    MemSize256 = 0,
    MemSize4K,
    MemSize32K,
    MemSize64K,
} MemSize_t;

#define MEM_FLAG_BUSY           1
#define MEM_FLAG_RD             2
#define MEM_FLAG_WR             4

__weak uint8_t MEM_erase_block(size_t offset);
__weak uint8_t MEM_read_page(uint8_t *dst, MemSize_t size);
__weak uint8_t MEM_program_page(uint8_t *src);

#endif