#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l475xx.h"


//-------------------------------------------------------------------------------------------------------------
// GPIO FUNCTIONS FUNCTIONS AND DEFINES
//-------------------------------------------------------------------------------------------------------------

#define  GPIO_MODE_INPUT                        (0x00000000u)   /*!< Input Floating Mode                   */
#define  GPIO_MODE_OUTPUT_PP                    (0x00000001u)   /*!< Output Push Pull Mode                 */
#define  GPIO_MODE_OUTPUT_OD                    (0x00000011u)   /*!< Output Open Drain Mode                */
#define  GPIO_MODE_AF_PP                        (0x00000002u)   /*!< Alternate Function Push Pull Mode     */
#define  GPIO_MODE_AF_OD                        (0x00000012u)   /*!< Alternate Function Open Drain Mode    */
#define  GPIO_MODE_ANALOG                       (0x00000003u)   /*!< Analog Mode  */
#define  GPIO_MODE_ANALOG_ADC_CONTROL           (0x0000000Bu)   /*!< Analog Mode for ADC conversion */
#define  GPIO_MODE_IT_RISING                    (0x10110000u)   /*!< External Interrupt Mode with Rising edge trigger detection          */
#define  GPIO_MODE_IT_FALLING                   (0x10210000u)   /*!< External Interrupt Mode with Falling edge trigger detection         */
#define  GPIO_MODE_IT_RISING_FALLING            (0x10310000u)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */
#define  GPIO_MODE_EVT_RISING                   (0x10120000u)   /*!< External Event Mode with Rising edge trigger detection              */
#define  GPIO_MODE_EVT_FALLING                  (0x10220000u)   /*!< External Event Mode with Falling edge trigger detection             */
#define  GPIO_MODE_EVT_RISING_FALLING           (0x10320000u)   /*!< External Event Mode with Rising/Falling edge trigger detection      */


#define  GPIO_SPEED_FREQ_LOW        						(0x00000000u)   /*!< range up to 5 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_MEDIUM     						(0x00000001u)   /*!< range  5 MHz to 25 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_HIGH       						(0x00000002u)   /*!< range 25 MHz to 50 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_VERY_HIGH  						(0x00000003u)   /*!< range 50 MHz to 80 MHz, please refer to the product datasheet */

#define GPIO_MODE             									(0x00000003u)
#define ANALOG_MODE           									(0x00000008u)
#define EXTI_MODE             									(0x10000000u)
#define GPIO_MODE_IT          									(0x00010000u)
#define GPIO_MODE_EVT         									(0x00020000u)
#define RISING_EDGE           									(0x00100000u)
#define FALLING_EDGE          									(0x00200000u)
#define GPIO_OUTPUT_TYPE      									(0x00000010u)

 /** @defgroup GPIO_pull GPIO pull
   * @brief GPIO Pull-Up or Pull-Down Activation
   * @{
   */
#define  GPIO_NOPULL        										(0x00000000u)   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        										(0x00000001u)   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      										(0x00000002u)   /*!< Pull-down activation                */


#define GPIO_PORT_GET_INDEX(__GPIOx__)    (((__GPIOx__) == (PORT_GPIOA))? 0uL :\
                                      ((__GPIOx__) == (PORT_GPIOB))? 1uL :\
                                      ((__GPIOx__) == (PORT_GPIOC))? 2uL :\
                                      ((__GPIOx__) == (PORT_GPIOD))? 3uL :\
                                      ((__GPIOx__) == (PORT_GPIOE))? 4uL :\
                                      ((__GPIOx__) == (PORT_GPIOF))? 5uL :\
                                      ((__GPIOx__) == (PORT_GPIOG))? 6uL : 7uL)


// my definition
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */


#define PORT_GPIOA               ((GPIO_Type *) GPIOA_BASE)
#define PORT_GPIOB               ((GPIO_Type *) GPIOB_BASE)
#define PORT_GPIOC               ((GPIO_Type *) GPIOC_BASE)
#define PORT_GPIOD               ((GPIO_Type *) GPIOD_BASE)
#define PORT_GPIOE               ((GPIO_Type *) GPIOE_BASE)
#define PORT_GPIOF               ((GPIO_Type *) GPIOF_BASE)
#define PORT_GPIOG               ((GPIO_Type *) GPIOG_BASE)

#define PIN_6                ((uint32_t)		6)  /* Pin 6 selected   */
#define PIN_7                ((uint32_t)		7)  /* Pin 6 selected   */
#define PIN_10               ((uint32_t)		10)  /* Pin 10 selected   */
#define PIN_11               ((uint32_t)		11)  /* Pin 11 selected   */

