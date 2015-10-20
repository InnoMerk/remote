#ifndef LCDTASK_H
#define LCDTASK_H

	#include "stm32f10x.h"
  #include "stm32f10x_rcc.h"
  #include "stm32f10x_gpio.h"
  
	#include "portmacro.h"
	#include "FreeRTOSConfig.h"
	#include "FreeRTOS.h"
	#include "croutine.h"
	#include "task.h"
	#include "queue.h"
	
	#include "loadCPU.h"
	#include "modeTask.h"

	
  void vLcdTask (void *pvParameters); 

#endif
