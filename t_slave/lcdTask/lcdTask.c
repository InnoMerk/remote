#include "lcdTask.h"


void vLcdTask (void *pvParameters)
{
	vTaskDelay(2000/ portTICK_PERIOD_MS) ;
	lcd_init();
	
	uint8_t lcdMode=0;
	uint8_t lcdCPU=0;
	uint8_t lcdLimit=0;
	uint8_t lcdHit=0;
	
	for(;;)
	{
//		if( xQueueReceive( xCPUloadLCD, &lcdCPU, ( TickType_t ) NULL ) )
//    {
//			continue;	
//		}		
		if( xQueueReceive( xModeQueueLCD, &lcdMode, ( TickType_t ) NULL) )
    {
      continue; 
    }
		if( xQueueReceive( xLimiterQueueLCD, &lcdLimit, ( TickType_t )NULL ) )
    {
			continue;  
    }
		if( xQueueReceive( xHitQueueLCD, &lcdHit, ( TickType_t ) NULL ) )
    {
			continue;  
    }
		
		lcd_clrscr();
		lcd_goto(1,0);
		lcd_itos(lcdCPU);
		lcd_putc('%');		
		lcd_goto(1,4);
		if(lcdHit==SHOT){lcd_putc('*');}
		
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
			case TEST_MODE:
			{
				lcd_goto(2,0);
				lcd_puts("TEST");
			 
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
		lcd_goto(1,7);
		if(lcdLimit==UP){lcd_putc('i');}
		if(lcdLimit==UP_LIMIT){lcd_putc('+');}
		if(lcdLimit==DOWN_LIMIT){lcd_putc('-');}
	  if(lcdLimit==DOWN){lcd_putc('!');}
	
		vTaskDelay(500/ portTICK_PERIOD_MS);
	}	//for
	vTaskDelete(NULL);

}