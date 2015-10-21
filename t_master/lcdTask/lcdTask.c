#include "lcdTask.h"


void vLcdTask (void *pvParameters)
{
	vTaskDelay(2000/ portTICK_PERIOD_MS);
	lcd_init();
	
	uint8_t masterMode=0;
	uint8_t lcdCPU=0;
	
	// add a comment
	// add once more 
	
	for(;;)
	{
		if( xQueueReceive( xCPUloadLCD, &lcdCPU, ( TickType_t ) 10 ) )
    {
			continue;	
		}		
		if( xQueueReceive( xModeQueueLCD, &masterMode, ( TickType_t ) NULL) )
    {
      continue; 
    }
		lcd_clrscr();
		lcd_goto(2,0);		
		lcd_puts("S^:");
		lcd_goto(1,0);		
		lcd_puts("M:");
		lcd_itos(lcdCPU);
		lcd_putc('%');
		switch (masterMode)
		{
			//--------------------
			case MASTER_DEFAULT_MODE:
			{
				lcd_goto(1,7);
				lcd_putc('D');
			 
				break;
			}
			//---------------------
			case MASTER_TEST_MODE:
			{
				lcd_goto(1,7);
				lcd_putc('T');
			 
				break;
			}
			//---------------------
			case MASTER_SETSLAVE_MODE:
			{
				lcd_goto(1,7);
				lcd_putc('S');
			 
				break;
			}
			//--------------------
			default:
			{	
				lcd_goto(1,7);
				lcd_putc('U');
			}	
		}
		vTaskDelay(500/ portTICK_PERIOD_MS);
	}	//for
	vTaskDelete(NULL);

}