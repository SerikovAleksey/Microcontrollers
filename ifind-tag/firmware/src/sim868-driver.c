#include "sim868-driver.h"
#include "gprs.h"

typedef struct {
	char buffer[256];
	int data_ptr;
	bool started;
	bool ready;
} reader_t;

typedef struct {
//	char syncword[4];
	char buffer[256];
	int data_ptr;
	bool synced;
	bool ready;
	bool recieved_sms;
//	uint8_t amount_crlf;
} sim_reader_t;

typedef struct
{
	uint8_t syncword[SMS_HEADER_LEN];
	uint8_t command[SMS_COMMAND_LEN];
	uint8_t result[SMS_COMMAND_RESULT_LEN];
} at_header_t;

static uint8_t gnss_byte;
static reader_t gnss_readers[2];
static reader_t* gnss_active_reader;
static uint8_t gnss_active_reader_index = 0;

static uint8_t sim_byte;
static sim_reader_t sim_readers[2];
static sim_reader_t* sim_active_reader;
static uint8_t sim_active_reader_index = 0;


static bool send1 = false;
static bool send2 = false;
static bool send3 = false;
static bool sms_init = false;

static int64_t t = 0;

static uint8_t sms_initialize = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		if (gnss_byte == '$')
		{
			gnss_active_reader->data_ptr = 0;
			gnss_active_reader->started = true;
			gnss_active_reader->ready = false;
		}
        
		if (gnss_active_reader->started)
		{
			gnss_active_reader->buffer[gnss_active_reader->data_ptr] = gnss_byte;
			gnss_active_reader->data_ptr++;
		}
        
		if (gnss_byte == '\n' || gnss_active_reader->data_ptr >= sizeof(gnss_active_reader->buffer) - 1)
		{
			gnss_active_reader->ready = gnss_byte == '\n' ? true : false;
            
			gnss_active_reader_index = !gnss_active_reader_index;
			gnss_active_reader = &gnss_readers[gnss_active_reader_index];
            
			gnss_active_reader->data_ptr = 0;
			gnss_active_reader->started = false;
			gnss_active_reader->ready = true;
		}
		HAL_UART_Receive_IT(&huart2, &gnss_byte, 1);
	}
}

void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		char syncword[6];
		memcpy(syncword, gnss_active_reader->buffer, 6);
		
//		if (!strcmp(syncword, (char*)"\r\n+CMT")) gnss_active_reader->recieved_sms = true; 
		
		
		gnss_active_reader->ready = true;
						
		gnss_active_reader_index = !gnss_active_reader_index;
		gnss_active_reader = &gnss_readers[gnss_active_reader_index];
		
		//		memset(&sim_active_reader->buffer, 0, 256);
		
