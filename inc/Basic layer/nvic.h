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

#ifndef __NVIC_H
#define __NVIC_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//#include "stdint.h"
#include  "stm32l475xx.h"


//***************************************************************************************************************************************************************
//NVIC -  nested vector interrupt controller< -------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************************************


/* defines ----------------------------------------------------------------------------------------------------------------------------------------------*/
#define _NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority, 4 bits for subpriority */
#define _NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority, 3 bits for subpriority */
#define _NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,2 bits for subpriority */
#define _NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority, 1 bit  for subpriority */
#define _NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority, 0 bit  for subpriority */



/* function declaration ------------------------------------------------------------------------------------------------------------------------------------*/
void NVIC_set_Priority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void __NVIC_set_Priority	(IRQn_Type IRQn, uint32_t priority);
void NVIC__EnableIRQ(IRQn_Type IRQn);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NVIC_H */

/**
  * @}
  */

  /**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
