#include "main.h"
#include "stdbool.h"
#include "gpio.h"
#include "tim.h"

void SystemClock_Config(void);

typedef enum
{
	IDLE    = 0,
	DOWN_DD,
	UP_DD
}state_t;

typedef struct
{
	uint32_t pin;
	GPIO_TypeDef port;
}hardware_t;


typedef struct
{
	hardware_t down_dd;
	hardware_t up_dd;
	hardware_t relay;
	uint32_t start_timestamp;
	uint32_t duration_time;
}stair_ctx;

state_t state;
state_t prev_state;
static uint32_t down_timestamp;
static uint32_t up_timestamp;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == DOWN_DD)
	{
		state = DOWN_DD;
		down_timestamp = HAL_GetTick();
	}
	if (GPIO_Pin == UP_DD)
	{
		state = UP_DD;
		up_timestamp = HAL_GetTick();
	}
}

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_TIM1_Init();
 
	state = IDLE;
	prev_state = IDLE;
	
	while (1)
	{
		switch (state)
		{
		case IDLE:
			break;
		case DOWN_DD:
			if (prev_state == IDLE)
			{
				HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_SET);
				prev_state = DOWN_DD;
			}
			else if (prev_state == UP_DD)
			{
				HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_RESET);
				prev_state = IDLE;

			}
			if (HAL_GetTick() - down_timestamp > 15000)
			{
				HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_RESET);
				prev_state = IDLE;
			}
			break;
			
		case UP_DD:
			if (prev_state == IDLE)
			{
				HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_SET);
				prev_state = UP_DD;
			}
			else if (prev_state == DOWN_DD)
			{
				HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_RESET);
				prev_state = IDLE;

			}
			if (HAL_GetTick() - up_timestamp > 15000)
			{
				HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_RESET);
				prev_state = IDLE;
			}
			break;
		default:
			break;
		} 
	}
  
}