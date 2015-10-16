#ifndef HOOKS_H
#define HOOKS_H
  
	#include "stm32f10x.h"
	
	#include "portmacro.h"
	#include "FreeRTOSConfig.h"
	#include "FreeRTOS.h"
	#include "croutine.h"
	#include "task.h"
	#include "queue.h"
    
	#include "printf.h"
	
	uint8_t  GetCPU_IDLE(void);
	void vApplicationIdleHook( void );
	void vApplicationMallocFailedHook( void );
	void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName );
	void vApplicationTickHook( void );
#endif 