#ifndef MODETASK_H
#define MODETASK_H

#define WAITING_MODE       10
#define STATIC_MODE        20
#define RANDOM_MODE				 30
#define RADIOCONTROL_MODE  40

#define UP                 10
#define DOWN               20 

#define UP_LIMIT           30
#define DOWN_LIMIT         40 

#define SHOT               50
#define EMPTY              60

#define FORWARD            10
#define REVERSE            20


#include "stm32f10x.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
 
  #include "portmacro.h"
	#include "FreeRTOSConfig.h"
	#include "FreeRTOS.h"
	#include "croutine.h"
	#include "task.h"
	#include "queue.h"
	

#include "radioTask.h"

	extern QueueHandle_t xModeQueueCTRL;
	extern QueueHandle_t xModeQueueLCD;
	extern QueueHandle_t xModeQueueRadioTx;
	
	void vModeTask (void *pvParameters); 

#endif