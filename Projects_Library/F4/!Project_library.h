/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PROJECT_LIBRARY_H
#define __PROJECT_LIBRARY_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/* Drivers -------------------------------------------------------------------*/
#include "ADC.h"
#include "USART2.h"
#include "USART1.h"
#include "SPI.h"
#include "clock.h"
#include "gpio.h"
#include "timers.h"
//#include "onewire.h"
#include "i2c.h"
#include "systick.h"


/* Modules -------------------------------------------------------------------*/
//#include "sx126x.h"
#include "temp_api.h"
#include "sim.h"
#include "gps.h"


/* Exported types ------------------------------------------------------------*/
#define weak    __weak


/* Exported constants --------------------------------------------------------*/
#include "Pin_Configuration.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//void TimingDelay_Decrement(void);

#endif /* __PROJECT_LIBRARY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
