#include "gpio.h"
#include "InitFunctions.h"
//-------------------------------------------------------------------------------------------------------------
// GPIO FUNCTION DEFINES
//-------------------------------------------------------------------------------------------------------------
/**
  \brief   			 Intrinsict function that finds leading zeros in a variable.
  \details 
  \param [in]    value  is ignored by the processor.
                 If required, a debugger can use it to store additional information about the breakpoint.
 */
uint8_t __find_leading_zeros_in_varaible(unsigned int var){
	return __clz(__rbit(var));
}

/**
  \brief   			 initialite the register for the pin. 
  \details 
  \param [in]    value  is ignored by the processor.
                 If required, a debugger can use it to store additional information about the breakpoint.
 */
void gpio_init_pin(GPIO_init_struct* pin){
	
	
	// enable the corresponding port
	RCC->AHB2ENR |= pin->GPIO_port_act; // TODO: make a better way of doing this
	
	// Configure the speed of the pin
	// 00: Low Speed, 01: Medium Speed, 10: High Speed, 11: Very high Speed
	pin->GPIO_port->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 << (pin->cur_pin_to_int * 2u));
	pin->GPIO_port->OSPEEDR |= (pin->Speed << (pin->cur_pin_to_int * 2u));
	
	// Configure the IO Output Type
	//	0: Output push-pull (reset state), 1:output open-drain
	pin->GPIO_port->OTYPER &= ~(GPIO_OTYPER_OT0 << pin->cur_pin_to_int) ;
	pin->GPIO_port->OTYPER |= (((pin->Mode & GPIO_OUTPUT_TYPE) >> 4u) << pin->cur_pin_to_int);
	
	// Activate the Pull-up or Pull down resistor for the current IO 
		// 00: No-pull up/down, 01: Pull-up, 10: Pull-down, 11: Reserved
  pin->GPIO_port->PUPDR &= ~(GPIO_PUPDR_PUPD0 << (pin->cur_pin_to_int * 2u));
  pin->GPIO_port->PUPDR |= (pin->Pull << (pin->cur_pin_to_int * 2u)); 
	
	// Configure IO Direction mode 
		// (Input: 00, Output:01, Alternate:10 or Analog:11)
	pin->GPIO_port->MODER &= ~(GPIO_MODER_MODE0 << (pin->cur_pin_to_int * 2u));
  pin->GPIO_port->MODER |= ((pin->Mode & GPIO_MODE) << (pin->cur_pin_to_int * 2u));
	
	// In case of Alternate function mode selection 
  if((pin->Mode == GPIO_MODE_AF_PP) || (pin->Mode == GPIO_MODE_AF_OD))
  {
	  // Configure Alternate function mapped with the current IO 
	  pin->GPIO_port->AFR[pin->cur_pin_to_int >> 3u] &= ~(0xFu << ((pin->cur_pin_to_int & 0x07u) * 4u));
    pin->GPIO_port->AFR[pin->cur_pin_to_int >> 3u] |= ((pin->Alternate) << ((pin->cur_pin_to_int & 0x07u) * 4u));
  }
	
	// Configure the External Interrupt or event for the current IO
	if((pin->Mode & EXTI_MODE) == EXTI_MODE)
  {
		// Enable interrupt module
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
		
		// external interrupt configuration: configure which pin and port will be used as interrupt
			// ...
		SYSCFG->EXTICR[pin->cur_pin_to_int >> 2u] &= ~(0x0FuL << (4u * (pin->cur_pin_to_int & 0x03u)));
		SYSCFG->EXTICR[pin->cur_pin_to_int >> 2u] |= (GPIO_PORT_GET_INDEX(pin->GPIO_port) << (4u * (pin->cur_pin_to_int & 0x03u)));
		
		// interupt mask register: enable interrupts	
			// 1:	interrupt is not masked(disabled), 0: interrupt request is masked
		EXTI->IMR1 |= 1<<pin->cur_pin_to_int;
		
		if((pin->Mode & FALLING_EDGE) == FALLING_EDGE){
			
			// interrupt will be configured as falling flank
				// 0: falling trigger disabled, 1: falling triger enabled
			EXTI->FTSR1 |= 1<<pin->cur_pin_to_int;
	  }
		else if((pin->Mode & RISING_EDGE) == RISING_EDGE){
			
			// interrupt will be configured as rising flank
				// 0: rising trigger disabled, 1: rising triger enabled
			EXTI->RTSR1 |= 1<<pin->cur_pin_to_int;
		}
			
	}
}

