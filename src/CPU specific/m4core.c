#include "m4core.h"
#include "rcc.h"
#include "main.h"
/**
  \brief   			 Basic functionality for 
  \details 
  \param [in]    
 */
 
//-------------------------------------------------------------------------------------------------------------
// m4core
//-------------------------------------------------------------------------------------------------------------

void init_coreM4(void){
	
	//Do not need to execute this again as it will be executed in .s file---------------------->>
		//SystemInit();
	
	// enable flash prefetch for code
	FLASH->ACR |= FLASH_ACR_PRFTEN;
	
	// define the interrupt priority grouping (more information in the prog. manual 228/262)
	M4_set_prio_group(_NVIC_PRIORITYGROUP_4);
	
	// set the priority m4 core SysTick timer when an interrupt occurs
	NVIC_set_Priority(SysTick_IRQn, TICK_INT_PRIO,0U);
	
	// configure the m4 core SysTick timer 
	M4_init_sys_tick(System_core_clock/SYS_TICK_FREQ_1KHZ);
}



__STATIC_INLINE void M4_set_prio_group(uint32_t PriorityGroup){
	
	// clear bits to change 
  SCB->AIRCR &= ~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk));  
	
	// Insert write key and priority group (priority grou have a value between 0-7)
  SCB->AIRCR  |=  ( (uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos ) | ( (PriorityGroup & (uint32_t)0x07UL) << SCB_AIRCR_PRIGROUP_Pos );              
}

__STATIC_INLINE void M4_init_sys_tick(uint32_t ticks)
{
  // set reload register - values higher than SysTick_LOAD_RELOAD_Msk will be trimmed
  SysTick->LOAD  = (uint32_t)(ticks - 1UL) & SysTick_LOAD_RELOAD_Msk;                         
	
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); // set Priority for Systick Interrupt */
	//_set_Priority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); // set Priority for Systick Interrupt */
  
	// Init the SysTick Counter Value */
	SysTick->VAL   = 0UL;
	
	// Clock Source selection: Procesor clock (AHB) | Enable SysTick IRQ | enable SysTick Timer 
  SysTick->CTRL  = STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE;                         
}

void periph_init_M4(void){
	PLL_InitTypeDef PLLStruct;
	PLLStruct.PLLState = RCC_PLL_ON;
  PLLStruct.PLLSource = RCC_PLLCFGR_PLLSRC_MSI;
  PLLStruct.PLLM = 1;
  PLLStruct.PLLN = 40;
  PLLStruct.PLLR = 2;
  PLLStruct.PLLP = 7;
  PLLStruct.PLLQ = 4;
	init_bus_tree(&PLLStruct);
}

void init_bus_tree(PLL_InitTypeDef* PLLStruct){
	
	// Selects the Frequency range of MSI: a selection ofMultiple Speed oscillator (MSI) clock range.
	RCC->CR &= ~RCC_CR_MSIRGSEL;
	RCC->CR |=  RCC_CR_MSIRANGE_6; // range 4 Mhz
	
  // Fine Calibration Adjusts the Multiple Speed oscillator (MSI) calibration value.
	RCC->ICSCR &= ~RCC_ICSCR_MSITRIM;
	RCC->ICSCR |= (RCC_MSICALIBRATION_DEFAULT << RCC_ICSCR_MSITRIM_Pos); 
	
	// Update the SystemCoreClock global variable */´  
	System_core_clock = GetSysClockFreq_RCC() >> (AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);
	
	// Re-configure the source of time base considering new system clocks settings*/
	M4_init_sys_tick(System_core_clock/SYS_TICK_FREQ_1KHZ);
	
	//--------------------------------------------->   PLL Configuration   <----------------------------------------------
  // Disable the main PLL. 
  RCC->CR &= ~ RCC_CR_PLLON;

	/* Wait till PLL is unlocked */
  while((RCC->CR & RCC_CR_PLLRDY) != 0U){
		// TODO : implement a tineout
  }
	
	 /* Configure the main PLL clock source, multiplication and division factors. */
	// reseting the PLL
	RCC->PLLCFGR &= RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | \
									RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLR | RCC_PLLCFGR_PLLP; 
	
	// writting the coefficients in PLL
	RCC->PLLCFGR |=((PLLStruct->PLLSource) | (((PLLStruct->PLLM) - 1U) << RCC_PLLCFGR_PLLM_Pos) | \
                 ((PLLStruct->PLLN) << RCC_PLLCFGR_PLLN_Pos) | \
                 ((((PLLStruct->PLLQ) >> 1U) - 1U) << RCC_PLLCFGR_PLLQ_Pos) | \
                 ((((PLLStruct->PLLR) >> 1U) - 1U) << RCC_PLLCFGR_PLLR_Pos) | \
                 (((PLLStruct->PLLP) >> 4U) << RCC_PLLCFGR_PLLP_Pos));

	
	/* Enable the main PLL. */
	RCC->CR |= RCC_CR_PLLON;;
	
  /* Enable PLL System Clock output. */
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

  /* Wait till PLL is locked/ready */
	while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY){
		// TODO: timeout
  }
	
	
	//--------------------------------------------------> FLASH LATENCY<--------------------------------------------------
	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
  // must be correctly programmed according to the frequency of the CPU clock
  //(HCLK) and the supply voltage of the device.

  // Increasing the number of wait states because of higher CPU frequency */
  FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
	
	//Check that the new number of wait states is taken into account to access the Flash bymemory by reading the FLASH_ACR register
	// defined in spec
  if((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS){
		stop_cpu();
    // TODO: need to do something
  }
	
	/* MSI is selected as System Clock Source */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// check if MSI selected as clock source
  while((RCC->CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SW_PLL << RCC_CFGR_SWS_Pos)){
		// TODO: define an timeout or error handler
	
  }
	/*---------------------------------------------- HCLK Configuration -------------------------------------------------------------*/
	RCC->CFGR &= ~RCC_CFGR_HPRE; 
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	/*----------------------------------------------- PCLK1 Configuration ------------------------------------------------------------*/
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;

	/*---------------------------------------------- PCLK2 Configuration -------------------------------------------------------------*/
	RCC->CFGR &= ~RCC_CFGR_PPRE2;
	RCC->CFGR |= (RCC_CFGR_PPRE1_DIV1 << 3U);


	// Update the SystemCoreClock global variable 
	System_core_clock = GetSysClockFreq_RCC() >> (AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);
	
	// Configure the source of time base considering new system clocks settings*/
	InitTick(_uwTickPrio);
	return;
}

void SystemInit(void){
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
