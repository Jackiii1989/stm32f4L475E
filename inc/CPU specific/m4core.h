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

#ifndef __COREM4_H
#define __COREM4_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//#include "stdint.h"
#include  "stm32l475xx.h"
#include  "nvic.h"


//********************************************************************************************************************************************************************************
//stm 32 cortex M4 < -------------------------------------------------------------------------------------------------------------------------------------------------------------
//********************************************************************************************************************************************************************************
/* STK Control / Status Register Definitions */
#define STK_CTRL_COUNTFLAG_Pos         16U                                            /*!< STK CTRL: COUNTFLAG Position */
#define STK_CTRL_COUNTFLAG_Msk         (1UL << STK_CTRL_COUNTFLAG_Pos)                /*!< STK CTRL: COUNTFLAG Mask */              

#define STK_CTRL_CLKSOURCE_Pos          2U                                            /*!< STK CTRL: CLKSOURCE Position */
#define STK_CTRL_CLKSOURCE_Msk         (1UL << STK_CTRL_CLKSOURCE_Pos)                /*!< STK CTRL: CLKSOURCE Mask */
#define STK_CTRL_CLKSOURCE  					  STK_CTRL_CLKSOURCE_Msk

#define STK_CTRL_TICKINT_Pos            1U                                            /*!< STK CTRL: TICKINT Position */
#define STK_CTRL_TICKINT_Msk           (1UL << STK_CTRL_TICKINT_Pos)                  /*!< STK CTRL: TICKINT Mask */
#define STK_CTRL_TICKINT           			STK_CTRL_TICKINT_Msk 

#define STK_CTRL_ENABLE_Pos             0U                                            /*!< STK CTRL: ENABLE Position */
#define STK_CTRL_ENABLE_Msk            (1UL /*<< STK_CTRL_ENABLE_Pos*/)               /*!< STK CTRL: ENABLE Mask */
#define STK_CTRL_ENABLE									STK_CTRL_ENABLE_Msk

/* STK Reload Register Definitions */
#define STK_LOAD_RELOAD_Pos             0U                                            /*!< STK LOAD: RELOAD Position */
#define STK_LOAD_RELOAD_Msk            (0xFFFFFFUL /*<< STK_LOAD_RELOAD_Pos*/)        /*!< STK LOAD: RELOAD Mask */

/* STK Current Register Definitions */
#define STK_VAL_CURRENT_Pos             0U                                            /*!< STK VAL: CURRENT Position */
#define STK_VAL_CURRENT_Msk            (0xFFFFFFUL /*<< STK_VAL_CURRENT_Pos*/)        /*!< STK VAL: CURRENT Mask */

/* STK Calibration Register Definitions */
#define STK_CALIB_NOREF_Pos            31U                                            /*!< STK CALIB: NOREF Position */
#define STK_CALIB_NOREF_Msk            (1UL << STK_CALIB_NOREF_Pos)                   /*!< STK CALIB: NOREF Mask */

#define STK_CALIB_SKEW_Pos             30U                                            /*!< STK CALIB: SKEW Position */
#define STK_CALIB_SKEW_Msk             (1UL << STK_CALIB_SKEW_Pos)                    /*!< STK CALIB: SKEW Mask */

#define STK_CALIB_TENMS_Pos             0U                                            /*!< STK CALIB: TENMS Position */
#define STK_CALIB_TENMS_Msk            (0xFFFFFFUL /*<< STK_CALIB_TENMS_Pos*/)        /*!< STK CALIB: TENMS Mask */


/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00  //Vector Table base offset field.This value must be a multiple of 0x200. */

typedef struct{
	uint32_t PLLState;   		// The new state of the PLL.
  uint32_t PLLSource;  		// RCC_PLLSource: PLL entry clock source.
  uint32_t PLLM;       		// PLLM: Division factor for PLL VCO input clock. This parameter must be a number between Min_Data = 1 and Max_Data = 16 on STM32L4Rx/STM32L4Sx devices.
                          // 	This parameter must be a number between Min_Data = 1 and Max_Data = 8 on the other devices */
  uint32_t PLLN;       		// PLLN: Multiplication factor for PLL VCO output clock.This parameter must be a number between Min_Data = 8 and Max_Data = 86
  uint32_t PLLP;       		// PLLP: Division factor for SAI clock. This parameter must be a value of @ref RCC_PLLP_Clock_Divider
  uint32_t PLLQ;       		// PLLQ: Division factor for SDMMC1, RNG and USB clocks.
  uint32_t PLLR;       		// PLLR: Division for the main system clock. User have to set the PLLR parameter correctly to not exceed max frequency 120MHZ
                          //  on STM32L4Rx/STM32L4Sx devices else 80MHz on the other devices.
                          //  This parameter must be a value of @ref RCC_PLLR_Clock_Divider

	
}PLL_InitTypeDef; 

void 										init_coreM4					(void);
__STATIC_INLINE void 		M4_set_prio_group		(uint32_t PriorityGroup);
__STATIC_INLINE void 		M4_init_sys_tick		(uint32_t ticks);
void 										SystemInit					(void);
void 										periph_init_M4			(void);
void 										init_bus_tree				(PLL_InitTypeDef* PLLStruct);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COREM4_H */

/**
  * @}
  */

  /**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
