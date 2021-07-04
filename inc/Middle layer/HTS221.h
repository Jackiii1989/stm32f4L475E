#ifndef HTS221_H
#define HTS221_H

#ifdef __cplusplus
 extern "C" {
#endif
//**************************************************************************************************************
#include "stm32l475xx.h"
//HTS221 (TEMPERATURE SENSOR) INIT FUNCTIONS (USING IC2) < -----------------------------------------------------------------------------------------
/* HTS221 (Humidity) I2C Address */
#define 											HTS221_I2C_ADDR      (uint8_t)0xBF // address of the sensor
#define 											HTS221_I2C_ADDR_W    (uint8_t)0xBE // address of the sensor
#define 											WHO_AM_I_REG	   		 (uint8_t)0x0F // register to identify the sensor
#define 											ANV_CONG_REG   			 (uint8_t)0x10
#define 											CTRL_REG1   				 (uint8_t)0x20
#define 											CTRL_REG2    				 (uint8_t)0x21
#define 											CTRL_REG3    				 (uint8_t)0x22
#define 											STATUS_REG3    			 (uint8_t)0x27
#define 											HUMIDITY_OUT_L    	 (uint8_t)0x28
#define 											HUMIDITY_OUT_H    	 (uint8_t)0x29
#define 											TEMP_OUT_L    	 		 (uint8_t)0x2A
#define 											TEMP_OUT_H    	 		 (uint8_t)0x2B
#define 											H0_rH_x2    	 		 	 (uint8_t)0x30
#define 											H1_rH_x2    	 		 	 (uint8_t)0x31
#define 											T0_degC_x8_reg    	 (uint8_t)0x32
#define 											T1_degC_x8_reg    	 (uint8_t)0x33
#define 											T0_T1_DEGC_H2_reg    (uint8_t)0x35

#define 											T0_OUT_L    	 			 (uint8_t)0x3C
#define 											T0_OUT_H    	 			 (uint8_t)0x3D

#define 											T1_OUT_L    	 			 (uint8_t)0x3E
#define 											T1_OUT_H    	 			 (uint8_t)0x3F

typedef enum 
{
HTS221_macro = 0
}sensor_enum;

void 				HTS221_temperature_sensor_init				(void);
uint32_t				HTS221_get_temperature								(void);


#ifdef __cplusplus
}
#endif

#endif /* HTS221_H */
