#ifndef LIMITSWITCH_H
#define LIMITSWITCH_H

  #include "stm32f10x.h"
  #include "stm32f10x_rcc.h"
  #include "stm32f10x_gpio.h"
  
	#include "portmacro.h"
	#include "FreeRTOSConfig.h"
	#include "FreeRTOS.h"
	#include "croutine.h"
	#include "task.h"
	#include "queue.h"
	
	#include "modeTask.h"

	extern QueueHandle_t xLimiterQueueCTRL;
	extern QueueHandle_t xLimiterQueueLCD;
	extern QueueHandle_t xLimitQueueRadioTx;
	
	void vLimitSwitchTask (void *pvParameters); 
	void vHitTask( void* pvParameters);

#endif