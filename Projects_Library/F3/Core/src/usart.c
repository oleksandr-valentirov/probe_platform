/*
* usart.c
*
*  Created on: Apr 15, 2021
*      Author: user
*/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "my_system.h"	// for logging LOGGING
#include "usart.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>


volatile bool uart1_initialized = false;


volatile uint8_t USART1_REC_BUF[16];
volatile static uint8_t USART1_RES_BUF_POS = 0;
static const uint8_t USART1_BUF_MASK = 0xF;

// -------------------------------------------------------------------------------------------- //
// ----------------------------------- USART_1 CODE SPACE ------------------------------------- //
// -------------------------------------------------------------------------------------------- //

/*
* @param	baud	desired baud rate
*/
void usart1_init(uint32_t baud){
  GPIO_InitTypeDef pin;
  
  // TX pin
  pin.GPIO_Mode = GPIO_Mode_AF_PP;
  pin.GPIO_Pin = GPIO_Pin_9;
  pin.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &pin);
  
  // RX pin
  pin.GPIO_Mode = GPIO_Mode_IPU;
  pin.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &pin);
  
  USART_InitTypeDef usart;
  usart.USART_BaudRate = baud;
  usart.USART_WordLength = USART_WordLength_8b;
  usart.USART_StopBits = USART_StopBits_1;
  usart.USART_Parity = USART_Parity_No;
  usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_Init(USART1, &usart);
  USART_Cmd(USART1, ENABLE);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  NVIC_EnableIRQ(USART1_IRQn);
  uart1_initialized = true;
  
#ifdef LOGGING
  DBG_printf("%s | speed %d\r\n", "UART_1 OK", baud);
#endif
}

/*
* @param	data		data to transmit
* @param	data_size	data size in bytes
*/
int8_t usart1_send(uint8_t* data, size_t data_size)
{
  uint8_t* ptr = data;
  uint16_t counter = 0;
  while(data_size)
  {
    counter = 500;
    do {
      counter--;
    } while(!READ_BIT(USART1->SR, USART_SR_TXE) || counter > 0);
    
    if (!READ_BIT(USART1->SR, USART_SR_TXE) && counter == 0)
      return -1;
    
    USART1->DR = *ptr++;
    data_size--;
  }
  return data_size;
}


void DBG_printf(const char *fmt, ...)
{
  char buff[256];
  
  if (!uart1_initialized)
  {
    usart1_init(115200);
    uart1_initialized = true;
  }
  
  va_list args;
  va_start(args, fmt);
  vsnprintf(buff, sizeof(buff), fmt, args);
  
  usart1_send((uint8_t *)buff, strlen(buff));
  va_end(args);
}


void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE)){
    uint8_t received_symbol = USART1->DR;
    if (' ' <= received_symbol && received_symbol <= '~'){
      // move ASCII symbol to the receive buffer
      USART1_REC_BUF[USART1_RES_BUF_POS] = received_symbol;
      USART1_RES_BUF_POS += 1;
      USART1_RES_BUF_POS &= USART1_BUF_MASK;
    } else if(received_symbol == '\n') {
      // command is ready to process
    }
  } else if(USART_GetITStatus(USART1, USART_IT_TXE)){
    
  }
}
