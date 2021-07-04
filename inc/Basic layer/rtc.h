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

#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stdint.h"
#include  "stm32l475xx.h"


//***************************************************************************************************************************************************************
//RTC communication protocol< ------------------------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************************************
#define RTC_TR_RESERVED_MASK    0x007F7F7FU
#define RTC_INIT_MASK           0xFFFFFFFFU

#define RTC_WRITEPROTECTION_DISABLE(Instance)       \
                        do{                         \
                            Instance->WPR = 0xCAU;  \
                            Instance->WPR = 0x53U;  \
                          } while(0U)

													


typedef enum 
{
	LSE = 0,
	LSI,
  HSE	
}CLK_src;

typedef struct
{
  uint8_t Hours;            // Specifies the RTC Time Hour.This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
                            // This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected 
  uint8_t Minutes;          // Specifies the RTC Time Minutes. This parameter must be a number between Min_Data = 0 and Max_Data = 59 //
  uint8_t Seconds;          // Specifies the RTC Time Seconds. This parameter must be a number between Min_Data = 0 and Max_Data = 59 //

} RTC_TimeFormat;

typedef struct
{
  char Hours[2];            // Specifies the RTC Time Hour.This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
                            // This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected 
  char Minutes[2];          // Specifies the RTC Time Minutes. This parameter must be a number between Min_Data = 0 and Max_Data = 59 //
  char Seconds[2];          // Specifies the RTC Time Seconds. This parameter must be a number between Min_Data = 0 and Max_Data = 59 //

} RTC_CharTimeFormat;

typedef struct
{
	RTC_TimeFormat time;						// defines the time
	CLK_src 			 clk;							// clock that will be used to source the rtc
	uint8_t       enable_wake_Up; 	// enable the RTC wakeup timer
	uint32_t      WakeUpTimeInSec;	// define the time for the RTC wakeup interrupt
	
	
}RTC_init_struct;

void 			RTC_enable				(CLK_src clock);
uint8_t 	RTC_ByteToBcd2		(uint8_t Value);
uint8_t   RTC_Bcd2ToByte  	(uint8_t Value);
void 			RTC_alarm_A_conf	(void);
void 			Auto_Wake_up_conf	(uint32_t time_in_sec);
void 			RTC_get_time			(RTC_TimeFormat* currentTime);
void 			RTC_init					(RTC_init_struct* rtc_init);
void 			Convert2String		(char* dst,RTC_TimeFormat* normalTimeFormat);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RTC_H */

/**
  * @}
  */

  /**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
