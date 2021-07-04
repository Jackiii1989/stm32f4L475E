#ifndef ISM43362_H
#define ISM43362_H

#ifdef __cplusplus
 extern "C" {
#endif
//**************************************************************************************************************
#include "stm32l475xx.h"
#include "spi.h"
#include "gpio.h"
#include "rcc.h"
#include "nvic.h"
#include "InitFunctions.h"
//ISM43362 (Invetek Wi-fi Module) < -----------------------------------------------------------------------------------------
#define ISM43362_INTERFACE_PRIO              0

void 				ISM43362_wifi_init				(void);
void 				ISM43362_pins_init				(void);
void 				ISM43362_reset_module			(void);
void 				ISM43362_toggle_reset_pin	(void);
//uint32_t		HTS221_get_temperature		(void);


#ifdef __cplusplus
}
#endif

#endif /* ISM43362 */
