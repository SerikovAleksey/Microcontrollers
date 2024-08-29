#pragma once

#include "gpio.h"
#include "usart.h"
#include "main.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
typedef struct
{
	uint16_t name_len;
	uint16_t full_len;
	char name[30];
	bool applied;
	char result[];
}current_command_t;

extern current_command_t current_command;

extern bool time_up;

static const char *cmds[] = {
	"AT\r\n",
	"AT+CMEE=0\r\n",
	"AT+GMR\r\n", //get version of FW
	"AT+GSN\r\n", //get IMEI
	"AT+CNMI=1,2,0,1,0\r\n",
	"AT+SCLASS0=0\r\n",
	"AT+CMGF=1\r\n",
	"AT+CGNSPWR=1\r\n", // power for GPS/GLONASS ON
	"AT+CGNSPWR?\r\n", //check power for GPS/GLONASS status
	"AT+CREG?\r\n",
	"AT+CSQ\r\n", //get RSSI
	"AT+CNETSCAN\r\n",
	"AT+CNMI=2,2,0,0,0\r\n",
	"AT+COPS?\r\n",
	"AT+CSDT=0\r\n",
	"AT+CSCS=\"GSM\"\r\n"
};

static const char *actions[] = {};

void sms_parse(const char* sms);

void send_command(UART_HandleTypeDef *huart, const char* data, uint16_t size);

void sms_handler();

void command_deinit(current_command_t* command);

void command_init(current_command_t* command, const char* data, uint16_t size);