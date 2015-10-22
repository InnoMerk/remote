#include "hooks.h"
 
//*******************************************************************
//             useful RTOS project functions
/*******************************************************************/
volatile u32 idle_counter=0;
static uint8_t CPU_IDLE = 0;

//---------------------------------
uint8_t  GetCPU_IDLE(void) {
        return CPU_IDLE;
}

    
void vApplicationIdleHook( void )
		{
		
			static portTickType LastTick; 
      static uint32_t count;             
      static uint32_t max_count=144824 ;                

        count++;                                                 
      
        if((xTaskGetTickCount() - LastTick  ) > 1000)    
					{ 
                LastTick = xTaskGetTickCount();
                if(count > max_count) max_count = count;         
                CPU_IDLE = 100 * count / max_count;               
                count = 0; 
				#ifdef DEBUG
				  // printf("\r\n cpu= %d",100-CPU_IDLE);
					// printf("\r\n max_count= %d",max_count);
				#endif
						
					}//if
	}
		/*******************************************************************/
		void vApplicationMallocFailedHook( void )
		{
				for( ;; );
		}
		/*******************************************************************/
		void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
		{
				printf("stack overflow %x %s\r\n", (unsigned int)pxTask, (portCHAR *)pcTaskName);
		 
				for( ;; );
		}
		/*******************************************************************/
		void vApplicationTickHook( void )
		{
		}
/*******************************************************************/
