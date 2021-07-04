#include "nvic.h"
/* 
	 https://microcontrollerslab.com/nested-vectored-interrupt-controller-nvic-arm-cortex-m/ 
		
	 The NVIC is on-chip controller that handles interrupts and it is closely couple with the processor core interface, 
   to enable low latency interrupt processing.

 */
 
//-------------------------------------------------------------------------------------------------------------
// NVIC
//-------------------------------------------------------------------------------------------------------------
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

void NVIC_set_Priority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority){

			uint32_t prioritygroup = ((uint32_t)((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos));
			uint32_t priority = EncodePriority(prioritygroup, PreemptPriority, SubPriority);
	
			if ((int32_t)(IRQn) >= 0)
			{
				NVIC->IP[((uint32_t)IRQn)]               = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
			}
			else
			{
				SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
			}

}

/**
  \brief   Enable Interrupt
  \details Enables a device specific interrupt in the NVIC interrupt controller. Has up to 240 interrupts whith each 
					 0-15 programmable interrupts (A higher level corresponfs to a lower level priority, so level 0 is the highest)
						

  \param   [in]: IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */

void NVIC__EnableIRQ(IRQn_Type IRQn){
	
	if ((int32_t)(IRQn) >= 0)
  {
    __COMPILER_BARRIER();
		  // interrupt set-enable register (ISER): each bit enables an interrupt. Each bits corresponds to the Interrupt number
		  NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    __COMPILER_BARRIER();
  }
	
}
