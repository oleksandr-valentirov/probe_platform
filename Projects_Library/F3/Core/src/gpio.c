/*
* gpio.c
*
*  Created on: Apr 15, 2021
*      Author: Aleks Valentirov
*/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


#define PORTA_ENABLE
//#define PORTB_ENABLE
#define PORTC_ENABLE
#define AFIO_ENABLE


volatile uint8_t port_status = 0;


void gpio_init(void){
  
#ifdef PORTA_ENABLE
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// port A clock enable
  port_status |= 1;
#endif
  
#ifdef PORTB_ENABLE
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// port B clock enable
  port_status |= 2;
#endif
  
#ifdef PORTC_ENABLE
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	// port C clock enable
  port_status |= 4;
#endif
  
#ifdef AFIO_ENABLE
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// AF clock enable
  port_status |= 8;
#endif
  
  GPIO_InitTypeDef pc_13;
  pc_13.GPIO_Pin = GPIO_Pin_13;
  pc_13.GPIO_Mode = GPIO_Mode_Out_PP;
  pc_13.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &pc_13);				// pin 13 config
  
}
