#ifndef _FLASH_
#define _FLASH_

//---------INCLUDES---------//
#include "stm32f1xx_hal.h"
#include <stdlib.h>

//---------DEFINES---------//
//#define ADDR_FLASH_PAGE_START ((uint32_t)0x0801F800) /* Base @ of Page 126, 1 Kbytes */
//#define ADDR_AMOUNT_PAGES     ((uint32_t)0x0801FC00)                                       // ��� stm32f103c8t6 (128kb flash)
//#define ADDR_REMAINEDR_SIZE   ((uint32_t)0x0801FC02)

#define ADDR_FLASH_PAGE_START ((uint32_t)0x08007800) /* Base @ of Page 30, 1 Kbytes */
#define ADDR_AMOUNT_PAGES     ((uint32_t)0x08007C00)                                     // ��� stm32f103c6t6 (32kb flash)
#define ADDR_REMAINEDR_SIZE   ((uint32_t)0x08007C02)

//---------VARIABLES---------//
static uint16_t amount_lists = 1; //Shows how many pages are written
static uint32_t address_list_now = ADDR_FLASH_PAGE_START; //Shows the last page of recording
static uint16_t buf_u16[600]; // ����� ���������� ������!!!!!
static uint8_t buf_u8[600]; // ����� ���������� ������!!!!!

//---------FUNCTIONS---------//
// Function to erase flash-memory page
uint32_t FLASH_Erase(uint32_t Address);

// Function to write in flash-memory page
uint32_t FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint16_t *Data, uint16_t len);

// Function to read feom flash-memory page
uint16_t *FLASH_Read (uint32_t Address, uint16_t *buf, uint16_t len);

// Function to make uint16_t buf from uint8_t
uint16_t *buf_u8_to_u16(uint8_t *buf, uint16_t len_buf);

// Function to make uint8_t buf from uint16_t
uint8_t *buf_u16_to_u8(uint16_t *buf, uint16_t len_buf);

// Function to save data in flash-memory
uint8_t Save_Data (uint16_t buf[], uint16_t amount_halfwords);

// Function to read data from flash-memory
uint16_t *Read_Data(uint32_t Add_page, uint16_t len);

#endif //_FLASH_