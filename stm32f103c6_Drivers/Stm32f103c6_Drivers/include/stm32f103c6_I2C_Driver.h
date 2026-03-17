/*
 * stm32f103c6_I2C_Driver.h
 *
 *  Created on: Mar 17, 2026
 *      Author: anaal
 */

#ifndef INCLUDE_STM32F103C6_I2C_DRIVER_H_
#define INCLUDE_STM32F103C6_I2C_DRIVER_H_

#include "stm32f103x6.h"
#include "stm32f103c6_GPIO_Driver.h"
#include "stm32f103c6_RCC_Driver.h"

typedef enum{
	I2C_EV_STOP,
	I2C_ERROR_AF,
	I2C_EV_ADDR_Matched,
	I2C_EV_DATA_REQ,
	I2C_EV_DATA_RCV
}SlaveState;

typedef struct{
	uint16_t DualAddressEn;
	uint16_t PrimarySlaveAddress;
	uint16_t SecondarySlaveAddress;
	uint32_t SlaveAdressingMode;		//@ref I2C_SLAVE_ADDRESSING_MODE
}I2C_SLAVE_DEVICE_ADD;

typedef struct{
	uint32_t				I2C_ModeSelection;	//@ref I2C_MODE
	uint32_t    			I2C_ClockSpeed;		//@ref I2C_CLK_SPEED
	uint32_t    			I2C_ClockStretching; //@ref I2C_CLK_STRETCH
	I2C_SLAVE_DEVICE_ADD	I2C_DeviceAddress;
	uint32_t				I2C_ACKControl;		//@ref I2C_ACK
	void(* P_SlaveEventCallBack)(SlaveState	State);
}I2C_Config;











//@ref I2C_CLK_SPEED
/* I2C Status register 2 (I2C_SR2
 * Bit 15 F/S: I2C master mode selection
0: Sm mode I2C
1: Fm mode I2C*/
//The following is the required sequence in master mode.
	//1-Program the peripheral input clock in I2C_CR2 Register in order to generate correct
	//timings
/* Configure the clock control registers ->Thigh = CCR * TPCLK1
• Configure the rise time register
• Program the I2C_CR1 register to enable the peripheral
• Set the START bit in the I2C_CR1 register to generate a Start condition*/
#define I2C_CLK_SPEED_STANDARD_50kHz		(50000U)
#define I2C_CLK_SPEED_STANDARD_100kHz		(100000U)
#define I2C_CLK_SPEED_FAST_400kHZ



//@ref I2C_CLK_STRETCH
/* I2C Control register 1 (I2C_CR1)
 * This bit is used to disable clock stretching in slave mode when ADDR or BTF flag is set, until
it is reset by software.
0: Clock stretching enabled
1: Clock stretching disabled*/
#define I2C_CLK_STRETCH_EN			(0x00000000)
#define I2C_CLK_STRETCH_DISABLE		(I2C_CR1_NOSTRETCH)



////@ref I2C_MODE

/* I2C Control register 1 (I2C_CR1)
 * Bit 1 SMBUS: SMBus mode
0: I2C mode
1: SMBus mode*/

#define I2C_MODE_I2C			(0x00000000)
#define I2C_MODE_SMBUS			(I2C_CR1_SMBUS)


//@ref I2C_SLAVE_ADDRESSING_MODE
/*I2C Own address register 1 (I2C_OAR1)
 * Bit 15 ADDMODE Addressing mode (slave mode)
0: 7-bit slave address (10-bit address not acknowledged)
1: 10-bit slave address (7-bit address not acknowledged)
 * */
#define I2C_SLAVE_7BIT_ADDRESSING_MODE			(0x00000000)
#define I2C_SLAVE_10BIT_ADDRESSING_MODE			(uint16_t)(1<<15)


//@ref I2C_ACK
/*
 * I2C Control register 1 (I2C_CR1)
 * Bit 10 ACK: Acknowledge enable
This bit is set and cleared by software and cleared by hardware when PE=0.
0: No acknowledge returned
1: Acknowledge returned after a byte is received (matched address or data)*/
#define I2C_ACK_ENABLE			(I2C_CR1_ACK)
#define I2C_ACK_DISABLE			(0x00000000)


#define I2C_EVENT_MASTER_BYTE_TRAMSIMITING	((uint32_t)0x00070080) //TRA,BUSY,MSL,TXE


typedef enum{
	WithStopCondition,
	WithoutStopCondition,
}StopCondition;

typedef enum{
	WithRepeatedStart,
	WithoutRepeatedStart,
}RepeatedStart;

typedef enum{
	FunctionStateEnable,
	FunctionStateDisable,
}FunctionState;

typedef enum{
	Reset = 0,
	Set = 1,
}FlagStatus;

typedef enum{
	I2C_FLAG_BUSY = 0,
	EV5 , //EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address
	EV6 ,
	EV7 ,
	EV8_1,
	EV8,

	MASTER_BYTE_TRAMSIMITING = ((uint32_t)0x00070080),  //TRA,BUSY,MSL,TXE
}Status;


typedef enum{
	I2C_Direction_Transmitter = 0,
	I2C_Direction_Receiver    = 1,
}I2CDirection;

void MCAL_I2C_Init(I2C_TypeDef *I2Cx , I2C_Config *I2Cx_Cfg);
void MCAL_I2C_DeInit(I2C_TypeDef *I2Cx);
void MCAL_I2C_GpioSetPins(I2C_TypeDef *I2Cx);

void MCAL_I2C_MasterTx(I2C_TypeDef *I2Cx , uint16_t DevAdd
					,  uint8_t* DataOut , uint32_t DateLen
					,  StopCondition Stop ,RepeatedStart Rstart);

void MCAL_I2C_MasterRx(I2C_TypeDef *I2Cx , uint16_t DevAdd
					,  uint8_t* DataOut , uint32_t DateLen
					,  StopCondition Stop ,RepeatedStart Rstart);

void MCAL_I2C_SlaveSendData(I2C_TypeDef *I2Cx , uint8_t Data);
uint8_t MCAL_I2C_SlaveReceiveData(I2C_TypeDef *I2Cx);


void MCAL_I2C_GenerateStart(I2C_TypeDef *I2Cx , FunctionState NewState
							,RepeatedStart Rstart);
void MCAL_I2C_GenerateStop(I2C_TypeDef *I2Cx , FunctionState NewState);
void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx , FunctionState NewState);
FlagStatus I2C_GetFlagStatus(I2C_TypeDef *I2Cx , Status flag);
void I2C_SendAddress(I2C_TypeDef *I2Cx , uint16_t Address , I2CDirection I2C_Dir);


#endif /* INCLUDE_STM32F103C6_I2C_DRIVER_H_ */
