#include "lcdTask.h"


void vLcdTask (void *pvParameters)
{
	vTaskDelay(1000/ portTICK_PERIOD_MS) ;
	lcd_init();
	
	uint8_t lcdMode=10;
	uint8_t lcdCPU=0;
	uint8_t lcdLimit=0;
	
	for(;;)
	{
		
		
	//	xQueueReceive( xCPUloadLCD, &lcdCPU, ( TickType_t ) NULL );
    
		xQueueReceive( xModeQueueLCD, &lcdMode, ( TickType_t ) NULL) ;
   
		xQueueReceive( xLimiterQueueLCD, &lcdLimit, ( TickType_t )NULL ); 
	
		lcd_clrscr();
		
		lcd_goto(2,0);
		lcd_puts("Mode :");	
		lcd_goto(2,7);
		
		switch (lcdMode)
		{
			//--------------------
			case WAITING_MODE:
			{
				lcd_putc('W');
				break;
			}
			//--------------------
			case STATIC_MODE:
			{
				lcd_putc('S');
				break;
			}	
			//--------------------
			case RANDOM_MODE:
			{			
				lcd_putc('R');
				break;
			}
				//--------------------
			case RADIOCONTROL_MODE:
			{
				lcd_putc('D');
				break;
			}	
			//--------------------
			default:
			{	
				lcd_goto(2,0);
				lcd_putc('E');
			}	
			/**/
		}
		lcd_goto(1,0);
		lcd_puts("Event:");
		lcd_goto(1,7);
		if(lcdLimit==SHOT){lcd_putc('*');}
		if(lcdLimit==UP){lcd_putc('i');}
		if(lcdLimit==UP_LIMIT){lcd_putc('+');}
		if(lcdLimit==DOWN_LIMIT){lcd_putc('-');}
	  if(lcdLimit==DOWN){lcd_putc('!');}
	
		vTaskDelay(500/ portTICK_PERIOD_MS);
	}	//for
	vTaskDelete(NULL);

}