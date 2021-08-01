/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H


#include "!Project_library.h"


/* -------------------------------------------------------------------------- */
/* LoRa --------------------------------------------------------------------- */
#define TXEN
#define RXEN
#define DIO1
#define DIO2

#define BUSY_PORT
#define BUSY_PIN

#define NRESET_PORT
#define NRESET_PIN

#define SPI_NUMBER                SPI3
#define SPI_PORT                  GPIOB
#define SPI_SCK                   GPIO_Pin_3
#define SPI_MISO                  GPIO_Pin_4
#define SPI_MOSI                  GPIO_Pin_5

#define SPI_CS_PORT               GPIOB
#define SPI_CS_PIN                GPIO_Pin_6
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */




#endif /* __PIN_CONFIG_H */
