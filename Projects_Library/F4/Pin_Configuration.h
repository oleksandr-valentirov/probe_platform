/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H


#include "!Project_library.h"

/* -------------------------------------------------------------------------- */
/* Альтернативные функции --------------------------------------------------- */

// SPI3
#define SPI3_MISO_AF            GPIO_PinSource4
#define SPI3_MOSI_AF            GPIO_PinSource5
#define SPI3_CLK_AF             GPIO_PinSource3


// USART 1
#define USART_1_TX_AF_SRC       GPIO_PinSource6
#define USART_1_RX_AF_SRC       GPIO_PinSource7
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
#define USART_1_PORT            GPIOB
#define USART_1_TX_PIN          GPIO_Pin_6
#define USART_1_RX_PIN          GPIO_Pin_7


// USART 2
#define USART_2_PORT            GPIOA
#define USART_2_TX_PIN          GPIO_Pin_2
#define USART_2_RX_PIN          GPIO_Pin_3


/* -------------------------------------------------------------------------- */
/* SIM800C ------------------------------------------------------------------ */
#define SIM_PWRKEY_PORT         GPIOA
#define SIM_PWRKEY_PIN          GPIO_Pin_6

#define SIM_STATUS_PORT         GPIOA
#define SIM_STATUS_PIN          GPIO_Pin_5
#define SIM_STATUS_EXTI_PORT    EXTI_PortSourceGPIOa
#define SIM_STATUS_EXTI_SRC     EXTI_PinSource5
#define SIM_STATUS_EXTI_LINE    EXTI_Line5

#define SIM_RI_PORT             GPIOA
#define SIM_RI_PIN              GPIO_Pin_7
#define SIM_RI_EXTI             EXTI_Line7

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/* GPS ---------------------------------------------------------------------- */
#define GPS_RESET_PORT          GPIOA
#define GPS_RESET_PIN           GPIO_Pin_4

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/* IMU ---------------------------------------------------------------------- */
#define IMU_INT_PORT            GPIOB
#define IMU_INT1_PIN            GPIO_Pin_9
#define IMU_INT2_PIN            GPIO_Pin_8

#define IMU_CS_PORT             GPIOA
#define IMU_CS_PIN              GPIO_Pin_14

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/* USB ---------------------------------------------------------------------- */
#define USB_PORT                GPIOA
#define USB_DP_PIN              GPIO_Pin_12
#define USB_PM_PIN              GPIO_Pin_11

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#endif /* __PIN_CONFIG_H */
