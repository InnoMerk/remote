#include "controlTask.h"

#define TIMEOUT 100
#define T1 GPIO_Pin_8 
#define T2 GPIO_Pin_9
#define T3 GPIO_Pin_10
#define T4 GPIO_Pin_11

#define MOVING_UP_STATE 10
#define UP_WAIT_STATE  20 
#define MOVING_DOWN_STATE 30
#define DOWN_WAIT_STATE  40 
#define STEP 500
//QueueHandle_t xDirectionQueue;




void outputsInit(void);

void CloseAllTransistors(void);

void openReversePair(void);
void openForwardPair(void);

void closeReversePair(void);
void closeForwardPair(void);

void vControlTask (void *pvParameters)
{
	outputsInit();
	
	/*xDirectionQueue = xQueueCreate( 8, sizeof( uint8_t ) );*/
	uint8_t rxMode=0; 
	uint8_t rxCPU=0;
	uint8_t rxLimit=0;
	
	for(;;)
	{
		
		
		if( xQueueReceive( xModeQueueCTRL, &rxMode, ( TickType_t ) 0 ) )     continue;			
		if( xQueueReceive( xLimiterQueueCTRL, &rxLimit, ( TickType_t ) 0 ) ) continue;
		
		
		switch (rxMode)
		{
			
			//------------------------------------------------------------------------------

			case WAITING_MODE:
			{
				
				if(DOWN!=rxLimit||DOWN_LIMIT!=rxLimit)
				{
					CloseAllTransistors();
					openReversePair();
				}
				if(DOWN_LIMIT == rxLimit||DOWN==rxLimit){CloseAllTransistors();}
				
				break;	
			}	
			//-------------------------------------------------------------------------------
			case STATIC_MODE:
			/*body*/
			{
				
				
				static uint8_t static_state =MOVING_UP_STATE;
				switch(static_state)
				{
					case MOVING_UP_STATE:
					{
						if(/*UP!=rxLimit||*/UP_LIMIT!=rxLimit)
						{
							CloseAllTransistors();
							openForwardPair();
						}
						if(/*UP==rxLimit||*/UP_LIMIT==rxLimit)
						{	
							CloseAllTransistors();
							static_state=UP_WAIT_STATE;
						}
						break;	
					}	
					case UP_WAIT_STATE:
					{
						if(SHOT==rxLimit)
						{
							CloseAllTransistors();
							openReversePair();
							static_state=MOVING_DOWN_STATE;
						}	
						break;	
					}
					case MOVING_DOWN_STATE:
					{	
						if(DOWN_LIMIT == rxLimit/*||DOWN==rxLimit*/)
						{
							  CloseAllTransistors();
								vTaskDelay(6000/portTICK_PERIOD_MS);
								static_state=MOVING_UP_STATE;
						}
						break;	
					}
					default:
					{
					}
				}	
			
				
				vTaskDelay(100 / portTICK_PERIOD_MS);
				break;
			}	
			//----------------------------------------------------------------------------
			case RANDOM_MODE:
			{
				static uint8_t random_state =MOVING_UP_STATE;
				static uint16_t timeUp=1500;
				static uint16_t timeDown=1500;
				static uint16_t UpTimeOut=0;
				static uint16_t DownTimeOut=0;
				
				
				switch(random_state)
				{
					case MOVING_UP_STATE:
					{
						timeUp=(rand()%6 + 1)*STEP;
						timeDown=(rand()%6 + 1)*STEP;
						
						if(UP!=rxLimit||UP_LIMIT!=rxLimit)
						{
							CloseAllTransistors();
							openForwardPair();
						}
						if(UP==rxLimit||UP_LIMIT==rxLimit)
						{	
							CloseAllTransistors();
							random_state=UP_WAIT_STATE;
						}
						
						break;	
					}	
					case UP_WAIT_STATE:
					{
						UpTimeOut+=100;
						if(SHOT==rxLimit)
						{
							CloseAllTransistors();
							openReversePair();
							UpTimeOut=0;
							timeDown=5000;
							random_state=MOVING_DOWN_STATE;
						}	
						if(UpTimeOut>timeUp)
						{
							CloseAllTransistors();
							openReversePair();
							UpTimeOut=0;
							random_state=MOVING_DOWN_STATE;
						}	
						break;	
					}
					case MOVING_DOWN_STATE:
					{	
						if(DOWN_LIMIT == rxLimit/*||DOWN==rxLimit*/)
						{
							  CloseAllTransistors();
								random_state=DOWN_WAIT_STATE;
						}
						break;	
					}
					case DOWN_WAIT_STATE:
					{
						DownTimeOut+=100;
						if(DownTimeOut>timeDown)
						{
							CloseAllTransistors();
							openForwardPair();
							DownTimeOut=0;
							random_state=MOVING_UP_STATE;
						}		
						break;	
					}
					default:
					{
					}
				}	
			
				break;
			}	
			//--------------------
			case RADIOCONTROL_MODE:
			{
				
			
				break;
			}	
			//--------------------
			default:
			{
			/*xQueueSend( xDirectionQueue, &txDirection, ( TickType_t ) 0 );	*/
			
				
			}		
			/**/
		}
	vTaskDelay(100/ portTICK_PERIOD_MS);
	}	
	vTaskDelete(NULL);

}








void outputsInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef gpio;
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10 | GPIO_Pin_11;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &gpio);
	
	CloseAllTransistors();
}

void CloseAllTransistors(void)
{
	GPIO_ResetBits(GPIOA, T1);
  GPIO_ResetBits(GPIOA, T2);
	GPIO_ResetBits(GPIOA, T3);
	GPIO_ResetBits(GPIOA, T4);
}

void openReversePair(void)
{
	GPIO_SetBits(GPIOA, T2);
	GPIO_SetBits(GPIOA, T4);
}
void openForwardPair(void)
{
  GPIO_SetBits(GPIOA, T1);
	GPIO_SetBits(GPIOA, T3);
}

void closeReversePair(void)
{
	GPIO_ResetBits(GPIOA, T2);
	GPIO_ResetBits(GPIOA, T4);	
}

void closeForwardPair(void)
{
	GPIO_ResetBits(GPIOA, T1);
	GPIO_ResetBits(GPIOA, T3);	
}


