#include "stm32f1xx.h"
#include "servo.h"


uint16_t base = MIDDLE; // A variable that show position of the servo at the moment (at the begining is equal to the MAX)
void servo_rotate(uint8_t angle)
{
	int x = MAX - angle * (MAX - MIN) / 180;
	uint16_t delta = 0;
	if (x == base)
	{
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3, base + STEP);
	}
	if (x > base)
	{
		while (x - base >= STEP_MIN)
		{
			if (delta > STEP_TEMP)
			{			
				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3, base + STEP);
				base += STEP;
				delta = 0;
			}
			else
			{
				delta++;
			}
		}
	}
	else
	{
		while (base - x >= STEP_MIN)
		{
			if (delta > STEP_TEMP)
			{			
				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3, base - STEP);
				base -= STEP;
				delta = 0;
			}
			else
			{
				delta++;
			}
		}
	}
}