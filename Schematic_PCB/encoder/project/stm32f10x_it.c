/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

extern uint32_t encoder_pulse1, encoder_pulse2; 
extern uint32_t count_temp1 , count_temp2 , count_test;
extern uint32_t count_recent1 , count_recent2 , count_update1, count_update2;
extern float motor_speed1, motor_speed2;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
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

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
void  TIM4_IRQHandler(void)
	{
		if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update)==SET)
			{
				TIM_ClearFlag(TIM4, TIM_FLAG_Update);
				encoder_pulse1 =  500000000 + TIM_GetCounter(TIM2) + 65536*count_temp1;
				encoder_pulse2 =  500000000 + TIM_GetCounter(TIM3) + 65536*count_temp2;
				count_recent1 = encoder_pulse1;
				count_recent2 = encoder_pulse2;		
		if (count_recent1 > count_update1)
			{
				motor_speed1 = (float)(count_recent1 - count_update1)*6000/85;  //ngat 10ms , encoder 85 xung
			}
		else if (count_recent1 < count_update1)
			{
				motor_speed1 = 0- (float)(count_update1 - count_recent1)*6000/85;  //ngat 10ms , encoder 85 xung 
			}
		else {	motor_speed1 = 0;  }	
		
		/*calculate motor 2 speed*/
		if (count_recent2 > count_update2)
			{
				motor_speed2 = (float)(count_recent2 - count_update2)*6000/85;  //ngat 10ms , encoder 85 xung
			}
		else if (count_recent2 < count_update2)
			{
				motor_speed2 = 0- (float)(count_update2 - count_recent2)*6000/85;  //ngat 10ms , encoder 85xung	
			}
		else{	motor_speed2 = 0;  }
		/*update count 1 and count 2*/
		count_update1 = count_recent1;
		count_update2 = count_recent2;	
		}
}
	
void TIM2_IRQHandler(void)
{
	uint16_t timer_temp1;
	if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)==SET)
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		timer_temp1 = TIM2->CNT;
		if (timer_temp1 == 65535) count_temp1--	;
	  if (timer_temp1 == 0)			count_temp1++;
	}
}
	
	void TIM3_IRQHandler(void)
{
	uint16_t timer_temp2;
	if (TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)==SET)
	{
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		timer_temp2 = TIM3->CNT;
		if (timer_temp2 == 65535) count_temp2--	;
	  if (timer_temp2 == 0)			count_temp2++;
	}
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

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


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
