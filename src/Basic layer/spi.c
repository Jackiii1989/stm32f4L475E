#include "spi.h"
#include "rcc.h"
#include "InitFunctions.h"

//-------------------------------------------------------------------------------------------------------------
void SPI_deinit (SPI_TypeDef* 	Instance){
	
	// SPE: SPI enable--> 0: disabled, 1; enabled
	Instance->CR1 &= ~SPI_CR1_SPE;
}

void SPI_init(SPI_HandleTypeDef* 	Handle){
	
	uint32_t frxth;
	
	// Enable clock for  SPI
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
	
	/* Align by default the rs fifo threshold on the data size */
  if (Handle->Init.DataSize > SPI_DATASIZE_8BIT)
  {
    frxth = SPI_RXFIFO_THRESHOLD_HF;
  }
  else
  {
    frxth = SPI_RXFIFO_THRESHOLD_QF;
  }

	
	SPI_DISABLE(Handle);
	
	//----------------------------- SPIx CR1 Configuration ------------------------------------//
		// Configure : SPI Mode, Communication Mode, Clock polarity and phase, NSS management,
		// Communication speed, First bit and CRC calculation state 
	Handle->Instance->CR1 |= (Handle->Init.Mode 		   				& (SPI_CR1_MSTR 	| SPI_CR1_SSI)      ) | \
                           (Handle->Init.Direction 	 				& (SPI_CR1_RXONLY | SPI_CR1_BIDIMODE) ) | \
                           (Handle->Init.CLKPolarity 				& SPI_CR1_CPOL											  ) | \
                           (Handle->Init.CLKPhase 	 				& SPI_CR1_CPHA												) | \
                           (Handle->Init.NSS 				 				& SPI_CR1_SSM												  ) | \
                           (Handle->Init.BaudRatePrescaler 	& SPI_CR1_BR_Msk											) | \
                           (Handle->Init.FirstBit  					& SPI_CR1_LSBFIRST										) | \
													 (Handle->Init.CRCCalculation 		& SPI_CR1_CRCEN												);
	
	//----------------------------- SPIx CR2 Configuration ------------------------------------//
		// Configure : NSS management, TI Mode, NSS Pulse, Data size and Rx Fifo threshold
	Handle->Instance->CR2 |=  (( Handle->Init.NSS >> 16U) & SPI_CR2_SSOE	) | \
                            (  Handle->Init.TIMode 			& SPI_CR2_FRF		) | \
                            (  Handle->Init.NSSPMode 		& SPI_CR2_NSSP	) | \
                            (  Handle->Init.DataSize 		& SPI_CR2_DS_Msk) | \
                            (  frxth 										& SPI_CR2_FRXTH	);
	
	
	
}

void SPI_transmit_bloc_mode (SPI_HandleTypeDef* handle, uint8_t *pRXData, uint16_t trSize, uint8_t *pTXData, uint16_t reSize)
 {
 
 	// init intern variable
  uint32_t             txallowed = 	1U		; // Variable used to alternate Rx and Tx during transfer
 	uint16_t _trSize 							 = 	trSize;
 	uint16_t _reSize 							 = 	reSize;
 	
  if ((handle->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) // Check if the SPI is already enabled
  {
		SPI_ENABLE(handle);
  }
 
 	// Transmit and Receive data in 8 Bit mode 
 	if (handle->Init.DataSize == SPI_DATASIZE_8BIT)
 	{
 		if (_trSize > 1U)
    {
			handle->Instance->DR = *((uint16_t *)pTXData);
      pTXData += sizeof(uint16_t);
      _trSize -= 2U;
    }
    else
    {
			*(__IO uint8_t *)&handle->Instance->DR = *pTXData;
      pTXData++;
      _trSize--;
    }
 
 		while (_trSize > 0U)
 		{
			// Check TXE flag 
 			if ( ( (handle->Instance->SR & SPI_SR_TXE) != SPI_SR_TXE) && (txallowed == 1U) ) // transmitter buffer empty?
 			{
 				if (_trSize > 1U)
        {
					handle->Instance->DR = *((uint16_t *)pTXData);
          pTXData += sizeof(uint16_t);
          _trSize -= 2U;
        }
        else
        {
          *(__IO uint8_t *)&handle->Instance->DR = *pTXData;
          pTXData++;
          _trSize--;
        }
        // Next Data is a reception (Rx). Tx not allowed
        txallowed = 0U;
 			}	
 			// Wait until RXNE flag is reset 
      if ( ((handle->Instance->SR & SPI_SR_TXE) == SPI_SR_TXE) && (_reSize > 0U) )
      {
        if (_reSize > 1U)
        {
          *((uint16_t *)pRXData) = (uint16_t)handle->Instance->DR;
          pRXData += sizeof(uint16_t);
          _reSize -= 2U;
 				
          if (_reSize <= 1U)
          {
            // Set RX Fifo threshold before to switch on 8 bit data size
            handle->Instance->CR2 |= SPI_RXFIFO_THRESHOLD;
          }
        }
        else
        {
					(*(uint8_t *)pRXData) = *(__IO uint8_t *)&handle->Instance->DR;
          pRXData++;
          _reSize--;
         }
         /* Next Data is a Transmission (Tx). Tx is allowed */
         txallowed = 1U;
 			 } 
 		 }
 	}
 	else if(handle->Init.DataSize == SPI_DATASIZE_16BIT)
 	{
 		; // NOT DONE YET
 	}
 
 
 }
	
	
	//-----------------------------------------------------  Check the end of the transaction ----------------------------------------------------------
