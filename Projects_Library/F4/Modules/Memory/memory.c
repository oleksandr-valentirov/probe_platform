#include "memory.h"


#define CS_LOW          CLEAR_BIT(MEM_SPI_PORT->ODR, MEM_SPI_PIN)
#define CS_HIGH         SET_BIT(MEM_SPI_PORT->ODR, MEM_SPI_PIN)


static struct {
    uint8_t page[MEM_BUF_SIZE - 2];
    uint16_t crc16;
} cur_data;


static uint8_t temp_arr[MEM_BUF_SIZE];
static uint8_t *temp_ptr = temp_arr;

static uint8_t *page_ptr = cur_data.page;

void MEM_append_data(uint8_t *data_ptr, uint8_t size)
{
    if (temp_arr != temp_ptr)
    {   // copy temp to page buffer
        for (uint8_t *i = temp_arr; i != temp_ptr; i++)
        {
            *(page_ptr++) = i;
        }
        temp_ptr = temp_arr;
    }
    
    uint8_t i = 0;
    for(; i < size && (page_ptr != &cur_data.page[MEM_PAGE_SIZE - 2]); i++)
    {
        *(page_ptr++) = *(data_ptr++);
    }

    if (page_ptr == &cur_data.page[MEM_PAGE_SIZE - 3])
    {   // ToDo - write data to memory
        MEM_write_page();
        if (i != (size - 1))
        {   // ToDo - store data to temporary storage
            
        }
    }
}



uint8_t MEM_erase(uint8_t *addr, MemSize_t size) {return 1;}

uint8_t MEM_read_page(uint8_t *dst) {return 1;}

uint8_t MEM_write_page(void) 
{
    
    page_ptr = cur_data.page;  // reset page ptr
    return 1;
}

uint16_t crc16(uint8_t *src)
{
    return 1;
}