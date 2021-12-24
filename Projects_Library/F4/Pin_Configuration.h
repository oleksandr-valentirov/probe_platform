/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H


#include "!Project_library.h"

/* -------------------------------------------------------------------------- */
/* Альтернативные функции --------------------------------------------------- */

//#define MCO1
#define MCO1_PORT               GPIOA
#define MCO1_PIN                GPIO_Pin_8
#define MCO1_AF_PIN             GPIO_PinSource8


// SPI3
#define SPI3_MISO_AF            GPIO_PinSource4
#define SPI3_MOSI_AF            GPIO_PinSource5
#define SPI3_CLK_AF             GPIO_PinSource3


// USART 1
#define USART_1_TX_AF_SRC       GPIO_PinSource9
#define USART_1_RX_AF_SRC       GPIO_PinSource10
// USART 2
#define USART_2_TX_AF_SRC       GPIO_PinSource2
#define USART_2_RX_AF_SRC       GPIO_PinSource3
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/* Переферия ---------------------------------------------------------------- */

// SPI3
#define SPI3_PORT               GPIOB
#define SPI3_MISO_PIN           GPIO_Pin_4
#define SPI3_MOSI_PIN           GPIO_Pin_5
#define SPI3_CLK_PIN            GPIO_Pin_3


// USART 1
#define USART_1_PORT            GPIOA
#define USART_1_TX_PIN          GPIO_Pin_9
#define USART_1_RX_PIN          GPIO_Pin_10


// USART 2
#define USART_2_TX_PORT         GPIOA
#define USART_2_TX_PIN          GPIO_Pin_2

#define USART_2_RX_PORT         GPIOA
#define USART_2_RX_PIN          GPIO_Pin_3


// USB
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/* SIM800C ------------------------------------------------------------------ */
#define SIM_PWRKEY_PORT         GPIOA
#define SIM_PWRKEY_PIN          GPIO_Pin_11

#define SIM_STATUS_PORT         GPIOA
#define SIM_STATUS_PIN          GPIO_Pin_12
#define SIM_STATUS_EXTI_PORT    EXTI_PortSourceGPIOA
#define SIM_STATUS_EXTI_SRC     EXTI_PinSource12
#define SIM_STATUS_EXTI_LINE    EXTI_Line12

#define SIM_RI_PORT             GPIOA
#define SIM_RI_PIN              GPIO_Pin_15
#define SIM_RI_EXTI             EXTI_Line15

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/* LoRa --------------------------------------------------------------------- */
//#define LORA_TXEN_PORT          GPIOB
//#define LORA_TXEN_PIN           GPIO_Pin_8
//
//#define LORA_RXEN_PORT          GPIOB
//#define LORA_RXEN_PIN           GPIO_Pin_7
//
//#define LORA_DIO1_PORT          GPIOB
//#define LORA_DIO1_PIN           GPIO_Pin_12
//
//#define LORA_DIO2_PORT          GPIOB
//#define LORA_DIO2_PIN           GPIO_Pin_13
//
//#define LORA_BUSY_PORT          GPIOB
//#define LORA_BUSY_PIN           GPIO_Pin_15
//
//#define LORA_NRESET_PORT        GPIOB
//#define LORA_NRESET_PIN         GPIO_Pin_14
//
//#define LORA_SPI_NUMBER         SPI3
//#define LORA_SPI_CS_PORT        GPIOB
//#define LORA_SPI_CS_PIN         GPIO_Pin_6
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#endif /* __PIN_CONFIG_H */
