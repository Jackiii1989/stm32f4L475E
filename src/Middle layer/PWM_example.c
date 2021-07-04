#include "tim.h"
#include "gpio.h"
#include "rcc.h"
#include "InitFunctions.h"

#define PWM_LENGTH                          ((uint8_t)1)
static const uint32_t 			GPIO_PWM_PIN_ARRAY[PWM_LENGTH]  									= {GPIO_PIN_15};
static const uint32_t 			GPIO_PWM_ALTERNATIVE_FUNCTION_PIN[PWM_LENGTH]  		= {GPIO_AF2_TIM2};
static GPIO_Type*   				GPIO_PWM_PORT_ARRAY[PWM_LENGTH] 									= {PORT_GPIOA };
static const uint32_t			  GPIO_PWM_ACT_PORT[PWM_LENGTH]										  = {RCC_AHB2ENR_GPIOAEN};

/* Exported functions ------------------------------------------------------------------------------ */
uint32_t 			get_SystemClock				(void);

void init_timer(void)
{
	{
		GPIO_init_struct gpio_pin;
		gpio_pin.cur_pin_to_int = __find_leading_zeros_in_varaible(GPIO_PIN_4);
		gpio_pin.GPIO_port = PORT_GPIOA;
		gpio_pin.GPIO_port_act = RCC_AHB2ENR_GPIOAEN;
		gpio_pin.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_pin.Pull = GPIO_PULLUP;
		gpio_pin.Speed = GPIO_SPEED_FREQ_HIGH;
		//gpio_pin.Alternate = GPIO_PWM_ALTERNATIVE_FUNCTION_PIN[0];
		gpio_init_pin(&gpio_pin);
		
		GPIO_pin_toggle(PORT_GPIOA, GPIO_PIN_4);
	  GPIO_pin_toggle(PORT_GPIOA, GPIO_PIN_4);
	}

	
	GPIO_init_struct gpio_pin;
	gpio_pin.cur_pin_to_int = __find_leading_zeros_in_varaible(GPIO_PWM_PIN_ARRAY[0]);
	gpio_pin.GPIO_port = GPIO_PWM_PORT_ARRAY[0];
	gpio_pin.GPIO_port_act = GPIO_PWM_ACT_PORT[0];
	gpio_pin.Mode = GPIO_MODE_AF_PP;
	gpio_pin.Pull = GPIO_PULLUP;
	gpio_pin.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpio_pin.Alternate = GPIO_PWM_ALTERNATIVE_FUNCTION_PIN[0];
	gpio_init_pin(&gpio_pin);

	// TIM2 Clock enable
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
  set_Priority(TIM2_IRQn, 0x0E, 0x00);
	//set_Priority(TIM2_IRQn, 0x0E, 0x00);
  //HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[button]));
 	__COMPILER_BARRIER();
  NVIC->ISER[(((uint32_t)TIM2_IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)TIM2_IRQn) & 0x1FUL));
  __COMPILER_BARRIER();
	// PWM Mode
	// PWM Mode can be selected independently on each channel  by writting in the OCxM bits in teh TIMc_CCMRx register.
	// The coresponding 

	uint32_t SystemCoreClock_loc = get_SystemClock();
	TIM_Base_config_init base_config; 
	base_config.Period = 100000;
	//base_config.PulseCC1 = base_config.Period/3;
	base_config.CounterMode = TIM_COUNTERMODE_UP;
	base_config.ClockDivision = 0;
	base_config.Prescaler = SystemCoreClock_loc/1000000;
	base_config.IntrptEn = UE_INTR_EN + CC1_INTR_EN;
	TIM_base_config(TIM2,  base_config);
	
	
	// NOTE: It does not work the below code
	//TIM_OC_config_init c1Config;
	//c1Config.OCMode =  TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; 
	//c1Config.OCPolarity = TIM_CCER_CC1P; // set the output polarity to high
	//c1Config.OCNPolarity = 0;
	//c1Config.Pulse = base_config.Period/3;
	//TIM_OC1_ConfigChannel(TIM2,c1Config);
	////TIM_CC1ChannelEnable(TIM2,TIM_CHANNEL_1);
		
	
	
	TIM_enable(TIM2);
}
