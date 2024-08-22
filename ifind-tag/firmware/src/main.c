//#include "stm32f1xx_hal.h"
#include "main.h"
#include "gpio.h"
//#include "tim.h"
#include "usart.h"
#include <string.h> 
#include <stdbool.h> 
#include "sim868-driver.h" 

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
	
	__HAL_TIM_CLEAR_FLAG(&htim1, TIM_SR_UIF); // очищаем флаг
	HAL_TIM_Base_Start_IT(&htim1);
	sim868_init();
	
	while (1)
	{
		sim868_handler();
	}
}