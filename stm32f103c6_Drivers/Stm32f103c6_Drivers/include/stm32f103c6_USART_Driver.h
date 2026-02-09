/*
 * stm32f103c6_USART_Driver.h
 *
 *  Created on: Jan 30, 2026
 *      Author: anaal
 */

#ifndef INCLUDE_STM32F103C6_USART_DRIVER_H_
#define INCLUDE_STM32F103C6_USART_DRIVER_H_

#include "stm32f103x6.h"
#include "stm32f103c6_GPIO_Driver.h"
#include "stm32f103c6_RCC_Driver.h"



//Configuration Structure
typedef struct{
	uint8_t 		USART_Mode;		//TX/RX Enable Or Disable
									//@ref This param must be set based on @ref USART_MODE_DEFINE
	uint32_t		BaudRate;		//This param must be set based on @ref USART_BaudRate_DEFINE

	uint8_t			PayLoad_Length;	//Program the M bit in USART_CR1 to define the word length.
									//This param must be set based on @ref USART_PayLoad_Length_DEFINE

	uint8_t         Parity;			//This param must be set based on @ref USART_Parity_DEFINE

	uint8_t			StopBits;		//This param must be set based on @ref USART_StopBits_DEFINE

	uint8_t			HwFlowControl;  //This param must be set based on @ref USART_HwFlowControl_DEFINE

	uint8_t         Enable_IQR;		//This param must be set based on @ref USART_Enable_IQR_DEFINE

	void(* P_IRQ_CallBack)(void);
}USART_Config;




//------------------------------------------------------------
//****************************Genaric Macros****************************
//------------------------------------------------------------
#define MACRO_USARTDIV_MUL100(PCLK, BAUD)   ((25U * (PCLK)) / (4U * (BAUD)))  // USARTDIV * 100
#define MACRO_USART_MANTISSA(PCLK, BAUD)    (MACRO_USARTDIV_MUL100((PCLK),(BAUD)) / 100U)

#define MACRO_USART_FRACTION(PCLK, BAUD) \
    (((((MACRO_USARTDIV_MUL100((PCLK),(BAUD)) - (MACRO_USART_MANTISSA((PCLK),(BAUD)) * 100U)) * 16U) + 50U) / 100U) & 0xFU)

#define MACRO_USART_BRR(PCLK, BAUD) \
    ((MACRO_USART_MANTISSA((PCLK),(BAUD)) << 4) | MACRO_USART_FRACTION((PCLK),(BAUD)))




//------------------------------------------------------------
//						Ref Macros
//------------------------------------------------------------

//@ref USART_MODE_DEFINE
#define USART_MODE_RX					(uint32_t)(1<<2) 		//Bit 2 RE: Receiver enable
#define USART_MODE_TX					(uint32_t)(1<<3) 		//Bit 3 TE: Transmitter enable
#define USART_MODE_TX_RX				(uint32_t)(1<<3|1<<2)


//@ref USART_BaudRate_DEFINE
#define USART_BaudRate_2400        		2400U
#define USART_BaudRate_9600        		9600U
#define USART_BaudRate_19200       		19200U
#define USART_BaudRate_57600       		57600U
#define USART_BaudRate_115200      		115200U
#define USART_BaudRate_230400      		230400U
#define USART_BaudRate_460800      		460800U
#define USART_BaudRate_921600      		921600U
#define USART_BaudRate_2250000     		2250000U
#define USART_BaudRate_4500000     		4500000U

//@ref USART_PayLoad_Length_DEFINE
/*Bit 12 M: Word length
This bit determines the word length. It is set or cleared by software.
0: 1 Start bit, 8 Data bits, n Stop bit
1: 1 Start bit, 9 Data bits, n Stop bit*/
#define USART_PayLoad_Length_8DBits		(uint32_t)(0)
#define USART_PayLoad_Length_9DBits		(uint32_t)(1<<12)

