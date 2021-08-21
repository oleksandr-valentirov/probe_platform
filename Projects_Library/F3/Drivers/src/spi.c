/*
* spi.c
*
*  Created on: Apr 15, 2021
*      Author: Aleks Valentirov
*/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"


void spi1_init(uint8_t crc_polynomial){
  /* pins
  * SPI1_REMAP = 0 | A7 - MOSI; A6 - MISO; A5 - SCK; A4 	- NSS;
  * SPI1_REMAP = 0 | B5 - MOSI; B4 - MISO; B3 - SCK; A15 - NSS;
  */
  
  GPIO_InitTypeDef pin;
  pin.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
  pin.GPIO_Mode = GPIO_Mode_AF_PP;
  pin.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &pin);
  
  pin.GPIO_Pin = GPIO_Pin_4;
  pin.GPIO_Mode = GPIO_Mode_Out_PP;
  pin.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &pin);
  
  
  // peripheral
  SPI_InitTypeDef spi;
  spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi.SPI_Mode = SPI_Mode_Master;
  spi.SPI_NSS = SPI_NSS_Soft;
  spi.SPI_DataSize = SPI_DataSize_8b;
  spi.SPI_FirstBit = SPI_FirstBit_MSB;
  spi.SPI_CPOL = SPI_CPOL_Low;
  spi.SPI_CPHA = SPI_CPHA_1Edge;
  spi.SPI_CRCPolynomial = crc_polynomial;
  spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  SPI_Init(SPI1, &spi);
  SPI_SSOutputCmd(SPI1, DISABLE);
  //	if (crc_polynomial > 0){
  //		SPI_CalculateCRC(SPI1, ENABLE);
  //	}
  SPI_Cmd(SPI1, ENABLE);
}


void spi1_send_data(uint8_t* data, uint32_t len){
  CLEAR_BIT(GPIOA->ODR, GPIO_BSRR_BS4);
  while (len--){
    SPI_I2S_SendData(SPI1, *data++);
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
  }
  SET_BIT(GPIOA->ODR, GPIO_BSRR_BS4);
}
