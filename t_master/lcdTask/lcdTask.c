#include "lcdTask.h"


void vLcdTask (void *pvParameters)
{
	vTaskDelay(2000/ portTICK_PERIOD_MS);
	lcd_init();
	
	uint8_t lcdMode=0;
	uint8_t lcdCPU=0;
	
	// add a comment
	// add once more 
	
	for(;;)
	{
		if( xQueueReceive( xCPUloadLCD, &lcdCPU, ( TickType_t ) 10 ) )
    {
			continue;	
		}		
		
		lcd_clrscr();
		lcd_goto(1,0);
		lcd_itos(lcdCPU);
		lcd_putc('%');		
		lcd_goto(2,0);
		lcd_puts("MASTER");
		
		
		vTaskDelay(500/ portTICK_PERIOD_MS);
	}	//for
	vTaskDelete(NULL);

}