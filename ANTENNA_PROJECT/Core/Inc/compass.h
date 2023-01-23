// header file for 3-Axis Digital Compass IC HMC5883L
#include "stm32f4xx_hal.h"
#include "encoder.h"
#include "servo.h"
#include "step_motor.h"

// Definitions to addresses location for read data from compass
#define HEAD 0x3c
#define X_MSB 0x03
#define X_LSB 0x04
#define Y_MSB 0x07
#define Y_LSB 0x08
#define Z_MSB 0x05
#define Z_LSB 0x06

extern I2C_HandleTypeDef hi2c1; //I2C

// function to read data from the compass from the one register
int read_compass_data (uint8_t first);

// function to set settings of the compass
void write_compass_settings (uint8_t first, uint8_t second);

// function to antenna is rotated to the north
void set_north();

// function calculate x, y, z values from compass
void data_from_compass();
