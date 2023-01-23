#include "stm32f4xx_hal.h"
#include "servo.h"

uint8_t Transmit_Angle_From_Servo_To_Display[12]; // A array to transmit data to display

uint16_t base = MAX; // A variable that show position of the servo at the moment (at the begining is equal to the MAX)

uint8_t servo_angle_joystick = 90; // A variable that show angle of the servo when it is rotated by joystick

void servo_rotate(uint8_t angle)
{
	int x = angle * (MAX - MIN) / 90 + MIN;
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

uint8_t* Preparing_Values_To_Send_(uint16_t* mas_of_value, uint8_t* result)
 {
  result[3] = mas_of_value[0] >> 24;
	result[2] = mas_of_value[0] >> 16;
	result[1] = mas_of_value[0] >> 8;
	result[0] = mas_of_value[0] % 256;

	result[7] = mas_of_value[1] >> 24;
	result[6] = mas_of_value[1] >> 16;
	result[5] = mas_of_value[1] >> 8;
	result[4] = mas_of_value[1] % 256;
	 
	result[11] = mas_of_value[2] >> 24;
	result[10] = mas_of_value[2] >> 16;
	result[9] = mas_of_value[2] >> 8;
	result[8] = mas_of_value[2] % 256;
  return result;
 }

void rotate_servo_by_joystick(uint8_t *array)
{
	if (array[0]|(array[1]<<8)|(array[2]<<16)|(array[3]<<24) == 3000 && servo_angle_joystick + 1 <= 90)
	{
		servo_rotate(servo_angle_joystick + 1);
		servo_angle_joystick++;

		uint16_t buf[3] = {0, servo_angle_joystick, servo_angle_joystick};
		Preparing_Values_To_Send_(buf, Transmit_Angle_From_Servo_To_Display);		
		HAL_UART_Transmit_IT(&huart4, Transmit_Angle_From_Servo_To_Display, 12);
	}
	if (array[4]|(array[5]<<8)|(array[6]<<16)|(array[7]<<24) == 3000 && servo_angle_joystick - 1 >= 0)
	{
		servo_rotate(servo_angle_joystick - 1);
		servo_angle_joystick--;
		
		uint16_t buf[3] = {0, servo_angle_joystick, servo_angle_joystick};
		Preparing_Values_To_Send_(buf, Transmit_Angle_From_Servo_To_Display);		
		HAL_UART_Transmit_IT(&huart4, Transmit_Angle_From_Servo_To_Display, 12);
	}
	if(servo_angle_joystick == 90 || servo_angle_joystick == 0)
	{
		uint16_t buf[3] = {0, servo_angle_joystick, servo_angle_joystick};
		Preparing_Values_To_Send_(buf, Transmit_Angle_From_Servo_To_Display);		
		HAL_UART_Transmit_IT(&huart4, Transmit_Angle_From_Servo_To_Display, 12);
	}
}
