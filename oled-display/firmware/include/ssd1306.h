/**
  ******************************************************************************
  * @file           : ssd1306.h
  * @brief          : SSD1306 driver header
  * @author         : MicroTechnics (microtechnics.ru)
  ******************************************************************************
  */

#ifndef SSD1306_H
#define SSD1306_H



/* Includes ------------------------------------------------------------------*/

#include "stm32f1xx_hal.h"
#include <string.h>



/* Declarations and definitions ----------------------------------------------*/

#define SSD1306_X_SIZE                                                  128
#define SSD1306_Y_SIZE                                                  64

#define SSD1306_BUFFER_SIZE                                             (SSD1306_X_SIZE *  SSD1306_Y_SIZE) / 8




/* Functions -----------------------------------------------------------------*/

extern void SSD1306_Init();
extern void SSD1306_ClearScreen();
extern void SSD1306_UpdateScreen();
extern void SSD1306_DrawFilledRect(uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd);
//extern void SSD1306_DrawPic(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);
extern void SSD1306_DrawPic(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
extern uint8_t SSD1306_IsReady();



#endif // #ifndef SSD1306_H
