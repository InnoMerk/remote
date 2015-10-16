#include "moveTask.h"
/*
#define MOVE_MAXVALUE 100
#define T1 GPIO_Pin_8 
#define T2 GPIO_Pin_9
#define T3 GPIO_Pin_10
#define T4 GPIO_Pin_11

void Move(void); 
void outputsInit(void);

void CloseAllTransistors(void);

void openReversePair(void);
void openForwardPair(void);

void closeReversePair(void);
void closeForwardPair(void);

uint8_t rxPosition=0; 
uint8_t rxDirection=0;

void vMoveTask(void *parameters)
{
	outputsInit();
	
	for(;;)
	{
		if( xQueueReceive( xDirectionQueue, &rxDirection, ( TickType_t ) 0 ) )
		{
		  uint8_t countMove=0;
			Move();
			
			do
			{
				vTaskDelay(50/ portTICK_PERIOD_MS);
				if( xQueueReceive( xLimiterQueueMOVE, &rxPosition, ( TickType_t ) 0 ) )
				{
					continue; 
				}
				countMove++;
			}
			
			while(UP_LIMIT!=rxPosition || DOWN_LIMIT!=rxPosition || countMove<MOVE_MAXVALUE);
		
			CloseAllTransistors();			
			countMove=0;
		}	
	
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}//for
} //task

void Move(void)
{
	//deadTime();
	
	if(FORWARD==rxDirection)
	{
		closeReversePair();
		vTaskDelay(100/portTICK_PERIOD_MS);
		openForwardPair();
		
	}
	
	if(REVERSE==rxDirection)
	{
		closeForwardPair();
		vTaskDelay(100/portTICK_PERIOD_MS);
		openReversePair();	
	}
}

void outputsInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef gpio;
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10 | GPIO_Pin_11;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpio);
	
	CloseAllTransistors();
}

void CloseAllTransistors(void)
{
	GPIO_ResetBits(GPIOC, T1);
  GPIO_ResetBits(GPIOC, T2);
	GPIO_ResetBits(GPIOC, T3);
	GPIO_ResetBits(GPIOC, T4);
}

void openReversePair(void)
{
	GPIO_SetBits(GPIOC, T2);
	GPIO_SetBits(GPIOC, T4);
}
void openForwardPair(void)
{
  GPIO_SetBits(GPIOC, T1);
	GPIO_SetBits(GPIOC, T3);
}

void closeReversePair(void)
{
	GPIO_ResetBits(GPIOC, T2);
	GPIO_ResetBits(GPIOC, T4);	
}

void closeForwardPair(void)
{
	GPIO_ResetBits(GPIOC, T1);
	GPIO_ResetBits(GPIOC, T3);	
}
*/