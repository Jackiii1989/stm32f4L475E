#include "InitFunctions.h"
#include <string.h>
#include "gpio.h"
//#include "stm32l4xx_hal.h"

//-------------------------------------------------------------------------------------------------------------
// RCC FUNCTIONS AND DEFINES
//-------------------------------------------------------------------------------------------------------------
volatile uint32_t uwTick;
static uint32_t 		System_core_clock 	 = 4000000;
static uint32_t     _uwTickPrio          = (1UL << __NVIC_PRIO_BITS); /* Invalid priority */
TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */
static const uint8_t  AHBPrescTable[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
//static const uint8_t  APBPrescTable[8]   = {0, 0, 0, 0, 1, 2, 3, 4};
//static const uint32_t MSIRangeTable[12]  = {100000, 200000, 400000, 800000, 1000000, 2000000, \
//                                            4000000, 8000000, 16000000, 24000000, 32000000, 48000000};

__weak uint32_t HAL_GetTick(void)
{
  return uwTick;
}																						
																						
	
__weak void HAL_IncTick(void)
{
  uwTick += (uint32_t)uwTickFreq;
}																						
	
/*
__STATIC_INLINE void set_prio_group(uint32_t PriorityGroup){
	
  uint32_t  reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);             // only values 0..7 are used  

  reg_value  =  SCB->AIRCR;                                                   // read old register configuration    
  reg_value &= ~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk)); // clear bits to change               
  reg_value  =  (reg_value                                   |
                ((uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << SCB_AIRCR_PRIGROUP_Pos)  );              // Insert write key and priority group 
  SCB->AIRCR =  reg_value;
}
*/

 __STATIC_INLINE uint32_t sys_tick_conf(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   				// Reload value impossible 
  }                                                                  
																																		 
  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         			// set reload register 
  //NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); 		// set Priority for Systick Interrupt 
	_set_Priority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); 					// set Priority for Systick Interrupt 
  SysTick->VAL   = 0UL;                                             			// Load the SysTick Counter Value 
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |                      
                   SysTick_CTRL_TICKINT_Msk   |                      
                   SysTick_CTRL_ENABLE_Msk;                         			// Enable SysTick IRQ and SysTick Timer 
  return (0UL);                                                     			// Function successful 
}

__STATIC_INLINE void _set_Priority(IRQn_Type IRQn, uint32_t priority)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->IP[((uint32_t)IRQn)]               = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
  else
  {
    SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
}



//uint32_t GetSysClockFreq_RCC(void){
//  uint32_t msirange = 0U, sysclockfreq = 0U;
//  uint32_t pllvco, pllsource, pllr, pllm;    /* no init needed */
//  uint32_t sysclk_source, pll_oscsource;
//
//  //sysclk_source = __HAL_RCC_GET_SYSCLK_SOURCE();
//	sysclk_source = _READ_REG_BIT(RCC->CFGR, RCC_CFGR_SWS);
//  //pll_oscsource = __HAL_RCC_GET_PLL_OSCSOURCE();
//	pll_oscsource = _READ_REG_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC);
//
//  if((sysclk_source == RCC_CFGR_SWS_MSI) ||
//     ((sysclk_source == RCC_CFGR_SWS_PLL) && (pll_oscsource == RCC_PLLCFGR_PLLSRC_MSI)))
//  {
//    /* MSI or PLL with MSI source used as system clock source */
//
//    /* Get SYSCLK source */
//    if(_READ_REG_BIT(RCC->CR, RCC_CR_MSIRGSEL) == 0U)
//    { /* MSISRANGE from RCC_CSR applies */
//      msirange = _READ_REG_BIT(RCC->CSR, RCC_CSR_MSISRANGE) >> RCC_CSR_MSISRANGE_Pos;
//    }
//    else
//    { /* MSIRANGE from RCC_CR applies */
//      msirange = _READ_REG_BIT(RCC->CR, RCC_CR_MSIRANGE) >> RCC_CR_MSIRANGE_Pos;
//    }
//    /*MSI frequency range in HZ*/
//    msirange = MSIRangeTable[msirange];
//
//    if(sysclk_source == RCC_CFGR_SWS_MSI)
//    {
//      /* MSI used as system clock source */
//      sysclockfreq = msirange;
//    }
//  }
//  else if(sysclk_source == RCC_CFGR_SWS_HSI)
//  {
//    /* HSI used as system clock source */
//    sysclockfreq = HSI_VALUE;
//  }
//  else if(sysclk_source == RCC_CFGR_SWS_HSE)
//  {
//    /* HSE used as system clock source */
//    sysclockfreq = HSE_VALUE;
//  }
//  else
//  {
//    /* unexpected case: sysclockfreq at 0 */
//  }
//
//  if(sysclk_source == RCC_CFGR_SWS_PLL)
//  {
//    /* PLL used as system clock  source */
//
//    /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE) * PLLN / PLLM
//    SYSCLK = PLL_VCO / PLLR
//    */
//    pllsource = _READ_REG_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC);
//
//    switch (pllsource)
//    {
//    case RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
//      pllvco = HSI_VALUE;
//      break;
//
//    case RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
//      pllvco = HSE_VALUE;
//      break;
//
//    case RCC_PLLSOURCE_MSI:  /* MSI used as PLL clock source */
//    default:
//      pllvco = msirange;
//      break;
//    }
//    pllm = (_READ_REG_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos) + 1U ;
//    pllvco = (pllvco * (_READ_REG_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)) / pllm;
//    pllr = ((_READ_REG_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos) + 1U ) * 2U;
//    sysclockfreq = pllvco / pllr;
//  }
//
//  return sysclockfreq;
//}

