#include "i2c.h"
#include "gpio.h"
#include "rcc.h"
#include "HTS221.h"
//-------------------------------------------------------------------------------------------------------------
// HTS221 (TEMPERATURE SENSOR) INIT FUNCTIONS (USING IC2)
//-------------------------------------------------------------------------------------------------------------
#define HTS221                          ((uint8_t)1)

static const uint32_t 			GPIO_HTS221_SCL_PIN_ARRAY[HTS221]  									= {GPIO_PIN_10};
static const uint32_t 			GPIO_HTS221_SDA_PIN_ARRAY[HTS221]  									= {GPIO_PIN_11};
static const uint32_t 			GPIO_HTS221_ALTERNATIVE_FUNCTION_PIN[HTS221]  			= {GPIO_AF4_I2C2};
static GPIO_Type*   				GPIO_HTS221_PORT_ARRAY[HTS221] 											= {PORT_GPIOB };
static const uint32_t			  GPIO_HTS221_ACT_PORT[HTS221]												= {RCC_AHB2ENR_GPIOBEN};


uint32_t T0_degC, T1_degC;

uint32_t T0_out;
uint32_t T1_out;

I2C_init_struct i2c_init;

void HTS221_temperature_sensor_init(void){
	
	// define the sensor ID
	uint8_t sensor_id = (uint8_t)HTS221_macro;
	//uint8_t who_am_I_reg = 0x0F; 
	//uint8_t av_conf_reg  = 0x10; // Humidity and temperature resolution mode
	//uint8_t ctrl_reg1  = 0x20;   // Humidity and temperature resolution mode
	//uint8_t status_reg   = 0x27; // status register
	uint8_t r_data = 0, w_data = 0;
	uint8_t buffer[4], tmp = 100;
	uint32_t T0_degC_x8_16u, T1_degC_x8_16u;

	// ------------------------------------------------> GPIO init <--------------------------------------------
	
	GPIO_init_struct gpio_pin = RESET_GPIO_INIT_STRUCT;
		
	// configure PIN SCL - PB10 and PIN SDA - PB11
	gpio_pin.pins = GPIO_HTS221_SCL_PIN_ARRAY[sensor_id] | GPIO_HTS221_SDA_PIN_ARRAY[sensor_id];
	gpio_pin.GPIO_port = GPIO_HTS221_PORT_ARRAY[sensor_id];
	gpio_pin.GPIO_port_act = GPIO_HTS221_ACT_PORT[sensor_id];
	gpio_pin.Mode = GPIO_MODE_AF_OD;
	gpio_pin.Pull = GPIO_PULLUP;
	gpio_pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_pin.Alternate = GPIO_HTS221_ALTERNATIVE_FUNCTION_PIN[sensor_id];
	gpio_init_pins(&gpio_pin);
	
	
	// ------------------------------------------------> I2C init <--------------------------------------------
	//I2C_init_struct i2c_init;
	i2c_init.I2Cx 						= I2C_2;
	i2c_init.Timing						= I2Cx_TIMING;
	i2c_init.OwnAddress1      = 0;
  i2c_init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  i2c_init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  i2c_init.OwnAddress2      = 0;
  i2c_init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  i2c_init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
	i2c_init.AnalogFilter		  = I2C_CR1_ANFOFF;
	I2C_init(&i2c_init);
	
	// ------------------------------------------------> init register <--------------------------------------------
	
	//-------------------------------------------------> playing around <---------------------------------------------------------------
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,WHO_AM_I_REG,&r_data,1);
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,CTRL_REG2,&r_data,1);
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,STATUS_REG3,&r_data,1);
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,CTRL_REG1,&r_data,1);
	
	w_data = 0x07;
	I2C_write(i2c_init.I2Cx, HTS221_I2C_ADDR_W,CTRL_REG1,&w_data,1);
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,CTRL_REG1,&r_data,1);
	
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,ANV_CONG_REG,&r_data,1);	
	
	w_data = 0x11;
	I2C_write(i2c_init.I2Cx, HTS221_I2C_ADDR_W,ANV_CONG_REG,&w_data,1);
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,ANV_CONG_REG,&r_data,1);	
	
	w_data = 0x3F;
	I2C_write(i2c_init.I2Cx, HTS221_I2C_ADDR_W,ANV_CONG_REG,&w_data,1);
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,ANV_CONG_REG,&r_data,1);
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,CTRL_REG2 ,&r_data,1);	
	
	w_data = 0x82;
	I2C_write(i2c_init.I2Cx, HTS221_I2C_ADDR_W,CTRL_REG2 ,&w_data,1);
	
	r_data = 0;
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,CTRL_REG2,&r_data,1);
	
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,T0_degC_x8_reg,buffer,1);
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,T1_degC_x8_reg,&buffer[1],1);
	
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,T0_T1_DEGC_H2_reg,&tmp,1);
	
	//-------------------------------------------------> playing around end <---------------------------------------------------------------
	
	//-------------------------------------------------> getting the coefficients <---------------------------------------------------------
	
	T0_degC_x8_16u = (((uint16_t)(tmp & 0x03)) << 8) | ((uint16_t)buffer[0]);
  T1_degC_x8_16u = (((uint16_t)(tmp & 0x0C)) << 6) | ((uint16_t)buffer[1]);
	T0_degC = T0_degC_x8_16u >> 3;
	T1_degC = T1_degC_x8_16u >> 3;
	
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,T0_OUT_L, buffer   ,1);
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,T0_OUT_H,&buffer[1],1);
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,T1_OUT_L,&buffer[2],1);
	I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,T1_OUT_H,&buffer[3],1);
	
	T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
  T1_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];

}

uint32_t HTS221_get_temperature(void)
{
 uint8_t buffer[4] = {0,0,0,0};
 uint32_t T_out, tmp32, value;
	
 I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,TEMP_OUT_L, buffer   ,1);
 I2C_read(i2c_init.I2Cx, HTS221_I2C_ADDR,TEMP_OUT_H,&buffer[1],1);	
 
 T_out =(((uint32_t)buffer[1])<<8) | (uint32_t)buffer[0];
 tmp32 = ((int32_t)(T_out - T0_out)) * ((int32_t)(T1_degC - T0_degC));
 value = tmp32 /(T1_out - T0_out) + T0_degC;
 return value;

}
