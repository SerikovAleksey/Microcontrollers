// Header file for 28BYJ-48 – 5V Stepper Motor
#include "stm32f4xx_hal.h"
#include "encoder.h"

extern TIM_HandleTypeDef htim2; // TIM2
extern TIM_HandleTypeDef htim3; // TIM3
extern UART_HandleTypeDef huart4; //UART4

// Define for GPIO pins 
#define IN1_PIN GPIO_PIN_6
#define IN1_PORT GPIOA
#define IN2_PIN GPIO_PIN_5
#define IN2_PORT GPIOA
#define IN3_PIN GPIO_PIN_7
#define IN3_PORT GPIOA
#define IN4_PIN GPIO_PIN_3
#define IN4_PORT GPIOA
#define TIME 1000 // It defines a rate the rotation of the motor

// function for rotate motor 
void step_motor_rotate(uint16_t angle);

// function to regulate speed rotations of the step motor
void speed_regulation(uint16_t delay);

// function of stopping rotation of the step motor and servo
void alarm_button();

// function to start working after an alarm
void continue_working();

// function to motor roteted by joystick
void rotate_motor_by_joustick(uint8_t *array);

// function to convert numbers to array for a transmit to the display
uint8_t* Preparing_Values_To_Send(uint16_t* mas_of_value, uint8_t* result);

// function to control the rotate angle a step motor by encoder
int rotate_step_motor_via_encoder(uint16_t angle);
