#ifndef SPI_H
#define SPI_H

#ifdef __cplusplus
 extern "C" {
#endif
//**************************************************************************************************************
#include "stm32l475xx.h"

//SPI communication protocol< -----------------------------------------------------------------------------------------

#define SPI_CR1_MODE_SLAVE                  (0x00000000U)
#define SPI_CR1_MODE_MASTER                 (SPI_CR1_MSTR | SPI_CR1_SSI)


/** @defgroup SPI_Direction SPI Direction Mode
  * @{
  */
#define SPI_DIRECTION_2LINES            (0x00000000U)
#define SPI_DIRECTION_2LINES_RXONLY     SPI_CR1_RXONLY
#define SPI_DIRECTION_1LINE             SPI_CR1_BIDIMODE
/**
  * @}
  */

/** @defgroup SPI_Data_Size SPI Data Size
  * @{
  */
#define SPI_DATASIZE_4BIT               (0x00000300U)
#define SPI_DATASIZE_5BIT               (0x00000400U)
#define SPI_DATASIZE_6BIT               (0x00000500U)
#define SPI_DATASIZE_7BIT               (0x00000600U)
#define SPI_DATASIZE_8BIT               (0x00000700U)
#define SPI_DATASIZE_9BIT               (0x00000800U)
#define SPI_DATASIZE_10BIT              (0x00000900U)
#define SPI_DATASIZE_11BIT              (0x00000A00U)
#define SPI_DATASIZE_12BIT              (0x00000B00U)
#define SPI_DATASIZE_13BIT              (0x00000C00U)
#define SPI_DATASIZE_14BIT              (0x00000D00U)
#define SPI_DATASIZE_15BIT              (0x00000E00U)
#define SPI_DATASIZE_16BIT              (0x00000F00U)
/**
  * @}
  */

/** @defgroup SPI_Clock_Polarity SPI Clock Polarity
  * @{
  */
#define SPI_POLARITY_LOW                (0x00000000U)
#define SPI_POLARITY_HIGH               SPI_CR1_CPOL
/**
  * @}
  */

/** @defgroup SPI_Clock_Phase SPI Clock Phase
  * @{
  */
#define SPI_PHASE_1EDGE                 (0x00000000U)
#define SPI_PHASE_2EDGE                 SPI_CR1_CPHA
/**
  * @}
  */

/** @defgroup SPI_Slave_Select_management SPI Slave Select Management
  * @{
  */
#define SPI_NSS_SOFT                    SPI_CR1_SSM
#define SPI_NSS_HARD_INPUT              (0x00000000U)
#define SPI_NSS_HARD_OUTPUT             (SPI_CR2_SSOE << 16U)
/**
  * @}
  */

/** @defgroup SPI_NSSP_Mode SPI NSS Pulse Mode
  * @{
  */
#define SPI_NSS_PULSE_ENABLE            SPI_CR2_NSSP
#define SPI_NSS_PULSE_DISABLE           (0x00000000U)
/**
  * @}
  */

/** @defgroup SPI_BaudRate_Prescaler SPI BaudRate Prescaler
  * @{
  */
#define SPI_BAUDRATEPRESCALER_2         (0x00000000U)
#define SPI_BAUDRATEPRESCALER_4         (SPI_CR1_BR_0)
#define SPI_BAUDRATEPRESCALER_8         (SPI_CR1_BR_1)
#define SPI_BAUDRATEPRESCALER_16        (SPI_CR1_BR_1 | SPI_CR1_BR_0)
#define SPI_BAUDRATEPRESCALER_32        (SPI_CR1_BR_2)
#define SPI_BAUDRATEPRESCALER_64        (SPI_CR1_BR_2 | SPI_CR1_BR_0)
#define SPI_BAUDRATEPRESCALER_128       (SPI_CR1_BR_2 | SPI_CR1_BR_1)
#define SPI_BAUDRATEPRESCALER_256       (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)
/**
  * @}
  */

/** @defgroup SPI_MSB_LSB_transmission SPI MSB LSB Transmission
  * @{
  */
#define SPI_FIRSTBIT_MSB                (0x00000000U)
#define SPI_FIRSTBIT_LSB                SPI_CR1_LSBFIRST
/**
  * @}
  */

/** @defgroup SPI_TI_mode SPI TI Mode
  * @{
  */
#define SPI_TIMODE_DISABLE              (0x00000000U)
#define SPI_TIMODE_ENABLE               SPI_CR2_FRF
/**
  * @}
  */

/** @defgroup SPI_CRC_Calculation SPI CRC Calculation
  * @{
  */
#define SPI_CRCCALCULATION_DISABLE      (0x00000000U)
#define SPI_CRCCALCULATION_ENABLE       SPI_CR1_CRCEN
/**
  * @}
  */

/** @defgroup SPI_CRC_length SPI CRC Length
  * @{
  * This parameter can be one of the following values:
  *     SPI_CRC_LENGTH_DATASIZE: aligned with the data size
  *     SPI_CRC_LENGTH_8BIT    : CRC 8bit
  *     SPI_CRC_LENGTH_16BIT   : CRC 16bit
  */
#define SPI_CRC_LENGTH_DATASIZE         (0x00000000U)
#define SPI_CRC_LENGTH_8BIT             (0x00000001U)
#define SPI_CRC_LENGTH_16BIT            (0x00000002U)


/** @defgroup SPI_FIFO_reception_threshold SPI FIFO Reception Threshold
  * @{
  * This parameter can be one of the following values:
  *     SPI_RXFIFO_THRESHOLD or SPI_RXFIFO_THRESHOLD_QF :
  *          RXNE event is generated if the FIFO
  *          level is greater or equal to 1/4(8-bits).
  *     SPI_RXFIFO_THRESHOLD_HF: RXNE event is generated if the FIFO
  *          level is greater or equal to 1/2(16 bits). */
#define SPI_RXFIFO_THRESHOLD            SPI_CR2_FRXTH
#define SPI_RXFIFO_THRESHOLD_QF         SPI_CR2_FRXTH
#define SPI_RXFIFO_THRESHOLD_HF         (0x00000000U)

#define SPI_DISABLE(__HANDLE__) (__HANDLE__)->Instance->CR1 &= ~SPI_CR1_SPE;

#define SPI_ENABLE(__HANDLE__)  (__HANDLE__)->Instance->CR1 |= SPI_CR1_SPE;

typedef enum 
{
	SPI_2,
	SPI_3
}SPI_enum;

typedef struct
{
  uint32_t Mode;                // Specifies the SPI operating mode. This parameter can be a value of @ref SPI_Mode
	
  uint32_t Direction;           // Specifies the SPI bidirectional mode state. This parameter can be a value of @ref SPI_Direction
	
  uint32_t DataSize;            // Specifies the SPI data size. This parameter can be a value of @ref SPI_Data_Size
	
  uint32_t CLKPolarity;         // Specifies the serial clock steady state. This parameter can be a value of @ref SPI_Clock_Polarity
	
  uint32_t CLKPhase;            // Specifies the clock active edge for the bit capture. This parameter can be a value of @ref SPI_Clock_Phase
	
  uint32_t NSS;                 // Specifies whether the NSS signal is managed by hardware (NSS pin) or by software using the SSI bit. This parameter can be a value of @ref SPI_Slave_Select_management
	
	uint32_t BaudRatePrescaler;   // Specifies the Baud Rate prescaler value which will be used to configure the transmit and receive SCK clock. This parameter can be a value of @ref SPI_BaudRate_Prescaler 
																// @note The communication clock is derived from the master clock. The slave clock does not need to be set. 
	
  uint32_t FirstBit;            // Specifies whether data transfers start from MSB or LSB bit. This parameter can be a value of @ref SPI_MSB_LSB_transmission
	
  uint32_t TIMode;              // Specifies if the TI mode is enabled or not. This parameter can be a value of @ref SPI_TI_mode 
	
  uint32_t CRCCalculation;      // Specifies if the CRC calculation is enabled or not. This parameter can be a value of @ref SPI_CRC_Calculation

  uint32_t CRCPolynomial;       // Specifies the polynomial used for the CRC calculation. This parameter must be an odd number between Min_Data = 1 and Max_Data = 65535

  uint32_t CRCLength;           // Specifies the CRC Length used for the CRC calculation. CRC Length is only used with Data8 and Data16, not other data size. This parameter can be a value of @ref SPI_CRC_length

  uint32_t NSSPMode;            // Specifies whether the NSSP signal is enabled or not. This parameter can be a value of @ref SPI_NSSP_Mode This mode is activated by the NSSP bit in the SPIx_CR2 register and
                                // it takes effect only if the SPI interface is configured as Motorola SPI master (FRF=0) with capture on the first edge (SPIx_CR1 CPHA = 0, CPOL setting is ignored)..
} SPI_InitTypeDef;




typedef struct __SPI_HandleTypeDef
{
  SPI_TypeDef                *Instance;      /*!< SPI registers base address               */

  SPI_InitTypeDef            Init;           /*!< SPI communication parameters             */

} SPI_HandleTypeDef;

void 				SPI_init 								(SPI_HandleTypeDef* 	Instance);
void 				SPI_transmit_bloc_mode 	(SPI_HandleTypeDef* handle, uint8_t *pRXData, uint16_t trSize, uint8_t *pTXData, uint16_t reSize);
//void 				I2C_write				(I2C_def_hardware* I2Cx, uint8_t DevAddress, uint8_t Reg, uint8_t* val, uint8_t NumbOfReads);

//**************************************************************************************************************

#ifdef __cplusplus
}
#endif

#endif /* ISPI_H */
