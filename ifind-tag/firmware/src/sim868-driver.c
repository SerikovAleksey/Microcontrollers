#include "sim868-driver.h"

typedef struct {
	char buffer[256];
	int data_ptr;
	bool started;
	bool ready;
} reader_t;

static uint8_t gnss_byte;
static reader_t gnss_readers[2];
static reader_t* gnss_active_reader;
static uint8_t gnss_active_reader_index = 0;

static uint8_t sim_byte;
static reader_t sim_readers[2];
static reader_t* sim_active_reader;
static uint8_t sim_active_reader_index = 0;


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
	if (huart->Instance == USART1)
	{
//		if (sim_byte == 'O')
//		{
//			sim_active_reader->data_ptr = 0;
//			sim_active_reader->started = true;
//			sim_active_reader->ready = false;
//		}
        
		if (sim_active_reader->started)
		{
			sim_active_reader->buffer[sim_active_reader->data_ptr] = sim_byte;
			sim_active_reader->data_ptr++;
		}
        
		if (sim_active_reader->data_ptr >= sizeof(sim_active_reader->buffer) - 1)
		{
//			sim_active_reader->ready = sim_byte == '\n' ? true : false;
            
			sim_active_reader_index = !sim_active_reader_index;
			sim_active_reader = &sim_readers[sim_active_reader_index];
            
			sim_active_reader->data_ptr = 0;
			sim_active_reader->started = false;
			sim_active_reader->ready = false;
		}
		HAL_UART_Receive_IT(&huart1, &sim_byte, 1);
	}
}

uint8_t ending = '\r';
uint8_t test[4] = { 'A', 'T', 'I', '\r' };
uint8_t sms[4] = { 'H', 'I', ',', 'I' };
uint8_t test_sim[7] = { 'A', 'T', '+', 'C', 'B', 'C', '\r' };
uint8_t get_tel[8] = { 'A', 'T', '+', 'C', 'C', 'I', 'D', '\r' };
uint8_t operator[9] = { 'A', 'T', '+', 'C', 'O', 'P', 'S', '?', '\r' };
uint8_t start[10] = { 'A', 'T', '+', 'C', 'M', 'G', 'F', '=', '1', '\r' };
uint8_t scan[12] = { 'A', 'T', '+', 'C', 'N', 'E', 'T', 'S', 'K', 'A', 'N', '\r' };
uint8_t tel[23] = { 'A', 'T', '+', 'C', 'M', 'G', 'S', '=', '"', '+', '7', '9', '1', '0', '4', '5', '8', '8', '3', '4', '1', '"', '\r' };
void sim868_init()
{
	gnss_active_reader = &gnss_readers[gnss_active_reader_index];
	sim_active_reader = &sim_readers[sim_active_reader_index];
	sim_active_reader->data_ptr = 0;
	sim_active_reader->started = true;
	sim_active_reader->ready = false;
	
	HAL_GPIO_WritePin(SIM_PWK_GPIO_Port, SIM_PWK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GNSS_EN_GPIO_Port, GNSS_EN_Pin, GPIO_PIN_RESET);
	HAL_Delay(1500);
	HAL_GPIO_WritePin(SIM_PWK_GPIO_Port, SIM_PWK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GNSS_EN_GPIO_Port, GNSS_EN_Pin, GPIO_PIN_SET);

	HAL_UART_Receive_IT(&huart1, &sim_byte, 1);
	HAL_UART_Receive_IT(&huart2, &gnss_byte, 1);
	
//	AT + CMGS = \"+91XXXXXXXXXX\"\r
//	HAL_Delay(2000);
//	
//	HAL_UART_Transmit_IT(&huart1, start, 10);
//	HAL_UART_Transmit_IT(&huart1, tel, 23);
//	HAL_UART_Transmit_IT(&huart1, sms, 4);
}

void sim868_handler()
{
	reader_t* inactive_gnss_reader = &gnss_readers[!gnss_active_reader_index];
	if (inactive_gnss_reader->ready) {
//		HAL_UART_Transmit_IT(&huart1, scan, 12);
//		HAL_UART_Transmit_IT(&huart1, tel, 23);
//		HAL_UART_Transmit_IT(&huart1, sms, 4);
		HAL_Delay(5000);
		inactive_gnss_reader->ready = false;
	}
//	HAL_UART_Transmit_IT(&huart1, test, 2);
//	HAL_Delay(1000);
	reader_t* inactive_sim_reader = &sim_readers[!sim_active_reader_index];
	if (inactive_sim_reader->ready) {
		
//		HAL_UART_Transmit_IT(&huart1, test, 2);
		inactive_sim_reader->ready = false;
	}
}