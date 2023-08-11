#include "led.h"

uint16_t BUF_DMA [ARRAY_LEN] = {0};
uint8_t mid_buf[LED_COUNT][3];
uint8_t rgb_temp[LED_COUNT][3];
uint16_t DMA_BUF_TEMP[24];
uint16_t mid_led[24];
uint8_t light = 1;
uint8_t min;
uint8_t state = 0;

int d = 0;

void led_init()
{
	for(int i = DELAY_LEN; i < ARRAY_LEN; i++)
	{
		BUF_DMA[i] = LOW;
	}
	
}

void rgb_value_to_buf(uint8_t Rpixel,uint8_t Gpixel,uint8_t Bpixel, uint16_t LedPos)
{
	for(uint16_t i = 0; i < 8; i++)
	{
		//for red led
		if (BitIsSet(Rpixel, (7 - i)) == 1)
		{
			BUF_DMA[DELAY_LEN + LedPos * 24 + i + 8] = HIGH;
		}
		else
		{
			BUF_DMA[DELAY_LEN + LedPos * 24 + i + 8] = LOW;
		}
		//for green led
		if (BitIsSet(Gpixel, (7 - i)) == 1)
		{
			BUF_DMA[DELAY_LEN + LedPos * 24 + i] = HIGH;
		}
		else
		{
			BUF_DMA[DELAY_LEN + LedPos * 24 + i] = LOW;
		}
		//for blue led
		if (BitIsSet(Bpixel, (7 - i)) == 1)
		{
			BUF_DMA[DELAY_LEN + LedPos * 24 + i + 16] = HIGH;
		}
		else
		{
			BUF_DMA[DELAY_LEN + LedPos * 24 + i + 16] = LOW;
		}
	}
}

void led_start()
{
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, (uint32_t*)&BUF_DMA, ARRAY_LEN);
}

void led_prepareValue (uint8_t* values)
{
	  for (uint16_t i = 0; i < LED_COUNT; i++)
	  {
		  for (uint8_t j = 0; j < 3; j++)
		  {
			  rgb_temp[i][j] = values[i * 3 + j];
			  mid_buf[i][j] = values[i * 3 + j];
		  }
	  }
}
	
void led_setValue()

{
	for (uint16_t i = 0; i < LED_COUNT; i++)
	{
		rgb_value_to_buf(rgb_temp[i][0], rgb_temp[i][1], rgb_temp[i][2], i);
	}
}

void moth(void)
{
	led_init();
	uint8_t buf[LED_COUNT * 3];
	buf[0] = 38;
	buf[1] = 23;
	buf[2] = 126;
	for (uint16_t i = 3; i < LED_COUNT * 3; i++)
	{
		buf[i] = 0;
	}
	
	led_prepareValue(buf);
	led_setValue();
	//led_start();
	for(int k = 0; k < LED_COUNT - 1; k++)
	{
		memcpy((void*)DMA_BUF_TEMP,(void*)(BUF_DMA + DELAY_LEN + 6 * 24),48);
		for(int i = LED_COUNT - 2; i >= 0; i--)
		{
			memcpy((void*)(i*24+BUF_DMA + 24 + DELAY_LEN),(void*)(i*24+BUF_DMA + DELAY_LEN),48);
		}
		memcpy((void*)(BUF_DMA + DELAY_LEN),(void*)DMA_BUF_TEMP,48);
		led_start();

		HAL_Delay(90);
	}

	for(int k = 0; k < LED_COUNT - 1; k++)
	{
		memcpy((void*)DMA_BUF_TEMP,(void*)(BUF_DMA + DELAY_LEN),48);
		for(int i = 0; i <  LED_COUNT - 1; i++)
		{
			memcpy((void*)(i*24+BUF_DMA + DELAY_LEN),(void*)(i*24+BUF_DMA + 24 + DELAY_LEN),48);
		}
		memcpy((void*)(BUF_DMA+DELAY_LEN+6*24),(void*)DMA_BUF_TEMP,48);
		
		led_start();
		HAL_Delay(90);
	}
			
}

void fire()
{
	led_init();
	uint8_t buf[LED_COUNT * 3];
	for (uint16_t i = 0; i < LED_COUNT * 3; i++)
	{
		if (i % 3 == 0)
		{
			buf[i] = 255;
			continue;
		}
		if ((i + 1) % 3 == 0)
		{
			buf[i] = 0;
			continue;
		}
		buf[i] = 10 + ((i + 2) / 3 - 1) * 100 / (LED_COUNT - 1); 
	}
	
	led_prepareValue(buf);
	led_setValue();
	
	led_start();

	memcpy((void*)DMA_BUF_TEMP,(void*)(BUF_DMA + DELAY_LEN),48);
	for(int i = 0; i < LED_COUNT ; i++)
	{
		memcpy((void*)mid_led,(void*)(i*24+BUF_DMA + DELAY_LEN + 24),48);
		memcpy((void*)(i*24+BUF_DMA + DELAY_LEN + 24),(void*)DMA_BUF_TEMP,48);
		led_start();
		HAL_Delay(200);
		memcpy((void*)(i*24+BUF_DMA + DELAY_LEN + 24),(void*)mid_led,48);
	}

}

