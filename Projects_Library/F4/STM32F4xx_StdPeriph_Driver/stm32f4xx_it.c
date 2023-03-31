/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
void OTG_FS_IRQHandler(void)
{
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}


void USART1_IRQHandler(void)
{
//    if(USART_GetITStatus(USART1, USART_IT_IDLE))
    {        
        DMA_USART1inTransferStop();
        UBX_ResetFlagMsgRx();
        USART1->SR;
        USART1->DR;
    }
}

void USART2_IRQHandler(void)
{
    if(READ_BIT(USART2->SR, USART_FLAG_RXNE))
    {
        USART2_putc(USART2->DR);
    }
    if(READ_BIT(USART2->SR, USART_FLAG_TC))
    {
        USART_ClearITPendingBit(USART2, USART_IT_TC);
        USART2_Transmit_Next_Byte();
    }
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(SIM_RI_EXTI))
    {
        EXTI_ClearITPendingBit(SIM_RI_EXTI);
        Sim_RIEventStart();
    }
}

void DMA2_Stream7_IRQHandler(void)
{
    UBX_SetFlagMsgTx();
    DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
}

void DMA1_Stream0_IRQHandler(void)
{       
    if(DMA_GetFlagStatus(DMA1_Stream0, DMA_FLAG_TCIF7) && !DMA_GetCmdStatus(DMA1_Stream5))
    {
        DMA_op_end_callback_decorator();
    }
    DMA_ClearFlag(DMA1_Stream0, DMA_IT_TCIF0);
    DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
}

void DMA1_Stream5_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_Stream5, DMA_FLAG_TCIF5))
    {
        DMA_op_end_callback_decorator();
    }
    DMA_ClearFlag(DMA1_Stream5, DMA_IT_TCIF5);
    DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
