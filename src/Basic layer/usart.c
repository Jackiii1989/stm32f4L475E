#include "usart.h"
#include "gpio.h"
#include "rcc.h"

#define COMsize                ((uint8_t)1)


static const uint32_t 			GPIO_COMPORT_TX_PIN_ARRAY[COMsize]  			= {GPIO_PIN_6};
static const uint32_t 			GPIO_COMPORT_RX_PIN_ARRAY[COMsize]  			= {GPIO_PIN_7};
static GPIO_Type*   				GPIO_COMPORT_PORT_ARRAY[COMsize] 			= {PORT_GPIOB };
static const uint32_t			  GPIO_COMPORT_ACT_ARRAY[COMsize]				= {RCC_AHB2ENR_GPIOBEN};

//static const IRQn_Type      COMPORT_IRQn[COMsize] 								= {EXTI15_10_IRQn};
static USART_Def* 					COM_USART[COMsize] 										= {USART1_STRUCT};


void com_port_init(Vir_com_def com_port){
	
	GPIO_init_struct gpio_pin = RESET_GPIO_INIT_STRUCT;
	gpio_pin.pins = GPIO_COMPORT_TX_PIN_ARRAY[com_port] |GPIO_COMPORT_RX_PIN_ARRAY[com_port];
	gpio_pin.GPIO_port = GPIO_COMPORT_PORT_ARRAY[com_port];
	gpio_pin.GPIO_port_act = GPIO_COMPORT_ACT_ARRAY[com_port];
	gpio_pin.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_pin.Mode = GPIO_MODE_AF_PP;//GPIO_MODE_AF_OD;
	gpio_pin.Pull = GPIO_NOPULL;
	gpio_pin.Alternate = GPIO_AF7_USART1;
	gpio_init_pins(&gpio_pin);
	
	uart_init(COM_USART[com_port]);
	
}

void uart_init(USART_Def* USARTx){
	
	uint32_t usartdiv;
	uint32_t pclk;
	if(USARTx == USART1_STRUCT)
	{
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	}
	
	//Disable UART.
	USARTx->CR1 &= ~USART_CR1_UE; 
	
	//-------------------------- USART CR1 Configuration: communication paramters ------------------------------------------
	// reset the register
	USARTx->CR1 &= ~(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8 ); 
 // 8-bit word length, no parity, RX & TX mode (dual) and oversampling 115200
	USARTx->CR1 |= (UART_WORDLENGTH_8B | UART_PARITY_NONE | (USART_CR1_TE |USART_CR1_RE) | UART_OVERSAMPLING_16);

	//-------------------------- USART CR2 Configuration: STOP Bit ------------------------------------------
	// add a stop bit to the message
	USARTx->CR2 &= ~USART_CR2_STOP;
	USARTx->CR2 |= UART_STOPBITS_1;
	
	/*-------------------------- USART CR3 Configuration: Flow Control -------------------------------------------------*/
	// no flow control
	USARTx->CR3 &= ~(USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT);
	USARTx->CR3 |= UART_HWCONTROL_NONE;
	
	// confgiure the BAUDRATE with the USARTDIV  prescaler 
	// clock source is in this case UART_CLOCKSOURCE_PCLK2
	pclk = GetSysClockFreq_RCC()>> (APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos] & 0x1FU);
	usartdiv = (uint16_t)((pclk + (115200/2U) )/115200);
	USARTx->BRR |= usartdiv;
	
	
	
	
	// In asynchronous mode, the following bits must be kept cleared:
  //	- LINEN and CLKEN bits in the USART_CR2 register,
  //	- SCEN, HDSEL and IREN  bits in the USART_CR3 register.
	USARTx->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USARTx->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
	
	//Enable UART
	USARTx->CR1 |= USART_CR1_UE; 
	
	//--------------------> Check the UART Idle State. <---------------------------------------
	
	/* Check if the Transmitter is enabled */
	if((USARTx->CR1 & USART_CR1_TE) == USART_CR1_TE){
		// Wait until TEACK flag is set 
		if((USARTx->ISR & USART_ISR_TEACK) == RESET){
			while(1){};
		}
	}
	
	if((USARTx->CR1 & USART_CR1_RE) == USART_CR1_RE){
		// Wait until REACK flag is set 
		if((USARTx->ISR & USART_ISR_REACK) == RESET){
			while(1){};
		}
	}
}	
	
 void transmit(char *data){
	
	uint8_t  *pData = (uint8_t*) data;
	 
	USART_Def* USARTx = COM_USART[VIRT_COM1]; // get the module
	uint16_t size =  strlen((char*)data);
	while (size){
		
		// check if data is transfer in the shift register
		if((USARTx->ISR & USART_ISR_TXE) != USART_ISR_TXE){
			// data is not in the shift register
			continue;
		}
		
		//tmpreg[0] = (USARTx->ISR);
		USARTx->TDR = *pData;
		//tmpreg[1] = USARTx->ISR;
		//wait till the transmit is complete
		while((USARTx->ISR & USART_ISR_TC) != USART_ISR_TC){
			; // till we are here, the trasmitions is not complete
		}
		// decrement size and increment memory adddress of the payload
		--size;++pData;
		//tmpreg[0] = (uint32_t)0;
		//tmpreg[1] = (uint32_t)0;
		
	}
	
	//--------------------> Send a whitespace for better spacing<---------------------------------------
	
	// check if data is transfer in the shift register
	if((USARTx->ISR & USART_ISR_TXE) == USART_ISR_TXE){
		
		// whitespace in hex
		USARTx->TDR = 0x0A;		
		
		//wait till the transmit is complete
		while((USARTx->ISR & USART_ISR_TC) != USART_ISR_TC){
				; // till we are here, the trasmitions is not complete
		}
	}
		
}

