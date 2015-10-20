#ifndef NRF24L01_H
#define NRF24L01_H 

#include "stm32f10x.h"
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
	

#include "hardwareInit.h"
#include "stm32nrf24l01regs.h"
#include "radioTask.h"//  bad practice

#ifdef __cplusplus
extern C {
#endif

typedef enum 
{
	NRF24L01_Transmit_Status_Lost =      0x00,     /*!< Message is lost, reached maximum number of retransmissions */
	NRF24L01_Transmit_Status_Ok =        0x01,     /*!< Message sent successfully */
	NRF24L01_Transmit_Status_Sending = 0xFF      /*!< Message is still sending */

} NRF24L01_Transmit_Status_t;

typedef enum 
{
	NRF24L01_DataRate_2M,                    /*!< Data rate set to 2Mbps */
	NRF24L01_DataRate_1M,                    /*!< Data rate set to 1Mbps */
	NRF24L01_DataRate_250k                  /*!< Data rate set to 250kbps */

}NRF24L01_DataRate_t;

typedef enum 
{
	NRF24L01_OutputPower_M18dBm,    /*!< Output power set to -18dBm */
	NRF24L01_OutputPower_M12dBm,    /*!< Output power set to -12dBm */
	NRF24L01_OutputPower_M6dBm,      /*!< Output power set to -6dBm */
	NRF24L01_OutputPower_0dBm          /*!< Output power set to 0dBm */

} NRF24L01_OutputPower_t;

//----------------------------------------------------------------------------------------------
//  prototypes
//----------------------------------------------------------------------------------------------	
void NRF24L01_PowerUpTx(void);
void NRF24L01_PowerUpRx(void);
void NRF24L01_PowerDown(void);

void init_NRF24L01(void);
void initr_NRF24L01(void);

void NRF24L01_SetLocalAddress(uint8_t* adr);
void NRF24L01_SetRemoteAddress(uint8_t* adr);

void NRF24L01_Transmit(uint8_t *data);

void NRF24L01_GetData(uint8_t *data);
void NRF24L01_SetChannel(uint8_t channel);

void NRF24L01_SetRF( NRF24L01_DataRate_t     DataRate,
                     NRF24L01_OutputPower_t  OutPwr);

NRF24L01_Transmit_Status_t NRF24L01_GetTransmissionStatus(void);

uint8_t NRF24L01_DataReady(void);

uint8_t NRF24L01_GetStatus(void);

uint8_t NRF24L01_Init( uint8_t channel,
                                    uint8_t payload_size);
									
uint8_t NRF24L01_GetRetransmissionsCount(void);



#ifdef __cplusplus
}
#endif

#endif

