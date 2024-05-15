#include "stm32f1xx_hal.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include <string.h> 

void SystemClock_Config(void);

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;


int main()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	
	while (1)
	{
	         
	}
}