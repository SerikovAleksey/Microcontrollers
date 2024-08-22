#include "sim868-driver.h"

typedef struct {
	char buffer[256];
	int data_ptr;
	bool started;
	bool ready;
} reader_t;

typedef struct {
	char syncword[4];
	char buffer[256];
	int data_ptr;
	bool synced;
	bool ready;
	bool recieved_sms;
	uint8_t amount_crlf;
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

static int64_t t = 0;

static uint8_t sms_initialize = 0;
static  bool time_up = false;
typedef struct
{
	uint16_t name_len;
	uint16_t full_len;
	char name[30];
	bool applied;
	char result[];
}current_command_t;

static current_command_t current_command;

const char *cmds[] = {
	"AT\r\n",
	"AT+CMEE=0\r\n",
	"AT+GMR\r\n",//get version of FW
	"AT+GSN\r\n",//get IMEI
	"AT+CNMI=1,2,0,1,0\r\n",
	"AT+SCLASS0=0\r\n",
	"AT+CMGF=1\r\n",
	"AT+CGNSPWR=1\r\n",// power for GPS/GLONASS ON
	"AT+CGNSPWR?\r\n",//check power for GPS/GLONASS status
	"AT+CREG?\r\n",
	"AT+CSQ\r\n",//get RSSI
	"AT+CNETSCAN\r\n",
	"AT+CNMI=2,2,0,0,0\r\n",
	"AT+COPS?\r\n",
	"AT+CSDT=0\r\n"
};

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
//			if (!strcmp((char*)"+CMT", (char*)sim_active_reader->syncword))
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
//			if (sim_active_reader->amount_crlf == 2 && sim_byte == '\n' && sim_active_reader->recieved_sms == false)
//			{
//				current_command.full_len = sim_active_reader->data_ptr;
//				sim_active_reader->ready = true;
//				
//				sim_active_reader_index = !sim_active_reader_index;
//				sim_active_reader = &sim_readers[sim_active_reader_index];
//            
//				sim_active_reader->data_ptr = 0;
//				sim_active_reader->synced = false;
//				sim_active_reader->ready = false;
//			}
//			
//			if (sim_active_reader->data_ptr > SMS_HEADER_LEN + current_command.name_len && sim_byte == '\n' && sim_active_reader->recieved_sms == true)
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
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
		
		sim_active_reader->ready = true;
						
		sim_active_reader_index = !sim_active_reader_index;
		sim_active_reader = &sim_readers[sim_active_reader_index];
		            
		sim_active_reader->data_ptr = 0;
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
	return 2;
}

static int reader_parse(sim_reader_t* reader)
{
	current_command.full_len = 0;
	for (uint16_t i = 0; i < 256; i++)
	{
		if (reader->buffer[i] == 0 && i > current_command.name_len) break;
		
		current_command.full_len++;
	}
	
	if (current_command.full_len < (current_command.name_len + 1 + 2)) return 2; // + 1 т.к. появляется лишний символ \r, + 2 т.к. идет перевод строки в конце\r\n
	
	uint16_t result_len = current_command.full_len - (current_command.name_len + 1 + 2);
	
	char result[result_len];
	memcpy(result, &reader->buffer[current_command.full_len - result_len - 2], result_len);
	memcpy(current_command.result, result, result_len);
	
	if (current_command_status(result, result_len)) return 1;
	
	current_command.applied = true;
	
	if (!strcmp(current_command.name, (char*)"AT+CMGF=1\r\n")) send2 = true;
	
	if (!strcmp(current_command.name, (char*)"AT+CNMI=2,2,0,0,0\r\n")) send1 = true;
	return 0;
}
void send_command(UART_HandleTypeDef *huart, const char* data, uint16_t size)
{
	current_command.name_len = size;
	current_command.name[0] = '\0';
	strcpy(current_command.name, data);
	strcpy(current_command.result, (char*)"EROR");
	current_command.full_len = 0;
	current_command.applied = false;
	
	HAL_UART_Transmit_IT(huart, (uint8_t*)data, size);
	HAL_Delay(300);
}

//int command_handler(UART_HandleTypeDef *huart, const uint8_t* command, uint16_t command_len)
//{
//	send_command(huart, command, command_len);
//	
//	return answer_parse(&sim_readers[!sim_active_reader_index]);
//}

void sms_start_receive()
{
//	t++;
	if (!send1 && time_up) 
	{
		send_command(&huart1, cmds[12], 19);
		time_up = false;
//		t = 0;
	}
	if (!send2 && time_up) 
	{
		send_command(&huart1, (uint8_t*)cmds[6], 11);
//		t = 250001;
		time_up = false;
	}     
}

void sim868_init()
{
	gnss_active_reader = &gnss_readers[gnss_active_reader_index];
	sim_active_reader = &sim_readers[sim_active_reader_index];
	
	HAL_GPIO_WritePin(SIM_PWK_GPIO_Port, SIM_PWK_Pin, GPIO_PIN_RESET); // включение сотовой связи
	HAL_GPIO_WritePin(GNSS_EN_GPIO_Port, GNSS_EN_Pin, GPIO_PIN_RESET); // включение gps
	HAL_Delay(2000);
	HAL_GPIO_WritePin(SIM_PWK_GPIO_Port, SIM_PWK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GNSS_EN_GPIO_Port, GNSS_EN_Pin, GPIO_PIN_SET);
	
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_IT(&huart1, (uint8_t*)sim_active_reader->buffer, 256);
	HAL_UART_Receive_IT(&huart2, &gnss_byte, 1);
	
}



void sim868_handler()
{
	reader_t* inactive_gnss_reader = &gnss_readers[!gnss_active_reader_index];
	
	sim_reader_t* inactive_sim_reader = &sim_readers[!sim_active_reader_index];
	
	if (inactive_gnss_reader->ready) {
		inactive_gnss_reader->ready = false;
	}
	sms_start_receive();
	
	if (inactive_sim_reader->ready) {
		reader_parse(inactive_sim_reader);
		inactive_sim_reader->ready = false;
	}
}