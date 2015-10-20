#include "radioTask.h"
#include "led.h"

#define ADR_0 0x10
#define ADR_1 0x11
#define ADR_2 0x12
#define ADR_3 0x13

#define COMMAND_GETSTATE 0x22
#define COMMAND_SETMODE  0x33 

void transmit_package(uint8_t *data);
void message_assembly(uint8_t *package,uint8_t mode);
uint32_t systime;
uint32_t SPI_idx;

char* SPI_TX_bufptr;
char* SPI_RX_bufptr;

char SPI_TX_buf[64];
char SPI_RX_buf[64];

char SPI_NULL_buf[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t SPI_buf_idx;

SemaphoreHandle_t xNRF_IRQ_Semaphore;

SemaphoreHandle_t xSPI_Mutex;

QueueHandle_t xSPI_RX_Queue;
QueueHandle_t xSPI_TX_Queue;
	
 

	
void vTransmitTask (void *pvParameters)
{
	
	
	
	vTaskDelay(1000/portTICK_PERIOD_MS);
	xNRF_IRQ_Semaphore = xSemaphoreCreateBinary();
	xSPI_Mutex = xSemaphoreCreateMutex();
	xSPI_RX_Queue = xQueueCreate(64, sizeof(char));
	xSPI_TX_Queue = xQueueCreate(64, sizeof(char));
  SystemInit();
  radiomodule_hardware_init();
	
	static uint8_t dataOut[32], i;
	uint32_t tr_count;


	init_NRF24L01();

	

  for(;;)
  {
		uint8_t static masterMode=0;
		if( xQueueReceive( xModeQueueCTRL, &masterMode, ( TickType_t ) NULL) )
    {
      continue; 
    }
		
		message_assembly(dataOut,masterMode);
		transmit_package(dataOut);
			
		
		vTaskDelay(500/portTICK_PERIOD_MS);
		
  }
	vTaskDelete(NULL);
}

void vReceiveTask (void *pvParameters)
{
	uint8_t dataIn[32];
	uint32_t rx_count = 0;
	vTaskDelay(5000/portTICK_PERIOD_MS);
	while (1)
	{
		if(xSemaphoreTake(xNRF_IRQ_Semaphore, 100)==pdTRUE)
		{	
			NRF24L01_GetData(dataIn);
			rx_count++;
			GPIO_ResetBits(GPIOC,GREEN);
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

void message_assembly(uint8_t *package,uint8_t mode)
{
	__NOP();
	package[0]=ADR_0;
	package[1]=ADR_1;
	package[2]=ADR_2;
	package[3]=ADR_3;
	switch (mode)
		{
			//--------------------
			case MASTER_DEFAULT_MODE:
			{
				__NOP();
				package[4]=0x00;
				break;
			}
			//---------------------
			case MASTER_TEST_MODE:
			{
				package[4]=COMMAND_GETSTATE; 
				break;
			}
			//---------------------
			case MASTER_SETSLAVE_MODE:
			{
				package[4]=COMMAND_SETMODE;
				//package[5]=slave_mode_select;
				package[5]=WAITING_SLAVE_MODE;
				break;
			}
			//--------------------
			default:
			{	
			package[4]=0x00;	
			}	
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