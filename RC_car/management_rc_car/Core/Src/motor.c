#include "motor.h"

uint16_t pwm_value = 512;

void start_settings()
{
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, MAX);
	HAL_Delay(2000);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, MIN);
	HAL_Delay(6000);
}

void speed_regulation(uint16_t val)
{
	if (val <= 512)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		pwm_value = MAX - (val * (MAX - MIN)) / 506;
		if (pwm_value > MIN)
		{
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, pwm_value);
		}
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		pwm_value = MIN + ((val - 507) * (MAX - MIN)) / 517;
		if (pwm_value > MIN)
		{
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, pwm_value);
		}
	}
}