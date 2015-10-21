#include "loadCPU.h"

 QueueHandle_t xCPUloadLCD;

void vGetLoadCPU( void *pvParameters )
{
uint8_t  cpuLCD;
xCPUloadLCD=xQueueCreate( 4, sizeof( uint8_t ) );	

 while (1) 
			{
			
				
			 cpuLCD=	(100 - GetCPU_IDLE());
			 xQueueSend( xCPUloadLCD,  &cpuLCD, ( TickType_t ) 0 ); 	//and sending calculated speeds in a queue to log and display
			//to sd card
			
				#ifdef DEBUG
				  // printf("\r\n cpu= %d",cpuLCD);
				#endif
//				lcd_clrscr();
//				lcd_goto(1,0);
//				lcd_puts("CPU=");
//				lcd_itos(cpuLCD);
//				lcd_putc('%');
//				lcd_goto(2,0);
//				lcd_puts(":)");
				vTaskDelay(100/ portTICK_PERIOD_MS);	
			}
}
 