//		gnss_active_reader->synced = false;
		gnss_active_reader->ready = false;
		
		__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);
		HAL_UART_AbortReceive(huart);
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
		HAL_UART_Receive_IT(huart, (uint8_t*)gnss_active_reader->buffer, 256);
	}
	if (huart->Instance == USART1)
	{
		char syncword[6];
		memcpy(syncword, sim_active_reader->buffer, 6);
		
		if (!strcmp(syncword, (char*)"\r\n+CMT")) sim_active_reader->recieved_sms = true; 
		
		
		sim_active_reader->ready = true;
						
		sim_active_reader_index = !sim_active_reader_index;
		sim_active_reader = &sim_readers[sim_active_reader_index];
		
//		memset(&sim_active_reader->buffer, 0, 256);
		
		sim_active_reader->synced = false;
		sim_active_reader->ready = false;
		
		__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);
		HAL_UART_AbortReceive(huart);
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
		HAL_UART_Receive_IT(huart, (uint8_t*)sim_active_reader->buffer, 256);
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1) 
	{
		time_up = true;
	}
}
int set_command_status(const char* data, char* status)
{
	if (strstr((char*)data, (char*)"OK"))
	{
		strncpy(status, "OK", 2);
		return 0;	
	}
	if (strstr((char*)data, (char*)"ERROR"))
	{
		strncpy(status, "ERROR", 5);
		return 1;	
	}
	if (strstr((char*)data, (char*)"WAIT"))
	{
		strncpy(status, "WAIT", 4);
		return 2;	
	}
	
	return 3;
}
void command_parse(const char* answer)
{		
	if (strstr(answer, current_command.name) == NULL) return; 
	
	if (!set_command_status(answer, current_command.status)) current_command.applied = true; 
	
	strncpy(current_command.answer, answer, strlen(answer));
	
	if (strstr(current_command.name, (char*)"AT+CSCS=\"GSM\"") && current_command.applied) 
	{
		send2 = true;
		command_deinit(&current_command);
	}
	
	if (strstr(current_command.name, (char*)"AT+CNMI=2,2,0,0,0") && current_command.applied) 
	{
		send1 = true;
		command_deinit(&current_command);
	}
	if (strstr(current_command.name, (char*)"AT+CMGF=1") && current_command.applied) 
	{
		send3 = true;
		command_deinit(&current_command);
	}
}
static int reader_parse(sim_reader_t* reader)
{
	if (!reader->recieved_sms)
	{
		command_parse(reader->buffer);
		return 1;
	}
	
	sms_parse(reader->buffer);
	reader->recieved_sms = false;
	return 0;
}



void sms_start_receive()
{
	if (!send1) 
	{
		send_command(&huart1, cmds[12], true);
		return;
	}
	if (!send2) 
	{
		send_command(&huart1, cmds[15], true);
		return;
	}
	if (!send3) 
	{
		send_command(&huart1, cmds[6], true);
		return;
	}
	
	sms_init = true;
}

void sim868_power_on(sim868_ctx ctx)
{
	gnss_active_reader = &gnss_readers[gnss_active_reader_index];
	sim_active_reader = &sim_readers[sim_active_reader_index];
	
//	__HAL_UART_ENABLE_IT(&ctx.gnss_uart, UART_IT_IDLE);
	HAL_UART_Receive_IT(&ctx.gnss_uart, (uint8_t*)gnss_active_reader->buffer, 256);
	
	__HAL_UART_ENABLE_IT(&ctx.sim_uart, UART_IT_IDLE);
	HAL_UART_Receive_IT(&ctx.sim_uart, (uint8_t*)sim_active_reader->buffer, 256);
	
	__HAL_TIM_CLEAR_FLAG(&ctx.tim, TIM_SR_UIF); // очищаем флаг
	HAL_TIM_Base_Start_IT(&ctx.tim);
	
	HAL_GPIO_WritePin(ctx.gnss_pwk_port, ctx.gnss_pwk_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ctx.sim_pwk_port, ctx.sim_pwk_pin, GPIO_PIN_RESET); // включение сотовой связи
//	HAL_GPIO_WritePin(ctx.gnss_pwk_port, ctx.gnss_pwk_pin, GPIO_PIN_RESET); // включение gps
	HAL_Delay(2000);
	HAL_GPIO_WritePin(ctx.sim_pwk_port, ctx.sim_pwk_pin, GPIO_PIN_SET);
}

void sim868_handler(sim868_ctx ctx)
{
	reader_t* inactive_gnss_reader = &gnss_readers[!gnss_active_reader_index];
	
	sim_reader_t* inactive_sim_reader = &sim_readers[!sim_active_reader_index];
	
	if (!sms_init) sms_start_receive();
//	send_command(&huart1, gnss_cmds[0], true);
	
	sms_handler();
	
	if (inactive_gnss_reader->ready) {
		inactive_gnss_reader->ready = false;
	}
	
	if (inactive_sim_reader->ready) {
		reader_parse(inactive_sim_reader);
		inactive_sim_reader->ready = false;
//		inactive_sim_reader->buffer[0] = '\0';
	}
}