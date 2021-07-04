#include "rtc.h"
#include "nvic.h"
#include <stdio.h>
/**
  \brief   			 Timer basic configuration 
  \details 
  \param [in]    The timer instance, which to write to and config structure need to configure the timer. 
 */
 
//-------------------------------------------------------------------------------------------------------------
// RTC
//-------------------------------------------------------------------------------------------------------------
void RTC_init(RTC_init_struct* rtc_init){

	uint32_t tmpTime = 0U;
	RTC_TimeFormat	bcdTimeFormat;	
	
	// Enable RTC module with the LSI clock
	RTC_enable(rtc_init->clk);
	
  // 1. Disable write protection for RTC Registers --> now the registers can be modified.
	// with this steps we can dissable the write protection:
  RTC->WPR = 0xCAU;
  RTC->WPR = 0x53U;
	//RTC_WRITEPROTECTION_DISABLE(RTC);

	//2. Enter the initialization mode by writing 1 (INIT) to RTC_ISR 
	// --> This means the calendar counter is stopped and we are allowed to update it
	RTC->ISR |= RTC_ISR_INIT;//(uint32_t)RTC_INIT_MASK;
	//RTC->ISR |= (uint32_t)RTC_INIT_MASK;

	//3. wait for the conformation of the initialization
	while(!(RTC->ISR & RTC_ISR_INITF)){
		RTC_WRITEPROTECTION_DISABLE(RTC);
		RTC->ISR = (uint32_t)RTC_INIT_MASK;
		
	}
	
	RTC->CR &= ~(RTC_CR_ALRAE | RTC_CR_ALRAIE);
	
	
	// Clear RTC_CR FMT, OSEL and POL Bits--> FMT: hour format(0:24h, 1:AM/PM), 
	// OSEL: Output selection (define which alarms(WUTR, ALRMAR, ALRMBR) are touted to RTC_ALARM). It is mutliplexer.
  // POL: Bits define the output polarity. When an alarm is asserted, what to send, an '1' or '0'
  RTC->CR &= ((uint32_t)~(RTC_CR_FMT | RTC_CR_OSEL | RTC_CR_POL));
	
	//4. Configure the RTC PRER: configure the both prescalers. The values have been ussed
	// which has been recomended in the reference manual 1229/1903 (Note)
	RTC->PRER  = ((uint32_t)(0x7F << 16) | (uint32_t)(0xFF));
		
	// convert normal time to bcd time 
	bcdTimeFormat.Seconds = RTC_ByteToBcd2(rtc_init->time.Seconds);
	bcdTimeFormat.Minutes = RTC_ByteToBcd2(rtc_init->time.Minutes);
  bcdTimeFormat.Hours   = RTC_ByteToBcd2(rtc_init->time.Hours);
	
	// unite the value in the register
	tmpTime = (((uint32_t)(bcdTimeFormat.Hours) << 16U) | \
            ((uint32_t)(bcdTimeFormat.Minutes) << 8U) | \
             (uint32_t)bcdTimeFormat.Seconds);
	
	//5. Load time and date value in the shadow register -> Setting the RTC_TR and RTC_DR
	RTC->TR = (uint32_t)(tmpTime & RTC_TR_RESERVED_MASK);
		
	//6. configure the time format
	// No need to configure as it already is right configured (0:24 hour format)
	
	// by pass the shadow register
	RTC->CR |= RTC_CR_BYPSHAD;
	
	if(rtc_init->enable_wake_Up == 1){
		Auto_Wake_up_conf(rtc_init->WakeUpTimeInSec);
	}
	
	//7. Exit the initialization mode --> the current calendar counter is automatically laoded
	RTC->ISR &= (uint32_t)(~RTC_ISR_INIT);
	
	//8. Enable the RTC Registers Write protection --> RTC register can no longer be modified 
	RTC->WPR = 0xFFU;
		
}

