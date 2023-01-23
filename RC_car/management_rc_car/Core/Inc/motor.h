// Header file for motor A2212/13T 1000KV

#include "stm32f1xx.h"

#define MAX 1300 // This value show the maximum of the PWM value which is sended to motor
#define MIN 800 // This value show the minimum of the PWM value which is sended to motor

extern TIM_HandleTypeDef htim2;

// Function to set a beginning settings for the motor
void start_settings();

// Function for manage the speed of the motor
void speed_regulation(uint16_t val);