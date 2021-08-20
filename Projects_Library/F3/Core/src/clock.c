/*
 * clock.c
 *
 *  Created on: Apr 18, 2021
 *      Author: Aleks Valentirov
 */


#include "stm32f10x_rcc.h"


// prescalers
#define AHB		 	(1)
#define APB2		(2)	// high-speed bus
#define APB1		(2)	// low-speed bus
// ------------------------------------


void clock_init(void){
	/*
	 * reset everything in RCC system
	 * this mean that MCU switches to HSI
	 */
//	RCC_DeInit();


	SystemInit();				// resets and initializes system clock. needed freq is defined in a file which contains this func.
	SystemCoreClockUpdate();	// update SystemCoreClock variable

	// prescalers
	// AHB
	RCC_HCLKConfig(
		#if 1==AHB
			RCC_SYSCLK_Div1
		#elif 2==AHB
			RCC_SYSCLK_Div2
		#elif 4==AHB
			RCC_SYSCLK_Div4
		#elif 8==AHB
			RCC_SYSCLK_Div8
		#elif 16==AHB
			RCC_SYSCLK_Div16
		#elif 32==AHB
			RCC_SYSCLK_Div32
		#elif 64==AHB
			RCC_SYSCLK_Div64
		#elif 128==AHB
			RCC_SYSCLK_Div128
		#elif 256==AHB
			RCC_SYSCLK_Div256
		#elif 512==AHB
			RCC_SYSCLK_Div512
		#elif 0==AHB
			#warning "AHB prescaler is not defined. Prescaler by SystemInit is used"
		#else
			#error "Bad AHB prescaler"
		#endif
	);


	// APB2
	RCC_PCLK2Config(
		#if 1==APB2
			RCC_HCLK_Div1
		#elif 2==APB2
			RCC_HCLK_Div2
		#elif 4==APB2
			RCC_HCLK_Div4
		#elif 8==APB2
			RCC_HCLK_Div8
		#elif 16==APB2
			RCC_HCLK_Div16
		#elif 0==APB2
			#warning "APB2 prescaler is not defined. Prescaler by SystemInit is used"
		#else
			#error "Bad APB2 prescaler"
		#endif
	);

	// APB1
	RCC_PCLK1Config(
		#if 1==APB1
			RCC_HCLK_Div1
		#elif 2==APB1
			RCC_HCLK_Div2
		#elif 4==APB1
			RCC_HCLK_Div4
		#elif 8==APB1
			RCC_HCLK_Div8
		#elif 16==APB1
			RCC_HCLK_Div16
		#elif 0==APB1
			#warning "APB1 prescaler is not defined. Prescaler by SystemInit is used"
		#else
			#error "Bad APB1 prescaler"
		#endif
	);
}
