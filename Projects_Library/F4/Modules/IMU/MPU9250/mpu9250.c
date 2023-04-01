#include "mpu9250.h"


enum mpu9250_addresses
{
    ACCEL_CONFIG        = 0x1C,
    ACCEL_CONFIG_2      = 0x1D,
    ACCEL_XOUT_H_REG    = 0x3B,
    TEMP_OUT_H          = 0x41,
    GYRO_XOUT_H         = 0x43,
    USER_CTRL           = 0x6A,
    WHO_AM_I            = 0x75,
};


static uint8_t tx[19];
static uint8_t rx[19];

static float ax, ay, az, gx, gy, gz, temperature;
uint8_t data_ready_flag = 0;


static uint8_t IMU_poll_ID(void);
static void SPI_callback(void);
static void IMU_Main(void);
static void IMU_read_data(void);
static void init_magnitometer(void);
static void init_accelerometer(void);
static void init_gyroscope(void);
static void init_interface(void);


void* IMU_Init(void)
{
    uint8_t ID = IMU_poll_ID();
    if(ID)
    {
        init_interface();
        init_gyroscope();
        init_accelerometer();
        return (void*)IMU_Main;
    }
    return NULL;
}

static void IMU_Main(void)
{
    if(SysTick_GetIMUFlag())
    {
        IMU_read_data();
    }
    if(data_ready_flag)
    {
        ax = (rx[1] << 8 | rx[2]) / 16384.0;
        ay = (rx[3] << 8 | rx[4]) / 16384.0;
        az = (rx[5] << 8 | rx[6]) / 16384.0;
        temperature = (((rx[7] << 8 | rx[8]) - 21) / 333.87) + 21;
        gx = (rx[9] << 8 | rx[10]) / 16384.0;
        gy = (rx[11] << 8 | rx[12]) / 16384.0;
        gz = (rx[13] << 8 | rx[14]) / 16384.0;
        data_ready_flag = 0;
    }
}


/* ----------- init ----------- */
static void init_magnitometer(void)
{
}

static void init_accelerometer(void)
{
}

static void init_gyroscope(void)
{
}

static void init_interface(void)
{
    uint8_t data[2] = {USER_CTRL | 0x80, 0};
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI3_WriteData_Pol(data, 2);
    SPI3_ReadData_Pol(data, 2);
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    
    /* disable I2C */
    data[0] = USER_CTRL;
    SET_BIT(data[1], 0x10);
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI3_WriteData_Pol(data, 2);
    SPI3_ReadData_Pol(data, 2);
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    
    /* check an output data[1] should be 2*/
//    data[0] = USER_CTRL | 0x80;
//    data[1] = 0;
//    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
//    SPI3_WriteData_Pol(data, 2);
//    SPI3_ReadData_Pol(data, 2);
//    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
}

/* ----------- read ----------- */
void read_magnitometer(void)
{
}

static void IMU_read_data(void)
{
    for(uint8_t i = 0; i < 16; i++){tx[i] = 0; rx[i] = 0;}
    tx[0] = ACCEL_XOUT_H_REG | 0x80;
    
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI_StartOp_DMA(tx, 16, rx, 16, SPI_callback);
}

static uint8_t IMU_poll_ID(void)
{
    uint8_t data[2] = {WHO_AM_I | 0x80, 0};
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI3_WriteData_Pol(data, 2);
    SPI3_ReadData_Pol(data, 2);
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    return data[1];
}

static void SPI_callback(void)
{
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    data_ready_flag = 1;
}