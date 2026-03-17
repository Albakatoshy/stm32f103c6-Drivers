/*
 * stm32f103c6_I2C_Driver.c
 *
 *  Created on: Mar 17, 2026
 *      Author: anaal
 */


#include "stm32f103c6_I2C_Driver.h"

I2C_Config Gl_I2Cx_Cfg[2] = {NULL , NULL};


#define I2C1_INDEX			0
#define I2C2_INDEX			1

void MCAL_I2C_Init(I2C_TypeDef *I2Cx , I2C_Config *I2Cx_Cfg)
{
	uint16_t TempReg = 0 , FreqRange = 0 , RccResult = 0;
	uint32_t pclk1 = 8000000;
	//Enable Rcc Clock
	if(I2Cx == I2C1){
		Gl_I2Cx_Cfg[I2C1_INDEX] = *I2Cx_Cfg;
		RCC_I2C1_CLK_EN();
	}
	else if(I2Cx == I2C2){
		Gl_I2Cx_Cfg[I2C2_INDEX] = *I2Cx_Cfg;
		RCC_I2C2_CLK_EN();
	}

	if(I2Cx_Cfg->I2C_ModeSelection == I2C_MODE_I2C){
		I2Cx->I2C_CR1 = I2C_MODE_I2C;
	}
	else if(I2Cx_Cfg->I2C_ModeSelection == I2C_MODE_SMBUS){
		I2Cx->I2C_CR1 = I2C_MODE_SMBUS;
	}
	//The following is the required sequence in master mode.
		//1-Program the peripheral input clock in I2C_CR2 Register in order to generate correct
		//timings
	TempReg = I2Cx->I2C_CR2;
	TempReg &= ~(I2C_CR2_FREQ_Msk);
	pclk1 = MCAL_RCC_GetPCLK1_Freq();
	FreqRange = (uint16_t)(pclk1/1000000);
	TempReg |= FreqRange;
	I2Cx->I2C_CR2 = TempReg;
	/* Configure the clock control registers ->Thigh = CCR * TPCLK1
	 * * The CCR register must be configured while the I2C peripheral is disabled (PE=0).
	 * * --- STANDARD MODE (Sm) ---
	 * Target Speed: Up to 100 kHz
	 * Duty Cycle:   50% (Thigh = Tlow)
	 * Formula:      CCR = APB1_Clock_Freq / (Target_I2C_Freq * 2)
	 * Note:         Minimum allowed CCR value is 0x04.
	 * * --- FAST MODE (Fm) ---
	 * Target Speed: Up to 400 kHz
	 * Duty Cycle:   Selectable via DUTY bit
	 * * If DUTY = 0 (Tlow / Thigh = 2):
	 * Formula:    CCR = APB1_Clock_Freq / (Target_I2C_Freq * 3)
	 * Note:       Minimum allowed CCR value is 0x04.
	 * * If DUTY = 1 (Tlow / Thigh = 16/9):
	 * Formula:    CCR = APB1_Clock_Freq / (Target_I2C_Freq * 25)
	 * Note:       Minimum allowed CCR value is 0x01.*/
	I2Cx->I2C_CR1 &=~ (I2C_CR1_PE);
	TempReg = 0;
	if((I2Cx_Cfg->I2C_ClockSpeed == I2C_CLK_SPEED_STANDARD_100kHz)||
	   (I2Cx_Cfg->I2C_ClockSpeed == I2C_CLK_SPEED_STANDARD_50kHz)){
		RccResult = (uint16_t)(pclk1/((I2Cx_Cfg->I2C_ClockSpeed)<<1));
		TempReg |= RccResult;
		I2Cx->I2C_CCR = TempReg;
	}

	/* ==============================================================================
	 * I2C MAXIMUM RISE TIME REGISTER (TRISE) CONFIGURATION
	 * ==============================================================================
	 * Because I2C uses open-drain lines with pull-up resistors, signals take time
	 * to rise from LOW to HIGH. The hardware must know the maximum allowed rise
	 * time to compensate its internal clock generation.
	 * * Formula from reference manual: TRISE = (Max_Rise_Time / TPCLK1) + 1
	 * * --- STANDARD MODE (Sm) ---
	 * I2C Spec Max Rise Time: 1000 ns
	 * Simplified Formula:     TRISE = APB1_Freq_in_MHz + 1*/
	I2Cx->I2C_TRISE = FreqRange +1;

	/*
	• Program the I2C_CR1 register to enable the peripheral*/
	TempReg =0;
	TempReg = I2Cx->I2C_CR1;
	TempReg |= (uint16_t)(I2Cx_Cfg->I2C_ClockStretching | I2Cx_Cfg->I2C_ACKControl |
			              I2Cx_Cfg->I2C_ModeSelection);
	I2Cx->I2C_CR1 = TempReg;


	TempReg = 0;
	if(I2Cx_Cfg->I2C_DeviceAddress.DualAddressEn){

		TempReg = I2C_OAR2_ENDUAL;
		TempReg |= (I2Cx_Cfg->I2C_DeviceAddress.SecondarySlaveAddress) << I2C_OAR2_ADD2_Pos ;
		I2Cx->I2C_OAR2 = TempReg;
	}
	TempReg = 0;
	TempReg |= (I2Cx_Cfg->I2C_DeviceAddress.PrimarySlaveAddress) << 1 ;
	TempReg |= (I2Cx_Cfg->I2C_DeviceAddress.SlaveAdressingMode);
	I2Cx->I2C_OAR1 = TempReg;


	//Interrupt Mode (Slave Mode) if slave callback pointer != null
	if(I2Cx_Cfg->P_SlaveEventCallBack != NULL){
		//enable slave IQR states mode
		I2Cx->I2C_CR2 |= (I2C_CR2_ITBUFEN);
		I2Cx->I2C_CR2 |= (I2C_CR2_ITERREN);
		I2Cx->I2C_CR2 |= (I2C_CR2_ITEVTEN);
		if(I2Cx==I2C1){
			NVIC_IRQ31_I2C1_EV_ENABLE;
			NVIC_IRQ32_I2C1_ER_ENABLE;
		}
		else if(I2Cx==I2C2){
			NVIC_IRQ33_I2C2_EV_ENABLE;
			NVIC_IRQ34_I2C2_ER_ENABLE;
		}

		I2Cx->I2C_SR1 = 0;
		I2Cx->I2C_SR2 = 0;
	}


	I2Cx->I2C_CR1 |= I2C_CR1_PE;

}


