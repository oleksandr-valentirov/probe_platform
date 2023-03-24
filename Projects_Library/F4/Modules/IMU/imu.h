#pragma once

#include "!Project_library.h"


<<<<<<< Updated upstream
#define FUNC_CFG_ACCESS                 0x01
#define PIN_CTRL                        0x02
#define FIFO_CTRL1                      0x07
#define FIFO_CTRL2                      0x08
#define FIFO_CTRL3                      0x09
#define FIFO_CTRL4                      0x0A
#define COUNTER_BDR_REG1                0x0B
#define COUNTER_BDR_REG2                0x0C
#define INT1_CTRL                       0x0D
#define INT2_CTRL                       0x0E
#define WHO_AM_I                        0x0F
#define CTRL1_XL                        0x10
#define CTRL2_G                         0x11
#define CTRL3_C                         0x12
#define CTRL4_C                         0x13
#define CTRL5_C                         0x14
#define CTRL6_C                         0x15
#define CTRL7_G                         0x16
#define CTRL8_XL                        0x17
#define CTRL9_XL                        0x18
#define CTRL10_C                        0x19
#define ALL_INT_SRC                     0x1A
#define WAKE_UP_SRC                     0x1B
#define TAP_SRC                         0x1C
#define D6D_SRC                         0x1D
#define STATUS_REG                      0x1E
#define OUT_TEMP_L                      0x20
#define OUT_TEMP_H                      0x21
#define OUTX_L_G                        0x22
#define OUTX_H_G                        0x23
#define OUTY_L_G                        0x24
#define OUTY_H_G                        0x25
#define OUTZ_L_G                        0x26
#define OUTZ_H_G                        0x27
#define OUTX_L_A                        0x28
#define OUTX_H_A                        0x29
#define OUTY_L_A                        0x2A
#define OUTY_H_A                        0x2B
#define OUTZ_L_A                        0x2C
#define OUTZ_H_A                        0x2D
#define EMB_FUNC_STATUS_MAINPAGE        0x35
#define FSM_STATUS_A_MAINPAGE           0x36
#define FSM_STATUS_B_MAINPAGE           0x37
#define MLC_STATUS_MAINPAGE             0x38
#define STATUS_MASTER_MAINPAGE          0x39
#define FIFO_STATUS1                    0x3A
#define FIFO_STATUS2                    0x3B
#define TIMESTAMP0                      0x40
#define TIMESTAMP1                      0x41
#define TIMESTAMP2                      0x42
#define TIMESTAMP3                      0x43
#define TAP_CFG0                        0x56
#define TAP_CFG1                        0x57
#define TAP_CFG2                        0x58
#define TAP_THS_6D                      0x59
#define INT_DUR2                        0x5A
#define WAKE_UP_THS                     0x5B
#define WAKE_UP_DUR                     0x5C
#define FREE_FALL                       0x5D
#define MD1_CFG                         0x5E
#define MD2_CFG                         0x5F
#define INT_OIS                         0x6F
#define CTRL1_OIS                       0x70
#define CTRL2_OIS                       0x71
#define CTRL3_OIS                       0x72
#define X_OFS_USR                       0x73
#define Y_OFS_USR                       0x74
#define Z_OFS_USR                       0x75
#define FIFO_DATA_OUT_TAG               0x78
#define FIFO_DATA_OUT_X_L               0x79
#define FIFO_DATA_OUT_X_H               0x7A
#define FIFO_DATA_OUT_Y_L               0x7B
#define FIFO_DATA_OUT_Y_H               0x7C
#define FIFO_DATA_OUT_Z_L               0x7D
#define FIFO_DATA_OUT_Z_H               0x7E


void IMU_Main(void);
=======
__weak void init_termometer(void);
__weak void init_barometer(void);
__weak void init_magnitometer(void);
__weak void init_accelerometer(void);
__weak void init_gyroscope(void);
>>>>>>> Stashed changes
