#ifndef USART_H
#define USART_H

#ifdef __cplusplus
 extern "C" {
#endif
//**************************************************************************************************************


#include "stm32l475xx.h"
#include <string.h>

typedef struct
{
  volatile uint32_t CR1;         /*!< USART Control register 1,                 Address offset: 0x00 */
  volatile uint32_t CR2;         /*!< USART Control register 2,                 Address offset: 0x04 */
  volatile uint32_t CR3;         /*!< USART Control register 3,                 Address offset: 0x08 */
  volatile uint32_t BRR;         /*!< USART Baud rate register,                 Address offset: 0x0C */
  volatile uint16_t GTPR;        /*!< USART Guard time and prescaler register,  Address offset: 0x10 */
  uint16_t  RESERVED2;       		 /*!< Reserved, 0x12                                                 */
  volatile uint32_t RTOR;        /*!< USART Receiver Time Out register,         Address offset: 0x14 */
  volatile uint16_t RQR;         /*!< USART Request register,                   Address offset: 0x18 */
  uint16_t  RESERVED3;       		 /*!< Reserved, 0x1A                                                 */
  volatile uint32_t ISR;         /*!< USART Interrupt and status register,      Address offset: 0x1C */
  volatile uint32_t ICR;         /*!< USART Interrupt flag Clear register,      Address offset: 0x20 */
  volatile uint16_t RDR;         /*!< USART Receive Data register,              Address offset: 0x24 */
  uint16_t  RESERVED4;       		 /*!< Reserved, 0x26                                                 */
  volatile uint16_t TDR;         /*!< USART Transmit Data register,             Address offset: 0x28 */
  uint16_t  RESERVED5;           /*!< Reserved, 0x2A                                                 */
} USART_Def;

#define USART1_STRUCT        ((USART_Def *) USART1_BASE)
#define UART_WORDLENGTH_8B    0x00000000U    // 8-bit long UART frame 
#define UART_STOPBITS_1       0x00000000U    // UART frame with 1 stop bit
#define UART_HWCONTROL_NONE   0x00000000U    /*!< No hardware control       */
#define UART_OVERSAMPLING_16  0x00000000U    /*!< Oversampling by 16 */
#define UART_PARITY_NONE      0x00000000U    /*!< No parity   */
#define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping     */

#define UART_CR1_CLEAR_MASK		~(USART_CR1_M  | USART_CR1_PCE |    \
															  USART_CR1_PS | USART_CR1_TE  |    \
															  USART_CR1_RE | USART_CR1_OVER8)  

#define UART_CR1_CONFIG 			(UART_WORDLENGTH_8B | UART_PARITY_NONE |  \
															(USART_CR1_TE  | USART_CR1_RE) | 115200)



void 							uart_init											(USART_Def* USARTx);


//VIRTUAL COM PORT< -----------------------------------------------------------------------------------------
typedef enum 
{
	VIRT_COM1 = 0
} Vir_com_def;

typedef enum
{
  RESET = 0,
  SET = !RESET
} FlagStatus, ITStatus;

void 							com_port_init										(Vir_com_def com_port);
void 							transmit												(char *data);

//**************************************************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* USART_H */