void MCAL_I2C_DeInit(I2C_TypeDef *I2Cx)
{
	if(I2Cx == I2C1){
		RCC_I2C1_CLK_RESET();
		NVIC_IRQ31_I2C1_EV_DISABLE;
		NVIC_IRQ32_I2C1_ER_DISABLE;
	}
	else if(I2Cx == I2C2){
		RCC_I2C2_CLK_RESET();
		NVIC_IRQ33_I2C2_EV_DISABLE;
		NVIC_IRQ34_I2C2_ER_DISABLE;
	}
}


void MCAL_I2C_GpioSetPins(I2C_TypeDef *I2Cx)
{
	/*
I2Cx_SCL I2C clock Alternate function open drain
I2Cx_SDA I2C Data I/O Alternate function open drain*/

	GPIO_PinConfig_t PinCfg;

	if(I2Cx == I2C1){
		/*
		 * I2C1_SCL -> PB6
		 * I2C1_SDA -> PB7
		 * */
		PinCfg.GPIO_PinNumber = GPIO_PIN_6;
		PinCfg.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
		PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_OPEN_DRAIN;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		PinCfg.GPIO_PinNumber = GPIO_PIN_7;
		PinCfg.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
		PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_OPEN_DRAIN;
		MCAL_GPIO_Init(GPIOB, &PinCfg);


	}
	else if(I2Cx == I2C2){
		/*
		 * I2C2_SCL -> PB10
		 * I2C1_SDA -> PB11
		 * */
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
		PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_OPEN_DRAIN;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		PinCfg.GPIO_PinNumber = GPIO_PIN_11;
		PinCfg.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
		PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_OPEN_DRAIN;
		MCAL_GPIO_Init(GPIOB, &PinCfg);
	}
}




