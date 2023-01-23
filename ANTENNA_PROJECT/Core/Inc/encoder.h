// Header file for AS5048B Magnetic Rotary Encoder 
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c2; //I2C

#define HIGH_BYTE_ANGLE_READ 0xFE 
#define LOW_BYTE_ANGLE_READ 0xFF

// function to read data from encoder
uint16_t read_encoder_angle();

// function to set a zero at the encoder
void set_encoder_zero();
