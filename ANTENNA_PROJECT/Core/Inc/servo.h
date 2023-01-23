// Header for D645MW digital super speed servo 
#include "stm32f4xx_hal.h"

#define STEP_MIN 1 // A minimal step to rotated the servo (degrees)
#define STEP 1 // A step to rotate the servo (degrees)
#define STEP_TEMP 30000 // A constant for controlling the speed of the servo (conventional units)
#define MAX 1760 // This correspond to the position of the servo at 90 degrees
#define MIN 880 // This correspond to the position of the servo at 0 degrees

extern TIM_HandleTypeDef htim3; //TIM3
extern UART_HandleTypeDef huart4; //UART4

// function to rotate servo
void servo_rotate(uint8_t angle);

// function to servo rotated by a joustick
void rotate_servo_by_joystick(uint8_t *array);

// function to convert numbers to array for a transmit to the display
uint8_t* Preparing_Values_To_Send_(uint16_t* mas_of_value, uint8_t* result);
