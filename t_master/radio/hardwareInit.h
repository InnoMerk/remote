//==============================================================================--
//
// Title:       init.h
// Purpose:     A short description of the interface.
//
// Created on:  16.05.2015 at 23:57:15 by Hell computers.
// Copyright:   null. All Rights Reserved.
//
//==============================================================================--

#ifndef __hardwareInit_H__
#define __hardwareInit_H__


#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "misc.h"

/* IRQ */
#define NRF24L01_IRQ_PORT          GPIOB   				   // PORT B
#define NRF24L01_IRQ_LINE          EXTI_Line1   			   // Line1
#define NRF24L01_IRQ_PIN           GPIO_Pin_1   		   // Pin1
#define NRF24L01_PINSOURCE         GPIO_PinSource1	   // Pin1
#define NRF24L01_PORTSOURCE        GPIO_PortSourceGPIOB
#define NRF24L01_IRQ_CHANNEL       EXTI1_IRQn



/* CE */
#define NRF24L01_CE_PORT			  GPIOB
#define NRF24L01_CE_PIN			    GPIO_Pin_0

/* SPI */
#define NRF24L01_SPI_PORT 			2

#if NRF24L01_SPI_PORT == 1

	#define NRF24L01_SPI  SPI1
	#define SPI_SCK_PIN   GPIO_Pin_5         // PA5
	#define SPI_MISO_PIN  GPIO_Pin_6        // PA6
	#define SPI_MOSI_PIN  GPIO_Pin_7        // PA7
	#define SPI_CS_PIN    GPIO_Pin_4          // PA4
	#define SPI_GPIO_PORT GPIOA
	#define SPI_IRQ_CHANNEL    SPI1_IRQn
	
#elif NRF24L01_SPI_PORT == 2

	#define SPI_PORT        SPI2
	#define SPI_SCK_PIN     GPIO_Pin_13       // PB13
	#define SPI_MISO_PIN    GPIO_Pin_14      // PB14
	#define SPI_MOSI_PIN    GPIO_Pin_15      // PB15
	#define SPI_CS_PIN      GPIO_Pin_12        // PB12
	#define SPI_GPIO_PORT   GPIOB
	#define SPI_IRQ_CHANNEL SPI2_IRQn
	
#elif NRF24L01_SPI_PORT == 3

	#define SPI_PORT      SPI3
	#define SPI_SCK_PIN   GPIO_Pin_3       // PB3  (JTDO)
	#define SPI_MISO_PIN  GPIO_Pin_4      // PB4  (NJTRST)
	#define SPI_MOSI_PIN  GPIO_Pin_5      // PB5
	#define SPI_CS_PIN    GPIO_Pin_6        // PB6
	#define SPI_GPIO_PORT GPIOB
	#define SPI_IRQ_CHANNEL    SPI3_IRQn
	
#endif
/* Pins operations */
#define NRF24L01_CE_LOW()			    GPIO_ResetBits(NRF24L01_CE_PORT, NRF24L01_CE_PIN)
#define NRF24L01_CE_HIGH()			    GPIO_SetBits(NRF24L01_CE_PORT, NRF24L01_CE_PIN)
#define NRF24L01_CSN_LOW()			GPIO_ResetBits(SPI_GPIO_PORT, SPI_CS_PIN)
#define NRF24L01_CSN_HIGH()			GPIO_SetBits(SPI_GPIO_PORT, SPI_CS_PIN)


void radiomodule_hardware_init (void);


#endif  /* ndef __init_H__ */
