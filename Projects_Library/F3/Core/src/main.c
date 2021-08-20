/*
 * main.c
 *
 *  Created on: Apr 15, 2021
 *      Author: Aleks Valentirov
 */

#include "stm32f10x.h"
#include "my_system.h"
#include "spi.h"


void main(void){
  my_system_init();
  while(1){
    //delay_ms(250);
    //TOGGLE_BIT(GPIOC->ODR, GPIO_BSRR_BS13);
  }
}
