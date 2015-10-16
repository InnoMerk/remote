#include "limitSwitch.h"

void initHardware(void);
QueueHandle_t xLimiterQueueCTRL;
QueueHandle_t xLimiterQueueLCD;

void vLimitSwitchTask( void* pvParameters)
{
	initHardware();
	xLimiterQueueCTRL = xQueueCreate( 4, sizeof( uint8_t ) );
	xLimiterQueueLCD  = xQueueCreate( 4, sizeof( uint8_t ) );
	
	
	for(;;)
	{
		uint8_t rxMode=0;
		uint8_t txPosition=0;
		if( xQueueReceive( xModeQueueLimit, &rxMode, ( TickType_t ) 10 ) )
    {
      continue; 
    }
		
			if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
			{
				vTaskDelay(10/ portTICK_PERIOD_MS);
				
				if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
				{
					vTaskDelay(10/ portTICK_PERIOD_MS);
					
					txPosition=UP_LIMIT;
					
					xQueueSend( xLimiterQueueLCD,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueCTRL,  &txPosition, ( TickType_t ) 0 );
					
					
					while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
					{
						
						continue;
					}
					txPosition=UP;
					
					xQueueSend( xLimiterQueueCTRL,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueLCD,  &txPosition, ( TickType_t ) 0 );
					
					
				}
			}	
			
			else if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7))
			{
				vTaskDelay(10/ portTICK_PERIOD_MS);
				if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7))
				{
					vTaskDelay(10/ portTICK_PERIOD_MS);
					
					txPosition=DOWN_LIMIT;
					
					xQueueSend( xLimiterQueueCTRL,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueLCD,  &txPosition, ( TickType_t ) 0 );
					
					
					while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7))
					{
						continue;
					}
					
					txPosition=DOWN;
					
					xQueueSend( xLimiterQueueCTRL,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueLCD,  &txPosition, ( TickType_t ) 0 );
					
				}
			}
			

			
			vTaskDelay(100/ portTICK_PERIOD_MS);
	}//for
	
	vTaskDelete(NULL);
}//task


void initHardware(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitTypeDef gpio;
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &gpio);

}