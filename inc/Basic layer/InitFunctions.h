#ifndef INIT_FUNCTIONS_H
#define INIT_FUNCTIONS_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32l475xx.h"

// RCC < ---------------------------------------------------------------------
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


#define HSI_VALUE    										16000000U /*!< Value of the Internal oscillator in Hz*/
#define HSE_VALUE   										8000000U /*!< Value of the External oscillator in Hz */
#define MSI_VALUE    										4000000U /*!< Value of the Internal oscillator in Hz*/
#define LSI_VALUE  											32000U                 /*!< LSI Typical Value in Hz*/


void 					InitTick							(uint32_t TickPriority);
uint32_t 			sys_tick_conf					(uint32_t ticks);
uint32_t 			GetSysClockFreq_RCC		(void);
void 					set_prio_group				(volatile uint32_t PriorityGroup);
void 					_set_Priority					(IRQn_Type IRQn, uint32_t priority);
void 					set_Priority					(uint32_t priority, uint32_t PreemptPriority, uint32_t SubPriority);
uint32_t 			EncodePriority 				(uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority);


/* Mix Defines ------------------------------------------------------------------*/ 
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------ 









// Mix < ----------------------------------------------------------------------------------------------------------
#define _REG_SET_BIT(REG, BIT)     	  						( (REG) |= (BIT) )
#define _REG_RESET_BIT(REG, BIT)   								( (REG) &= ~(BIT))
#define _WRITRE_TO_REG(REG, VAL)   								( (REG)  = (VAL) )
#define _READ_REG_BIT(REG, BIT)    								((REG) & (BIT))
#define _MODIFY_REG(REG, CLEARMASK, SETMASK)  		_WRITRE_TO_REG(  (REG), ( (REG) & (~(CLEARMASK)) ) | (SETMASK)  )


#define _RCC_PLL_CONFIG(__PLLSOURCE__, __PLLM__, __PLLN__, __PLLP__, __PLLQ__,__PLLR__ ) \
                  _MODIFY_REG(RCC->PLLCFGR, \
                             (RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | \
                              RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLR | RCC_PLLCFGR_PLLP), \
                             ((__PLLSOURCE__) | \
                              (((__PLLM__) - 1U) << RCC_PLLCFGR_PLLM_Pos) | \
                              ((__PLLN__) << RCC_PLLCFGR_PLLN_Pos) | \
                              ((((__PLLQ__) >> 1U) - 1U) << RCC_PLLCFGR_PLLQ_Pos) | \
                              ((((__PLLR__) >> 1U) - 1U) << RCC_PLLCFGR_PLLR_Pos) | \
                              (((__PLLP__) >> 4U) << RCC_PLLCFGR_PLLP_Pos)))

// Intialize functions < ---------------------------------------------------------
/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */
///******************************************************************************/
//// Core & Periph Init < ---------------------------------------------------------
//#define HSI_VALUE    										16000000U /*!< Value of the Internal oscillator in Hz*/
//#define HSE_VALUE   										8000000U /*!< Value of the External oscillator in Hz */
//#define MSI_VALUE    										4000000U /*!< Value of the Internal oscillator in Hz*/
//#define LSI_VALUE  											32000U                 /*!< LSI Typical Value in Hz*/
//
//#define HSE_STARTUP_TIMEOUT    					100U   /*!< Time out for HSE start up, in ms */
//#define LSE_STARTUP_TIMEOUT    					5000U   /*!< Time out for LSE start up, in ms */

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

	
}PLLInitTypeDef; 

typedef enum
{
  OK_       = 0x00,
  ERROR_    = 0x01,
  BUSY_     = 0x02,
  TIMEOUT_  = 0x03
} statusTypeDef;

void 					core_init							(void);
void 					periph_init						(void);
void 					SystemInit						(void);
statusTypeDef _periph_init					(PLLInitTypeDef* PLLStruct);

typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} TickFreqTypeDef;

// SysTick < ---------------------------------------------------------
void 					InitTick							(uint32_t TickPriority);
uint32_t 			sys_tick_conf					(uint32_t ticks);
uint32_t 			HAL_GetTick						(void);
void 					HAL_IncTick						(void);


#ifdef __cplusplus
}
#endif

#endif /* INIT_FUNCTIONS_H */
