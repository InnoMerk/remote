//==============================================================================
// Include files
#include "stm32_nrf24l01.h"
#include "stm32_nrf24l01regs.h"
#include "prj_so.h"
#include "prj_config.h"
//==============================================================================
// Defines
#define NRF24L01_CLEAR_INTERRUPTS			NRF24L01_WriteBit(7, 4, Bit_SET); NRF24L01_WriteBit(7, 5, Bit_SET); NRF24L01_WriteBit(7, 6, Bit_SET)
#define NRF24L01_FLUSH_TX()						NRF24L01_CSN_LOW(); SPI_Send(NRF24L01_FLUSH_TX_MASK); NRF24L01_CSN_HIGH()
#define NRF24L01_FLUSH_RX()						NRF24L01_CSN_LOW(); SPI_Send(NRF24L01_FLUSH_RX_MASK); NRF24L01_CSN_HIGH()
//==============================================================================
// Types
typedef struct {
	uint8_t PayloadSize;						//Payload size
	uint8_t Channel;								//Channel selected
	NRF24L01_OutputPower_t OutPwr;	//Output power
	NRF24L01_DataRate_t DataRate;		//Data rate
} NRF24L01_t;

//==============================================================================
// Private functions definitions
void NRF24L01_InitPins(void);
void NRF24L01_WriteBit(uint8_t reg, uint8_t bit, uint8_t value);
uint8_t NRF24L01_ReadBit(uint8_t reg, uint8_t bit);
uint8_t NRF24L01_ReadRegister(uint8_t reg);
void NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t count);
void NRF24L01_WriteRegister(uint8_t reg, uint8_t value);
void NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t count);
void NRF24L01_SoftwareReset(void);
uint8_t NRF24L01_RxFifoEmpty(void);

//==============================================================================
// Private variables
NRF24L01_t NRF24L01_Struct;

//==============================================================================
// Functions 
/* SPI function BEGIN */
uint8_t SPI_Send(uint8_t dataOut)
{
char cIn;
	xSemaphoreTake(xSPI1_Mutex, portMAX_DELAY);
	SPI_I2S_SendData(NRF24L01_SPI, dataOut);
	xQueueReceive(xSPI1_RX_Queue, &cIn, NULL);
	xSemaphoreGive(xSPI1_Mutex);
	return cIn;
}

void SPI_ReadMulti(uint8_t* dataIn, uint8_t dummy, uint32_t count)
{
uint32_t i;
	xSemaphoreTake(xSPI1_Mutex, portMAX_DELAY);
	for (i = 1; i < count; i++) xQueueSend(xSPI1_TX_Queue, &dummy, portMAX_DELAY);
	SPI_I2S_SendData(NRF24L01_SPI, dummy);
	for (i = 0; i < count; i++) xQueueReceive(xSPI1_RX_Queue, &dataIn[i], NULL);
	xSemaphoreGive(xSPI1_Mutex);
}

void SPI_WriteMulti(uint8_t* dataOut, uint32_t count)
{
uint32_t i;
char cIn;
	xSemaphoreTake(xSPI1_Mutex, portMAX_DELAY);
	for (i = 1; i < count; i++) xQueueSend(xSPI1_TX_Queue, &dataOut[i], portMAX_DELAY);
	SPI_I2S_SendData(NRF24L01_SPI, dataOut[0]);
	for (i = 0; i < count; i++) xQueueReceive(xSPI1_RX_Queue, &cIn, NULL);
	xSemaphoreGive(xSPI1_Mutex);
}

void SPI_SendMulti(uint8_t* dataOut, uint8_t* dataIn, uint32_t count)
{
uint32_t i;
	xSemaphoreTake(xSPI1_Mutex, portMAX_DELAY);
	for (i = 1; i < count; i++) xQueueSend(xSPI1_TX_Queue, &dataOut[i], portMAX_DELAY);
	SPI_I2S_SendData(NRF24L01_SPI, dataOut[0]);
	for (i = 0; i < count; i++) xQueueReceive(xSPI1_RX_Queue, &dataIn[i], NULL);
	xSemaphoreGive(xSPI1_Mutex);
}
/* SPI function END*/

