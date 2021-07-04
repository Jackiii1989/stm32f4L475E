#include "i2c.h"
#include "gpio.h"
#include "rcc.h"

//-------------------------------------------------------------------------------------------------------------
void I2C_init(I2C_init_struct* i2c_init){
	
	if (i2c_init->I2Cx == I2C_1){
		RCC->APB1ENR1  |=  RCC_APB1ENR1_I2C1EN; // active the I2Cx module
		RCC->APB1RSTR1 |=  RCC_APB1RSTR1_I2C1RST; // set force reset for the clock
		RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST; // release reset for the clock
	}
	else if(i2c_init->I2Cx == I2C_2){
		RCC->APB1ENR1  |=  RCC_APB1ENR1_I2C2EN;
		RCC->APB1RSTR1 |=  RCC_APB1RSTR1_I2C2RST; // set force reset for the clock
		RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C2RST; // release reset for the clock
	}
	else{
		RCC->APB1ENR1  |=  RCC_APB1ENR1_I2C3EN;
		RCC->APB1RSTR1 |=  RCC_APB1RSTR1_I2C3RST; // set force reset for the clock
		RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C3RST; // release reset for the clock
	}
	
	
	// Disable the selected I2Cx peripheral 
	i2c_init->I2Cx->CR1 &= ~I2C_CR1_PE;
	
	// I2Cx timing configuration wich defines the frequency range
	// here we define the SCLL (SCL low period) and SCLH (SCl high period) -  more information in the manual
	i2c_init->I2Cx->TIMINGR = i2c_init->Timing & TIMING_CLEAR_MASK;
	
	// I2Cx OAR1 Configuration: Disable own address & 
	i2c_init->I2Cx->OAR1 &= ~I2C_OAR1_OA1EN;
	i2c_init->I2Cx->OAR1 |= (I2C_OAR1_OA1EN |i2c_init->OwnAddress1);	//// this is apperently not needed in master mode
	
	// I2Cx CR configuration: enable autoend, enable NACK (should be only disabled in Slave process)
	//i2c_init->I2Cx->CR2 |= (I2C_CR2_AUTOEND| I2C_CR2_NACK); // this bit can not be in the register I2C_CR2_NACK);
	
	// IC2x OAR2: disable Own Address2
	i2c_init->I2Cx->OAR2 &= I2C_DUALADDRESS_DISABLE;
	
	// IC2x CR1: general call and Nostretch Mode
	i2c_init->I2Cx->CR1 =i2c_init->GeneralCallMode | i2c_init->NoStretchMode | i2c_init->AnalogFilter;
	
	// Enable I2Cx peripheral 
	//i2c_init->I2Cx->CR1 |= I2C_ANALOGFILTER_ENABLE; // enable analog filter: value 000
	i2c_init->I2Cx->CR1 |= I2C_CR1_PE;
}

