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
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int i = 0;

extern uint16_t BUF_DMA[ARRAY_LEN];
uint16_t Button = 0;
uint16_t But = 0;
uint8_t check = 0;
uint8_t s = 0;
int time = 0;
int t = 0;
uint8_t flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == BUTTON_Pin) But++;
	if (But == 2 || But == 3) Button++;
	if (But == 2 || But == 3) check++;
	if (But == 12) But = 1;
}
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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim2);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  moth();
	  HAL_Delay(1000);
//	  color(6);
//	  if (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin))
//	  {
//		  while (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin))
//		  {
//			  time++;
//			  HAL_Delay(10);
//				
//			  if (time > 30)
//			  {
//				  flag = 1;
//				  if (s == 0 && Button != 1) 
//				  {
//					  light_regulator(0);
//				  }
//				  if (s == 1 && Button != 1) 
//				  {
//					  light_regulator(1);
//				  }
//			  }
//		  }
//		  if (time < 20)
//		  {
//			  //HAL_Delay(400);
//			  time = 0;
//		  }
//		  if (s == 1) 
//		  {
//			  s = 0;
//		  }
//		  else s = 1;
//		  time = 0;
//		  if (flag == 1) But--;
//		  Button = But;
//		  flag = 0;
//	  }
//	  if (Button == 1 && check == 0) 
//	  {	
//		  s = 0;
//		  fire();
//	  }			
//	  if (Button == 2 && check == 1) 
//	  {	
//		  s = 0;
//		  moth();
//	  }
//	  if (Button == 3 && check == 2)
//	  {
//		  s = 0;
//		  color(0);
//		  check++;
//	  }
//	  if (Button == 4 && check == 3)
//	  {
//		  s = 0;
//		  color(1);
//		  check++;
//	  }
//	  if (Button == 5 && check == 4)
//	  {
//		  s = 0;
//		  color(2);
//		  check++;
//	  }
//	  if (Button == 6 && check == 5)
//	  {
//		  s = 0;
//		  color(3);
//		  check++;
//	  }
//	  if (Button == 7 && check == 6)
//	  {
//		  s = 0;
//		  color(4);
//		  check++;
//	  }
//	  if (Button == 8 && check == 7)
//	  {
//		  s = 0;
//		  color(5);
//		  check++;
//	  }
//	  if (Button == 9 && check == 8)
//	  {
//		  s = 0;
//		  color(6);
//		  check++;
//	  }
//	  if (Button == 10 && check == 9)
//	  {
//		  s = 0;
//		  color(7);
//		  check++;
//	  }
//	  if (Button == 11 && check == 10)
//	  {
//		  s = 0;
//		  color(8);
//		  check = 0;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
