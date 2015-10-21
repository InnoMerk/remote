#ifndef RADIOTASK_H
#define RADIOTASK_H

#define LOCAL_NRF_ADDRESS {0xE7,0xE7,0xE7,0xE7,0xE7};
#define REMOTE_NRF_ADDRESS {0x7E,0x7E,0x7E,0x7E,0x7E};
#define NRF_CHANNEL 15
#define NRF_PAYLOAD 32              /* max = 32 bytes */
//#define NRF_DATARATE NRF24L01_DataRate_250k
//#define NRF_TX_POWER NRF24L01_OutputPower_0dBm
#define NRF_DATARATE NRF24L01_DataRate_2M
#define NRF_TX_POWER NRF24L01_OutputPower_M18dBm


#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_exti.h"
#include "misc.h"
	
#include "portmacro.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "croutine.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
	
#include "loadCPU.h"
#include "modeTask.h"

#include "hardwareInit.h"
#include "stm32nrf24l01regs.h"
#include "stm32nrf24l01.h"	
	
//-----------------------------------------------------------	
extern uint32_t systime;
extern uint32_t SPI_idx;

extern char* SPI_TX_bufptr;
extern char* SPI_RX_bufptr;

extern char SPI_TX_buf[];
extern char SPI_RX_buf[];

extern char SPI_NULL_buf[];

extern uint8_t SPI_buf_idx;
//-----------------------------------------------------------

extern SemaphoreHandle_t xNRF_IRQ_Semaphore;
extern SemaphoreHandle_t xSPI_Mutex;

extern QueueHandle_t xSPI_RX_Queue;
extern QueueHandle_t xSPI_TX_Queue;
extern QueueHandle_t xQueueRemoteLCD;

void vReceiveTask (void *pvParameters);
void vTransmitTask (void *pvParameters);




#endif