/**
  \brief   			 Function to init several pins
  \details 
  \param [in]    A pointer to the structure, where the information for the pins are hold.
 */

void gpio_init_pins(GPIO_init_struct* pins){
	
	// the init the gpio we need to position of the bit which defines which pin to activate
	uint8_t temp_bit_position = __find_leading_zeros_in_varaible(pins->pins);
	
	while(temp_bit_position < 31) // value greater then 31 means we only have leading zeros
	{ 
		// we save the position of the pin and call the func to init the pin
		pins->cur_pin_to_int = temp_bit_position;
		gpio_init_pin(pins);
		
		//remove this pin as we are done with it
		pins->pins &= ~(1<<temp_bit_position); 
		
		// look for the next pin to configure
		temp_bit_position = __find_leading_zeros_in_varaible(pins->pins);
	};
}
/**
  \brief   			 Function to set a pin
  \details 
  \param [in]   Providing the port and pin which need to be changed
 */


 void GPIO_pin_set(GPIO_Type* GPIOx, const uint32_t Pins){
	// the lower 2 Bytes are setting  the pins to high (16 pins == 2 Bytes)
	GPIOx->BSRR |= Pins;
}

/**
  \brief   			 Function to reset a pin
  \details 
  \param [in]   Providing the port and pin which need to be changed
 */

void GPIO_pin_reset(GPIO_Type* GPIOx, const uint32_t Pins){
		// the upper 2 Bytes are reset register for the pins
	// that is why '<< GPIO_NUMBER'
	GPIOx->BSRR |= (Pins<< GPIO_NUMBER);
}

/**
  \brief   			 Function to toggle a pin
  \details 
  \param [in]   Providing the port and pin which need to be changed
 */


void GPIO_pin_toggle(GPIO_Type* GPIOx, const uint32_t Pins){
	// get current Ouput Data Register value
	uint32_t odr = GPIOx->ODR;
	
	// Set selected pins that were at low level, and reset ones that were high 
  GPIOx->BSRR = ((odr &  Pins) << GPIO_NUMBER) | (~odr & Pins);
}



//-------------------------------------------------------------------------------------------------------------
// LED FUNCTIONS FUNCTIONS AND DEFINES
//-------------------------------------------------------------------------------------------------------------
const uint32_t 			GPIO_PIN_ARRAY[LEDsize]  			= {GPIO_PIN_5 , GPIO_PIN_14};
GPIO_Type*   				GPIO_PORT_ARRAY[LEDsize] 			= {PORT_GPIOA , PORT_GPIOB};
const uint32_t			GPIO_PORT_ACT_ARRAY[LEDsize]	= {RCC_AHB2ENR_GPIOAEN , RCC_AHB2ENR_GPIOBEN};


void Led_init_array(Led_def* pLed, uint32_t size){

	while(size){
		LED_init(*pLed);	
		++pLed;--size;
		
	} 
	
}

void LED_init(Led_def Led){
	GPIO_init_struct gpio_pin = RESET_GPIO_INIT_STRUCT;
	gpio_pin.cur_pin_to_int = __find_leading_zeros_in_varaible(GPIO_PIN_ARRAY[Led]);
	gpio_pin.GPIO_port = GPIO_PORT_ARRAY[Led];
	gpio_pin.GPIO_port_act = GPIO_PORT_ACT_ARRAY[Led];
	gpio_pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_pin.Mode = GPIO_MODE_OUTPUT_PP;//GPIO_MODE_AF_OD;
	gpio_pin.Pull = GPIO_PULLUP;
	//gpio_pin.Alternate = GPIO_AF4_I2C2;
	gpio_init_pin(&gpio_pin);
}

