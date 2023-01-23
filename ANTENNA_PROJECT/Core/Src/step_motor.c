#include "stm32f4xx_hal.h"
#include "step_motor.h"

uint8_t Transmit_Angle_To_Display[12]; // array to transmit data to display

void step_motor_rotate (uint16_t angle)
	{
		for(int i = 0; i < 5; ++i) // due to rotate the whole antenna to 360 degrees need rotate motor 5 time to 360
			{
			if (angle > 180)
			{
				int step = 512 * (360 - angle) / 360;
				for(int x = 0; x < step; x = x + 1)
			{
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
				speed_regulation(TIME);
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
				speed_regulation(TIME);
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
				speed_regulation(TIME);
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
				speed_regulation(TIME);
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
				speed_regulation(TIME);
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
				speed_regulation(TIME);
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
				speed_regulation(TIME);
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
				speed_regulation(TIME);
			}
			}
			else
			{
				int step = 512 * angle / 360;
				for(int x = 0; x < step ; x = x + 1)
				{
					HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
					HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
					HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
					HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
					speed_regulation(TIME);
					HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
					HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
					HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
					HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
					speed_regulation(TIME);
					HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
					HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
					HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
					HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
					speed_regulation(TIME);
					HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
					HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
					HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
					HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
					speed_regulation(TIME);
					HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
					HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
					HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
					HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
					speed_regulation(TIME);
					HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
					HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
					HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
					HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
					speed_regulation(TIME);
					HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
					HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
					HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
					HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
					speed_regulation(TIME);
					HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
					HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
					HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
					HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
					speed_regulation(TIME);
				}
			}
		}
	}


void speed_regulation (uint16_t time)
{
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  while (__HAL_TIM_GET_COUNTER(&htim2) < time);
}
	
void alarm_button ()
{
	
	HAL_TIM_Base_Stop(&htim2);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
}

void continue_working () 
{
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

uint8_t* Preparing_Values_To_Send(uint16_t* mas_of_value, uint8_t* result)
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

void rotate_motor_by_joustick(uint8_t *array)
{
	if (array[0]|(array[1]<<8)|(array[2]<<16)|(array[3]<<24) == 4000)
	{
		step_motor_rotate(1);
		
		uint16_t buf[3] = {read_encoder_angle(), 0, read_encoder_angle()};
		Preparing_Values_To_Send(buf, Transmit_Angle_To_Display);		
		HAL_UART_Transmit_IT(&huart4, Transmit_Angle_To_Display, 12);
	}
	if (array[4]|(array[5]<<8)|(array[6]<<16)|(array[7]<<24) == 4000)
	{
		step_motor_rotate(359);
		
		uint16_t buf[3] = {read_encoder_angle(), 0, read_encoder_angle()};
		Preparing_Values_To_Send(buf, Transmit_Angle_To_Display);		
		HAL_UART_Transmit_IT(&huart4, Transmit_Angle_To_Display, 12);
	}
}

int rotate_step_motor_via_encoder(uint16_t angle)
{
	if (angle <= 180 && read_encoder_angle() < angle)
	{
		while (read_encoder_angle() <= angle - 1)
		{
			step_motor_rotate(1);
		}
		return 0;
	}
	
	if (angle < 180 && read_encoder_angle() > angle)
	{
		while (read_encoder_angle() >= angle + 1)
		{
			step_motor_rotate(359);
		}
				return 0;
	}
	
	if (angle > 180 && read_encoder_angle() < angle)
	{
		while (read_encoder_angle() <= angle - 1)
		{
			step_motor_rotate(1);
		}
				return 0;
	}
	
	if (angle > 180 && read_encoder_angle() > angle)
	{
		while (read_encoder_angle() >= angle + 1)
		{
			step_motor_rotate(359);
		}
				return 0;
	}
}