void RTC_get_time(RTC_TimeFormat* currentTime){
	
	uint32_t tmpreg = 0U;
	RTC_TimeFormat	bcdTimeFormat;
	
	// wait till the RSF bit is set
	//while( (RTC->ISR & RTC_ISR_RSF ) != RTC_ISR_RSF){
	//}
	
  // Get the TR register 
  tmpreg = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK);
	
	// Fill the structure fields with the read parameters
	bcdTimeFormat.Hours = (uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16U);
  bcdTimeFormat.Minutes = (uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> 8U);
  bcdTimeFormat.Seconds = (uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU));

	// Convert the time structure parameters to Binary format
  currentTime->Hours = (uint8_t)RTC_Bcd2ToByte(bcdTimeFormat.Hours);
  currentTime->Minutes = (uint8_t)RTC_Bcd2ToByte(bcdTimeFormat.Minutes);
  currentTime->Seconds = (uint8_t)RTC_Bcd2ToByte(bcdTimeFormat.Seconds);
	
}

uint8_t RTC_ByteToBcd2(uint8_t Value)
{
  uint32_t bcdhigh = 0U;

  while (Value >= 10)
  {
    bcdhigh++;
    Value -= 10;
  }

  return ((uint8_t)(bcdhigh << 4U) | Value);
}

uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
  uint32_t tmp = 0U;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}


void Convert2String(char* dst,RTC_TimeFormat* normalTimeFormat){
	sprintf(dst, "Currnet time %02d:%02d:%02d\n", normalTimeFormat->Hours, normalTimeFormat->Minutes,normalTimeFormat->Seconds);
}

void RTC_convert2char(RTC_CharTimeFormat* dst, RTC_TimeFormat* BcdTimeFormat)
{
	
	dst->Seconds[0]	= (char)(BcdTimeFormat->Seconds & 0x0F)+ '0';
	dst->Seconds[1]	= (char)((BcdTimeFormat->Seconds>>4) & 0x0F)+ '0';
	dst->Minutes[0]	= (char)(BcdTimeFormat->Minutes & 0x0F)+ '0';
	dst->Minutes[1]	= (char)((BcdTimeFormat->Minutes>>4) & 0x0F)+ '0';
	dst->Hours[0]	  = (char)(BcdTimeFormat->Hours & 0x0F)+ '0';
	dst->Hours[1]  	= (char)((BcdTimeFormat->Hours>>4) & 0x0F)+ '0';
}

