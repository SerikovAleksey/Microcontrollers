#include "encoder.h"
#include "stm32f4xx_hal.h"

uint8_t tx_data[256];
uint8_t rx_data[256];

uint16_t read_encoder_angle() 
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t z = 0;
	HAL_I2C_Mem_Read(&hi2c2, 0x80, HIGH_BYTE_ANGLE_READ, 1, rx_data, 8, 10);
	x = ((rx_data[0] << 8) | rx_data[1]);
	HAL_I2C_Mem_Read(&hi2c2, 0x80, LOW_BYTE_ANGLE_READ, 1, rx_data, 1, 10);
	y = rx_data[0];
	z = 360 - (x) * 360 / 65535;
	if (z == 360) z = 0;
	return z;
}

void set_encoder_zero() 
{
	uint16_t lb = 0;
	uint16_t hb = 0;
	
	HAL_I2C_Mem_Read(&hi2c2, 0x80, LOW_BYTE_ANGLE_READ, 1, rx_data, 8, 10);
	lb = ((rx_data[0] << 8) | rx_data[1]);
	
	HAL_I2C_Mem_Read(&hi2c2, 0x80, HIGH_BYTE_ANGLE_READ, 1, rx_data, 8, 10);
	hb = rx_data[0];
	
	tx_data[0] = 0x17;
	tx_data[1] = (lb >> 8);
	HAL_I2C_Master_Transmit(&hi2c2, 0x80, tx_data, 2, 10);
	
	tx_data[0] = 0x16;
	tx_data[1] = hb;
	HAL_I2C_Master_Transmit(&hi2c2, 0x80, tx_data, 2, 10);
}
