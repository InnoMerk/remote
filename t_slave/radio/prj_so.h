//==============================================================================
//
// Title:       prj_so.h
// Purpose:     A short description of the interface.
//
// Created on:  08.06.2015 at 20:58:47 by Hell computers.
// Copyright:   null. All Rights Reserved.
//
//==============================================================================

#ifndef __prj_so_H__
#define __prj_so_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "init.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables
extern uint32_t systime;
extern uint32_t SPI_idx;
extern char* SPI_TX_bufptr;
extern char* SPI_RX_bufptr;
extern char SPI_TX_buf[];
extern char SPI_RX_buf[];
extern char SPI_NULL_buf[];
extern uint8_t SPI_buf_idx;
extern SemaphoreHandle_t xNRF_IRQ_Semaphore;
extern SemaphoreHandle_t xSPI1_Mutex;
extern QueueHandle_t xSPI1_RX_Queue;
extern QueueHandle_t xSPI1_TX_Queue;



//==============================================================================
// Global functions

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __prj_so_H__ */
