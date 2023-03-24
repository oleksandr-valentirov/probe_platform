#include "imu.h"

<<<<<<< Updated upstream
static uint8_t rx_buf[64];
static uint8_t tx_buf[64];

static void IMU_read(uint8_t addr, uint8_t size);
static void IMU_write(uint8_t addr, uint8_t size);



void IMU_Main(void)
{
}


static void IMU_read(uint8_t addr, uint8_t size)
{
    DMA_SPI3inTransaction(&rx_buf, size);
}

static void IMU_write(uint8_t addr, uint8_t size)
{
    DMA_SPI3outTransaction(&tx_buf, size);
=======

__weak void init_gyroscope(void)
{
}

__weak void init_accelerometer(void)
{
}

__weak void init_magnitometer(void)
{
}

__weak void init_barometer(void)
{
}

__weak void init_termometer(void)
{
>>>>>>> Stashed changes
}