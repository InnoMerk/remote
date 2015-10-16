#ifndef LED_H
#define LED_H
 #include "stm32f10x.h" 
 #include "stm32f10x_gpio.h"
 #include "stm32f10x_rcc.h"

 #define GREEN  GPIO_Pin_9
 #define BLUE   GPIO_Pin_8

void initLED(void);
#endif