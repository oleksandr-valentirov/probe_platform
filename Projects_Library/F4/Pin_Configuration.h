/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H


#include "!Project_library.h"

/* -------------------------------------------------------------------------- */
/* Alternate functions ------------------------------------------------------ */

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
// USART 6
#define USART_6_TX_AF_SRC       GPIO_PinSource6
#define USART_6_RX_AF_SRC       GPIO_PinSource7
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/* Peripherals -------------------------------------------------------------- */

/* SPI3 */
#define SPI3_PORT               GPIOC
#define SPI3_MISO_PIN           GPIO_Pin_11
#define SPI3_MOSI_PIN           GPIO_Pin_12
#define SPI3_CLK_PIN            GPIO_Pin_10


// USART 1
#define USART_1_PORT            GPIOB
#define USART_1_TX_PIN          GPIO_Pin_6
#define USART_1_RX_PIN          GPIO_Pin_7


/* USART 2 */
#define USART_2_PORT            GPIOA
#define USART_2_TX_PIN          GPIO_Pin_2
#define USART_2_RX_PIN          GPIO_Pin_3


/* USART 6 */
#define USART_6_PORT            GPIOC
#define USART_6_TX_PIN          GPIO_Pin_6
#define USART_6_RX_PIN          GPIO_Pin_7


/* -------------------------------------------------------------------------- */
/* SIM800C ------------------------------------------------------------------ */
#define SIM_PWRKEY_PORT         GPIOA
#define SIM_PWRKEY_PIN          GPIO_Pin_5

#define SIM_STATUS_PORT         GPIOA
#define SIM_STATUS_PIN          GPIO_Pin_4
#define SIM_STATUS_EXTI_PORT    EXTI_PortSourceGPIOa
#define SIM_STATUS_EXTI_SRC     EXTI_PinSource5
#define SIM_STATUS_EXTI_LINE    EXTI_Line5

#define SIM_RI_PORT             GPIOC
#define SIM_RI_PIN              GPIO_Pin_5
#define SIM_RI_EXTI             EXTI_Line5


/* -------------------------------------------------------------------------- */
/* GPS ---------------------------------------------------------------------- */
#define GPS_PORT                GPIOB
#define GPS_RESET_PIN           GPIO_Pin_3
#define GPS_EXTINT_PIN          GPIO_Pin_4


/* -------------------------------------------------------------------------- */
/* IMU ---------------------------------------------------------------------- */
#define IMU_INT_PORT            GPIOB
#define IMU_INT1_PIN            GPIO_Pin_9
#define IMU_INT2_PIN            GPIO_Pin_8

#define IMU_CS_PORT             GPIOA
#define IMU_CS_PIN              GPIO_Pin_15


/* -------------------------------------------------------------------------- */
/* USB ---------------------------------------------------------------------- */
#define USB_PORT                GPIOA
#define USB_DP_PIN              GPIO_Pin_12
#define USB_PM_PIN              GPIO_Pin_11
#define USB_VBUS_PIN            GPIO_Pin_9


/* -------------------------------------------------------------------------- */
/* Memory ------------------------------------------------------------------- */
#define MEM_SPI_ORDER           
#define MEM_CS_PORT             GPIOD
#define MEM_CS_PIN              GPIO_Pin_2


/* -------------------------------------------------------------------------- */
/* PWM ---------------------------------------------------------------------- */
#define PWM_PORT_0              GPIOA
#define PWM_P0_PIN_0            GPIO_Pin_6
#define PWM_P0_PIN_1            GPIO_Pin_7

#define PWM_PORT_1              GPIOB
#define PWM_P1_PIN_0            GPIO_Pin_0
#define PWM_P1_PIN_1            GPIO_Pin_1


/* -------------------------------------------------------------------------- */
/* Header ------------------------------------------------------------------- */
#define HEADER_PORT_0           GPIOC
#define HEADER_PORT_1           GPIOA
#define HEADER_P0_PIN_0         GPIO_Pin_3
#define HEADER_P1_PIN_0         GPIO_Pin_0
#define HEADER_P0_PIN_1         GPIO_Pin_1

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#endif /* __PIN_CONFIG_H */
