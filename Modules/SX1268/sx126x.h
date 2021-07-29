/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SX1268_H
#define __SX1268_H

#include "!Project_library.h"


typedef struct {
    // GPS data
    int16_t x;
    int16_t y;
    int16_t z;
    
    // ms5803 data
    uint16_t pressure;
    
    // magnitometer data
    int16_t field_x;
    int16_t field_y;
    int16_t field_z;
} telemetry;


typedef struct {
    int8_t mcu_temperature;
    int8_t ms5803_temperature;
} temp_stats;


typedef struct {
    uint8_t device_id;
    uint8_t packet_num;
    
} packet;


#endif