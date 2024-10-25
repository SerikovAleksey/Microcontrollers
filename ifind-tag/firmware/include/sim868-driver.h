#pragma once

#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include <stdbool.h>

#define SMS_HEADER_LEN			 4
#define SMS_INFO_LEN			 46
#define SMS_DATA_LEN			 20
#define SMS_COMMAND_LEN			 20
#define SMS_COMMAND_RESULT_LEN   10

typedef struct
{
	UART_HandleTypeDef sim_uart;
	UART_HandleTypeDef gnss_uart;
	TIM_HandleTypeDef tim;
	uint16_t sim_pwk_pin;
	GPIO_TypeDef * sim_pwk_port;
	uint16_t gnss_pwk_pin;
	GPIO_TypeDef * gnss_pwk_port;
} sim868_ctx;

void sim868_handler(sim868_ctx ctx);
void sim868_power_on(sim868_ctx ctx);