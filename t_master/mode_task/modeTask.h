#ifndef MODETASK_H
#define MODETASK_H

#define MASTER_DEFAULT_MODE               10
#define MASTER_TEST_MODE                  20
#define MASTER_SETSLAVE_MODE              30

#define WAITING_SLAVE_MODE       10
#define STATIC_SLAVE_MODE        20
#define RANDOM_SLAVE_MODE				 30
#define RADIOCONTROL_SLAVE_MODE  40
#define TEST_SLAVE_MODE          50


#include "stm32f10x.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
 
#include "portmacro.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "croutine.h"
#include "task.h"
#include "queue.h"
	


	extern QueueHandle_t xModeQueueCTRL;
	extern QueueHandle_t xModeQueueLCD;
	
	void vModeTask (void *pvParameters); 

#endif