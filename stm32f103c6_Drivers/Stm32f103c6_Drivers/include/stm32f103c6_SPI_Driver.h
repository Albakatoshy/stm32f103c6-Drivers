/*
 * stm32f103c6_SPI_Driver.h
 *
 *  Created on: Feb 9, 2026
 *      Author: anaal
 */

#ifndef INCLUDE_STM32F103C6_SPI_DRIVER_H_
#define INCLUDE_STM32F103C6_SPI_DRIVER_H_

#include "stm32f103x6.h"
#include "stm32f103c6_GPIO_Driver.h"

typedef struct{
	uint8_t TXE:1;
	uint8_t RXE:1;
	uint8_t ERRI:1;
	uint8_t Reserved:5;
}S_IRQ_SRC;


//CONFIGURATION REGESTER
typedef struct{
	uint16_t	DeviceMode;				//@ref SPI_DEVICE_MODE
	uint16_t    CommunicationMode;		//@ref SPI_COMMUNICATION_MODE
	uint16_t	FrameFormat;			//@ref SPI_FRAME_FORMAT
	uint16_t    DataSize;				//@ref SPI_DATA_SIZE
	uint16_t	CLKPolarity;			//@ref SPI_CLK_POLARITY
	uint16_t    CLKPhase;				//@ref SPI_CLK_PHASE
	uint16_t    NSS;					//@ref SPI_NSS_CFG
	uint16_t	SpiBaudRatePreScaler;	//@ref SPI_BAUDRATE_PRESCALER
	uint16_t    IRQ_ENABLE;				//@ref SPI_IRQ
	void(* P_IRQ_CallBack)(S_IRQ_SRC);
}SPI_Config;


//				@ref SPI_DEVICE_MODE
/*Bit 2MSTR: Master selection
0: Slave configuration
1: Master configuration*/
#define SPI_DEVICE_MODE_MASTER			(0x1U<<2)
#define SPI_DEVICE_MODE_SLAVE			(0x00000000U)

//				@ref SPI_COMMUNICATION_MODE
/*Bit 15 BIDIMODE: Bidirectional data mode enable
0: 2-line unidirectional data mode selected
1: 1-line bidirectional data mode selected
Note: This bit is not used in I2S mode
Bit 14 BIDIOE: Output enable in bidirectional mode
This bit combined with the BIDImode bit selects the direction of transfer in bidirectional mode
0: Output disabled (receive-only mode)
1: Output enabled (transmit-only mode)*/
#define SPI_2LINE_UNIDRECTIONAL					(0x00000000U)
/*Bit 10
RXONLY: Receive only
This bit combined with the BIDImode bit selects the direction of transfer in 2-line
unidirectional mode. This bit is also useful in a multislave system in which this particular
slave is not accessed, the output from the accessed slave is not corrupted.
0: Full duplex (Transmit and receive)
1: Output disabled (Receive-only mode)*/
#define SPI_2LINE_UNIDRECTIONAL_RX_ONLY			(0x1U<<10)
/*Bit 14 BIDIOE: Output enable in bidirectional mode
This bit combined with the BIDImode bit selects the direction of transfer in bidirectional mode
0: Output disabled (receive-only mode)
1: Output enabled (transmit-only mode)
*/
#define SPI_1LINE_BIDIRECTIONAL_RX_ONLY			(0x1U<<15)
#define SPI_1LINE_BIDIRECTIONAL_TX_ONLY			((0x1U<<15)|(0x1U<<14))





//					//@ref SPI_DATA_SIZE
/*Bit 11 DFF: Data frame format
0: 8-bit data frame format is selected for transmission/reception
1: 16-bit data frame format is selected for transmission/reception*/
#define SPI_DATA_SIZE_8BIT					(0x00000000U)
#define SPI_DATA_SIZE_16BIT					(0x1U<<11)


//					@ref SPI_FRAME_FORMAT
/*Bit 7
LSBFIRST: Frame format
0: MSB transmitted first
1: LSB transmitted first*/

#define SPI_FRAME_FORMAT_MSB_FIRST			(0x00000000U)
#define SPI_FRAME_FORMAT_LSB_FIRST			(0x1U<<7)



//					@ref SPI_CLK_POLARITY
/*Bit1 CPOL: Clock polarity
0: CK to 0 when idle
1: CK to 1 when idle
Note: This bit should not be changed when communication is ongoing.
It is not used in I2S mode*/
#define SPI_CLK_POLARITY_IDLE_LOW			(0x00000000U)
#define SPI_CLK_POLARITY_IDLE_HIGH			(0x1U<<1)