//-------------------------------------------------------------------------------------------------------------
void I2C_read(I2C_def_hardware* I2Cx, uint8_t DevAddress, uint8_t Reg, uint8_t* val, uint8_t NumbOfReads){
	
	// wait till the I2C is not bussy - it is cleared when a stop condition has been detected
	while((I2Cx->ISR &I2C_ISR_BUSY) == I2C_ISR_BUSY){
		// #TODO: create some TIMEOUT handling when it is here stuck for ever
		// the I2C building block has been wrong terminated: 
		// 	- eiter a stop request has not been send in the previous communication or something else
	};
	
	// create a pointer to the memory, to easily iterate over
	uint8_t* pVal = val;

	
	// reset the register -init with zero is a save thing
	I2Cx->CR2 = 0;
	
	// set the device address and how many bytes we will send
	I2Cx->CR2 |= (DevAddress & I2C_CR2_SADD) | (1 << I2C_CR2_NBYTES_Pos); 
	
	// start the communication 
	I2Cx->CR2 |= I2C_GENERATE_START_WRITE; 


	// this bit is set by the hardware, when IC2_TXDR is empty or data to be transmitted 
	// The TXIS bit is set after each byte transmission and the number of TXIS events during
	// the transfer corresponds to the value programmed in NBYTES.
	while((I2Cx->ISR &I2C_ISR_TXIS) != I2C_ISR_TXIS){

		// #TODO: create some TIMEOUT handling when it is here stuck for ever	
		
		// check for the NACKF error
		if((I2Cx->ISR &I2C_ISR_NACKF) == I2C_ISR_NACKF)
		{
			while(1)
			{
				// we have an error: the Address was not acknowledge
				// by I2C standard the slave should acknowledge the address, if it is the right one
			}
		}
	}
	
	// send register address
	I2Cx->TXDR = Reg;
	
	
	
	// In the case, when NBYTES data have been transferred, the TCR flag is set and the SCL line is stretched
	// to low until NBYTES is written to a non-zero value. 
	while((I2Cx->ISR & I2C_ISR_TC) != I2C_ISR_TC){
		// #TODO: create some TIMEOUT handling when it is here stuck for ever
	};
	
	//--------------------------------------------------------------------------------------------
	// read  the data of the register of the i2c slave 
	//--------------------------------------------------------------------------------------------
	// how many bytes to read, doing a read operation, activate autoendmode:  STOP is automatically genereated after the last byte 
	I2Cx->CR2 |= (NumbOfReads << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN | I2C_CR2_STOP;// | I2C_CR2_AUTOEND;//| I2C_CR2_AUTOEND;  
	
	// start the measurment 
	I2Cx->CR2 |= I2C_GENERATE_START_WRITE; 
	 
	// loop till I2C_ISR_TC (transfer complete) is not set. The I2C_ISR_TC is set when all the number of bytes,
	// defined in NBYTES has been received
	while((I2Cx->ISR & I2C_ISR_TC) != I2C_ISR_TC){
		// #TODO: create some TIMEOUT handling when it is here stuck for ever
		if((I2Cx->ISR & I2C_ISR_RXNE) == I2C_ISR_RXNE)
		{
			*pVal =I2Cx->RXDR;
			++pVal;
		}
	};

	// generate a stop message if the AUTOEND is not set. 
	/*if( (I2Cx->CR2 & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND)
	{	
		I2Cx->CR2 = I2C_CR2_STOP; 
	}*/
	I2Cx->CR2 = I2C_CR2_STOP;
	// clear the flag
	I2Cx->ICR = I2C_ISR_STOPF; 
}

void I2C_write(I2C_def_hardware* I2Cx, uint8_t DevAddress, uint8_t Reg, uint8_t* val, uint8_t NumbOfReads){
	
	// wait till the I2C is not bussy
	while((I2Cx->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY){
		// #TODO: create some TIMEOUT handling when it is here stuck for ever
		// the I2C building block has been wrong terminated: 
		// 	- eiter a stop request has not been send in the previous communication or something else
	};
	
	// create a pointer to the memory, to easily iterate over
	uint8_t* pVal = val;

	
	// reset the register -init with zero is a save thing
	I2Cx->CR2 = 0;
	
	// set the device address and how many bytes we will send
	I2Cx->CR2 |= (DevAddress & I2C_CR2_SADD) | (1 << I2C_CR2_NBYTES_Pos); 
	
	// start the communication 
	I2Cx->CR2 |= I2C_GENERATE_START_WRITE; 


	// this bit is set by the hardware, when IC2_TXDR is empty or data to be transmitted 
	// The TXIS bit is set after each byte transmission and the number of TXIS events during
	// the transfer corresponds to the value programmed in NBYTES.
	while((I2Cx->ISR & I2C_ISR_TXIS) != I2C_ISR_TXIS){

		// #TODO: create some TIMEOUT handling when it is here stuck for ever	
		
		// check for the NACKF error
		if((I2Cx->ISR &I2C_ISR_NACKF) == I2C_ISR_NACKF)
		{
			while(1)
			{
				// we have an error: the Address was not acknowledge
				// by I2C standard the slave should acknowledge the address, if it is the right one
			}
		}
	}
	
	// send register address
	I2Cx->TXDR = Reg;
	
	
	
	// In the case, when NBYTES data have been transferred, the TCR flag is set and the SCL line is stretched
	// to low until NBYTES is written to a non-zero value. 
	while((I2Cx->ISR & I2C_ISR_TC) != I2C_ISR_TC){
		// #TODO: create some TIMEOUT handling when it is here stuck for ever
	};
	
	//--------------------------------------------------------------------------------------------
	// write  the data of the register of the i2c slave 
	//--------------------------------------------------------------------------------------------
	// how many bytes to read, doing a write operation, activate autoendmode:  STOP is automatically genereated after the last byte 
	I2Cx->CR2 |= (NumbOfReads << I2C_CR2_NBYTES_Pos) | I2C_CR2_STOP;//| I2C_CR2_AUTOEND;  
	
	// start the measurment 
	I2Cx->CR2 |= I2C_GENERATE_START_WRITE; 
	 
	while((I2Cx->ISR & I2C_ISR_TXIS) != I2C_ISR_TXIS){};
		
	I2Cx->TXDR = *pVal;
	++pVal;	
	// loop till I2C_ISR_TC (transfer complete) is not set. The I2C_ISR_TC is set when all the number of byteshas been received.
	// the value is copied from NBYTES 
	while((I2Cx->ISR & I2C_ISR_TC) != I2C_ISR_TC){
		// #TODO: create some TIMEOUT handling when it is here stuck for ever
		if((I2Cx->ISR & I2C_ISR_TXIS) == I2C_ISR_TXIS)
		{
			I2Cx->TXDR = *pVal;
			++pVal;
		}
	};

	// generate a stop message if the AUTOEND is not set. 
	/*if( (I2Cx->CR2 & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND)
	{	
		I2Cx->CR2 = I2C_CR2_STOP; 
	}*/
	I2Cx->CR2 = I2C_CR2_STOP;
	// clear the flag
	I2Cx->ICR = I2C_ISR_STOPF; 
}