void MCAL_I2C_MasterTx(I2C_TypeDef *I2Cx , uint16_t DevAdd
					,  uint8_t* DataOut , uint32_t DateLen
					,  StopCondition Stop ,RepeatedStart Rstart)
{
	int l_counter = 0;

	// Set the START bit in the I2C_CR1 register to generate a Start condition
	MCAL_I2C_GenerateStart(I2Cx, FunctionStateEnable , Rstart);

	//Transfer sequence
	//EV5: SB=1, cleared by reading SR1 register
	while(!(I2C_GetFlagStatus(I2Cx, EV5)));
	//followed by writing DR register with Address
	I2C_SendAddress(I2Cx, DevAdd, I2C_Direction_Transmitter);
	//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2
	while(!(I2C_GetFlagStatus(I2Cx, EV6)));
	//EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR.
	//check TRA,BUSY,MSL,TXE
	while(!(I2C_GetFlagStatus(I2Cx, MASTER_BYTE_TRAMSIMITING)));

	for(l_counter = 0 ; l_counter<DateLen ; l_counter++){
		I2Cx->I2C_DR = DataOut[l_counter];
		//EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register
		while(!(I2C_GetFlagStatus(I2Cx, EV8)));
	}
	//EV8_2: TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition

	if(Stop == WithStopCondition){
		MCAL_I2C_GenerateStop(I2Cx, FunctionStateEnable);
	}

}


void MCAL_I2C_MasterRx(I2C_TypeDef *I2Cx , uint16_t DevAdd
					,  uint8_t* DataOut , uint32_t DateLen
					,  StopCondition Stop ,RepeatedStart Rstart)
{
	int l_counter = 0;
	uint8_t index = I2Cx == I2C1 ? I2C1_INDEX:I2C2_INDEX;
	// Set the START bit in the I2C_CR1 register to generate a Start condition
	MCAL_I2C_GenerateStart(I2Cx, FunctionStateEnable , Rstart);

	//Transfer sequence
	//EV5: SB=1, cleared by reading SR1 register
	while(!(I2C_GetFlagStatus(I2Cx, EV5)));
	//followed by writing DR register with Address
	I2C_SendAddress(I2Cx, DevAdd, I2C_Direction_Receiver);
	//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2
	while(!(I2C_GetFlagStatus(I2Cx, EV6)));

	I2C_AcknowledgeConfig(I2Cx , FunctionStateEnable);

	if(DateLen){
		for(l_counter = DateLen ; l_counter>1 ; l_counter--){
			//EV7
			while(!(I2C_GetFlagStatus(I2Cx, EV7)));
			*DataOut = I2Cx->I2C_DR;
			DataOut++;
		}
		I2C_AcknowledgeConfig(I2Cx , FunctionStateDisable);
	}

	if(Stop == WithStopCondition){
		MCAL_I2C_GenerateStop(I2Cx, FunctionStateEnable);
	}

	if(Gl_I2Cx_Cfg[index].I2C_ACKControl == I2C_ACK_ENABLE){
		I2C_AcknowledgeConfig(I2Cx , FunctionStateEnable);
	}
}

void MCAL_I2C_SlaveSendData(I2C_TypeDef *I2Cx , uint8_t Data)
{
	I2Cx->I2C_DR = Data;
}


