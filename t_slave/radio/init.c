//==============================================================================
// Include files
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "misc.h"
#include "prj_extperiph.h"

//==============================================================================
// Defines
#define APB2PeripheralClkEnabled (\              RCC_APB2Periph_GPIOB	|	\
																	RCC_APB2Periph_AFIO		|	\
																	RCC_APB2Periph_SPI1		)

void init_rcc(void)
{
	RCC_APB2PeriphClockCmd(APB2PeripheralClkEnabled, ENABLE);
}


void init_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTIInit;

	GPIO_StructInit(&GPIO_InitStructure);
 
  // init SPI1-NRF24L01+
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN | SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure SPI_MASTER NSS pin */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(NRF24L01_IRQ_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// EXTI pin
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	// Configure EXTI line1
	EXTI_StructInit(&EXTIInit);
	EXTIInit.EXTI_Line = EXTI_Line1;          		// EXTI will be on line 2
	EXTIInit.EXTI_LineCmd = ENABLE;               // EXTI1 enabled
	EXTIInit.EXTI_Mode = EXTI_Mode_Interrupt;     // Generate IRQ
	EXTIInit.EXTI_Trigger = EXTI_Trigger_Falling; // IRQ on signal falling
	EXTI_Init(&EXTIInit);

}

void init_nvic()
{
NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* SPI2 interrupt - NRF24L01+ */
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0C;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Configure EXTI interrupt (NRF24L01 IRQ) */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0B;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

}

void init_spi(void)
{
SPI_InitTypeDef SPI_InitStructure;
	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(NRF24L01_SPI,&SPI_InitStructure);
	// NSS must be set to '1' due to NSS_Soft settings (otherwise it will be Multimaster mode).
	SPI_NSSInternalSoftwareConfig(NRF24L01_SPI,SPI_NSSInternalSoft_Set);
 	NRF24L01_CSN_HIGH();
	NRF24L01_CE_LOW();
 	SPI_Cmd(NRF24L01_SPI,ENABLE);
	SPI_I2S_ITConfig(NRF24L01_SPI, SPI_I2S_IT_RXNE, ENABLE);
}

void radiomodule_hardware_init (void)
{
	init_rcc();
	init_gpio();
	init_nvic();
	init_spi();
}
