/**
  ******************************************************************************
  * @file    BSP/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Defines -------------------------------------------------------------------*/
/* #define USE_LPS22HB_TEMP */

/* Includes ------------------------------------------------------------------*/
#include "InitFunctions.h"
#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include "rtc.h"
#include "m4core.h"
//#include "HTS221.h"
#include "ISM43362.h"
#include <string.h>
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//void QSPI_demo(void);
//void QSPI_MemoryMapped_demo(void);
//void Temperature_Test(void);
//void Humidity_Test(void);
//void Pressure_Test(void);
//void Gyro_Test(void);
//void Magneto_Test(void);
//void Accelero_Test(void);
void 				HTS221_temperature_sensor_init				(void);
void 				init_timer														(void);
uint32_t 		Serial_Scanf(uint32_t value);
void 				Error_Handler(void);
void        HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
#define stop_cpu()   __breakpoint(0)
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