uint8_t MCAL_I2C_SlaveReceiveData(I2C_TypeDef *I2Cx)
{
	return (uint8_t)I2Cx->I2C_DR;
}


void MCAL_I2C_GenerateStart(I2C_TypeDef *I2Cx , FunctionState NewState
							,RepeatedStart Rstart)
{
	if(Rstart == WithoutRepeatedStart){
		//check if the bus is idle or not
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	}
/*Bit 8 START: Start generation
This bit is set and cleared by software and cleared by hardware when start is sent or PE=0.
In Master Mode:
0: No Start generation
1: Repeated start generation
In Slave mode:
0: No Start generation
1: Start generation when the bus is free*/
	if(NewState== FunctionStateEnable){
		I2Cx->I2C_CR1 |= (I2C_CR1_START);
	}
	else{
		I2Cx->I2C_CR1 &= ~(I2C_CR1_START);
	}
}

void MCAL_I2C_GenerateStop(I2C_TypeDef *I2Cx , FunctionState NewState)
{
/*
 * Bit 9 STOP: Stop generation
The bit is set and cleared by software, cleared by hardware when a Stop condition is
detected, set by hardware when a timeout error is detected.
In Master Mode:
0: No Stop generation.
1: Stop generation after the current byte transfer or after the current Start condition is sent.
In Slave mode:
0: No Stop generation.
1: Release the SCL and SDA lines after the current byte transfer.*/
	if(NewState== FunctionStateEnable){
		I2Cx->I2C_CR1 |= (I2C_CR1_STOP);
	}
	else{
		I2Cx->I2C_CR1 &= ~(I2C_CR1_STOP);
	}
}

void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx , FunctionState NewState){
	/*Bit 10 ACK: Acknowledge enable
This bit is set and cleared by software and cleared by hardware when PE=0.
0: No acknowledge returned
1: Acknowledge returned after a byte is received (matched address or data)*/
	if(NewState== FunctionStateEnable){
		I2Cx->I2C_CR1 |= (I2C_CR1_ACK);
	}
	else{
		I2Cx->I2C_CR1 &= ~(I2C_CR1_ACK);
	}

}


FlagStatus I2C_GetFlagStatus(I2C_TypeDef *I2Cx , Status flag)
{
	volatile uint32_t DummyRead ;
	FlagStatus BitStatus = Reset;
	uint32_t flag1=0 ,flag2=0;
	uint32_t LastEvent = 0;
	switch(flag){
		case I2C_FLAG_BUSY:
			/* I2C Status register 2 (I2C_SR2
			 * Bit 1 BUSY: Bus busy
			0: No communication on the bus
			1: Communication ongoing on the bus– Set by hardware on detection of SDA or SCL low – cleared by hardware on detection of a Stop condition.
			It indicates a communication in progress on the bus. This information is still updated when
			the interface is disabled (PE=0).*/
			if((I2Cx->I2C_SR2)&(I2C_SR2_BUSY)){
				BitStatus = Set;
			}
			else {
				BitStatus = Reset;
			}
			break;
		case EV5:
			/*I2C Status register 1 (I2C_SR1)
			Bit 0 SB: Start bit (Master mode)
			0: No Start condition
			1: Start condition generated.– Set when a Start condition generated.– Cleared by software by reading the SR1 register followed by writing the DR register, or by
			hardware when PE=0*/
			if((I2Cx->I2C_SR1)&(I2C_SR1_SB)){
				BitStatus = Set;
			}
			else {
				BitStatus = Reset;
			}
			break;

		case EV6:
			/*I2C Status register 1 (I2C_SR1)
			Bit 1 ADDR: Address sent (master mode)/matched (slave mode)
			This bit is cleared by software reading SR1 register followed reading SR2, or by hardware
			when PE=0.
			Address matched (Slave)
			0: Address mismatched or not received.
			1: Received address matched.*/
			if((I2Cx->I2C_SR1)&(I2C_SR1_ADDR)){
				BitStatus = Set;
			}
			else {
				BitStatus = Reset;
			}
			break;
		case EV7:
			/*RxNE: Data register not empty (receivers)
			0: Data register empty
			1: Data register not empty*/
			if((I2Cx->I2C_SR1)&(I2C_SR1_RXNE)){
				BitStatus = Set;
			}
			else {
				BitStatus = Reset;
			}
			break;

		case EV8_1:
		case EV8:
			/*I2C Status register 1 (I2C_SR1)
			Bit 7 TxE: Data register empty (transmitters)
			0: Data register not empty
			1: Data register empty*/
			if((I2Cx->I2C_SR1)&(I2C_SR1_TXE)){
				BitStatus = Set;
			}
			else {
				BitStatus = Reset;
			}
			break;

		case MASTER_BYTE_TRAMSIMITING:
			flag1 = I2Cx->I2C_SR1;
			flag2  = I2Cx->I2C_SR2;
			flag2 = flag2<<16;
			LastEvent = (flag1|flag2)&((uint32_t)(0xFFFFFFFF));
			if((LastEvent & flag) == flag){
				BitStatus = Set;
			}
			else {
				BitStatus = Reset;
			}
			break;


	}

	return BitStatus;
}