void Led_on(Led_def Led){
	GPIO_pin_set(GPIO_PORT_ARRAY[Led], GPIO_PIN_ARRAY[Led]);
}

void Led_off(Led_def Led){
	GPIO_pin_reset(GPIO_PORT_ARRAY[Led], GPIO_PIN_ARRAY[Led]);
}

void Led_toggle(Led_def Led){
	GPIO_pin_toggle(GPIO_PORT_ARRAY[Led], GPIO_PIN_ARRAY[Led]);
}
//-------------------------------------------------------------------------------------------------------------
// LED FUNCTIONS FUNCTIONS AND DEFINES
//-------------------------------------------------------------------------------------------------------------

#define BUTTONsize                          ((uint8_t)1)

static const uint32_t 			GPIO_BUTTON_PIN_ARRAY[BUTTONsize]  			= {GPIO_PIN_13};
static GPIO_Type*   				GPIO_BUTTON_PORT_ARRAY[BUTTONsize] 			= {PORT_GPIOC };
static const uint32_t			  GPIO_BUTTON_ACT_ARRAY[BUTTONsize]				= {RCC_AHB2ENR_GPIOCEN};
static const IRQn_Type      BUTTON_IRQn[BUTTONsize] 								= {EXTI15_10_IRQn};
static const uint32_t				BUTTON_CONF_ARRAY[]											= {GPIO_MODE_INPUT, EXTI_MODE};

static Button_state_def current_USER_button_state;

void BUTTON_init(Button_def button, Button_state_def button_state){
	
	GPIO_init_struct gpio_pin = RESET_GPIO_INIT_STRUCT;
	
	current_USER_button_state = button_state; 
	
	gpio_pin.cur_pin_to_int = __find_leading_zeros_in_varaible(GPIO_BUTTON_PIN_ARRAY[button]);
	gpio_pin.GPIO_port = GPIO_BUTTON_PORT_ARRAY[button];
	gpio_pin.GPIO_port_act = GPIO_BUTTON_ACT_ARRAY[button];
	gpio_pin.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_pin.Mode =  BUTTON_CONF_ARRAY[button_state]; // 0:polling, 1: interrupt
	gpio_pin.Pull = GPIO_PULLUP;
	gpio_init_pin(&gpio_pin);
	
	
	if (current_USER_button_state == INTERRUPT_BUTTON)
	{
		set_Priority((IRQn_Type)(BUTTON_IRQn[button]), 0x0F, 0x00);
		__COMPILER_BARRIER();
		NVIC->ISER[(((uint32_t)BUTTON_IRQn[button]) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)BUTTON_IRQn[button]) & 0x1FUL));
		__COMPILER_BARRIER();
	}
	
}

void BUTTON_blocking_until_pressed(Button_def button){
		if( current_USER_button_state == INTERRUPT_BUTTON)
	{
		// you should never call this function if it is defined as interrupt.
		while(1){};
	}	
	while(get_button_state(button) == GPIO_PIN__STATE_RESET){/*empty body*/}; 
	while(get_button_state(button) == GPIO_PIN__STATE_SET){/*empty body*/}; 
}

GPIO_Pin_def get_button_state(Button_def button){
	
	if ((GPIO_BUTTON_PORT_ARRAY[button]->IDR & GPIO_BUTTON_PIN_ARRAY[USER_BUTTON]) != 0x00u)
	{
		return GPIO_PIN__STATE_SET;
	}
	else
	{
		return GPIO_PIN__STATE_RESET;
	}
	
}

/**
  \brief   			 Read Pin Value
  \details 
  \param [in]    value  is ignored by the processor.
                 If required, a debugger can use it to store additional information about the breakpoint.
 */
GPIO_Pin_def read_pin(GPIO_Type* GPIOx, const uint32_t GPIO_Pin){
	
	if ((GPIOx->IDR & GPIO_Pin) != 0x00u)
	{
		return GPIO_PIN__STATE_SET;
	}
	else
	{
		return GPIO_PIN__STATE_RESET;
	}
	
}


void USER_BUTTON_callback_function(void){
	// do something
	
}