uint8_t NRF24L01_Init(uint8_t channel, uint8_t payload_size) {
	/* Max payload is 32bytes */
	if (payload_size > 32) {
		payload_size = 32;
	}

	/* Fill structure */
	NRF24L01_Struct.Channel = !channel; /* Set channel to some different value for NRF24L01_SetChannel() function */
	NRF24L01_Struct.PayloadSize = payload_size;
	NRF24L01_Struct.OutPwr = NRF24L01_OutputPower_0dBm;
	NRF24L01_Struct.DataRate = NRF24L01_DataRate_2M;

	/* Reset nRF24L01+ to power on registers values */
	NRF24L01_SoftwareReset();

	/* Channel select */
	NRF24L01_SetChannel(channel);

	/* Set pipeline to max possible 32 bytes */
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P0, NRF24L01_Struct.PayloadSize); // Auto-ACK pipe
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P1, NRF24L01_Struct.PayloadSize); // Data payload pipe
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P2, NRF24L01_Struct.PayloadSize);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P3, NRF24L01_Struct.PayloadSize);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P4, NRF24L01_Struct.PayloadSize);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P5, NRF24L01_Struct.PayloadSize);

	/* Set RF settings (2mbps, output power) */
	NRF24L01_SetRF(NRF24L01_Struct.DataRate, NRF24L01_Struct.OutPwr);

	/* Config register */
	NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG);

	/* Enable auto-acknowledgment for all pipes */
	NRF24L01_WriteRegister(NRF24L01_REG_EN_AA, 0x3F);

	/* Enable RX addresses */
	NRF24L01_WriteRegister(NRF24L01_REG_EN_RXADDR, 0x3F);

	/* Auto retransmit delay: 1000 (4x250) us and Up to 15 retransmit trials */
	NRF24L01_WriteRegister(NRF24L01_REG_SETUP_RETR, 0x4F);

	/* Dynamic length configurations: No dynamic length */
	NRF24L01_WriteRegister(NRF24L01_REG_DYNPD, (0 << NRF24L01_DPL_P0) | (0 << NRF24L01_DPL_P1) | (0 << NRF24L01_DPL_P2) | (0 << NRF24L01_DPL_P3) | (0 << NRF24L01_DPL_P4) | (0 << NRF24L01_DPL_P5));

	/* Clear FIFOs */
	NRF24L01_FLUSH_TX();
	NRF24L01_FLUSH_RX();

	/* Clear interrupts */
	NRF24L01_CLEAR_INTERRUPTS;

	/* Go to RX mode */
	NRF24L01_PowerUpRx();

	/* Return OK */
	return 1;
}

void NRF24L01_GetLocalAddress(uint8_t *adr) {
	NRF24L01_CE_LOW();
	NRF24L01_ReadRegisterMulti(NRF24L01_REG_RX_ADDR_P1, adr, 5);
	NRF24L01_CE_HIGH();
}

void NRF24L01_SetLocalAddress(uint8_t *adr) {
	NRF24L01_CE_LOW();
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, adr, 5);
	NRF24L01_CE_HIGH();
}

void NRF24L01_SetRemoteAddress(uint8_t *adr) {
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, adr, 5);
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, adr, 5);
}

void NRF24L01_WriteBit(uint8_t reg, uint8_t bit, uint8_t value) {
	uint8_t tmp;
	/* Read register */
	tmp = NRF24L01_ReadRegister(reg);
	/* Make operation */
	if (value) {
		tmp |= 1 << bit;
	} else {
		tmp &= ~(1 << bit);
	}
	/* Write back */
	NRF24L01_WriteRegister(reg, tmp);
}

uint8_t NRF24L01_ReadBit(uint8_t reg, uint8_t bit) {
	uint8_t tmp;
	tmp = NRF24L01_ReadRegister(reg);
	if (!NRF24L01_CHECK_BIT(tmp, bit)) {
		return 0;
	}
	return 1;
}

uint8_t NRF24L01_ReadRegister(uint8_t reg) {
	uint8_t value;
	NRF24L01_CSN_LOW();
	SPI_Send(NRF24L01_READ_REGISTER_MASK(reg));
	value = SPI_Send(NRF24L01_NOP_MASK);
	NRF24L01_CSN_HIGH();

	return value;
}

void NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t count) {
	NRF24L01_CSN_LOW();
	SPI_Send(NRF24L01_READ_REGISTER_MASK(reg));
	SPI_ReadMulti(data, NRF24L01_NOP_MASK, count);
	NRF24L01_CSN_HIGH();
}

void NRF24L01_WriteRegister(uint8_t reg, uint8_t value) {
	NRF24L01_CSN_LOW();
	SPI_Send(NRF24L01_WRITE_REGISTER_MASK(reg));
	SPI_Send(value);
	NRF24L01_CSN_HIGH();
}

void NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t count) {
	NRF24L01_CSN_LOW();
	SPI_Send(NRF24L01_WRITE_REGISTER_MASK(reg));
	SPI_WriteMulti(data, count);
	NRF24L01_CSN_HIGH();
}

void NRF24L01_PowerUpTx(void) {
	NRF24L01_CLEAR_INTERRUPTS;
	NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG | (0 << NRF24L01_PRIM_RX) | (1 << NRF24L01_PWR_UP));
}

void NRF24L01_PowerUpRx(void) {
	/* Clear RX buffer */
	NRF24L01_FLUSH_RX();
	/* Clear interrupts */
	NRF24L01_CLEAR_INTERRUPTS;
	/* Disable RX/TX mode */
	NRF24L01_CE_LOW();
	/* Setup RX mode */
	NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG | 1 << NRF24L01_PWR_UP | 1 << NRF24L01_PRIM_RX);
	/* Start listening */
	NRF24L01_CE_HIGH();
}

void NRF24L01_PowerDown(void) {
	NRF24L01_CE_LOW();
	NRF24L01_WriteBit(NRF24L01_REG_CONFIG, NRF24L01_PWR_UP, Bit_RESET);
}

void NRF24L01_Transmit(uint8_t *data) {
	uint8_t count = NRF24L01_Struct.PayloadSize;

	/* Chip enable put to low, disable it */
	NRF24L01_CE_LOW();

	/* Go to power up tx mode */
	NRF24L01_PowerUpTx();

	/* Clear TX FIFO from NRF24L01+ */
	NRF24L01_FLUSH_TX();

	/* Send payload to nRF24L01+ */
	NRF24L01_CSN_LOW();
	/* Send write payload command */
	SPI_Send(NRF24L01_W_TX_PAYLOAD_MASK);
	/* Fill payload with data*/
	SPI_WriteMulti(data, count);
	/* Disable SPI */
	NRF24L01_CSN_HIGH();

	/* Send data! */
	NRF24L01_CE_HIGH();
}

void NRF24L01_GetData(uint8_t* data) {
	/* Pull down chip select */
	NRF24L01_CSN_LOW();
	/* Send read payload command*/
	SPI_Send(NRF24L01_R_RX_PAYLOAD_MASK);
	/* Read payload */
	SPI_SendMulti(data, data, NRF24L01_Struct.PayloadSize);
	/* Pull up chip select */
	NRF24L01_CSN_HIGH();

	/* Reset status register, clear RX_DR interrupt flag */
	NRF24L01_WriteRegister(NRF24L01_REG_STATUS, (1 << NRF24L01_RX_DR));
}

uint8_t NRF24L01_DataReady(void) {
	uint8_t status = NRF24L01_GetStatus();

	if (NRF24L01_CHECK_BIT(status, NRF24L01_RX_DR)) {
		return 1;
	}
	return !NRF24L01_RxFifoEmpty();
}

uint8_t NRF24L01_RxFifoEmpty(void) {
	uint8_t reg = NRF24L01_ReadRegister(NRF24L01_REG_FIFO_STATUS);
	return NRF24L01_CHECK_BIT(reg, NRF24L01_RX_EMPTY);
}

uint8_t NRF24L01_GetStatus(void) {
	uint8_t status;

	NRF24L01_CSN_LOW();
	/* First received byte is always status register */
	status = SPI_Send(NRF24L01_NOP_MASK);
	/* Pull up chip select */
	NRF24L01_CSN_HIGH();

	return status;
}

NRF24L01_Transmit_Status_t NRF24L01_GetTransmissionStatus(void) {
	uint8_t status = NRF24L01_GetStatus();
	if (NRF24L01_CHECK_BIT(status, NRF24L01_TX_DS)) {
		/* Successfully sent */
		return NRF24L01_Transmit_Status_Ok;
	} else if (NRF24L01_CHECK_BIT(status, NRF24L01_MAX_RT)) {
		/* Message lost */
		return NRF24L01_Transmit_Status_Lost;
	}

	/* Still sending */
	return NRF24L01_Transmit_Status_Sending;
}

