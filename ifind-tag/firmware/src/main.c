//#include "stm32f1xx_hal.h"
#include "main.h"
#include "gpio.h"
//#include "tim.h"
#include "usart.h"
#include <string.h> 
#include <stdbool.h> 
#include "sim868-driver.h" 

#include <stdio.h>

void SystemClock_Config(void);
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim1;

int main()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_TIM1_Init();
	
	sim868_ctx ctx = { huart1, huart2, htim1, SIM_PWK_Pin, SIM_PWK_GPIO_Port, GNSS_EN_Pin, GNSS_EN_GPIO_Port };
	
	sim868_power_on(ctx);
	
	while (1)
	{
		sim868_handler(ctx);
	}
}