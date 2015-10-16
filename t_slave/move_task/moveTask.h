#ifndef MOVETASK
#define MOVETASK

	#include "stm32f10x.h"
  #include "stm32f10x_rcc.h"
  #include "stm32f10x_gpio.h"
  
	#include "portmacro.h"
	#include "FreeRTOSConfig.h"
	#include "FreeRTOS.h"
	#include "croutine.h"
	#include "task.h"
	#include "queue.h"
	
	#include "limitSwitch.h"
	#include "controlTask.h"
	
	void vMoveTask (void *pvParameters); 

#endif