#include "lcdTask.h"


void vLcdTask (void *pvParameters)
{
	vTaskDelay(2000/ portTICK_PERIOD_MS) ;
	lcd_init();
	
	uint8_t lcdMode=10;
	uint8_t lcdCPU=0;
	uint8_t lcdLimit=0;
	uint8_t lcdHit=0;
	
	for(;;)
	{
		
		
		//xQueueReceive( xCPUloadLCD, &lcdCPU, ( TickType_t ) NULL );
    
		xQueueReceive( xModeQueueLCD, &lcdMode, ( TickType_t ) NULL) ;
   
		xQueueReceive( xLimiterQueueLCD, &lcdLimit, ( TickType_t )NULL ); 
	
		lcd_clrscr();
		lcd_goto(1,0);
		lcd_itos(lcdCPU);
		lcd_putc('%');		
		
		
		switch (lcdMode)
		{
			
			//--------------------
			case WAITING_MODE:
			{
				lcd_goto(2,0);
				lcd_puts("WAITING");
			 
				break;
			}
			//--------------------
			case STATIC_MODE:
			{
				lcd_goto(2,0);
				lcd_puts("STATIC");
			
				break;
			}	
			//--------------------
			case RANDOM_MODE:
			{			
				lcd_goto(2,0);
				lcd_puts("RANDOM");
			
				break;
			}
				//--------------------
			case RADIOCONTROL_MODE:
			{
				lcd_goto(2,0);
				lcd_puts("RADIO");
				break;
			}	
			//--------------------
			default:
			{	
				lcd_goto(2,0);
				lcd_puts("SLAVE");
			}	
			/**/
		}
		lcd_goto(1,4);
		if(lcdLimit==SHOT){lcd_putc('*');}
		lcd_goto(1,7);
		if(lcdLimit==UP){lcd_putc('i');}
		if(lcdLimit==UP_LIMIT){lcd_putc('+');}
		if(lcdLimit==DOWN_LIMIT){lcd_putc('-');}
	  if(lcdLimit==DOWN){lcd_putc('!');}
	
		vTaskDelay(500/ portTICK_PERIOD_MS);
	}	//for
	vTaskDelete(NULL);

}