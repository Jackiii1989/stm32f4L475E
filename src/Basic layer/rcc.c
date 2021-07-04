#include "rcc.h"
#include "InitFunctions.h"
#include "stdint.h"

uint32_t GetSysClockFreq_RCC(void){
  uint32_t msirange = 0U, sysclockfreq = 0U;
  uint32_t pllvco, pllsource, pllr, pllm;    /* no init needed */
  uint32_t sysclk_source, pll_oscsource;

  //sysclk_source = __HAL_RCC_GET_SYSCLK_SOURCE();
	sysclk_source = RCC->CFGR & RCC_CFGR_SWS;
  //pll_oscsource = __HAL_RCC_GET_PLL_OSCSOURCE();
	pll_oscsource = RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC;

  if((sysclk_source == RCC_CFGR_SWS_MSI) ||
     ((sysclk_source == RCC_CFGR_SWS_PLL) && (pll_oscsource == RCC_PLLCFGR_PLLSRC_MSI)))
  {
    /* MSI or PLL with MSI source used as system clock source */

    /* Get SYSCLK source */
    if((RCC->CR & RCC_CR_MSIRGSEL) == 0U)
    { /* MSISRANGE from RCC_CSR applies */
      msirange = (RCC->CSR & RCC_CSR_MSISRANGE) >> RCC_CSR_MSISRANGE_Pos;
    }
    else
    { /* MSIRANGE from RCC_CR applies */
      msirange = (RCC->CR & RCC_CR_MSIRANGE) >> RCC_CR_MSIRANGE_Pos;
    }
    /*MSI frequency range in HZ*/
    msirange = MSIRangeTable[msirange];

    if(sysclk_source == RCC_CFGR_SWS_MSI)
    {
      /* MSI used as system clock source */
      sysclockfreq = msirange;
    }
  }
  else if(sysclk_source == RCC_CFGR_SWS_HSI)
  {
    /* HSI used as system clock source */
    sysclockfreq = HSI_VALUE;
  }
  else if(sysclk_source == RCC_CFGR_SWS_HSE)
  {
    /* HSE used as system clock source */
    sysclockfreq = HSE_VALUE;
  }
  else
  {
    /* unexpected case: sysclockfreq at 0 */
  }

  if(sysclk_source == RCC_CFGR_SWS_PLL)
  {
    /* PLL used as system clock  source */

    /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE) * PLLN / PLLM
    SYSCLK = PLL_VCO / PLLR
    */
    pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);

    switch (pllsource)
    {
    case RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
      pllvco = HSI_VALUE;
      break;

    case RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
      pllvco = HSE_VALUE;
      break;

    case RCC_PLLSOURCE_MSI:  /* MSI used as PLL clock source */
    default:
      pllvco = msirange;
      break;
    }
    pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos) + 1U ;
    pllvco = (pllvco * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)) / pllm;
    pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos) + 1U ) * 2U;
    sysclockfreq = pllvco / pllr;
  }

  return sysclockfreq;
}

uint32_t get_SystemClock(void){
return GetSysClockFreq_RCC() >> 
								(AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);
}

void delay_us(uint32_t n)
{
  volatile        uint32_t ct = 0;
  uint32_t        loop_per_us = 0;
  static uint32_t cycle_per_loop = 0;

  /* calibration happen on first call for a duration of 1 ms * nbcycle per loop */
  /* 10 cycle for STM32L4 */
  if (cycle_per_loop == 0 ) 
  {
     uint32_t cycle_per_ms = (System_core_clock/1000UL);
     uint32_t t = 0;
     ct = cycle_per_ms;
     t = HAL_GetTick();
     while(ct) ct--;
     cycle_per_loop = HAL_GetTick()-t;
     if (cycle_per_loop == 0) cycle_per_loop = 1;
  }

  loop_per_us = System_core_clock/1000000UL/cycle_per_loop;
  ct = n * loop_per_us;
  while(ct) ct--;
  return;
}

void delay_ms(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;


  while ((HAL_GetTick() - tickstart) < wait)
  {
  }
}
