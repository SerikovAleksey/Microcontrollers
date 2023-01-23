// Header for MG90 digital super speed servo 
#include "stm32f1xx.h"

#define STEP_MIN 1 // A minimal step to rotated the servo (degrees)
#define STEP 1 // A step to rotate the servo (degrees)
#define STEP_TEMP 1000 // A constant for controlling the speed of the servo (conventional units)
#define MAX 1800 // This correspond to the position of the servo at extreme (wheels rotated in left)
#define MIN 500 // This correspond to the position of the servo at extreme (wheels rotated in right)
#define MIDDLE (MAX+MIN)/2 // This corrrespond to the position of the servo at middle (wheels don't rotated)

extern TIM_HandleTypeDef htim3; //TIM3

// function to rotate servo
void servo_rotate(uint8_t angle);
