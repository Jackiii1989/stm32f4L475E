#include "tim.h"
#include "stdint.h"
/**
  \brief   			 Timer basic configuration 
  \details 
  \param [in]    The timer instance, which to write to and config structure need to configure the timer. 
 */
 
void TIM_base_config(TIM_TypeDef * TIMx, TIM_Base_config_init base_config){

	uint32_t temp_dier;
  // Select the Counter Mode
	// TIM_CR1_DIR_Msk: 0 for upconting,TIM_CR1_CMS: 0 for edge aligned, TIM_SMCR_SMS_Msk: 0 slave mode reset		 
	TIMx->CR1 &=~ (TIM_CR1_DIR_Msk | TIM_CR1_CMS_Msk | TIM_SMCR_SMS_Msk); 																													
  TIMx->CR1 |= base_config.CounterMode;
  
  // Set the clock division 
  TIMx->CR1 &= ~TIM_CR1_CKD;
  TIMx->CR1 |= (uint32_t)base_config.ClockDivision;
  
  // Set the Autoreload value 
  TIMx->ARR = (uint32_t)base_config.Period;

	  // Set the Autoreload value 
  TIMx->CCR1 = (uint32_t)base_config.PulseCC1;
	
  // Set the Prescaler value 
  TIMx->PSC = base_config.Prescaler;
	
	
	// configuration the interrupt on the timer overflow
	temp_dier = TIMx->DIER;
	if(base_config.IntrptEn & UE_INTR_EN){	
		temp_dier |= TIM_DIER_UIE ;
	}
	if(base_config.IntrptEn & CC1_INTR_EN){
		temp_dier |= TIM_DIER_CC1IE;
	}	
	TIMx->DIER = temp_dier;
}

void TIM_OC1_ConfigChannel (TIM_TypeDef * TIMx,TIM_OC_config_init OC_config){

	// Set the Output Polarity and N Polarity level */
  TIMx->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
  TIMx->CCER |= (OC_config.OCPolarity | OC_config.OCNPolarity);

  // Set the Capture Compare Register value
  TIMx->CCR1 = OC_config.Pulse;
	
	// Set the Output Compare Mode Bits */
  TIMx->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_CC1S);
  TIMx->CCMR1  |= OC_config.OCMode;
	//TIMx->CCMR2  |= ;
	
	// enable the  compare output channel 1 
	TIMx->CCER |= TIM_CCER_CC1E;
}



void TIM_enable	(TIM_TypeDef * TIMx){
	TIMx->CR1 |= TIM_CR1_CEN;
}
