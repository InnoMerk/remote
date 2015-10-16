#ifndef LOADCPU_H
#define LOADCPU_H

#include "hooks.h"
#include "printf.h"
#include "lcd_cfg.h"
#include "hd44780.h" 

extern QueueHandle_t xCPUloadLCD;
//extern QueueHandle_t xCPUloadSD;
void vGetLoadCPU( void *pvParameters );


#endif