/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping                       */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping            */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping             */
#if defined(STM32L476xx) || defined(STM32L486xx)
#define GPIO_AF0_LCDBIAS       ((uint8_t)0x00)  /* LCDBIAS Alternate Function mapping                        */
#endif /* STM32L476xx || STM32L486xx */
#define GPIO_AF0_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping                          */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */
#define GPIO_AF1_TIM5          ((uint8_t)0x01)  /* TIM5 Alternate Function mapping */
#define GPIO_AF1_TIM8          ((uint8_t)0x01)  /* TIM8 Alternate Function mapping */
#define GPIO_AF1_LPTIM1        ((uint8_t)0x01)  /* LPTIM1 Alternate Function mapping */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /* IR Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /* TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping */
#define GPIO_AF2_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
#define GPIO_AF2_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */
#define GPIO_AF3_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping  */
#define GPIO_AF3_TIM1_COMP2    ((uint8_t)0x03)  /* TIM1/COMP2 Break in Alternate Function mapping  */
#define GPIO_AF3_TIM1_COMP1    ((uint8_t)0x03)  /* TIM1/COMP1 Break in Alternate Function mapping  */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping   */
#define GPIO_AF4_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping   */
#define GPIO_AF4_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping   */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /* SPI1 Alternate Function mapping   */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2 Alternate Function mapping   */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_SPI3          ((uint8_t)0x06)  /* SPI3 Alternate Function mapping   */
#define GPIO_AF6_DFSDM1        ((uint8_t)0x06)  /* DFSDM1 Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping     */
#define GPIO_AF7_USART2        ((uint8_t)0x07)  /* USART2 Alternate Function mapping     */
#define GPIO_AF7_USART3        ((uint8_t)0x07)  /* USART3 Alternate Function mapping     */

/**
  * @brief   AF 8 selection
  */
#define GPIO_AF8_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping   */
#define GPIO_AF8_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping   */
#define GPIO_AF8_LPUART1       ((uint8_t)0x08)  /* LPUART1 Alternate Function mapping */


/**
  * @brief   AF 9 selection
  */
#define GPIO_AF9_CAN1          ((uint8_t)0x09)  /* CAN1 Alternate Function mapping  */
#define GPIO_AF9_TSC           ((uint8_t)0x09)  /* TSC Alternate Function mapping   */

/**
  * @brief   AF 10 selection
  */
#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
#define GPIO_AF10_OTG_FS       ((uint8_t)0x0A)  /* OTG_FS Alternate Function mapping */
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */
#define GPIO_AF10_QUADSPI      ((uint8_t)0x0A)  /* QUADSPI Alternate Function mapping */

#if defined(STM32L476xx) || defined(STM32L486xx)
/**
  * @brief   AF 11 selection
  */
#define GPIO_AF11_LCD          ((uint8_t)0x0B)  /* LCD Alternate Function mapping */
#endif /* STM32L476xx || STM32L486xx */

/**
  * @brief   AF 12 selection
  */
#define GPIO_AF12_FMC          ((uint8_t)0x0C)  /* FMC Alternate Function mapping     */
#define GPIO_AF12_SWPMI1       ((uint8_t)0x0C)  /* SWPMI1 Alternate Function mapping  */
#define GPIO_AF12_COMP1        ((uint8_t)0x0C)  /* COMP1 Alternate Function mapping   */
#define GPIO_AF12_COMP2        ((uint8_t)0x0C)  /* COMP2 Alternate Function mapping   */
#define GPIO_AF12_SDMMC1       ((uint8_t)0x0C)  /* SDMMC1 Alternate Function mapping  */

/**
  * @brief   AF 13 selection
  */
#define GPIO_AF13_SAI1         ((uint8_t)0x0D)  /* SAI1 Alternate Function mapping */
#define GPIO_AF13_SAI2         ((uint8_t)0x0D)  /* SAI2 Alternate Function mapping */
#define GPIO_AF13_TIM8_COMP2   ((uint8_t)0x0D)  /* TIM8/COMP2 Break in Alternate Function mapping  */
#define GPIO_AF13_TIM8_COMP1   ((uint8_t)0x0D)  /* TIM8/COMP1 Break in Alternate Function mapping  */

/**
  * @brief   AF 14 selection
  */
#define GPIO_AF14_TIM2         ((uint8_t)0x0E)  /* TIM2 Alternate Function mapping */
#define GPIO_AF14_TIM15        ((uint8_t)0x0E)  /* TIM15 Alternate Function mapping */
#define GPIO_AF14_TIM16        ((uint8_t)0x0E)  /* TIM16 Alternate Function mapping */
#define GPIO_AF14_TIM17        ((uint8_t)0x0E)  /* TIM17 Alternate Function mapping */
#define GPIO_AF14_LPTIM2       ((uint8_t)0x0E)  /* LPTIM2 Alternate Function mapping */
#define GPIO_AF14_TIM8_COMP1   ((uint8_t)0x0E)  /* TIM8/COMP1 Break in Alternate Function mapping  */

