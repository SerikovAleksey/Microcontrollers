#include "compass.h"

uint8_t tx_i2c_data[5]; // array with data that are transmitted to compass
uint8_t rx_i2c_data[5]; // array that save data from compass

// Variables to read data from compass
int16_t xx;
int16_t yy;
int16_t zz;
_Bool set_zero = 0; // Variable shows that has been called function "set_north"

void write_compass_settings (uint8_t first, uint8_t second)
{
	tx_i2c_data[0] = first;
	tx_i2c_data[1] = second;
	HAL_I2C_Master_Transmit(&hi2c1, HEAD, tx_i2c_data, 2, 10);
}

int read_compass_data (uint8_t first)
{
	tx_i2c_data[0] = first;
	HAL_I2C_Master_Transmit(&hi2c1, HEAD, tx_i2c_data, 1, 10);
	HAL_I2C_Master_Receive(&hi2c1, HEAD, rx_i2c_data, 1, 10);
	return rx_i2c_data[0];
}

void data_from_compass()
{
	uint8_t mx = read_compass_data(X_MSB);
	uint8_t lx = read_compass_data(X_LSB);
	xx = ((lx << 8) | mx);

	uint8_t my = read_compass_data(Y_MSB);
	uint8_t ly = read_compass_data(Y_LSB);
	yy = ((ly << 8) | my);

	uint8_t mz = read_compass_data(Z_MSB);
	uint8_t lz = read_compass_data(Z_LSB);
	zz = ((lz << 8) | mz);
}

void set_north()
{
	servo_rotate(90);
	data_from_compass();
	
	while(xx > 1000 || xx < -1000 || zz < 0)
	{
		data_from_compass();
		step_motor_rotate(1);
	}
	
	if (set_zero == 0) set_encoder_zero();
	set_zero = 1;
}