void I2C_SendAddress(I2C_TypeDef *I2Cx , uint16_t Address , I2CDirection I2C_Dir)
{
	uint8_t index = I2Cx == I2C1 ? I2C1_INDEX:I2C2_INDEX;
	if(Gl_I2Cx_Cfg[index].I2C_DeviceAddress.SlaveAdressingMode == I2C_SLAVE_7BIT_ADDRESSING_MODE){
		Address =Address <<1;
		if(I2C_Dir == I2C_Direction_Transmitter){
		    // Reset Bit 0 for Write
		    Address &= ~(1<<0);
		}
		else{
		    // Set Bit 0 for Read
		    Address |= (1<<0);
		}
		I2Cx->I2C_DR = Address;
	}
	else{

	}
}



/*
 * 	.weak	I2C1_EV_IRQHandler
	.thumb_set I2C1_EV_IRQHandler,Default_Handler

	.weak	I2C1_ER_IRQHandler
	.thumb_set I2C1_ER_IRQHandler,Default_Handler

	.weak	I2C2_EV_IRQHandler
	.thumb_set I2C2_EV_IRQHandler,Default_Handler

	.weak	I2C2_ER_IRQHandler
	.thumb_set I2C2_ER_IRQHandler,Default_Handler*/

/* ================================================================
 * I2C IRQ Handler Logic (Slave Mode State Machine)
 * ================================================================ */

