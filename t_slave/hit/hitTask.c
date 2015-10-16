#include "hitTask.h"

void initHit(void);
QueueHandle_t xHitQueueCTRL;
QueueHandle_t xHitQueueLCD;


void vHitTask( void* pvParameters)
{
	initHit();
	xHitQueueCTRL = xQueueCreate( 8, sizeof( uint8_t ) );
	xHitQueueLCD = xQueueCreate( 8, sizeof( uint8_t ) );
	
	
	for(;;)
	{
		uint8_t txHit=0;
		txHit = EMPTY;
		xQueueSend( xHitQueueCTRL,  &txHit, ( TickType_t ) 0 );
		xQueueSend( xHitQueueLCD,  &txHit, ( TickType_t ) 0 );
		
		
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
			{
				vTaskDelay(10/ portTICK_PERIOD_MS);
				
				if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
				{
					vTaskDelay(10/ portTICK_PERIOD_MS);
					
					txHit= SHOT;
					xQueueSend( xHitQueueLCD,  &txHit, ( TickType_t ) 0 );
					xQueueSend( xHitQueueCTRL,  &txHit, ( TickType_t ) 0 );
					
					while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
					{
						vTaskDelay(10);
						continue;
					}
					
				}
			}	
			vTaskDelay(100/ portTICK_PERIOD_MS);
	}//for
	
	vTaskDelete(NULL);
}//task


void initHit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitTypeDef gpio;
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpio.GPIO_Pin = GPIO_Pin_5;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &gpio);

}