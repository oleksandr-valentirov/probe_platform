#include "AT25DF321A.h"


typedef struct
{
    size_t cur_offset;
    uint8_t flags;
} state;

static uint8_t flags = 0;


#define GetBusyFlag     READ_BIT(flags, MEM_FLAG_BUSY)
#define SetBusyFlag     SET_BIT(flags, MEM_FLAG_BUSY)
#define ClearBusyFlag   CLEAR_BIT(flags, MEM_FLAG_BUSY)

#define GetRDFlag       READ_BIT>flags, MEM_FLAG_RD)
#define SetRDFlag       SET_BIT(flags, MEM_FLAG_RD)
#define ClearRDFlag     CLEAR_BIT(flags, MEM_FLAG_RD)


/* Reading ------------------------------------------------------------------ */
static void MEM_SetReadBitCallback(void) {SetRDFlag;}

static void MEM_ClearReadBitCallback(void) {ClearRDFlag;}

uint8_t MEM_read_page(uint8_t *dst, MemSize_t size)
{
    if (GetBusyFlag) {return 1;}
    
    SetBusyFlag;
    SPI3_RegisterCallback(&MEM_SetReadBitCallback, SPI_Flash);
    return 0;
}
/* -------------------------------------------------------------------------- */


/* Writing ------------------------------------------------------------------ */
static void MEM_SetWriteBitCallback(void)
{
}

static void MEM_ClearWriteBitCallback(void)
{
}

uint8_t MEM_program_page(uint8_t *src)
{
    if (GetBusyFlag || !SPI3_IsFree()) {return 1;}
    
    SetBusyFlag;
    if (!SPI3_SetMutex(0, 0, SPI_Flash)) {return 1;}
    SPI3_RegisterCallback(&MEM_SetWriteBitCallback, SPI_Flash);
    return 0;
}
/* --------------------------------------------------------------------------d */