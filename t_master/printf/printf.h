#ifndef PRINTF_H
#define PRINTF_H

 #include "stm32f10x_gpio.h"
 #include "stm32f10x_rcc.h"
 #include "stm32f10x_usart.h"
 
 
 
#include "stdio.h"
	
 
 
 #define N 1  //number of USART
 #define USARTx  USART1
 
 int fputc(int ch, FILE *f) ;
 int sendchar(int ch);

 

 void PrintfConfig(void);
  
#endif



