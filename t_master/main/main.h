#ifndef  MAIN_H
 #define MAIN_H
	//---------------------------------------------system---------------------------------------------------
	#include "stm32f10x.h"
	#include "stm32f10x_conf.h"
	
	//----------------------------------------------std-----------------------------------------------------
	
	//---------------------------------------------freeRTOS-------------------------------------------------
	#include "portmacro.h"
	#include "FreeRTOSConfig.h"
	#include "FreeRTOS.h"
	#include "croutine.h"
	#include "task.h"
	#include "queue.h"
	//----------------------------------------------user-----------------------------------------
	#include "lcd_cfg.h"
	#include "hd44780.h" 
	#include "hooks.h"
	
	
	
	//#include "printf.h"
	#include "ledOS.h"
	#include "loadCPU.h"
	#include "lcdTask.h"
	
#endif
	 
	//------------------------------------------global variable-------------------------------------

	//------------------------------------------prj functions----------------------------------------
