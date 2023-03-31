#include "imu.h"

static uint8_t rx_buf[64];
static uint8_t tx_buf[64];

static void IMU_read(uint8_t addr, uint8_t size);
static void IMU_write(uint8_t addr, uint8_t size);



__weak void* IMU_Init(void)
{
    return NULL;
}


static void IMU_read(uint8_t addr, uint8_t size)
{
}

static void IMU_write(uint8_t addr, uint8_t size)
{
}

/* ----------- init ----------- */
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
}

/* ----------- read ----------- */
__weak void read_gyroscope(void)
{
}

__weak void read_accelerometer(void)
{
}

__weak void read_magnitometer(void)
{
}

__weak void read_barometer(void)
{
}

__weak void read_termometer(void)
{
}