void RTC_enable(CLK_src clock){
	
	// Enable PWR Clock --> Allow access to the RTC
	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
	
	//Disable backup domain write protection by setting teh DBP bit. (Reference man. -> page 1231/1903)
	PWR->CR1 |=PWR_CR1_DBP;
	
	switch(clock)
  {
		
		case LSE:
			
			// Enable the Internal Low Speed oscillator (LSE)
			RCC->BDCR |= RCC_BDCR_LSEON;
			
			// wait till the LSI is ready 
			while((RCC->BDCR & RCC_BDCR_LSERDY) == 0U){
			//TODO: implement a timeout
				};
			
			// switch to the clock
			RCC->BDCR |= RCC_BDCR_RTCSEL_0; 
			break;

		case LSI:
				
			// Enable the Internal Low Speed oscillator (LSI).
				RCC->CSR |= RCC_CSR_LSION;
			
			// wait till the LSI is ready 
			while((RCC->CSR & RCC_CSR_LSIRDY) == 0U){
				//TODO: implement a timeout
			};
			
			// select LSI clock to source the RTC
			RCC->BDCR |= RCC_BDCR_RTCSEL_1; 
			break;
	
		 case HSE:
			 // not implemented
			 break;
	}
	
	// Enable the the RTC module
	RCC->BDCR |= RCC_BDCR_RTCEN; 
	
	return;
}
// NOT TESTED YET
void RTC_alarm_A_conf(void){
	
	// 1. Disable write protection for RTC Registers --> now the registers can be modified.
	// with this steps we can dissable the write protection:
  //RTC->WPR = 0xCAU;
  //RTC->WPR = 0x53U;
	
	//Disable Alarm 
	RTC->CR &=  ~RTC_CR_ALRAE;
	
	// poll ALRAWF (ALaRm A Write Flag) until it is set --> we move on
	// when we are alowed to write/access the register
	while(( RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF){
		// TODO: set a timeout
	}
	
	// configure the alarm
	RTC->ALRMAR |= (RTC_ALRMAR_MSK1 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK4); // 
	
	
  NVIC_set_Priority(RTC_Alarm_IRQn, 0x10, 0x00);
  NVIC__EnableIRQ(RTC_Alarm_IRQn);

	
	//Re-enable the alarm and interrupt enable for Alarm A  
	RTC->CR |= RTC_CR_ALRAIE;
	
	//enable the write protection
	//RTC->WPR = 0xFFU;
	
}

void Auto_Wake_up_conf(uint32_t time_in_sec){
	
	// Enable RTC module with the LSI/LSE clock
	//RTC_enable();
	
	
	
  //// 1. Disable write protection for RTC Registers --> now the registers can be modified.
	//// with this steps we can dissable the write protection:
  //RTC->WPR = 0xCAU;
  //RTC->WPR = 0x53U;
	
	////2. Enter the initialization mode by writing 1 (INIT) to RTC_ISR 
	//// --> This means the calendar counter is stopped and we are allowed to update it
	//RTC->ISR |= RTC_ISR_INIT;//(uint32_t)RTC_INIT_MASK;
	////RTC->ISR |= (uint32_t)RTC_INIT_MASK;
	
	////3. wait for the conformation of the initialization
	//while(!(RTC->ISR & RTC_ISR_INITF)){
	//	RTC_WRITEPROTECTION_DISABLE(RTC);
	//	RTC->ISR = (uint32_t)RTC_INIT_MASK;	
	//}
	
	//RTC->CR &= ~(RTC_CR_ALRAE | RTC_CR_ALRAIE);
	
	////4. Configure the RTC PRER: configure the both prescalers. The values have been ussed
	//// which has been recomended in the reference manual 1229/1903 (Note)
	//RTC->PRER  = ((uint32_t)(0x7F << 16) | (uint32_t)(0xFF));

	// Disable the wake up timer
	RTC->CR &= ~RTC_CR_WUTE;
	
	// Ensure access to Wakeup auto-reload counter and bits WUCKSEL is allowed
	// we are doing this by polling the WUTFWF flag
	while(( RTC->ISR & RTC_ISR_WUTWF ) != RTC_ISR_WUTWF){
		// TODOD: A Time out
	} 
	
	// --> Configure the wakeup auto-reload value(WUT) and the wakeup clock selection (WUCKSEL) in Control register (CR) <-- //
	
	// select the desired clock source
	RTC->CR   |= RTC_CR_WUCKSEL_2;        // select the  ck_spre for clock selection (10xx bits defined in the reference manual)
	
	// configure the counting value
	RTC->WUTR  = (time_in_sec - 1) & RTC_WUTR_WUT_Msk; // configure the counting value
	
	NVIC_set_Priority(RTC_WKUP_IRQn, 0x10, 0x00);
	NVIC_EnableIRQ(RTC_WKUP_IRQn);
	
	//__HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_IT();
	// difference between event mask register and interrupt mask register
	//EXTI->EMR1 |= EXTI_IMR1_IM20;
	EXTI->IMR1 |= EXTI_IMR1_IM20;
  //__HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();
	EXTI->RTSR1 |= EXTI_IMR1_IM20;
	
	// reset the int Wakeup timer flag
	RTC->ISR &= ~RTC_ISR_WUTF;
	
	// enable the wake up timer and enable wake_iup interrupt
	RTC->CR |= RTC_CR_WUTE | RTC_CR_WUTIE;
	
	//7. Exit the initialization mode --> the current calendar counter is automatically laoded
	//RTC->ISR &= (uint32_t)(~RTC_ISR_INIT);
	
	//8. Enable the RTC Registers Write protection --> RTC register can no longer be modified 
	//RTC->WPR = 0xFFU;
}
