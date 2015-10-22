#include "modeTask.h"

#define STEP 10
#define MAX_MODE 40

QueueHandle_t xModeQueueCTRL;
QueueHandle_t xModeQueueLCD;
QueueHandle_t xModeQueueRadioTx;

void init_button();

void vModeTask( void *pvParameters )
{
	init_button();
	static uint8_t  mode = STEP;
	xModeQueueCTRL=xQueueCreate( 4, sizeof( uint8_t ) );	
	xModeQueueLCD=xQueueCreate( 4, sizeof( uint8_t ) );
	xModeQueueRadioTx=xQueueCreate( 4, sizeof( uint8_t ) );
	while (1) 
	{
		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			vTaskDelay(10/ portTICK_PERIOD_MS);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			{
				vTaskDelay(10/ portTICK_PERIOD_MS);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
				{
					continue;
				}
				
				mode+=STEP;
				if (mode>MAX_MODE) 
				{
					mode=STEP;
				}
				
				xQueueSend( xModeQueueCTRL,  &mode, ( TickType_t ) 0 );
				xQueueSend( xModeQueueLCD,  &mode, ( TickType_t ) 0 );
				xQueueSend( xModeQueueRadioTx,  &mode, ( TickType_t ) 0 );
			}
		}	
		uint8_t tmp=0;
		
		if(xQueueReceive(xSlaveModeChangeQueue, &tmp, NULL)==pdTRUE)
		{
			mode += tmp;
			if(mode > MAX_MODE) {mode %= MAX_MODE;}
				
			xQueueSend( xModeQueueCTRL,  &mode, ( TickType_t ) 0 );
			xQueueSend( xModeQueueLCD,  &mode, ( TickType_t ) 0 );
			xQueueSend( xModeQueueRadioTx,  &mode, ( TickType_t ) 0 );
		}
		
		vTaskDelay(100/ portTICK_PERIOD_MS);
	}
}			
			
void init_button()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitTypeDef gpio;
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpio.GPIO_Pin =GPIO_Pin_0;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &gpio);
}			