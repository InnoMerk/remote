#include "main.h"

void gpio_init(void);
uint8_t status_trace=0;
int main()
	{
		//-------------------------------------------------start initialisation---------------------------------------
		
		//PrintfConfig();
		gpio_init(); 
	
		
		
		//-------------------------------------------------tasks start------------------------------------------------
		
		xTaskCreate( vLedTask,
		                    (const char*)"LedTask",
						    configMINIMAL_STACK_SIZE,
						    NULL,
						    tskIDLE_PRIORITY + 1,
						    NULL);
		
		xTaskCreate( vGetLoadCPU,
		                     (const char*)"getLoadCPU",
							 configMINIMAL_STACK_SIZE,
							 NULL,
							 tskIDLE_PRIORITY + 1,
							 NULL);
			 
		xTaskCreate(vLcdTask,
		                    (const char*)"lcdTask",
							configMINIMAL_STACK_SIZE,
							NULL,
							tskIDLE_PRIORITY + 1,
							NULL);
							
	  xTaskCreate(vTransmitTask,
		                    (const char*)"transmit",
							configMINIMAL_STACK_SIZE,
							NULL,
							tskIDLE_PRIORITY + 1,
							NULL);
		xTaskCreate(vReceiveTask,
		                    (const char*)"receive",
							configMINIMAL_STACK_SIZE,
							NULL,
							tskIDLE_PRIORITY + 1,
							NULL);	
		xTaskCreate( vModeTask,
								 (const char*)"modeTask",
								 configMINIMAL_STACK_SIZE,
								 NULL,
								 tskIDLE_PRIORITY + 1,
								 NULL
							 );												
		
		vTaskStartScheduler();  //l88
		
		for(;;){__NOP();}
	}
	
	
void gpio_init(void)
{
      GPIO_InitTypeDef port;
		  //---------leds init---------------------------------
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 
			GPIO_StructInit(&port);
			port.GPIO_Mode = GPIO_Mode_Out_PP;
			port.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
			port.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(GPIOA, &port);
}
	
#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  
  //printf ("\r\n  file %s /n line= %d",file,line);
  
  while (1)
  {
 __NOP();
	}
}
#endif