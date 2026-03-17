
#include "i2c_slave_EEPROM.h"

void EEPROM_Init(void){
	//I2C Init
	I2C_Config I2C1_Cfg;
	I2C1_Cfg.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1_Cfg.I2C_ClockSpeed = I2C_CLK_SPEED_STANDARD_100kHz;
	I2C1_Cfg.I2C_ClockStretching = I2C_CLK_STRETCH_EN;
	I2C1_Cfg.I2C_ModeSelection = I2C_MODE_I2C;
	I2C1_Cfg.P_SlaveEventCallBack = NULL;

	MCAL_I2C_GpioSetPins(I2C1);
	MCAL_I2C_Init(I2C1, &I2C1_Cfg);
}

unsigned char EEPROM_WriteNbytes(unsigned int MemoryAddress , unsigned char *bytes , uint8_t DataLen){

	uint8_t i = 0;
	uint8_t buffer[256];
	buffer[0] = (uint8_t)(MemoryAddress>>8); //upper memory address
	buffer[1] = (uint8_t)(MemoryAddress);	 //lower memory address

	for (i = 2 ; i<(DataLen+2) ; i++){
		buffer[i] = bytes[i-2];
	}

	MCAL_I2C_MasterTx(I2C1, EEPROM_SlaveAddress,
			buffer, DataLen+2, WithStopCondition, WithoutRepeatedStart);

	return 0;
}

unsigned char EEPROM_Readbyte(unsigned int MemoryAddress , uint8_t *DataOut , uint8_t DataLen){

	uint8_t buffer[2];
	buffer[0] = (uint8_t)(MemoryAddress>>8); //upper memory address
	buffer[1] = (uint8_t)(MemoryAddress);	 //lower memory address
	MCAL_I2C_MasterTx(I2C1, EEPROM_SlaveAddress
			, &buffer, 2, WithoutStopCondition, WithoutRepeatedStart);
	MCAL_I2C_MasterRx(I2C1, EEPROM_SlaveAddress
			, DataOut, DataLen, WithStopCondition, WithRepeatedStart);

	return 0;
}
