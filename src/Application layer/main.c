

/* Includes ------------------------------------------------------------------*/
#include "main.h"

int main(void)
{
	
	// configure the systick timer and enable FLASH prefetch
	init_coreM4();
  
  /* Configure the System clock to have a frequency of 80 MHz */
	periph_init_M4();

  /* Configure the User LED */
  //BSP_LED_Init(LED2);
  //LED_init( USER_LED2);
	Led_def Led[] = {USER_LED2, USER_LED1};
	Led_init_array(Led, 2);
	
	
  //Led_on(USER_LED2);
	//delay_ms(3);
	//Led_on(USER_LED1);
	//
	//Led_toggle(USER_LED2);
	//delay_ms(4);
	//Led_toggle(USER_LED1);
	
	//Led_toggle(USER_LED2);
	//Led_toggle(USER_LED1);
	//
	//Led_off(USER_LED2);
	//Led_off(USER_LED1);
	
	//com_port_init(VIRT_COM1);
  //transmit("Press Button");
	
	
	//BUTTON_init(USER_BUTTON, POLLING_BUTTON);
	//BUTTON_init(USER_BUTTON, INTERRUPT_BUTTON);
	//BUTTON_blocking_until_pressed(USER_BUTTON);
  
 
	
	
	//transmit("We are starting");
	//transmit("second transmit");
	//Led_on(USER_LED2);
	//HTS221_temperature_sensor_init();
	//Led_on(USER_LED1);   
	//volatile uint32_t temperature;
	//temperature = 0;
  //init_timer();
	
	// RTC example
		//RTC_init_struct rtc_init;
		//rtc_init.clk = LSI;
		//rtc_init.time.Seconds = 17;
		//rtc_init.time.Minutes = 42;
		//rtc_init.time.Hours   = 10;
		//rtc_init.enable_wake_Up = 1;
		//rtc_init.WakeUpTimeInSec = 2;
		//RTC_init(&rtc_init);
	
	
	ISM43362_wifi_init();
	
	while(1)
 {
	//temperature = HTS221_get_temperature();
 };
}


/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case (GPIO_PIN_1):
    {
      //SPI_WIFI_ISR();
      break;
    }
    default:
    {
      break;
    }
  }
}

/*****************************END OF FILE***********************************************/
