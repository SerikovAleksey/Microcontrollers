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
	//	if (huart->Instance == USART1)
	//	{
	//		if (!sim_active_reader->synced)
	//		{
	//			sim_active_reader->syncword[0] = sim_active_reader->syncword[1]; 
	//			sim_active_reader->syncword[1] = sim_active_reader->syncword[2]; 
	//			sim_active_reader->syncword[2] = sim_active_reader->syncword[3]; 
	//			sim_active_reader->syncword[3] = sim_byte; 
	//			
	//			if (!strcmp((char*)"\r\n+C", (char*)sim_active_reader->syncword))
	//			{
	//				memcpy(sim_active_reader->buffer, sim_active_reader->syncword, SMS_HEADER_LEN);
	//				sim_active_reader->synced = true;
	//				sim_active_reader->data_ptr = 4;
	//				sim_active_reader->recieved_sms = true;
	//			}
	////			if (!strcmp((char*)"AT+C", (char*)sim_active_reader->syncword))
	////			{
	////				memcpy(sim_active_reader->buffer, sim_active_reader->syncword, SMS_HEADER_LEN);
	////				sim_active_reader->synced = true;
	////				sim_active_reader->data_ptr = 4;
	////				sim_active_reader->recieved_sms = false;
	////			}
	//		}
	//		else
	//		{
	//			sim_active_reader->buffer[sim_active_reader->data_ptr] = sim_byte;
	//			sim_active_reader->data_ptr++;
	//			
	//			if (sim_byte == '\n') sim_active_reader->amount_crlf++;
	//			
	////			if (sim_active_reader->amount_crlf == 2 && sim_byte == '\n' && sim_active_reader->recieved_sms == false)
	////			{
	////				current_command.full_len = sim_active_reader->data_ptr;
	////				sim_active_reader->ready = true;
	////				
	////				sim_active_reader_index = !sim_active_reader_index;
	////				sim_active_reader = &sim_readers[sim_active_reader_index];
	////            
	////				sim_active_reader->data_ptr = 0;
	////				sim_active_reader->synced = false;
	////				sim_active_reader->ready = false;
	////			}
	//			
	//			if (sim_active_reader->amount_crlf == 2 && sim_byte == '\n' && sim_active_reader->recieved_sms == true)
	//			{
	//				sim_active_reader->ready = true;
	//				
	//				sim_active_reader_index = !sim_active_reader_index;
	//				sim_active_reader = &sim_readers[sim_active_reader_index];
	//            
	//				sim_active_reader->data_ptr = 0;
	//				sim_active_reader->synced = false;
	//				sim_active_reader->ready = false;
	//			}
	//		}
	//		HAL_UART_Receive_IT(&huart1, &sim_byte, 1);
	//	}
}

void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{
		char syncword[6];
		memcpy(syncword, sim_active_reader->buffer, 6);
		
		if (!strcmp(syncword, (char*)"\r\n+CMT")) sim_active_reader->recieved_sms = true; 
		
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
		
		sim_active_reader->ready = true;
						
		sim_active_reader_index = !sim_active_reader_index;
		sim_active_reader = &sim_readers[sim_active_reader_index];
		            
		sim_active_reader->synced = false;
		sim_active_reader->ready = false;
		
		HAL_UART_AbortReceive(&huart1);
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
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
int current_command_status(char* current_status, uint16_t size)
{
	char status[size];
	memcpy(status, current_status, size);
	if (!strcmp((char*)"OK", (char*)status)) return 0;
	if (!strcmp((char*)"EROR", (char*)status)) return 1;
	if (!strcmp((char*)"WAIT", (char*)status)) return 2;
	return 3;
}
int command_parse(const char* answer)
{
	current_command.full_len = 0;
	for (uint16_t i = 0; i < 256; i++)
	{
		if (answer[i] == 0 && i > current_command.name_len) break;
		
		current_command.full_len++;
	}
	
	if (current_command.full_len < (current_command.name_len + 1 + 2)) return 2; // + 1 т.к. появляется лишний символ \r, + 2 т.к. идет перевод строки в конце\r\n
	
//	uint16_t result_len = current_command.full_len - (current_command.name_len + 1 + 2);
	uint16_t result_len = 2;
	
	char result[result_len];
//	memcpy(result, &answer[current_command.full_len - result_len - 2], result_len);
	memcpy(result, &answer[current_command.name_len + 1], result_len);
	memcpy(current_command.result, result, result_len);
	
	if (current_command_status(result, result_len)) return 1;
	
	current_command.applied = true;
	
//	if (!strcmp(current_command.name, (char*)"AT+CMGF=1\r\n")) 
	if (!strcmp(current_command.name, (char*)"AT+CSCS=\"GSM\"\r\n")) 
	{
		send2 = true;
		command_deinit(&current_command);
	}
	
	if (!strcmp(current_command.name, (char*)"AT+CNMI=1,2,0,1,0\r\n")) 
	{
		send1 = true;
		command_deinit(&current_command);
	}
	if (!strcmp(current_command.name, (char*)"AT+CMGF=1\r\n")) 
	{
		send3 = true;
		command_deinit(&current_command);
	}
	return 0;
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
	if (!send1 && time_up) 
	{
		send_command(&huart1, cmds[4], 19);
		time_up = false;
	}
	if (!send2 && time_up) 
	{
		send_command(&huart1, cmds[15], 17);
		time_up = false;
	}
	if (!send3 && time_up) 
	{
		send_command(&huart1, cmds[6], 11);
		time_up = false;
	}
	if (send1 && send2 && send3) sms_init = true;
}

void sim868_init(sim868_ctx ctx)
{
	gnss_active_reader = &gnss_readers[gnss_active_reader_index];
	sim_active_reader = &sim_readers[sim_active_reader_index];
	
	HAL_GPIO_WritePin(ctx.sim_pwk_port, ctx.sim_pwk_pin, GPIO_PIN_RESET); // включение сотовой связи
	HAL_GPIO_WritePin(ctx.gnss_pwk_port, ctx.gnss_pwk_pin, GPIO_PIN_RESET); // включение gps
	HAL_Delay(2000);
	HAL_GPIO_WritePin(ctx.sim_pwk_port, ctx.sim_pwk_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ctx.gnss_pwk_port, ctx.gnss_pwk_pin, GPIO_PIN_SET);
	
	__HAL_UART_ENABLE_IT(&ctx.sim_uart, UART_IT_IDLE);
	HAL_UART_Receive_IT(&ctx.sim_uart, (uint8_t*)sim_active_reader->buffer, 256);
	HAL_UART_Receive_IT(&ctx.gnss_uart, &gnss_byte, 1);
	
	__HAL_TIM_CLEAR_FLAG(&ctx.tim, TIM_SR_UIF); // очищаем флаг
	HAL_TIM_Base_Start_IT(&ctx.tim);
	
}

void sim868_handler(sim868_ctx ctx)
{
	reader_t* inactive_gnss_reader = &gnss_readers[!gnss_active_reader_index];
	
	sim_reader_t* inactive_sim_reader = &sim_readers[!sim_active_reader_index];
	
	if (!sms_init) sms_start_receive();
	
	sms_handler();
	
	if (inactive_gnss_reader->ready) {
		inactive_gnss_reader->ready = false;
	}
	
	if (inactive_sim_reader->ready) {
		reader_parse(inactive_sim_reader);
		inactive_sim_reader->ready = false;
	}
}