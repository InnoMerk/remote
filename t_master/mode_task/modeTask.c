#include "modeTask.h"

void initHardware(void);
void initSlaveModeButton(void);

QueueHandle_t xSlaveModeQueue;

QueueHandle_t xModeQueueCTRL;
QueueHandle_t xModeQueueLCD;
void init_button();

void vModeTask( void *pvParameters )
{
	init_button();
	static uint8_t  mode = 10;
	xModeQueueCTRL=xQueueCreate( 4, sizeof( uint8_t ) );	
	xModeQueueLCD=xQueueCreate( 4, sizeof( uint8_t ) );
	
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
				
				mode+=10;
				if (mode>30) 
				{
					mode=10;
				}
				
				xQueueSend( xModeQueueCTRL,  &mode, ( TickType_t ) 0 );
				xQueueSend( xModeQueueLCD,  &mode, ( TickType_t ) 0 );
			}
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


void vSlaveModeTask( void* pvParameters)
{
	initSlaveModeButton();
	static uint8_t  mode = 10;

	xSlaveModeQueue = xQueueCreate( 4, sizeof( uint8_t ) );
	
	for(;;) 
	{
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
		{
			vTaskDelay(10/ portTICK_PERIOD_MS);
			if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
			{
				vTaskDelay(10/ portTICK_PERIOD_MS);
				while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
				{
					continue;
				}
				
				mode+=10;
				if (mode>50) 
				{
					mode=10;
				}
				
				
				xQueueSend( xSlaveModeQueue,  &mode, ( TickType_t ) 0 );
			}
		}	
		vTaskDelay(100/ portTICK_PERIOD_MS);
	}
}

void initSlaveModeButton(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef gpio;
	
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpio.GPIO_Pin = GPIO_Pin_5;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &gpio);
}