void NRF24L01_SoftwareReset(void) {
	uint8_t data[5];

	NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, 		NRF24L01_REG_DEFAULT_VAL_CONFIG);
	NRF24L01_WriteRegister(NRF24L01_REG_EN_AA,		NRF24L01_REG_DEFAULT_VAL_EN_AA);
	NRF24L01_WriteRegister(NRF24L01_REG_EN_RXADDR, 	NRF24L01_REG_DEFAULT_VAL_EN_RXADDR);
	NRF24L01_WriteRegister(NRF24L01_REG_SETUP_AW, 	NRF24L01_REG_DEFAULT_VAL_SETUP_AW);
	NRF24L01_WriteRegister(NRF24L01_REG_SETUP_RETR, 	NRF24L01_REG_DEFAULT_VAL_SETUP_RETR);
	NRF24L01_WriteRegister(NRF24L01_REG_RF_CH, 		NRF24L01_REG_DEFAULT_VAL_RF_CH);
	NRF24L01_WriteRegister(NRF24L01_REG_RF_SETUP, 	NRF24L01_REG_DEFAULT_VAL_RF_SETUP);
	NRF24L01_WriteRegister(NRF24L01_REG_STATUS, 		NRF24L01_REG_DEFAULT_VAL_STATUS);
	NRF24L01_WriteRegister(NRF24L01_REG_OBSERVE_TX, 	NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX);
	NRF24L01_WriteRegister(NRF24L01_REG_RPD, 		NRF24L01_REG_DEFAULT_VAL_RPD);

	//P0
	data[0] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, data, 5);

	//P1
	data[0] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, data, 5);

	NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P2, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P3, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P4, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P5, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5);

	//TX
	data[0] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, data, 5);

	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P0, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P0);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P1, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P1);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P2, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P2);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P3, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P3);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P4, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P4);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P5, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P5);
	NRF24L01_WriteRegister(NRF24L01_REG_FIFO_STATUS, NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS);
	NRF24L01_WriteRegister(NRF24L01_REG_DYNPD, 		NRF24L01_REG_DEFAULT_VAL_DYNPD);
	NRF24L01_WriteRegister(NRF24L01_REG_FEATURE, 	NRF24L01_REG_DEFAULT_VAL_FEATURE);
}

uint8_t NRF24L01_GetRetransmissionsCount(void) {
	/* Low 4 bits */
	return NRF24L01_ReadRegister(NRF24L01_REG_OBSERVE_TX) & 0x0F;
}

void NRF24L01_SetChannel(uint8_t channel) {
	if (channel <= 125 && channel != NRF24L01_Struct.Channel) {
		/* Store new channel setting */
		NRF24L01_Struct.Channel = channel;
		/* Write channel */
		NRF24L01_WriteRegister(NRF24L01_REG_RF_CH, channel);
	}
}

void NRF24L01_SetRF(NRF24L01_DataRate_t DataRate, NRF24L01_OutputPower_t OutPwr) {
	uint8_t tmp = 0;
	NRF24L01_Struct.DataRate = DataRate;
	NRF24L01_Struct.OutPwr = OutPwr;

	if (DataRate == NRF24L01_DataRate_2M) {
		tmp |= 1 << NRF24L01_RF_DR_HIGH;
	} else if (DataRate == NRF24L01_DataRate_250k) {
		tmp |= 1 << NRF24L01_RF_DR_LOW;
	}
	/* If 1Mbps, all bits set to 0 */

	if (OutPwr == NRF24L01_OutputPower_0dBm) {
		tmp |= 3 << NRF24L01_RF_PWR;
	} else if (OutPwr == NRF24L01_OutputPower_M6dBm) {
		tmp |= 2 << NRF24L01_RF_PWR;
	} else if (OutPwr == NRF24L01_OutputPower_M12dBm) {
		tmp |= 1 << NRF24L01_RF_PWR;
	}

	NRF24L01_WriteRegister(NRF24L01_REG_RF_SETUP, tmp);
}

void init_NRF24L01(void)
{
uint8_t LocalAddress[] = LOCAL_NRF_ADDRESS;
uint8_t RemoteAddress[] = REMOTE_NRF_ADDRESS;
	NRF24L01_Init(NRF_CHANNEL, NRF_PAYLOAD);
	NRF24L01_SetRF(NRF_DATARATE, NRF_TX_POWER);
	NRF24L01_SetLocalAddress(LocalAddress);
	NRF24L01_SetRemoteAddress(RemoteAddress);
}

void initr_NRF24L01(void)
{
uint8_t LocalAddress[] = REMOTE_NRF_ADDRESS;
uint8_t RemoteAddress[] = LOCAL_NRF_ADDRESS;
	NRF24L01_Init(NRF_CHANNEL, NRF_PAYLOAD);
	NRF24L01_SetRF(NRF_DATARATE, NRF_TX_POWER);
	NRF24L01_SetLocalAddress(LocalAddress);
	NRF24L01_SetRemoteAddress(RemoteAddress);
}
