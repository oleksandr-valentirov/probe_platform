#include "mpu9250.h"
#include "usbd_cdc_if.h"


enum mpu9250_addresses
{
    SMPLRT_DIV          = 0x19,
    CONFIG              = 0x1A,
    GYRO_CONFIG         = 0x1C,
    ACCEL_CONFIG        = 0x1C,
    ACCEL_CONFIG_2      = 0x1D,
    ACCEL_XOUT_H_REG    = 0x3B,
    TEMP_OUT_H          = 0x41,
    GYRO_XOUT_H         = 0x43,
    USER_CTRL           = 0x6A,
    PWR_MGMT_1          = 0x6B,
    PWR_MGMT_2          = 0x6C,
    WHO_AM_I            = 0x75,
};


static uint8_t tx[19];
static uint8_t rx[19];

static float temperature;
uint8_t data_ready_flag = 0;


static uint8_t IMU_poll_ID(void);
static void SPI_callback(void);
static void IMU_SensorMain(int* ax, int* ay, int* az, int* gx, int* gy, int* gz);
static void IMU_read_data(void);
static void init_magnitometer(void);
static void init_accelerometer(void);
static void init_gyroscope(void);
static void init_interface(void);


void* IMU_SensorInit(void)
{
    uint8_t ID = IMU_poll_ID();
    if(ID)
    {
        init_interface();
        init_gyroscope();
        init_accelerometer();
        return (void*)IMU_SensorMain;
    }
    return NULL;
}

static void IMU_SensorMain(int* ax, int* ay, int* az, int* gx, int* gy, int* gz)
{
    char accel_res[19] = {0};

    if(SysTick_GetIMUFlag())
    {
        IMU_read_data();
    }
    if(data_ready_flag)
    {
        *ax = ((rx[1] << 8) | rx[2]) / 8192.0;
        *ay = ((rx[3] << 8) | rx[4]) / 8192.0;
        *az = ((rx[5] << 8) | rx[6]) / 8192.0;
        temperature = (((rx[7] << 8 | rx[8]) - 21) / 333.87) + 21;
        *gx = (rx[9] << 8 | rx[10]) / 131.0;
        *gy = (rx[11] << 8 | rx[12]) / 131.0;
        *gz = (rx[13] << 8 | rx[14]) / 131.0;

        data_ready_flag = 0;
    }
}


/* ----------- init ----------- */
static void init_magnitometer(void)
{
}

static void init_accelerometer(void)
{
    uint8_t data[2] = {PWR_MGMT_1, 0x81};
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI3_WriteData_Pol(data, 2);
    SPI3_ReadData_Pol(data, 2);
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    
    data[0] = PWR_MGMT_2;
    data[1] = 0;
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI3_WriteData_Pol(data, 2);
    SPI3_ReadData_Pol(data, 2);
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    
    data[0] = ACCEL_CONFIG;
    data[1] = 0x08;
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI3_WriteData_Pol(data, 2);
    SPI3_ReadData_Pol(data, 2);
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    
    data[0] = ACCEL_CONFIG_2;
    data[1] = 0x05;
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI3_WriteData_Pol(data, 2);
    SPI3_ReadData_Pol(data, 2);
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    
    data[0] = CONFIG;
    data[1] = 0x05;
    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
    SPI3_WriteData_Pol(data, 2);
    SPI3_ReadData_Pol(data, 2);
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    
    /* check. an output should be 3 */
//    data[0] = ACCEL_CONFIG_2;
//    data[1] = 0;
//    GPIO_ResetBits(IMU_CS_PORT, IMU_CS_PIN);    // Set CS
//    SPI3_WriteData_Pol(data, 2);
//    SPI3_ReadData_Pol(data, 2);
//    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
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
    
    /* check. an output data[1] should be 2*/
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

void IMU_poll_accel(int* ax, int* ay, int* az)
{
}

static void SPI_callback(void)
{
    GPIO_SetBits(IMU_CS_PORT, IMU_CS_PIN);      // Reset CS
    data_ready_flag = 1;
}