//==============================================================================
//
// Title:       prj_config.h
// Purpose:     A short description of the interface.
//
// Created on:  09.07.2015 at 22:05:53 by Hell computers.
// Copyright:   null. All Rights Reserved.
//
//==============================================================================

#ifndef __prj_config_H__
#define __prj_config_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files


//==============================================================================
// Constants
#define LOCAL_NRF_ADDRESS {0xE7,0xE7,0xE7,0xE7,0xE7};
#define REMOTE_NRF_ADDRESS {0x7E,0x7E,0x7E,0x7E,0x7E};
#define NRF_CHANNEL 15
#define NRF_PAYLOAD 32              /* max = 32 bytes */
#define NRF_DATARATE NRF24L01_DataRate_250k
#define NRF_TX_POWER NRF24L01_OutputPower_0dBm


//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __prj_config_H__ */
