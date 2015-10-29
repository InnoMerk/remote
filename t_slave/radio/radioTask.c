#include "radioTask.h"
#include "led.h"

#define ADR_0 0x10
#define ADR_1 0x11
#define ADR_2 0x12
#define ADR_3 0x13

#define COMMAND_GETSTATE   0x22
#define COMMAND_SETMODE    0x33 
#define UNEXPECTED_COMMAND 0xEC

uint32_t systime;
uint32_t SPI_idx;

char* SPI_TX_bufptr;
char* SPI_RX_bufptr;

char SPI_TX_buf[64];
char SPI_RX_buf[64];

char SPI_NULL_buf[32] = {0} ;

uint8_t SPI_buf_idx;

SemaphoreHandle_t xNRF_IRQ_Semaphore;

SemaphoreHandle_t xSPI_Mutex;

QueueHandle_t xSPI_RX_Queue;
QueueHandle_t xSPI_TX_Queue;

QueueHandle_t xSlaveModeChangeQueue;
	
static uint8_t tmp_mode_tx=0;
static uint8_t tmp_limit_tx=0; 
void tx_message_assembly(uint8_t mode);
void rx_message_parse(uint8_t* package);



void vTranceiveTask (void *pvParameters)
{
  radiomodule_hardware_init();
	
	xNRF_IRQ_Semaphore  =   xSemaphoreCreateBinary();
	xSPI_Mutex          =   xSemaphoreCreateMutex();
	xSPI_RX_Queue       =   xQueueCreate(64, sizeof(char));
	xSPI_TX_Queue       =   xQueueCreate(64, sizeof(char));
	
	xSlaveModeChangeQueue = xQueueCreate(4,sizeof(uint8_t));
	
	initr_NRF24L01();
	
	for(;;)
	{
		
		
		xQueueReceive(xModeQueueRadioTx,&tmp_mode_tx,NULL);
		xQueueReceive(xLimitQueueRadioTx,&tmp_limit_tx,NULL);
		
		if(xSemaphoreTake(xNRF_IRQ_Semaphore, 10)==pdTRUE)
		{
			GPIO_SetBits(GPIOC,BLUE);
			
			uint8_t dataIn[32]={0};
			 
			NRF24L01_GetData(dataIn);
			
			uint8_t rx_command=dataIn[4];
			
			if(rx_command==COMMAND_SETMODE)
			{
				xQueueSend(xSlaveModeChangeQueue,(dataIn+5),NULL);
				//take  sum of remote clicks = N,from dataIn[5]
				//and get them to the modeTask 
			}
			
			tx_message_assembly(rx_command);
			
			NRF24L01_Transmit_Status_t transmissionStatus;
			do 
			{
				vTaskDelay(1);
				transmissionStatus = NRF24L01_GetTransmissionStatus();
			}
			while (transmissionStatus == NRF24L01_Transmit_Status_Sending);
			
			NRF24L01_PowerUpRx();
			GPIO_ResetBits(GPIOC,BLUE);
			
			vTaskDelay(50/portTICK_PERIOD_MS);
		}
		else
		{		
				vTaskDelay(100/portTICK_PERIOD_MS);
		}	
	}	
	vTaskDelete(NULL);
}


 
//-----------------------------------------------------------------------------------------
 void EXTI1_IRQHandler(void) // nRF24_IRQ_PIN
{
	EXTI_ClearITPendingBit(NRF24L01_IRQ_LINE);
	xSemaphoreGiveFromISR(xNRF_IRQ_Semaphore, NULL);
}

void SPI2_IRQHandler(void)
{
	char cIn;
	char cOut;

	cIn = SPI_I2S_ReceiveData(SPI2);
	xQueueSendFromISR(xSPI_RX_Queue, &cIn, NULL);
	if (xQueueReceiveFromISR(xSPI_TX_Queue, &cOut, NULL))
	{
		SPI_I2S_SendData(SPI2, cOut);
	}
}



void tx_message_assembly(uint8_t query_name)
{
	uint8_t package[32];
	
	package[0]=ADR_0;
	package[1]=ADR_1;
	package[2]=ADR_2;
	package[3]=ADR_3;
	
	if((query_name==COMMAND_GETSTATE)||(query_name==COMMAND_SETMODE))
	{
		package[4]=query_name;
		package[5]=tmp_mode_tx;
		package[6]=tmp_limit_tx;
	}
	else
	{
		package[4]=UNEXPECTED_COMMAND;
		package[5]=query_name;
	}		
	
	NRF24L01_Transmit(package);
}	
		



void rx_message_parse(uint8_t* package)
{
//adress confirm
switch (package[4])
		{
			//--------------------
			case COMMAND_GETSTATE:
			{
				
				break;
			}
			//---------------------
			case COMMAND_SETMODE:
			{
				
				break;	
			}
			//---------------------
			default:
			{	
				
			}	
		}	
}