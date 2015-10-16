#ifndef CONTROLTASK_H
#define CONTROLTASK_H




	
	#include "stm32f10x.h"
 
  #include "portmacro.h"
	#include "FreeRTOSConfig.h"
	#include "FreeRTOS.h"
	#include "croutine.h"
	#include "task.h"
	#include "queue.h"
	
	#include "stdlib.h"
	
	#include "loadCPU.h"
 

	#include "modeTask.h"
	#include "limitSwitch.h"
	#include "hitTask.h"
	

	extern QueueHandle_t xDirectionQueue;
	void vControlTask (void *pvParameters); 


#endif