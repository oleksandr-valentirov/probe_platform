#include "gpio.h"


static void USART1_pins_init(void);


/**
 * @brief Настраивает пины, ориентируется на Pin_Configuration.h
 * @retval None
 */
void MyGPIO_Init(void)
{
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOA);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
#ifdef MCO1
    GPIO_InitTypeDef mco_1;
    mco_1.GPIO_Pin = MCO1_PIN;
    mco_1.GPIO_Speed = GPIO_Low_Speed;
    mco_1.GPIO_Mode = GPIO_Mode_AF;
    mco_1.GPIO_OType = GPIO_OType_PP;
    mco_1.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(MCO1_PORT, &mco_1);
    GPIO_PinAFConfig(MCO1_PORT, MCO1_AF_PIN, GPIO_AF_MCO);
#endif
    
    USART1_pins_init();
}


static void USART1_pins_init(void)
{
    GPIO_InitTypeDef pin;

    // TX pin
    pin.GPIO_OType = GPIO_OType_PP;
    pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
    pin.GPIO_Mode = GPIO_Mode_AF;
    pin.GPIO_Pin = USART_1_TX_PIN;
    pin.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(USART_1_TX_PORT, &pin);
    GPIO_PinAFConfig(USART_1_TX_PORT, USART_1_TX_AF_SRC, GPIO_AF_USART1);

    // RX pin
    pin.GPIO_Pin = USART_1_RX_PIN;
    GPIO_Init(USART_1_RX_PORT, &pin);
    GPIO_PinAFConfig(USART_1_RX_PORT, USART_1_RX_AF_SRC, GPIO_AF_USART1);
}