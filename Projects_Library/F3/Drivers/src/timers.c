/*
* timers.c
*
*  Created on: May 2, 2021
*      Author: user
*/


#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "my_system.h"
#include "usart.h"

#include <stdbool.h>


volatile bool tim2_initialized = false;
volatile uint8_t tim2_count = 0;


void tim2_pwm_init(void){
  // turn on clock signal
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  // reset timer
  TIM_Cmd(TIM2, DISABLE);
  TIM_DeInit(TIM2);
  
  
  // get APB1 clock to config prescaler
  RCC_ClocksTypeDef clocks;
  RCC_GetClocksFreq(&clocks);
  
  TIM_TimeBaseInitTypeDef tim2;
  tim2.TIM_Period = 0xFFFF - 1019;
  tim2.TIM_CounterMode = TIM_CounterMode_Down;
  tim2.TIM_Prescaler = 279;
  
  TIM_TimeBaseInit(TIM2, &tim2);
  tim2_initialized = true;
  
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  
//  NVIC_InitTypeDef nvic;
//  nvic.NVIC_IRQChannel = TIM2_IRQn;
//  nvic.NVIC_IRQChannelPreemptionPriority = 0;
//  nvic.NVIC_IRQChannelSubPriority = 0;
//  nvic.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&nvic);
  NVIC_EnableIRQ(TIM2_IRQn);
  
#ifdef LOGGING
  DBG_printf("%s\r\n", "TIM2 is OK");
#endif
}


void tim2_disable(void){
  TIM_Cmd(TIM2, DISABLE);
  TIM_DeInit(TIM2);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}

void TIM2_IRQHandler(void) {
  tim2_count++;
  TOGGLE_BIT(GPIOC->ODR, GPIO_BSRR_BS13);
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
