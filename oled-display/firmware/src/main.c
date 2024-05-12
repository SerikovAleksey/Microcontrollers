#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"
#include "usart.h"
#include <string.h> 

//#include "ssd1306.h"
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
void SystemClock_Config(void);

uint8_t flag = 0;
uint8_t command = 0;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{
		if (command == 'r') flag = 1;
		if (command == 's') flag = 0;
	}
	HAL_UART_Receive_IT(&huart1, &command, 1);
}

int main()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	
	SSD1306_Init();
	SSD1306_ClearScreen();
	while (SSD1306_IsReady() == 0) ;
//	float data[9] = {0.2348, 1.5478, 88.9567, 100.4589, 3.0234, 7.0005, 9.8907, 23.2951, 4.9084 };
//	uint32_t a = 3181772803;
//	uint8_t sw[4] = { 0x02, 0x00, 0xA6, 0xBD };
//	uint8_t data8[36] = {0, };
//	HAL_UART_Receive_IT(&huart1, &command, 1);
//
//	memcpy(data8, (uint8_t*)data, sizeof(data));
	
	while (1)
	{
//		for (uint8_t  i = 0; i < 8; i++)
		{
//			SSD1306_DrawFilledRect(i * 16, i * 16 + 8, 16, 48);
			SSD1306_DrawPic(32, 0, 64, 64, 1);
			SSD1306_UpdateScreen();
			while (SSD1306_IsReady() == 0) ;
      
			HAL_Delay(25);
		}
    
//		SSD1306_ClearScreen();
		while (SSD1306_IsReady() == 0) ;
//		if (flag)
//		{
////			for (uint16_t i = 0; i < 4; i++)
////			{
////				HAL_UART_Transmit(&huart1, &sw[i], 1, 100);
////			}
////			for (uint16_t i = 0; i < 9; i++)
////			{
////				HAL_UART_Transmit(&huart1, (uint8_t*)&data[i], 4, 100);
////			}
//			HAL_UART_Transmit_IT(&huart1, sw, 4);
//			HAL_Delay(10);
//			HAL_UART_Transmit_IT(&huart1, (uint8_t*)data, sizeof(data));
//			HAL_Delay(10);
//		}
//		
//		data[0] += 1;
//		data[8] += 1;
	}
}

