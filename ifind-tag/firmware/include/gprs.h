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
	char name[30];
	bool applied;
	char status[5];
	char answer[256];
}command_t;

extern command_t current_command;

extern bool time_up;

static const char *cmds[] = {
	"AT",
	"AT+CMEE=0",
	"AT+GMR", //get version of FW
	"AT+GSN", //get IMEI
	"AT+CNMI=1,2,0,1,0",
	"AT+SCLASS0=0",
	"AT+CMGF=1",
	"AT+CGNSPWR=1", // power for GPS/GLONASS ON
	"AT+CGNSPWR?", //check power for GPS/GLONASS status
	"AT+CREG?",
	"AT+CSQ", //get RSSI
	"AT+CNETSCAN",
	"AT+CNMI=2,2,0,0,0",
	"AT+COPS?",
	"AT+CSDT=0",
	"AT+CSCS=\"GSM\""
};

static const char *actions[] = {};

void sms_parse(const char* sms);

void send_command(UART_HandleTypeDef *huart, const char* data, bool need_for_delay);

void sms_handler();

void command_deinit(command_t* command);

void command_init(command_t* command, const char* data);