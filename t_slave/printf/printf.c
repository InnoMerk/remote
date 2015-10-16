#include "printf.h"
void USART1_Config(void);

FILE __stdout;
struct __FILE {int handle;};



//-------------------------------------------------
int fputc(int ch, FILE *f) 
	{
		return (sendchar(ch));
	}
//-------------------------------------------------
int sendchar(int ch)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, ch);
	return 0;
	}
//------------------------------------------------	
	void PrintfConfig(void)
   {
	  
		#if N == 1
		 USART1_Config();
		#endif 
	 }
	 
 //------------------------------------------------------------------------------------------ 
void USART1_Config(void)
	{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitTypeDef initGPIO;
   // Enable USART1 clock and release reset.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);
  
  /* Configure the GPIO ports( USART1 Transmit and Receive Lines) */
  /* Configure the USART1_Tx as Alternate function Push-Pull */
  GPIO_StructInit(&initGPIO);
	initGPIO.GPIO_Mode  = GPIO_Mode_AF_PP;
  initGPIO.GPIO_Pin   = GPIO_Pin_9;
  initGPIO.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &initGPIO);
  
  /* Configure the USART1_Rx as input floating */
  initGPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  initGPIO.GPIO_Pin = GPIO_Pin_10 ;
  GPIO_Init(GPIOA, &initGPIO);
  
/* USART1 configuration ------------------------------------------------------*/
/* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
*/
  USART_InitTypeDef initUSART;
	
  initUSART.USART_BaudRate = 115200;
	initUSART.USART_WordLength = USART_WordLength_8b;
  initUSART.USART_StopBits = USART_StopBits_1;
  initUSART.USART_Parity = USART_Parity_No;
  initUSART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  initUSART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure the USART1 */
  USART_Init(USART1, &initUSART);

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
						 
  
						
	}	 