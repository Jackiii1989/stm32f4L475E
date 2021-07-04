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

#define HSI_VALUE    										16000000U /*!< Value of the Internal oscillator in Hz*/
#define HSE_VALUE   										8000000U /*!< Value of the External oscillator in Hz */
#define MSI_VALUE    										4000000U /*!< Value of the Internal oscillator in Hz*/
#define LSI_VALUE  											32000U                 /*!< LSI Typical Value in Hz*/

#define  TICK_INT_PRIO            		 0x0FU // tick interrupt priority
#define  SYS_TICK_FREQ_1KHZ 					 1000
#define  PLL_TIMEOUT_VAL               2U    // 2 ms (minimum Tick + 1)
#define CLOCKSWITCH_TIMEOUT_VALUE  		 5000U // 5 s 
	
	
#define RCC_MSICALIBRATION_DEFAULT     0U            /*!< Default MSI calibration trimming value */
#define RCC_PLL_NONE                   0x00000000U   /*!< PLL configuration unchanged */
#define RCC_PLL_OFF                    0x00000001U   /*!< PLL deactivation */
#define RCC_PLL_ON                     0x00000002U   /*!< PLL activation */

#define RCC_PLLSOURCE_NONE             0x00000000U             /*!< No clock selected as PLL entry clock source  */
#define RCC_PLLSOURCE_MSI              RCC_PLLCFGR_PLLSRC_MSI  /*!< MSI clock selected as PLL entry clock source */
#define RCC_PLLSOURCE_HSI              RCC_PLLCFGR_PLLSRC_HSI  /*!< HSI clock selected as PLL entry clock source */
#define RCC_PLLSOURCE_HSE              RCC_PLLCFGR_PLLSRC_HSE  /*!< HSE clock selected as PLL entry clock source */


static uint32_t 		System_core_clock 	 = 4000000;
static uint32_t     _uwTickPrio          = (1UL << __NVIC_PRIO_BITS); /* Invalid priority */
//TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */
static const uint8_t  AHBPrescTable[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static const uint8_t  APBPrescTable[8]   = {0, 0, 0, 0, 1, 2, 3, 4};
static const uint32_t MSIRangeTable[12]  = {100000, 200000, 400000, 800000, 1000000, 2000000, \
                                            4000000, 8000000, 16000000, 24000000, 32000000, 48000000};

//RCC defines ----------------------------------------------------------------------------------------------------------------------------------------------
uint32_t 			GetSysClockFreq_RCC		(void);
uint32_t 			get_SystemClock				(void);
void 					delay_us							(uint32_t n);
void 					delay_ms							(uint32_t Delay);
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