//@ref USART_Parity_DEFINE
/*Bit 9 PS: Parity selection
This bit selects the odd or even parity when the parity generation/detection is enabled (PCE
bit set). It is set and cleared by software. The parity will be selected after the current byte.
0: Even parity
1: Odd parity*/
/*Bit 10 PCE: Parity control enable
This bit selects the hardware parity control (generation and detection). When the parity
control is enabled, the computed parity is inserted at the MSB position (9th bit if M=1; 8th bit
if M=0) and parity is checked on the received data. This bit is set and cleared by software.
Once it is set, PCE is active after the current byte (in reception and in transmission).
0: Parity control disabled
1: Parity control enabled*/
#define USART_NoParity					(uint32_t)(0)
#define USART_EvenParity				(uint32_t)(1<<10)
#define USART_OddParity					(uint32_t)(1<<10|1<<9)


//@ref USART_StopBits_DEFINE
/*Bits 13:12 STOP: STOP bits
	These bits are used for programming the stop bits.
	00: 1 Stop bit
	01: 0.5 Stop bit
	10: 2 Stop bits
	11: 1.5 Stop bit
	The 0.5 Stop bit and 1.5 Stop bit are not available for UART4 & UART5.*/
#define USART_StopBits_1				(uint32_t)(0)
#define USART_StopBits_2				(uint32_t)(2<<12)
#define USART_StopBits_half				(uint32_t)(1<<12)
#define USART_StopBits_1_half			(uint32_t)(3<<12)




//@ref USART_HwFlowControl_DEFINE
/*
 * Bit 9 CTSE: CTS enable
0: CTS hardware flow control disabled
1: CTS mode enabled, data is only transmitted when the CTS input is asserted (tied to 0).*/
/*
 * Bit 8 RTSE: RTS enable
0: RTS hardware flow control disabled
1: RTS interrupt enabled, data is only requested when there is space in the receive buffer.*/

#define USART_No_HwFlowControl			(uint32_t)(0)
#define USART_HwFlowControl_RTS			(uint32_t)(1<<8)
#define USART_HwFlowControl_CTS			(uint32_t)(1<<9)
#define USART_HwFlowControl_CTS_RTS			(uint32_t)((1<<9)|(1<<8))



//@ref USART_Enable_IQR_DEFINE
#define USART_Enable_IQR_None			(uint32_t)(0)
/*Bit 7 TXEIE: TXE interrupt enable
This bit is set and cleared by software.
0: Interrupt is inhibited
1: A USART interrupt is generated whenever TXE=1 in the USART_SR register*/
#define USART_Enable_IQR_TXE			(uint32_t)(1<<7)

/*Bit 6 TCIE: Transmission complete interrupt enable
This bit is set and cleared by software.
0: Interrupt is inhibited
1: A USART interrupt is generated whenever TC=1 in the USART_SR register*/
#define USART_Enable_IQR_TC				(uint32_t)(1<<6)

/*Bit 5 RXNEIE: RXNE interrupt enable
This bit is set and cleared by software.
0: Interrupt is inhibited
1: A USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register*/
#define USART_Enable_IQR_RXNE			(uint32_t)(1<<5)

/*Bit 8 PEIE: PE interrupt enable
This bit is set and cleared by software.
0: Interrupt is inhibited
1: A USART interrupt is generated whenever PE=1 in the USART_SR register*/
#define USART_Enable_IQR_PE			(uint32_t)(1<<8)



enum PollingMechanism{
	Enable,
	Disable,
};
//------------------------------------------------------------
//****************************API's****************************
//------------------------------------------------------------

void MCAL_USART_Init(USART_TypeDef *USARTx , USART_Config *USARTx_Config );
void MCAL_USART_DeInit(USART_TypeDef *USARTx);
void MCAL_USART_GPIO_SetPins(USART_TypeDef *USARTx);
void MCAL_USART_TransmitData(USART_TypeDef *USARTx , uint16_t *pTxBuffer , enum PollingMechanism PollEn );
void MCAL_USART_ReceiveData(USART_TypeDef *USARTx , uint16_t *pRxBuffer , enum PollingMechanism PollEn );
void MCAL_USART_Wait_TC(USART_TypeDef *USARTx);


#endif /* INCLUDE_STM32F103C6_USART_DRIVER_H_ */
