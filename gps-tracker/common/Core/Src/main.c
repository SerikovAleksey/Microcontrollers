/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "nmea.h"
#include "button.h"
#include "flash.h"
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define rx_buffer_size 1000 // received buffer size
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rx_buffer[rx_buffer_size] = { 0, }; //received buffer
uint8_t push_on_off = 0;
uint8_t push_transmit = 0; 
uint8_t push_make_point = 0;

//uint8_t check_power = 0;
uint8_t check_capacity = 0; //responsible for flash-memory recording

uint8_t tx_buf1[24] = { '$', 'P', 'C', 'A', 'S', '0', '3', '1', '1', '1', '1', '1', '1', '1', '1', '0', '1', '0', '0', '1', '0', '*', '0', '2' };
uint8_t tx_buf2[11] = { '$', 'P', 'C', 'A', 'S', '0', '1', '1', '*', '1', 'D'};
uint8_t tx_buf3[12] = { '$', 'P', 'C', 'A', 'S', '0', '4', ',', '6', '*', '1', 'F'};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart) {
	if (huart == &huart1) {
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);


		HAL_UART_AbortReceive_IT(&huart1);
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart1, (uint8_t*)rx_buffer, rx_buffer_size);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart1) {
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);

		HAL_UART_AbortReceive_IT(&huart1);
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart1, (uint8_t*)rx_buffer, rx_buffer_size);
	}
}


//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if (GPIO_Pin == BUT_ON_OFF) // on or off record track
//	{
//		uint16_t t0 = TIM2->CNT;
//		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
//		uint16_t t1 = Read_Button_Time_Push(GPIO_Pin, t0);
//		if (t1 < 1500) push_on_off = 1;
//		
//		if (Read_Button_Time_Push(GPIO_Pin, t0) > 3000) push_on_off = 2;
//	}
//	
//	if (GPIO_Pin == BUT_MAKE_POINT) // make a point
//	{
//		uint16_t t0 = TIM2->CNT;
////		 HAL_GPIO_WritePin(GPIOB, Yellow_led_Pin, GPIO_PIN_SET);
//		if (Read_Button_Time_Push(GPIO_Pin, t0) < 1000) push_make_point = 1;
//	}
//	
//	if (GPIO_Pin == BUT_TRANSMIT_DATA) // send data
//	{
//		uint16_t t0 = TIM2->CNT;
////		HAL_GPIO_WritePin(GPIOB, Yellow_led_Pin, GPIO_PIN_SET);
//		if (Read_Button_Time_Push(GPIO_Pin, t0) < 1000) push_transmit = 1;
//	}
//	
//}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_IT(&huart1, (uint8_t*)rx_buffer, rx_buffer_size);
	HAL_TIM_Base_Start_IT(&htim2);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  
	  uint8_t *buffer = get_transmit_buf().buf;
	  uint16_t size = get_transmit_buf().size;
	  uint16_t u16_size = 0;
		
	  if ((size / 2) % 2 != 0)	
	  {
		  u16_size = size / 2 + 1;
	  }
	  else u16_size = size / 2;
//		
//		
	  if (check_valid_data(rx_buffer) == -1 && !check_capacity) 
	  {
		  HAL_GPIO_WritePin(GPIOB, Red_led_Pin, GPIO_PIN_SET);
		 // Delay(300);
		  HAL_Delay(500);
		  HAL_GPIO_WritePin(GPIOB, Red_led_Pin, GPIO_PIN_RESET);
		  HAL_Delay(500);
	  }
	  else if (check_valid_data(rx_buffer) && !push_on_off && !check_capacity)
	  {
		  HAL_GPIO_WritePin(GPIOB, Green_led_Pin, GPIO_PIN_SET);
		  HAL_Delay(500);
		  HAL_GPIO_WritePin(GPIOB, Green_led_Pin, GPIO_PIN_RESET);
		  HAL_Delay(500);
	  }
//		
//		
	  if (size >= 980 && size < 1010)
	  {
//		  Save_Data(buf_u8_to_u16(buffer, size), u16_size + 1);
//		  Save_Data(&amount_lists, 1);
		  set_ind_c(0);
		  set_ind_t(0);
		  amount_lists++;
	  } // save data on one page flash-memory
		
	  if (push_on_off == 1 && !push_make_point)
	  {
		  nmea_handler(rx_buffer, AVERAGE);
		  HAL_Delay(1000);
		  HAL_GPIO_WritePin(Green_led_GPIO_Port, Green_led_Pin, GPIO_PIN_SET);
		  
	  } // start receive data
	  
	  if (check_valid_data(rx_buffer) == 1)
	  {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	  }
	  if (push_on_off == 1 && push_make_point) 
	  {
		  nmea_handler(rx_buffer, SPECIAL);
		  push_make_point = 0;
		  HAL_GPIO_WritePin(GPIOB, Yellow_led_Pin, GPIO_PIN_SET);
	  } // make a special point
	  //Delay(300);
	  HAL_GPIO_WritePin(GPIOB, Yellow_led_Pin, GPIO_PIN_RESET);
		
		
	  if (push_on_off == 2) 
	  {
		  Save_Data(buf_u8_to_u16(buffer, size), u16_size + 1);
		  Save_Data(&amount_lists, 1);
		  push_on_off = 0;
		  set_ind_c(0);
		  set_ind_t(0);
		  amount_lists++;
	  } //stop receive and save data 
		
	  if (push_on_off == 0) 
	  {
		  HAL_GPIO_WritePin(GPIOB, Green_led_Pin, GPIO_PIN_RESET);
	  }
		
	  if (push_transmit)
	  {
//		  uint16_t lists = 0;
//		  if ((*((uint32_t*)(ADDR_AMOUNT_PAGES))) == 0xFF) lists = 0;
//		  else lists = (*((uint32_t*)(ADDR_AMOUNT_PAGES)));
//		  for (uint16_t i = 0; i < lists; i++)
//		  {
//			  uint32_t add = ((uint32_t)(ADDR_FLASH_PAGE_START - ((uint32_t)(0x0000400 * (uint32_t)i))));
//			  uint16_t len = (*((uint32_t*)(ADDR_FLASH_PAGE_START - ((uint32_t)(0x0000400 * (uint32_t)i)))));
//			  transmit_data(buf_u16_to_u8(Read_Data(add, len), len), len * 2);
//		  }
		  HAL_GPIO_WritePin(GPIOB, Yellow_led_Pin, GPIO_PIN_SET);
		  Delay(2000);
		  HAL_GPIO_WritePin(GPIOB, Yellow_led_Pin, GPIO_PIN_RESET);
		  push_transmit = 0;
	  }
		
//	  if (address_list_now == ((uint32_t)0x08007800))
//	  {
//		  check_capacity = 1;
//		  push_on_off = 0;
//	  } //check flash-memory capacity
//		
//	  if (check_capacity)
//	  {
//		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
//	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim2.Instance)
	{
		Read_Button_Time_Push();
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
