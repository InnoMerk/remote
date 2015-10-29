#include "ledOS.h"
#include "printf.h"
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
extern uint8_t tst;	
void vLedTask (void *pvParameters)
{
 
	uint8_t state=0; 
  initLED();	
	while(1)
    {			
			
			if (state == 0)
			{
					GPIO_ResetBits(GPIOC,GREEN);
//				    GPIO_SetBits(GPIOC,BLUE);
					state = 1;
			}
			else
			{
					
				GPIO_SetBits(GPIOC,GREEN);
//				  GPIO_ResetBits(GPIOC,BLUE);
					state = 0;
			}
		
			vTaskDelay(500 / portTICK_PERIOD_MS);	
		}
					
}
 //********************************************************************