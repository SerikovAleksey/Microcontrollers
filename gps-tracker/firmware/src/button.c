#include "button.h"

extern uint8_t push_on_off; // responsible for pressing the button to start recording coordinates
extern uint8_t push_transmit; // responsible for pressing the button to transmit recording coordinates
extern uint8_t push_make_point; //responsible for pressing the button to make special coordinates

uint16_t Read_Button_Time_Push ()
{
	if (!HAL_GPIO_ReadPin(BUT_TYPE, BUT_ON_OFF))
	{
		t_on++;		
	}
	else
	{
		t_on = 0;
	}
	
	if (!HAL_GPIO_ReadPin(BUT_TYPE, BUT_MAKE_POINT))
	{
		t_make++;		
	}
	else
	{
		t_make = 0;
	}
	
	if (!HAL_GPIO_ReadPin(BUT_TYPE, BUT_TRANSMIT_DATA))
	{
		t_transmit++;		
	}
	else
	{
		t_transmit = 0;
	}
	
	if (t_on >= 100 && t_on <= 500) push_on_off = 1;
	if (t_on >= 1000) push_on_off = 2;
	if (t_make >= 100 && t_make <= 500) push_make_point = 1;
	if (t_transmit >= 100 && t_transmit <= 500) push_transmit = 1;
} // ReadButton


void Delay(uint16_t time_ms)
{
	uint16_t tt = 0;
		uint16_t t2 = 0;

	while (tt < time_ms)
		{
			t2 = TIM2->CNT;
			if(TIM2->CNT >= t2) tt += TIM2->CNT - t2;
			else tt += TIM2->CNT + 5000 - t2;
		}
}


