/*
 * my_system.c
 *
 *  Created on: Apr 15, 2021
 *      Author: user
 */

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "my_system.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "clock.h"
#include "timers.h"


static volatile uint32_t ticks_delay = 0;


void my_system_init(void){
  clock_init();
  gpio_init();
  usart1_init(115200);
  tim2_pwm_init();

  // -------- LOGGING WHICH NEEDS MINIMUM SETUP - CLOCKS AND UART
  #ifdef LOGGING
  RCC_ClocksTypeDef clocks;
  RCC_GetClocksFreq(&clocks);
  DBG_printf("System clock  - %d Hz\r\n", clocks.SYSCLK_Frequency);
  DBG_printf("AHB clock - %d Hz\r\n", clocks.HCLK_Frequency);
  DBG_printf("APB2 clock - %d Hz\r\n", clocks.PCLK2_Frequency);
  DBG_printf("APB1 clock - %d Hz\r\n", clocks.PCLK1_Frequency);
  DBG_printf("ADC  clock - %d Hz\r\n", clocks.ADCCLK_Frequency);
  DBG_printf("Port A - %s\r\n", (port_status & 1) ? "ON" : "OFF");
  DBG_printf("Port B - %s\r\n", (port_status & 2) ? "ON" : "OFF");
  DBG_printf("Port C - %s\r\n", (port_status & 4) ? "ON" : "OFF");
  DBG_printf("AFIO   - %s\r\n", (port_status & 8) ? "ON" : "OFF");
  #endif
  // all logging beyond this code snippet must be implemented on module level

  //	spi1_init(0x89);
  //	microsd_init();
}


// ---------------------------------- SysTick timer
void delay_ms(uint32_t milliseconds) {
  SysTick_Config(SystemCoreClock / 1000);

  uint32_t start = ticks_delay;
  while((ticks_delay - start) < milliseconds);
  ticks_delay = 0;
  turn_off_systick();
}

void delay_us(uint32_t microseconds) {
  SysTick_Config(SystemCoreClock / 1000000);

  uint32_t start = ticks_delay;
  while((ticks_delay - start) < microseconds);
  ticks_delay = 0;
  turn_off_systick();
}

void SysTick_Handler(void) {
  ticks_delay++;
}

void turn_off_systick(void) {
  CLEAR_BIT(SysTick->CTRL, (SysTick_CTRL_CLKSOURCE_Msk |
                            SysTick_CTRL_TICKINT_Msk   |
                            SysTick_CTRL_ENABLE_Msk)
  );
}
// -------------------------------------------------------
