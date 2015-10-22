#include "limitSwitch.h"

void initHardware(void);

QueueHandle_t xLimiterQueueCTRL;
QueueHandle_t xLimiterQueueLCD;
QueueHandle_t xLimitQueueRadioTx;

uint8_t static txPosition=0;

void vLimitSwitchTask( void* pvParameters)
{
	
	initHardware();
	xLimiterQueueCTRL = xQueueCreate( 4, sizeof( uint8_t ) );
	xLimiterQueueLCD  = xQueueCreate( 4, sizeof( uint8_t ) );
	xLimitQueueRadioTx  = xQueueCreate( 4, sizeof( uint8_t ) );
	
	xTaskCreate(vHitTask,
							 (const char*)"HitTask",
								configMINIMAL_STACK_SIZE,
							  NULL,
							  tskIDLE_PRIORITY + 1,
							  NULL
							 );
	
	
	for(;;)
	{
		uint8_t rxMode=0;
		
		
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
		{
				vTaskDelay(10/ portTICK_PERIOD_MS);
				
				if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
				{
					vTaskDelay(10/ portTICK_PERIOD_MS);
					
					txPosition=UP_LIMIT;
					
					xQueueSend( xLimiterQueueLCD,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueCTRL,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimitQueueRadioTx,  &txPosition, ( TickType_t ) 0 );
					
					while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
					{
						
						continue;
					}
					txPosition=UP;
					
					xQueueSend( xLimiterQueueCTRL,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueLCD,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimitQueueRadioTx,  &txPosition, ( TickType_t ) 0 );
					
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
					xQueueSend( xLimitQueueRadioTx,  &txPosition, ( TickType_t ) 0 );
					
					while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7))
					{
						continue;
					}
					
					txPosition=DOWN;
					
					xQueueSend( xLimiterQueueCTRL,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueLCD,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimitQueueRadioTx,  &txPosition, ( TickType_t ) 0 );
				}
			}
			

			
			vTaskDelay(100/ portTICK_PERIOD_MS);
	}//for
	
	vTaskDelete(NULL);
}//task

void vHitTask( void* pvParameters)
{
	
	
	for(;;)
	{
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
		{
				uint8_t txHit=EMPTY;
		
				vTaskDelay(10/ portTICK_PERIOD_MS);
				
				if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
				{
					vTaskDelay(10/ portTICK_PERIOD_MS);
					
					txHit= SHOT;
					xQueueSend( xLimiterQueueLCD,  &txHit, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueCTRL,  &txHit, ( TickType_t ) 0 );
					xQueueSend( xLimitQueueRadioTx,  &txHit, ( TickType_t ) 0 );
					
					while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
					{
						continue;
					}
					xQueueSend( xLimiterQueueLCD,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimiterQueueCTRL,  &txPosition, ( TickType_t ) 0 );
					xQueueSend( xLimitQueueRadioTx,  &txPosition, ( TickType_t ) 0 );
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
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpio.GPIO_Pin = GPIO_Pin_5;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &gpio);

}


