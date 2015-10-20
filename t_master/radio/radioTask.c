#include "radioTask.h"
#include "led.h"


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
	NRF24L01_Transmit_Status_t transmissionStatus;

	init_NRF24L01();

	for (i = 0; i < 32; i++)
	{
		dataOut[i] = i + 30;
	}

  while (1)
  {
    NRF24L01_Transmit(dataOut);
    GPIO_SetBits(GPIOC,GREEN);
    do 
		{
			vTaskDelay(1);
			transmissionStatus = NRF24L01_GetTransmissionStatus();
		}
    while (transmissionStatus == NRF24L01_Transmit_Status_Sending);
    
    NRF24L01_PowerUpRx();
    if (transmissionStatus == NRF24L01_Transmit_Status_Lost)
    {
			__NOP();
    }
		if (transmissionStatus == NRF24L01_Transmit_Status_Ok)
    {
			dataOut[0] = tr_count++;
			
    }
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