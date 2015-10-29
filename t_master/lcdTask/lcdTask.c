#include "lcdTask.h"
void print_slave_state(uint16_t tmp);
void print_master_mode(uint8_t* tmp);

uint8_t static masterMode=10;
static uint16_t receive_tmp=0;
void vLcdTask (void *pvParameters)
{
	vTaskDelay(2000/ portTICK_PERIOD_MS);
	lcd_init();
	
	uint8_t lcdCPU=0;
	
	// add a comment
	// add once more 
	
	for(;;)
	{
		lcd_clrscr();//
		
	
		xQueueReceive( xCPUloadLCD, &lcdCPU, ( TickType_t ) 10 ) ;		
		xQueueReceive( xModeQueueLCD, &masterMode, ( TickType_t ) NULL); 
    xQueueReceive(xQueueRemoteLCD,&receive_tmp,( TickType_t ) NULL);
    
		
		//----------------------------------------
		lcd_goto(2,0);		
		
		lcd_puts("S:");
		
		print_slave_state(receive_tmp);
		//----------------------------------------
		lcd_goto(1,0);
		
		lcd_puts("M:");
		lcd_itos(lcdCPU);
		lcd_putc('%');
		
		print_master_mode(&masterMode);
	
		vTaskDelay(400/ portTICK_PERIOD_MS);
	}	//for
	vTaskDelete(NULL);

}

void print_slave_state( uint16_t tmp)
{
	if(masterMode==MASTER_GETSTATE_MODE||masterMode==MASTER_SETSTATE_MODE)
	{
		uint8_t tmp_limit = (uint8_t) tmp;
		uint8_t tmp_mode=(uint8_t)(tmp>>8);	
		
		switch (tmp_mode)
		{
			
			//--------------------
			case WAITING_SLAVE_MODE:
			{
				lcd_puts("WAIT");
				break;
			}
			//--------------------
			case STATIC_SLAVE_MODE:
			{
				lcd_puts("STAT");
				break;
			}	
			//--------------------
			case RANDOM_SLAVE_MODE:
			{			
				lcd_puts("RAND");
				break;
			}
				//--------------------
			case RADIOCONTROL_SLAVE_MODE:
			{
				lcd_puts("DIST");
				break;
			}	
			default:
			{	
				lcd_puts("Err ");
			}	
	
		}//case
		lcd_goto(2,7);
		if(masterMode==MASTER_GETSTATE_MODE)
		{	
			if(tmp_limit==SLAVE_SHOT)           {lcd_putc('*');vTaskDelay(200/portTICK_PERIOD_MS);}
			else if(tmp_limit==SLAVE_UP)        {lcd_putc('i');}
			else if(tmp_limit==SLAVE_UP_LIMIT)  {lcd_putc('+');}
			else if(tmp_limit==SLAVE_DOWN_LIMIT){lcd_putc('-');}
			else if(tmp_limit==SLAVE_DOWN)      {lcd_putc('!');}
		}//if
	}	//if
	else
		lcd_puts("unLink");

}

void print_master_mode(uint8_t* tmp)
{
	switch (*tmp)
		{
			//--------------------
			case MASTER_DEFAULT_MODE:
			{
				lcd_goto(1,7);
				lcd_putc('D');
			 
				break;
			}
			//---------------------
			case MASTER_GETSTATE_MODE:
			{
				lcd_goto(1,7);
				lcd_putc('G');
			 
				break;
			}
			//---------------------
			case MASTER_SETSTATE_MODE:
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
}

