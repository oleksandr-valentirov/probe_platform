#ifndef _MEMORY_H
#define _MEMORY_H

#include "!Project_library.h"

typedef enum
{
    MemSizePage,
    MemSize4K,
    MemSize32K,
    MemSize64K,
    MemSizeFullChip
} MemSize_t;

#define MEM_FLAG_BUSY           1

#define MEM_BIT_ORDER           ((SPI_CR1_LSBFIRST) & (0))  // MSB first

#define MEM_BUF_SIZE            64
#define MEM_PAGE_SIZE           256
#define MEM_PAGE_NUM            65536


uint8_t MEM_erase(uint8_t *addr, MemSize_t size);
uint8_t MEM_read_page(uint8_t *dst);
uint8_t MEM_write_page(void);

#endif