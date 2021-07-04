/**
  ******************************************************************************
  * @file    stm32l475xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32L475xx Device Peripheral Access Layer Header File.
  *
  *          This file contains:
  *           - Data structures and the address mapping for all peripherals
  *           - Peripheral's registers declarations and bits definition
  *           - Macros to access peripheral’s registers hardware
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Apache License, Version 2.0,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/Apache-2.0
  *
  ******************************************************************************
  */

#ifndef __RCC_H
#define __RCC_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stdint.h"
#include  "stm32l475xx.h"

#define 											TIM_COUNTERMODE_UP     0x00000000U      /*!< Counter used as up-counter   */
#define 											TIM_CHANNEL_1          0x00000000U      /*!< Capture/compare channel 1 identifier      */

#define 											UE_INTR_EN          	 0x00000001U      // Capture/compare channel 
#define 											CC1_INTR_EN            0x00000002U      // Capture/compare channel 


typedef struct
{
  uint32_t Prescaler;         			// Specifies the prescaler value used to divide the TIM clock.This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF 
  uint32_t CounterMode;       			// Specifies the counter mode.
  uint32_t Period;     							// Specifies the period value to be loaded into the active Auto-Reload Register at the next update event. 
																		// This parameter can be a value of @ref TIM_Output_Compare_and_PWM_modes 
  uint32_t PulseCC1;         					// Specifies the pulse value to be loaded into the Capture Compare Register.
																		// This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF 
	uint8_t  IntrptEn; 								// This bits defines if the interrupt is enabled on overflow
  uint32_t ClockDivision;     			// Specifies the clock division. This parameter can be a value of @ref TIM_ClockDivision */

  uint32_t RepetitionCounter;  /*!< Specifies the repetition counter value. Each time the RCR downcounter
                                    reaches zero, an update event is generated and counting restarts
                                    from the RCR value (N).
                                    This means in PWM mode that (N+1) corresponds to:
                                        - the number of PWM periods in edge-aligned mode
                                        - the number of half PWM period in center-aligned mode
                                     GP timers: this parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.
                                     Advanced timers: this parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  uint32_t AutoReloadPreload;  // Specifies the auto-reload preload.This parameter can be a value of @ref TIM_AutoReloadPreload */
} TIM_Base_config_init;


typedef struct
{
  uint32_t OCMode;        /*!< Specifies the TIM mode.
                               This parameter can be a value of @ref TIM_Output_Compare_and_PWM_modes */
  uint32_t Pulse;         /*!< Specifies the pulse value to be loaded into the Capture Compare Register.
                               This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF */
  uint32_t OCPolarity;    /*!< Specifies the output polarity.
                               This parameter can be a value of @ref TIM_Output_Compare_Polarity */
  uint32_t OCNPolarity;   /*!< Specifies the complementary output polarity.
                               This parameter can be a value of @ref TIM_Output_Compare_N_Polarity
                               @note This parameter is valid only for timer instances supporting break feature. */
  uint32_t OCFastMode;    /*!< Specifies the Fast mode state.
                               This parameter can be a value of @ref TIM_Output_Fast_State
                               @note This parameter is valid only in PWM1 and PWM2 mode. */
  uint32_t OCIdleState;   /*!< Specifies the TIM Output Compare pin state during Idle state.
                               This parameter can be a value of @ref TIM_Output_Compare_Idle_State
                               @note This parameter is valid only for timer instances supporting break feature. */
  uint32_t OCNIdleState;  /*!< Specifies the TIM Output Compare pin state during Idle state.
                               This parameter can be a value of @ref TIM_Output_Compare_N_Idle_State
                               @note This parameter is valid only for timer instances supporting break feature. */
} TIM_OC_config_init;


//function defines ----------------------------------------------------------------------------------------------------------------------------------------------
void TIM_base_config           	(TIM_TypeDef * TIMx, TIM_Base_config_init base_config);
void TIM_OC1_ConfigChannel			(TIM_TypeDef * TIMx,TIM_OC_config_init OC_config);
void TIM_CC1ChannelEnable				(TIM_TypeDef *TIMx, uint32_t Channel);						
void TIM_enable									(TIM_TypeDef * TIMx);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RCC_H */

/**
  * @}
  */

  /**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
