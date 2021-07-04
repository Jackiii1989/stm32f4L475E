
#include "ISM43362.h"


SPI_HandleTypeDef ism43362_h;
uint8_t Prompt[6];
//-------------------------------------------------------------------------------------------------------------
// ISM43362 (WIFI) FUNCTIONS (USING SPI)
//-------------------------------------------------------------------------------------------------------------
void ISM43362_pins_init(void){
	
	
	GPIO_init_struct gpio_pin = RESET_GPIO_INIT_STRUCT;
	
	GPIO_pin_reset(PORT_GPIOB, GPIO_PIN_13);
	// configure Wake up pin - PB13
	gpio_pin.cur_pin_to_int = __find_leading_zeros_in_varaible(GPIO_PIN_13);
	gpio_pin.GPIO_port = PORT_GPIOB;
	gpio_pin.GPIO_port_act = RCC_AHB2ENR_GPIOBEN;
	gpio_pin.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_pin.Pull = GPIO_NOPULL;
	gpio_pin.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_init_pin(&gpio_pin);
	
	// configure Data ready pin
	gpio_pin.cur_pin_to_int = __find_leading_zeros_in_varaible(GPIO_PIN_1);
	gpio_pin.GPIO_port = PORT_GPIOE;
	gpio_pin.GPIO_port_act = RCC_AHB2ENR_GPIOEEN;
	gpio_pin.Mode = GPIO_MODE_IT_RISING;
	gpio_init_pin(&gpio_pin);
	
	// configure Reset pin PE8
	gpio_pin.cur_pin_to_int = __find_leading_zeros_in_varaible(GPIO_PIN_8);
	gpio_pin.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_pin(&gpio_pin);
	
	// configure SPI NSS pin pin PE0
	GPIO_pin_set(PORT_GPIOE, GPIO_PIN_0);
	gpio_pin.cur_pin_to_int = __find_leading_zeros_in_varaible(GPIO_PIN_0);
	gpio_pin.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpio_init_pin(&gpio_pin);
	
	// configure SPI CLK pin - PC10
	// configure SPI MOSI pin - PC12
	// configure SPI MISO pin - PC11
	gpio_pin.cur_pin_to_int = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	gpio_pin.GPIO_port = PORT_GPIOC;
	gpio_pin.GPIO_port_act = RCC_AHB2ENR_GPIOCEN;
	gpio_pin.Mode = GPIO_MODE_AF_PP;
	gpio_pin.Pull = GPIO_NOPULL;
	gpio_pin.Alternate = GPIO_AF6_SPI3;
	gpio_init_pins(&gpio_pin);
	
}

void ISM43362_wifi_init(void){
	
	uint8_t count = 0;
	
	ISM43362_pins_init();
	ism43362_h.Instance = SPI3;
	ism43362_h.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	ism43362_h.Init.Direction         = SPI_DIRECTION_2LINES;
  ism43362_h.Init.CLKPhase          = SPI_PHASE_1EDGE;
  ism43362_h.Init.CLKPolarity       = SPI_POLARITY_LOW;
  ism43362_h.Init.DataSize          = SPI_DATASIZE_16BIT;
  ism43362_h.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  ism43362_h.Init.TIMode            = SPI_TIMODE_DISABLE;
  ism43362_h.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  ism43362_h.Init.CRCPolynomial     = 0;
  ism43362_h.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
  ism43362_h.Init.NSS               = SPI_NSS_SOFT;
  ism43362_h.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	
	SPI_init(&ism43362_h);
	
	// Set Priority and enable Interrupt for Data Ready pin - PE1
	NVIC_set_Priority(EXTI1_IRQn, ISM43362_INTERFACE_PRIO, 0x00);
	NVIC__EnableIRQ(EXTI1_IRQn);
	
   // Set Priority and enable Interrupt for Data Ready pin - PE1
	NVIC_set_Priority(SPI3_IRQn, ISM43362_INTERFACE_PRIO, 0x00);
	NVIC__EnableIRQ(SPI3_IRQn);
	
	// first call used for calibration
  delay_us(10);
	
	ISM43362_reset_module();
	
	// wifi enable nss ->PE8
	GPIO_pin_reset(PORT_GPIOE, GPIO_PIN_8);
	delay_ms(100);
	
	// wait while the chip is ready
	while(read_pin(PORT_GPIOE, GPIO_PIN_1) ==  GPIO_PIN__STATE_SET){
		SPI_transmit_bloc_mode(&ism43362_h , &Prompt[count], (uint16_t) 1, &Prompt[count], (uint16_t) 1);
		count += 2;
		
	};
			
	
	
}

void ISM43362_reset_module (void){
	
	ISM43362_toggle_reset_pin();
	GPIO_pin_reset(PORT_GPIOE, GPIO_PIN_0); // enable NSS PE0
	delay_ms(100);
}

void ISM43362_toggle_reset_pin(void){
	
	uint32_t tickstart = HAL_GetTick();
	
	GPIO_pin_reset(PORT_GPIOE, GPIO_PIN_8);
	delay_ms(500);
	GPIO_pin_set(PORT_GPIOE, GPIO_PIN_8);
	delay_ms(500);
}
