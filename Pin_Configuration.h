/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H


#include "!Project_library.h"

/* -------------------------------------------------------------------------- */
/* Альтернативные функции --------------------------------------------------- */

//#define MCO1
#define MCO1_PORT       GPIOA
#define MCO1_PIN        GPIO_Pin_8
#define MCO1_AF_PIN     GPIO_PinSource8


// SPI3


// USART 1


// USART 2

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/* Переферия ---------------------------------------------------------------- */

// SPI3
#define SPI3_PORT               GPIOB
#define SPI3_MISO_PIN           GPIO_Pin_4
#define SPI3_MOSI_PIN           GPIO_Pin_5
#define SPI3_CLK_PIN            GPIO_Pin_3

#define SPI3_CS_PORT            GPIOB
#define SPI3_CS_PIN             GPIO_Pin_6


// USART 1
#define USART_1_TX_PORT         GPIOA
#define USART_1_TX_PIN          GPIO_Pin_9
#define USART_1_TX_AF_SRC       GPIO_PinSource9

#define USART_1_RX_PORT         GPIOA
#define USART_1_RX_PIN          GPIO_Pin_10
#define USART_1_RX_AF_SRC       GPIO_PinSource10


// USART 2
#define USART_2_TX_PORT         GPIOA
#define USART_2_TX_PIN          GPIO_Pin_2
#define USART_2_TX_AF_SRC       GPIO_PinSource2

#define USART_2_RX_PORT         GPIOA
#define USART_2_RX_PIN          GPIO_Pin_3
#define USART_2_RX_AF_SRC       GPIO_PinSource3



// USB
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */



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
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#endif /* __PIN_CONFIG_H */
