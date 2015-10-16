#include "led.h"

void initLED(void)
	{
			
		  
		  GPIO_InitTypeDef port;
		  //---------leds init---------------------------------
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	 
			GPIO_StructInit(&port);
			port.GPIO_Mode = GPIO_Mode_Out_PP;
			port.GPIO_Pin = GREEN|BLUE;
			port.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(GPIOC, &port);
	}	