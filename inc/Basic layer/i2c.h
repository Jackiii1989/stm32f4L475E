#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
 extern "C" {
#endif
//**************************************************************************************************************


#include "stm32l475xx.h"

//I2C communication protocol< -----------------------------------------------------------------------------------------

#define TIMING_CLEAR_MASK     					(0xF0FFFFFFU) 
#define I2Cx_TIMING     								((uint32_t)0x00702681)
#define I2C_ADDRESSINGMODE_7BIT   			(0x00000001U)
#define I2C_DUALADDRESS_DISABLE   			(0x00000000U)
#define I2C_GENERALCALL_DISABLE   			(0x00000000U)
#define I2C_NOSTRETCH_DISABLE     			(0x00000000U)

#define  I2C_NO_STARTSTOP               (0x00000000U)
#define  I2C_GENERATE_STOP              (uint32_t)(0x80000000U | I2C_CR2_STOP)
#define  I2C_GENERATE_START_READ        (uint32_t)(0x80000000U | I2C_CR2_START | I2C_CR2_RD_WRN)
#define  I2C_GENERATE_START_WRITE       (uint32_t)(0x80000000U | I2C_CR2_START)

#define I2C_ANALOGFILTER_ENABLE   0x00000000U
//#define I2C_ANALOGFILTER_DISABLE  I2C_CR1_ANFOFF

#define I2C_MEMADD_SIZE_8BIT      (0x00000001U)

typedef enum 
{
	I2C_1 = 0,
	I2C_2	
}I2C_enum;


typedef struct
{
  volatile uint32_t CR1;         /*!< I2C Control register 1,            Address offset: 0x00 */
  volatile uint32_t CR2;         /*!< I2C Control register 2,            Address offset: 0x04 */
  volatile uint32_t OAR1;        /*!< I2C Own address 1 register,        Address offset: 0x08 */
  volatile uint32_t OAR2;        /*!< I2C Own address 2 register,        Address offset: 0x0C */
  volatile uint32_t TIMINGR;     /*!< I2C Timing register,               Address offset: 0x10 */
  volatile uint32_t TIMEOUTR;    /*!< I2C Timeout register,              Address offset: 0x14 */
  volatile uint32_t ISR;         /*!< I2C Interrupt and status register, Address offset: 0x18 */
  volatile uint32_t ICR;         /*!< I2C Interrupt clear register,      Address offset: 0x1C */
  volatile uint32_t PECR;        /*!< I2C PEC register,                  Address offset: 0x20 */
  volatile uint32_t RXDR;        /*!< I2C Receive data register,         Address offset: 0x24 */
  volatile uint32_t TXDR;        /*!< I2C Transmit data register,        Address offset: 0x28 */
} I2C_def_hardware;

#define I2C_1                			((I2C_def_hardware*) I2C1_BASE)
#define I2C_2                			((I2C_def_hardware*) I2C2_BASE)
#define I2C_3                			((I2C_def_hardware*) I2C3_BASE)
typedef struct
{
	I2C_def_hardware* I2Cx;				// register I2C base address
	uint32_t Timing;              // Specifies the I2C_TIMINGR_register value.This parameter calculated	by referring to I2C initialization section in Reference manual
  uint32_t OwnAddress1;         // Specifies the first device own address. This parameter can be a 7-bit or 10-bit address. 
  uint32_t AddressingMode;      // Specifies if 7-bit or 10-bit addressing mode is selected. This parameter can be a value of @ref I2C_ADDRESSING_MODE 
  uint32_t DualAddressMode;     // Specifies if dual addressing mode is selected. This parameter can be a value of @ref I2C_DUAL_ADDRESSING_MODE 
  uint32_t OwnAddress2;         // Specifies the second device own address if dual addressing mode is selected. This parameter can be a 7-bit address.
  uint32_t OwnAddress2Masks;    // Specifies the acknowledge mask address second device own address if dual addressing mode is selected This parameter can be a value of @ref I2C_OWN_ADDRESS2_MASKS 
  uint32_t GeneralCallMode;     // Specifies if general call mode is selected.This parameter can be a value of @ref I2C_GENERAL_CALL_ADDRESSING_MODE 
  uint32_t NoStretchMode;       // Specifies if nostretch mode is selected.This parameter can be a value of @ref I2C_NOSTRETCH_MODE 
	uint32_t AnalogFilter;        // Specifies the analog filter
	
	
}I2C_init_struct;

void 				I2C_init				(I2C_init_struct* i2c_init);
void 				I2C_read				(I2C_def_hardware* I2Cx, uint8_t DevAddress, uint8_t Reg, uint8_t* val, uint8_t NumbOfReads);
void 				I2C_write				(I2C_def_hardware* I2Cx, uint8_t DevAddress, uint8_t Reg, uint8_t* val, uint8_t NumbOfReads);

//**************************************************************************************************************

#ifdef __cplusplus
}
#endif

#endif /* I2C_H */