//					@ref SPI_CLK_PHASE
/*Bit 0 CPHA: Clock phase
0: The first clock transition is the first data capture edge
1: The second clock transition is the first data capture edge*/
#define SPI_CLK_PHASE_1st_FirstDataCaptureEdge		(0x00000000U)
#define SPI_CLK_PHASE_2nd_FirstDataCaptureEdge		(0x1U<<0)




//					@ref SPI_NSS_CFG
/*Bit 9
SSM: Software slave management
When the SSM bit is set, the NSS pin input is replaced with the value from the SSI bit.
0: Software slave management disabled
1: Software slave management enabled
Note: This bit is not used in I2S mode
Bit 8 SSI: Internal slave select
This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the
NSS pin and the IO value of the NSS pin is ignored.*/

//SPI control register 2 (SPI_CR2)
/*Bit 2 SSOE: SS output enable
0: SS output is disabled in master mode and the cell can work in multimaster configuration
1: SS output is enabled in master mode and when the cell is enabled. The cell cannot work
in a multimaster environment.*/

#define SPI_NSS_HW_SLAVE								(0x00000000U)
#define SPI_NSS_HW_MASTER_SS_OUTPUT_ENABLE				(0x1U<<2)
#define SPI_NSS_HW_MASTER_SS_OUTPUT_DISABLE				~(0x1U<<2)

#define SPI_NSS_SW_NSS_SSI_RESET						(0x1U<<9)
#define SPI_NSS_SW_NSS_SSI_SET							((0x1U<<8)|(0x1U<<9))


//		@ref SPI_BAUDRATE_PRESCALER
/*Bits 5:3BR[2:0]: Baud rate control
000: fPCLK/2
001: fPCLK/4
010: fPCLK/8
011: fPCLK/16
100: fPCLK/32
101: fPCLK/64
110: fPCLK/128
111: fPCLK/256*/
/* SPI CR1 BR[2:0] field is bits 5:3 */
#define SPI_CR1_BR_Pos              (3U)
#define SPI_CR1_BR_Msk              (0x7U << SPI_CR1_BR_Pos)

/* Baud rate prescaler (write into CR1) */
#define SPI_BAUDRATE_PRESCALER_2    (0x0U << SPI_CR1_BR_Pos)  /* 000 */
#define SPI_BAUDRATE_PRESCALER_4    (0x1U << SPI_CR1_BR_Pos)  /* 001 */
#define SPI_BAUDRATE_PRESCALER_8    (0x2U << SPI_CR1_BR_Pos)  /* 010 */
#define SPI_BAUDRATE_PRESCALER_16   (0x3U << SPI_CR1_BR_Pos)  /* 011 */
#define SPI_BAUDRATE_PRESCALER_32   (0x4U << SPI_CR1_BR_Pos)  /* 100 */
#define SPI_BAUDRATE_PRESCALER_64   (0x5U << SPI_CR1_BR_Pos)  /* 101 */
#define SPI_BAUDRATE_PRESCALER_128  (0x6U << SPI_CR1_BR_Pos)  /* 110 */
#define SPI_BAUDRATE_PRESCALER_256  (0x7U << SPI_CR1_BR_Pos)  /* 111 */



//@ref 						SPI_IRQ
#define SPI_IRQ_NONE				(0x00000000U)
#define SPI_IRQ_TXEIE				(0x1U<<7)
#define SPI_IRQ_RXNEIE				(0x1U<<6)
#define SPI_IRQ_ERRIE				(0x1U<<5)


enum SpiPollingMechanism{
	PollingEnable,
	PollingDisable,
};



/*
 * ==============================================================================
 * 										API's
 * ==============================================================================
*/

void MCAL_SPI_Init(SPI_TypeDef *SPIx , SPI_Config *SPI_Config);
void MCAL_SPI_DeInit(SPI_TypeDef *SPIx , SPI_Config *SPI_Config);
void MCAL_SPI_GPIO_SetPins(SPI_TypeDef *SPIx);
void MCAL_SPI_SendData(SPI_TypeDef *SPIx , uint16_t *pTxBuffer , enum SpiPollingMechanism PollEn );
void MCAL_SPI_ReceiveData(SPI_TypeDef *SPIx , uint16_t *pRxBuffer , enum SpiPollingMechanism PollEn );
void MCAL_SPI_TX_RX(SPI_TypeDef *SPIx , uint16_t *pTxBuffer , enum SpiPollingMechanism PollEn );

#endif /* INCLUDE_STM32F103C6_SPI_DRIVER_H_ */