void InitTick(uint32_t TickPriority)
{
	uint32_t prioritygroup;
	
	(void)sys_tick_conf(System_core_clock/SYS_TICK_FREQ_1KHZ);
	 if (TickPriority < (1UL << __NVIC_PRIO_BITS)){
			//HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
			prioritygroup = ((uint32_t)((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos));
			_set_Priority(SysTick_IRQn,EncodePriority(prioritygroup, TickPriority, 0U));
			_uwTickPrio = TICK_INT_PRIO;
	 }
}

void set_Priority(uint32_t priority, uint32_t PreemptPriority, uint32_t SubPriority){

			uint32_t prioritygroup = ((uint32_t)((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos));
		_set_Priority(SysTick_IRQn,EncodePriority(prioritygroup, PreemptPriority, SubPriority));
		_uwTickPrio = TICK_INT_PRIO;
}

__STATIC_INLINE uint32_t EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);   /* only values 0..7 are used          */
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(__NVIC_PRIO_BITS)) ? (uint32_t)(__NVIC_PRIO_BITS) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits     = ((PriorityGroupTmp + (uint32_t)(__NVIC_PRIO_BITS)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(__NVIC_PRIO_BITS));

  return (
           ((PreemptPriority & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
           ((SubPriority     & (uint32_t)((1UL << (SubPriorityBits    )) - 1UL)))
         );
}
																									
																						
																						
void core_init(void){
	
	//set_prio_group(_NVIC_PRIORITYGROUP_4);
	
	InitTick(TICK_INT_PRIO);
}	
	
void periph_init(void){
	PLLInitTypeDef PLLStruct;
	PLLStruct.PLLState = RCC_PLL_ON;
  PLLStruct.PLLSource = RCC_PLLCFGR_PLLSRC_MSI;
  PLLStruct.PLLM = 1;
  PLLStruct.PLLN = 40;
  PLLStruct.PLLR = 2;
  PLLStruct.PLLP = 7;
  PLLStruct.PLLQ = 4;
	_periph_init(&PLLStruct);
}

statusTypeDef _periph_init(PLLInitTypeDef* PLLStruct){
	
	//statusTypeDef status;
	uint32_t tickstart;
	
	//uint32_t sysclk_source; 
  //uint32_t pll_config;
	//sysclk_source  = __HAL_RCC_GET_SYSCLK_SOURCE();
	//pll_config 		 = __HAL_RCC_GET_PLL_OSCSOURCE();
	
	// Else, keep current flash latency while decreasing applies 
  // Selects the Multiple Speed oscillator (MSI) clock range.
  //__HAL_RCC_MSI_RANGE_CONFIG(RCC_OscInitStruct->MSIClockRange);
	//__HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_6);
	//SET_BIT(RCC->CR, RCC_CR_MSIRGSEL);     
	//RCC->CR |= RCC_CR_MSIRGSEL;
	_MODIFY_REG(RCC->CR, RCC_CR_MSIRANGE, (RCC_CR_MSIRANGE_6));
	
  // Adjusts the Multiple Speed oscillator (MSI) calibration value.
	 _MODIFY_REG(RCC->ICSCR, RCC_ICSCR_MSITRIM, (RCC_MSICALIBRATION_DEFAULT) << RCC_ICSCR_MSITRIM_Pos);
	// Update the SystemCoreClock global variable */´  
	//SystemCoreClock = HAL_RCC_GetSysClockFreq() >> (AHBPrescTable[READ_BIT(RCC->CFGR, RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);
	System_core_clock = GetSysClockFreq_RCC() >> (AHBPrescTable[_READ_REG_BIT(RCC->CFGR, RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);
	// Configure the source of time base considering new system clocks settings*/
  //(void)HAL_InitTick(uwTickPrio);
	InitTick(_uwTickPrio);
	
	//PLL Configuration
  // Disable the main PLL. 
  //__HAL_RCC_PLL_DISABLE();
	//CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
	_REG_RESET_BIT(RCC->CR, RCC_CR_PLLON);
	
	/* Get Start Tick*/
  tickstart = HAL_GetTick();
	/* Wait till PLL is ready */
  //while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != 0U)
	while(_READ_REG_BIT(RCC->CR, RCC_CR_PLLRDY) != 0U)
  {
    if((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VAL)
    {
      return TIMEOUT_;
    }
  }
	
	  /* Configure the main PLL clock source, multiplication and division factors. */
  _RCC_PLL_CONFIG(PLLStruct->PLLSource,
                       PLLStruct->PLLM,
                       PLLStruct->PLLN,
                       PLLStruct->PLLP,
                       PLLStruct->PLLQ,
                       PLLStruct->PLLR);
	
	/* Enable the main PLL. */
  //__HAL_RCC_PLL_ENABLE();
	_REG_SET_BIT(RCC->CR, RCC_CR_PLLON);
	
  /* Enable PLL System Clock output. */
  //__HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_SYSCLK);
	_REG_SET_BIT(RCC->PLLCFGR, (RCC_PLLCFGR_PLLREN));
  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL is ready */
  //while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0U)
	while(_READ_REG_BIT(RCC->CR, RCC_CR_PLLRDY) == 0U)
  {
    if((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VAL)
    {
      return TIMEOUT_;
    }
  }
	
	
	//----------------------------------------------------------------------------------------------------
	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
  // must be correctly programmed according to the frequency of the CPU clock
  //(HCLK) and the supply voltage of the device.

  // Increasing the number of wait states because of higher CPU frequency */
	// Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
  //__HAL_FLASH_SET_LATENCY(FLASH_LATENCY_4);
	_MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (FLASH_ACR_LATENCY_4WS));
	//Check that the new number of wait states is taken into account to access the Flash
  //memory by reading the FLASH_ACR register
  if(_READ_REG_BIT(FLASH->ACR, FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS){
      return ERROR_;
  }
	/* MSI is selected as System Clock Source */
	//MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_ClkInitStruct->SYSCLKSource);
	_MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
	/* Get Start Tick*/
  tickstart = HAL_GetTick();
  //while(__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_ClkInitStruct->SYSCLKSource << RCC_CFGR_SWS_Pos))
  while(_READ_REG_BIT(RCC->CFGR, RCC_CFGR_SWS) != (RCC_CFGR_SW_PLL << RCC_CFGR_SWS_Pos))
  {
    if((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
    {
       return TIMEOUT_;
    }
  }
	/*-------------------------- HCLK Configuration --------------------------*/
	//MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
	_MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
	/*-------------------------- PCLK1 Configuration ---------------------------*/
	//MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_ClkInitStruct->APB1CLKDivider);
	_MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV1);
	/*-------------------------- PCLK2 Configuration ---------------------------*/
	//MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_ClkInitStruct->APB2CLKDivider) << 3U));
	_MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_CFGR_PPRE1_DIV1) << 3U));
	/* Update the SystemCoreClock global variable */
  //SystemCoreClock = HAL_RCC_GetSysClockFreq() >> (AHBPrescTable[READ_BIT(RCC->CFGR, RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);
  /* Configure the source of time base considering new system clocks settings*/
  //status = HAL_InitTick(uwTickPrio);
	// Update the SystemCoreClock global variable */
  //System_core_clock = HAL_RCC_GetSysClockFreq() >> (AHBPrescTable[READ_BIT(RCC->CFGR, RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);
	System_core_clock = GetSysClockFreq_RCC() >> (AHBPrescTable[_READ_REG_BIT(RCC->CFGR, RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);
	// Configure the source of time base considering new system clocks settings*/
	InitTick(_uwTickPrio);
	return OK_;
}

void SystemInit_(void){
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
	
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set MSION bit */
  RCC->CR |= RCC_CR_MSION;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON , HSION, and PLLON bits */
  RCC->CR &= (uint32_t)0xEAF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00001000;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}


