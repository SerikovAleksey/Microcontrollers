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

void sim868_handler();
void sim868_init();