static void I2C_EV_IRQHandler_Core(I2C_TypeDef *I2Cx) {
    uint32_t itevten   = I2Cx->I2C_CR2 & I2C_CR2_ITEVTEN;
    uint32_t itbufen   = I2Cx->I2C_CR2 & I2C_CR2_ITBUFEN;
    uint32_t eventFlag = 0;

    uint8_t index = (I2Cx == I2C1) ? I2C1_INDEX : I2C2_INDEX;

    // 1. Handle ADDR: Address matched (Slave mode)
    eventFlag = I2Cx->I2C_SR1 & I2C_SR1_ADDR;
    if(itevten && eventFlag) {
        // Clear ADDR flag: Read SR1, then read SR2
        volatile uint32_t dummyRead = I2Cx->I2C_SR1;
        dummyRead = I2Cx->I2C_SR2;
        (void)dummyRead;

        // Notify application
        if(Gl_I2Cx_Cfg[index].P_SlaveEventCallBack) {
            Gl_I2Cx_Cfg[index].P_SlaveEventCallBack(I2C_EV_ADDR_Matched);
        }
    }

    // 2. Handle RxNE: Data register not empty (Slave Receiver)
    eventFlag = I2Cx->I2C_SR1 & I2C_SR1_RXNE;
    if(itevten && itbufen && eventFlag) {
        // We do not read the DR here. We notify the application,
        // and the application will call MCAL_I2C_SlaveReceiveData()
        if(Gl_I2Cx_Cfg[index].P_SlaveEventCallBack) {
            Gl_I2Cx_Cfg[index].P_SlaveEventCallBack(I2C_EV_DATA_RCV);
        }
    }

    // 3. Handle TxE: Data register empty (Slave Transmitter)
    eventFlag = I2Cx->I2C_SR1 & I2C_SR1_TXE;
    // Also check the TRA (Transmitter) bit in SR2 to ensure we are actually transmitting
    uint32_t isTransmitting = I2Cx->I2C_SR2 & I2C_SR2_TRA;
    if(itevten && itbufen && eventFlag && isTransmitting) {
        // Master is requesting data. Notify application to call MCAL_I2C_SlaveSendData()
        if(Gl_I2Cx_Cfg[index].P_SlaveEventCallBack) {
            Gl_I2Cx_Cfg[index].P_SlaveEventCallBack(I2C_EV_DATA_REQ);
        }
    }

    // 4. Handle STOPF: Stop detection (Slave mode)
    eventFlag = I2Cx->I2C_SR1 & I2C_SR1_STOPF;
    if(itevten && eventFlag) {
        // Clear STOPF flag: Read SR1, then write to CR1
        volatile uint32_t dummyRead = I2Cx->I2C_SR1;
        (void)dummyRead;
        I2Cx->I2C_CR1 |= 0x0000;

        // Notify application
        if(Gl_I2Cx_Cfg[index].P_SlaveEventCallBack) {
            Gl_I2Cx_Cfg[index].P_SlaveEventCallBack(I2C_EV_STOP);
        }
    }
}

static void I2C_ER_IRQHandler_Core(I2C_TypeDef *I2Cx) {
    uint32_t iterren = I2Cx->I2C_CR2 & I2C_CR2_ITERREN;
    uint8_t index = (I2Cx == I2C1) ? I2C1_INDEX : I2C2_INDEX;

    // 1. Handle AF: Acknowledge Failure
    if(iterren && (I2Cx->I2C_SR1 & I2C_SR1_AF)) {
        // Clear AF flag by writing 0
        I2Cx->I2C_SR1 &= ~(I2C_SR1_AF);

        // Notify application
        if(Gl_I2Cx_Cfg[index].P_SlaveEventCallBack) {
            Gl_I2Cx_Cfg[index].P_SlaveEventCallBack(I2C_ERROR_AF);
        }
    }

    // 2. Clear other errors to prevent infinite interrupt loops
    // Bus Error
    if(iterren && (I2Cx->I2C_SR1 & I2C_SR1_BERR)) {
        I2Cx->I2C_SR1 &= ~(I2C_SR1_BERR);
    }
    // Arbitration Lost
    if(iterren && (I2Cx->I2C_SR1 & I2C_SR1_ARLO)) {
        I2Cx->I2C_SR1 &= ~(I2C_SR1_ARLO);
    }
    // Overrun/Underrun
    if(iterren && (I2Cx->I2C_SR1 & I2C_SR1_OVR)) {
        I2Cx->I2C_SR1 &= ~(I2C_SR1_OVR);
    }
}


/* ================================================================
 * Actual ISR Hooks (Linked by Startup Code)
 * ================================================================ */

void I2C1_EV_IRQHandler(void) {
    I2C_EV_IRQHandler_Core(I2C1);
}

void I2C1_ER_IRQHandler(void) {
    I2C_ER_IRQHandler_Core(I2C1);
}

void I2C2_EV_IRQHandler(void) {
    I2C_EV_IRQHandler_Core(I2C2);
}

void I2C2_ER_IRQHandler(void) {
    I2C_ER_IRQHandler_Core(I2C2);
}




