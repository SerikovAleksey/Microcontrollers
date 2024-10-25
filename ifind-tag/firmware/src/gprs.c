#include "gprs.h"

typedef struct
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t day;
	uint8_t month;
	uint8_t year;
}date_t;
typedef struct
{
	char phone_num[12];
	date_t date;
	int data_ptr;
	uint16_t data_len;
	bool accepted;
	char sms[100];
}sms_info_t;

sms_info_t current_sms;

command_t current_command;
bool time_up = false;

bool sms_sent = false;

void command_init(command_t* command, const char* data)
{
	command->name_len = strlen(data);
	command->name[0] = '\0';
	strcpy(command->name, data);
	strcpy(command->status, (char*)"WAIT");
	command->applied = false;
}

void command_deinit(command_t* command)
{
	command->name_len = 0;
	command->name[0] = '\0';
	command->answer[0] = '\0';
	command->status[0] = '\0';
	strcpy(command->status, (char*)"WAIT");
	command->applied = false;
}

void send_command(UART_HandleTypeDef *huart, const char* data, bool need_for_delay)
{
	if (!time_up && need_for_delay) return;
	
	command_init(&current_command, data);	
	char cmd[strlen(data) + 2];
	strcpy(cmd, data);
	strcat(cmd, "\r\n");
	
	HAL_UART_Transmit_IT(huart, (uint8_t*)cmd, strlen(cmd));
	HAL_Delay(300);
	
	time_up = false;
}

int send_sms(char *data)
{
	char set_phone_num[24] = "AT+CMGS=\""; //  AT+CMGS="+7xxxxxxxxxx"
	strcat(set_phone_num, current_sms.phone_num);
	strcat(set_phone_num, "\"");
//	send_command(&huart1, set_phone_num, false);

	uint8_t ctrlz[] = { 0x1A };
	
//	HAL_UART_Transmit_IT(&huart1, (uint8_t*)data, strlen(data));
//	HAL_Delay(300);
//	HAL_UART_Transmit_IT(&huart1, (uint8_t*)ctrlz, 1);
	return 0;
}

void sms_parse(const char* sms)
{
	current_sms.data_ptr = 0;
	uint8_t amount_back_slesh_n = 0;
	uint8_t amount_sign = 0;
	uint16_t start_data_index;
	bool start_write_data = false;
	
	while (amount_back_slesh_n !=3)
	{
		if (sms[current_sms.data_ptr] == (char)'"') amount_sign++;
		if (sms[current_sms.data_ptr] == (char)'\n') amount_back_slesh_n++;
		if (amount_sign == 1)
		{
			memcpy(current_sms.phone_num, &sms[current_sms.data_ptr  + 1], 12);
			current_sms.data_ptr  += 12;	
		}
//		if (amount_sign == 5)
//		{
//			memcpy(sms_info.date.year, (uint8_t*)sms[sms_info.data_ptr + 1], 2);
//			memcpy(sms_info.date.month, (uint8_t*)sms[sms_info.data_ptr + 4], 2);
//			memcpy(sms_info.date.day, (uint8_t*)sms[sms_info.data_ptr + 7], 2);
//			memcpy(sms_info.date.hour, (uint8_t*)sms[sms_info.data_ptr + 10], 2);
//			memcpy(sms_info.date.minute, (uint8_t*)sms[sms_info.data_ptr + 13], 2);
//			memcpy(sms_info.date.second, (uint8_t*)sms[sms_info.data_ptr + 16], 2);
//			sms_info.data_ptr += 18;
//			amount_sign++;
//		}
		if (amount_back_slesh_n == 2 && !start_write_data)
		{
			start_data_index = current_sms.data_ptr + 1;
			start_write_data = true;
		}
		if (amount_back_slesh_n == 3)
		{
			strncpy(current_sms.sms, &sms[start_data_index], current_sms.data_ptr - start_data_index - 1);
		}
		current_sms.data_ptr++;
	}
	current_sms.data_len = current_sms.data_ptr - start_data_index - 2;
	current_sms.sms[current_sms.data_len] = '\0';
	current_sms.accepted = true;
}

void analyze_sms_data(char* data)
{
	if (!current_command.applied)
	{
		send_command(&huart1, data, true);
		return;
	}
	send_sms(current_command.answer);
	current_sms.accepted = false;
	command_deinit(&current_command);
}
void sms_handler()
{
	if (!current_sms.accepted) return;
	
	analyze_sms_data(current_sms.sms);
}