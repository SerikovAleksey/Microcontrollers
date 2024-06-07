//#include "stm32f1xx_hal.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include <string.h> 

void SystemClock_Config(void);

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

//uint8_t buf[256];
//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if (huart->Instance == USART1)
//	{
//		HAL_UART_Receive_IT(&huart1, (uint8_t*)buf, 1);
//	}
//}

int main()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
//	HAL_UART_Receive_IT(&huart1, (uint8_t*)buf, 1);
//	
//	uint8_t test[2] = {'A', 'T' };
//	
//	HAL_UART_Transmit(&huart1, test, 2, 100);
	while (1)
	{
	         
	}
}