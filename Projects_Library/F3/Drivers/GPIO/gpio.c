#include "gpio.h"


static void USART_12_pins_init(void);
static void SPI_pins_init(void);


/**
 * @brief Настраивает пины, ориентируется на Pin_Configuration.h
 * @retval None
 */
void MyGPIO_Init(void)
{
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOA);
    RCC_APB2PeriphClockCmd(
//                           RCC_APB2Periph_GPIOC | 
//                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_AFIO, ENABLE);
    
#ifdef MCO
#endif
    
    OneWire_Write_Pin_Init();
    USART_12_pins_init();
    SPI_pins_init();
    
#ifdef __SX1268_H
    LoRa_PinsInit();
#endif
}


static void USART_12_pins_init(void)
{
//    GPIO_InitTypeDef pin;
//
//    // USART1 TX pin
//    pin.GPIO_OType = GPIO_OType_PP;
//    pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    pin.GPIO_Mode = GPIO_Mode_AF;
//    pin.GPIO_Pin = USART_1_TX_PIN;
//    pin.GPIO_Speed = GPIO_High_Speed;
//    GPIO_Init(USART_1_TX_PORT, &pin);
//    GPIO_PinAFConfig(USART_1_TX_PORT, USART_1_TX_AF_SRC, GPIO_AF_USART1);
//
//    // USART1 RX pin
//    pin.GPIO_Pin = USART_1_RX_PIN;
//    GPIO_Init(USART_1_RX_PORT, &pin);
//    GPIO_PinAFConfig(USART_1_RX_PORT, USART_1_RX_AF_SRC, GPIO_AF_USART1);
//    
//    // USART2 TX pin
//    pin.GPIO_Pin = USART_2_TX_PIN;
//    GPIO_Init(USART_2_TX_PORT, &pin);
//    GPIO_PinAFConfig(USART_2_TX_PORT, USART_2_TX_AF_SRC, GPIO_AF_USART2);
//    
//    // USART2 RX pin
//    pin.GPIO_Pin = USART_2_RX_PIN;
//    GPIO_Init(USART_2_RX_PORT, &pin);
//    GPIO_PinAFConfig(USART_2_RX_PORT, USART_2_TX_AF_SRC, GPIO_AF_USART2);
}


static void SPI_pins_init(void)
{
//    GPIO_InitTypeDef pin;
//    
//    // SPI3
//    pin.GPIO_OType = GPIO_OType_PP;
//    pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    pin.GPIO_Mode = GPIO_Mode_AF;
//    pin.GPIO_Pin = SPI3_MISO_PIN | SPI3_MOSI_PIN | SPI3_CLK_PIN;
//    pin.GPIO_Speed = GPIO_High_Speed;
//    GPIO_Init(SPI3_PORT, &pin);
//    
//    // SPI3 AF
//    GPIO_PinAFConfig(SPI3_PORT, SPI3_MISO_AF, GPIO_AF_SPI3);
//    GPIO_PinAFConfig(SPI3_PORT, SPI3_MOSI_AF, GPIO_AF_SPI3);
//    GPIO_PinAFConfig(SPI3_PORT, SPI3_CLK_AF, GPIO_AF_SPI3);
}


