#include "memory.h"


__weak uint8_t MEM_erase_block(size_t offset) {return 1;}

__weak uint8_t MEM_read_page(uint8_t *dst, MemSize_t size) {return 1;}

__weak uint8_t MEM_program_page(uint8_t *src) {return 1;}