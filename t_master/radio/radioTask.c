#include "radioTask.h"
#include "led.h"

#define ADR_0 0x10
#define ADR_1 0x11
#define ADR_2 0x12
#define ADR_3 0x13

#define COMMAND_GETSTATE 0x22
#define COMMAND_SETMODE  0x33 

void transmit_package(uint8_t* data);
void tx_message_assembly(uint8_t* package,uint8_t masterMode);
void rx_message_parse(uint8_t* package,uint16_t* report);
	
uint32_t systime;
uint32_t SPI_idx;

char* SPI_TX_bufptr;
char* SPI_RX_bufptr;

char SPI_TX_buf[64];
char SPI_RX_buf[64];

char SPI_NULL_buf[64] = {0};

uint8_t SPI_buf_idx;

SemaphoreHandle_t xNRF_IRQ_Semaphore;

SemaphoreHandle_t xSPI_Mutex;

QueueHandle_t xSPI_RX_Queue;
QueueHandle_t xSPI_TX_Queue;

QueueHandle_t xQueueRemoteLCD;	
 

	
void vTransmitTask (void *pvParameters)
{
	
	
	
	xNRF_IRQ_Semaphore = xSemaphoreCreateBinary();
	xSPI_Mutex = xSemaphoreCreateMutex();
	
	xSPI_RX_Queue = xQueueCreate(64, sizeof(char));
	xSPI_TX_Queue = xQueueCreate(64, sizeof(char));
	
	xQueueRemoteLCD = xQueueCreate(4,sizeof(uint16_t));
	
	radiomodule_hardware_init();
	init_NRF24L01();
	uint8_t static master_mode=0;
	
	xTaskCreate(vReceiveTask,
		                    (const char*)"receive",
							configMINIMAL_STACK_SIZE,
							NULL,
							tskIDLE_PRIORITY + 1,
							NULL);	
 
  for(;;)
  {
		
		uint8_t  data_out[32]={0};
		
		xQueueReceive( xModeQueueCTRL, &master_mode, ( TickType_t ) NULL) ;
		
		tx_message_assembly(data_out,master_mode);
		transmit_package(data_out);
			
		vTaskDelay(500/portTICK_PERIOD_MS);
		
  }
	vTaskDelete(NULL);
}



void vReceiveTask (void *pvParameters)
{

	for(;;)
	{
		uint16_t tmp=0;
		uint8_t dataIn[32]={0};
		
		if(xSemaphoreTake(xNRF_IRQ_Semaphore, 100)==pdTRUE)
		{	
			GPIO_ResetBits(GPIOC,GREEN);
			
			NRF24L01_GetData(dataIn);
			
			rx_message_parse(dataIn,&tmp);
			
			xQueueSend(xQueueRemoteLCD,&tmp,NULL);
		
			vTaskDelay(50/portTICK_PERIOD_MS);
		}	
		else
		{	
			vTaskDelay(100/portTICK_PERIOD_MS);
		}		
	}  
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
void rx_message_parse(uint8_t* package, uint16_t* report)
{
//adress confirm
switch (package[4])
		{
			//--------------------
			case COMMAND_GETSTATE:
			{
				*report = (uint16_t)(package[5]<<8);
				*report|=  package[6];
				break;
			}
			//---------------------
			case COMMAND_SETMODE:
			{
				*report = (uint16_t)(package[5]<<8)+10;
				*report|=  package[6];
				break;	
			}
			//---------------------
			default:
			{	
				*report = 0xFFFF;	
			}	
		}	
}
void tx_message_assembly(uint8_t *package,uint8_t masterMode)
{
	package[0]=ADR_0;
	package[1]=ADR_1;
	package[2]=ADR_2;
	package[3]=ADR_3;
	
	package[4] = 0x00;
	
	if(MASTER_GETSTATE_MODE ==  masterMode)
	{
		package[4] = COMMAND_GETSTATE;
	}
	if(MASTER_SETSTATE_MODE ==  masterMode)
	{
		package[4] = COMMAND_SETMODE;
		xQueueReceive(xSlaveModeQueue,(package+5),100);
	}	
}



void transmit_package(uint8_t *data)
{
	if(data[4])
	{	
		NRF24L01_Transmit_Status_t transmissionStatus;
		
		NRF24L01_Transmit(data);
	
    GPIO_SetBits(GPIOC,GREEN);
    
		do 
		{
			vTaskDelay(1);
			transmissionStatus = NRF24L01_GetTransmissionStatus();
		}
    while (transmissionStatus == NRF24L01_Transmit_Status_Sending);
    NRF24L01_PowerUpRx();
	}
}