uint8_t* fill_array(uint8_t* buf, uint8_t r, uint8_t g, uint8_t b)
{
	for (uint16_t i = 0; i < LED_COUNT * 3; i++)
	{
		if ((i + 3) % 3 == 0)
		{
			buf[i] = r;
			continue;
		}
		if ((i + 2) % 3 == 0)
		{
			buf[i] = g;
			continue;
		}
		buf[i] = b; 
	}
	return buf;
}

void color(uint8_t num)
{
	led_init();
	uint8_t buf[LED_COUNT * 3];
	switch (num)
	{
	case 0: {
		fill_array(buf, 64, 0, 0); //red
		break;
	}
	case 1: {
		fill_array(buf, 0, 64, 0); //green
		break;
	}
	case 2: {
		fill_array(buf, 0, 0, 64); //blue
		break;
	}
	case 3: {
		fill_array(buf, 64, 0, 64); //violet
		break;
	}
	case 4: {
		fill_array(buf, 128, 10, 73); //pink
		break;
	}
	case 5: {
		fill_array(buf, 138, 226, 43); //olive
		break;
	}
	case 6: {
		fill_array(buf, 63, 128, 106); //aquamarine
		break;
		}
	case 7: {
		fill_array(buf, 53, 71, 17); // salat
		break;
	}
	case 8: {
		fill_array(buf, 0, 0, 0); // off
		break;
	}
	}
	
	led_prepareValue(buf);
	led_setValue();
	led_start();

}

uint8_t Min(uint16_t i,  uint8_t rgb_temp[][3])
{
			if(rgb_temp[i][0] == 0 && rgb_temp[i][1] == 0)
			{ 
				min = rgb_temp[i][2];
			}
			else if (rgb_temp[i][0] == 0 && rgb_temp[i][2] == 0)
			{ 
				min = rgb_temp[i][1];
			}
			else if(rgb_temp[i][1] == 0 && rgb_temp[i][2] == 0)
			{ 
				min = rgb_temp[i][0];
			}
			
			else if (rgb_temp[i][0] == 0) 
			{
				if (rgb_temp[i][1] < rgb_temp[i][2]) min = rgb_temp[i][1];
				else min = rgb_temp[i][2];
			}
			else if (rgb_temp[i][1] == 0) 
			{
				if (rgb_temp[i][0] < rgb_temp[i][2]) min = rgb_temp[i][0];
				else min = rgb_temp[i][2];
			}
			else if (rgb_temp[i][2] == 0) 
			{
				if (rgb_temp[i][0] < rgb_temp[i][1]) min = rgb_temp[i][0];
				else min = rgb_temp[i][1];
			}
			else if (rgb_temp[i][2] != 0 && rgb_temp[i][1] != 0 && rgb_temp[i][0] != 0)
			{
				if (rgb_temp[i][0] < rgb_temp[i][1]) min = rgb_temp[i][0];
				else min = rgb_temp[i][1];
				if (min < rgb_temp[i][2]) min = min;
				else min = rgb_temp[i][2];
			}
			return min;
}

void light_regulator(uint8_t num)
{
	light = 1;
	if (num == 0)
	{
		for(int i = 0; i < LED_COUNT;i++)
		{
			if(rgb_temp[i][0] > 0) rgb_temp[i][0] -= rgb_temp[i][0] / Min(i, rgb_temp);
			if(rgb_temp[i][1] > 0) rgb_temp[i][1] -= rgb_temp[i][1] / Min(i, rgb_temp);
			if(rgb_temp[i][2] > 0) rgb_temp[i][2] -= rgb_temp[i][2] / Min(i, rgb_temp);
			
			rgb_value_to_buf(rgb_temp[i][0],rgb_temp[i][1], rgb_temp[i][2],i);
		}
		led_start();
		HAL_Delay(10);
	}
	else
	{
		for(int i = 0;i < LED_COUNT;i++)
		{
			if(rgb_temp[i][0] < 255 && rgb_temp[i][0] <= mid_buf[i][0]) rgb_temp[i][0] += mid_buf[i][0] / Min(i, mid_buf);
			if(rgb_temp[i][1] < 255 && rgb_temp[i][1] <= mid_buf[i][1]) rgb_temp[i][1] += mid_buf[i][1] / Min(i, mid_buf);
			if(rgb_temp[i][2] < 255 && rgb_temp[i][2] <= mid_buf[i][2]) rgb_temp[i][2] += mid_buf[i][2] / Min(i, mid_buf);
			
			rgb_value_to_buf(rgb_temp[i][0],rgb_temp[i][1], rgb_temp[i][2],i);
		}
		led_start();
		HAL_Delay(10);
	}
	
}


