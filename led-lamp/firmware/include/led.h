// Header file for LED WS2812b

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim3;


#define LED_COUNT 6 // Amount of led
#define LED_IN_GROUP 6
#define GROUP_COUNT LED_COUNT / LED_IN_GROUP
#define DELAY_LEN 40 // 
#define ARRAY_LEN DELAY_LEN + 24 * LED_COUNT
#define HIGH 65 // Number appropriates 1  65
#define LOW 26 // Number appropriate 0   26
#define BitIsSet(reg, bit) ((reg & (1<<bit)) != 0) 

// Led initilization
void led_init();
 
void rgb_value_to_buf(uint8_t Rpixel,uint8_t Gpixel,uint8_t Bpixel, uint16_t LedPos);

void led_start();

void led_setValue();

void led_prepareValue (uint8_t* values);


void moth(void);

void fire(void);

void color(uint8_t num);

void light_regulator(uint8_t num);