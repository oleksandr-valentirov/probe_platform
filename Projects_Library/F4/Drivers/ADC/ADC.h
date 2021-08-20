/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H


#include "!Project_library.h"


void ADC1_Init(void);

uint8_t ADC_TempMeasure(uint16_t* res);
uint8_t ADC_VrefMeasure(uint16_t* res);
uint16_t get_calib_vref(void);

bool ADC1_Busy (void);

#endif /* __ADC_H */
