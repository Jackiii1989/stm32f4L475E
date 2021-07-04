*********************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_it.h"



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
  * @brief   This function WakeUP Alarm IRQ Handler. (Periodic Autowakeup)
  * @param  None
  * @retval None
  */
RTC_TimeFormat currentTime;
char data[64];

void RTC_WKUP_IRQHandler(void){

	if((RTC->ISR & RTC_ISR_WUTF) == RTC_ISR_WUTF){
		RTC_get_time(&currentTime);
		Convert2String(data,&currentTime);
		transmit(data);
		RTC->ISR &= ~RTC_ISR_WUTF;
		EXTI->PR1  &= ~EXTI_IMR1_IM20;
		
	}	
}

/**
  * @brief   This function handles NMI exception.
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
  HAL_IncTick();
}

/**
  * @brief  This function handles interrupts 10-15 pins  - EXTI15_10_IRQHandler
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)  /* PUSH_BUTTON_EXTI_IRQHandler */
{
	 // EXTI line interrupt detected 
   if((EXTI->PR1 & GPIO_PIN_13) != 0x00u)
  {
    USER_BUTTON_callback_function();
		EXTI->PR1 |= GPIO_PIN_13;
  }
}

/**
  * @brief  This function handles TIM3 Handler.
  * @param  None
  * @retval None
  */
__IO uint32_t Val2;
__IO uint32_t Val1;
void TIM2_IRQHandler(void)
{
		
	  if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF)
  {
		//Led_on(USER_LED1);
		//GPIO_pin_reset(PORT_GPIOA, GPIO_PIN_4);
		//Val2 = TIM3->CNT;
    TIM2->SR &= ~(TIM_SR_UIF);
  }else if((TIM2->SR & TIM_SR_CC1IF) == TIM_SR_CC1IF)
	{
		//Led_off(USER_LED1);
		//GPIO_pin_set(PORT_GPIOA, GPIO_PIN_4);
		//Val1 = TIM3->CNT;
		TIM2->SR &= ~(TIM_SR_CC1IF);
	} 
}


/**
  * @brief  This function handles interrupts for the  GPIO1 .
  * @param  None
  * @retval None
  */
/*
void EXTI1_IRQHandler(void)
{
 
	// EXTI line interrupt detected 
	if((EXTI->PR1 & GPIO_PIN_1) != 0x00u)
  {
		HAL_GPIO_EXTI_Callback(GPIO_Pin);
		EXTI->PR1 |= GPIO_PIN_1;
    
  }
}
*/

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xxxx.s).                                             */
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