/**
  * @brief   AF 15 selection
  */
#define GPIO_AF15_EVENTOUT     ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)   			 ((AF) <= (uint8_t)0x0F)

#define GPIO_NUMBER            (16u)

typedef enum
{
	GPIO_PIN__STATE_UNDEFINED = 0U,
  GPIO_PIN__STATE_RESET,
  GPIO_PIN__STATE_SET,
	GPIO_PIN__STATE_ERROR 
}GPIO_Pin_def;

typedef struct
{
  volatile uint32_t MODER;       /*!< GPIO port mode register,               Address offset: 0x00      */
  volatile uint32_t OTYPER;      /*!< GPIO port output type register,        Address offset: 0x04      */
  volatile uint32_t OSPEEDR;     /*!< GPIO port output speed register,       Address offset: 0x08      */
  volatile uint32_t PUPDR;       /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  volatile uint32_t IDR;         /*!< GPIO port input data register,         Address offset: 0x10      */
  volatile uint32_t ODR;         /*!< GPIO port output data register,        Address offset: 0x14      */
  volatile uint32_t BSRR;        /*!< GPIO port bit set/reset  register,     Address offset: 0x18      */
  volatile uint32_t LCKR;        /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  volatile uint32_t AFR[2];      /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
  volatile uint32_t BRR;         /*!< GPIO Bit Reset register,               Address offset: 0x28      */
  volatile uint32_t ASCR;        /*!< GPIO analog switch control register,   Address offset: 0x2C     */

} GPIO_Type;

typedef struct
{
	uint32_t 	 cur_pin_to_int; 	// the current pin to init
  uint32_t 	 pins;        	/*!< Specifies the GPIO pins to be configured. This parameter can be any value of @ref GPIO_pins */
	GPIO_Type* GPIO_port; 			// Define the port;
	uint32_t   GPIO_port_act;		// define which port to activate
  uint32_t 	 Mode;       			/*!< Specifies the operating mode for the selected pins.This parameter can be a value of @ref GPIO_mode */
  uint32_t 	 Pull;       			/*!< Specifies the Pull-up or Pull-Down activation for the selected pins. This parameter can be a value of @ref GPIO_pull */
  uint32_t 	 Speed;      			/*!< Specifies the speed for the selected pins. This parameter can be a value of @ref GPIO_speed */
  uint32_t 	 Alternate;  			/*!< Peripheral to be connected to the selected pins. This parameter can be a value of @ref GPIOEx_Alternate_function_selection */
}GPIO_init_struct;


#define RESET_GPIO_INIT_STRUCT {(uint32_t)   0, \
																(uint32_t)   0, \
																(GPIO_Type*) 0, \
																(uint32_t)   0, \
																(uint32_t)   0, \
																(uint32_t)   0, \
																(uint32_t)   0, \
																(uint32_t)   0 }

void 			gpio_init_pin												(GPIO_init_struct* pin);
void 			gpio_init_pins											(GPIO_init_struct* pin);
void 			GPIO_pin_set												(GPIO_Type* GPIOx, const uint32_t Pin);
void 			GPIO_pin_reset											(GPIO_Type* GPIOx, const uint32_t Pin);
void 			GPIO_pin_toggle											(GPIO_Type* GPIOx, const uint32_t Pin);
uint8_t 	__find_leading_zeros_in_varaible		(unsigned int var);


//-------------------------------------------------------------------------------------------------------------
// LED FUNCTIONS FUNCTIONS AND DEFINES
//-------------------------------------------------------------------------------------------------------------

typedef enum 
{
USER_LED1 = 0,
USER_LED2 = 1
}Led_def;

#define LEDsize                          ((uint8_t)2)


void 							LED_init												(Led_def Led);
void 							Led_on													(Led_def Led);
void 							Led_off													(Led_def Led);
void 							Led_toggle											(Led_def Led);
void 							Led_init_array									(Led_def* Led, uint32_t size);
							

//-------------------------------------------------------------------------------------------------------------
// BUTTON FUNCTIONS FUNCTIONS AND DEFINES
//-------------------------------------------------------------------------------------------------------------
typedef enum 
{
	USER_BUTTON = 0
}Button_def;

typedef enum 
{
	POLLING_BUTTON = 0,
	INTERRUPT_BUTTON
}Button_state_def;

void 							BUTTON_init											(Button_def button, Button_state_def button_state);
GPIO_Pin_def 			read_pin												 (GPIO_Type* GPIOx, const uint32_t GPIO_Pin);
void 							BUTTON_blocking_until_pressed		(Button_def button);
GPIO_Pin_def 			get_button_state								(Button_def button);
void 							USER_BUTTON_callback_function   (void);


#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */
