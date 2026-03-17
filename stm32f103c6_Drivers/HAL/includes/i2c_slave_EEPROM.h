/*
 * i2c_slave_EEPROM.h
 *
 *  Created on: Mar 17, 2026
 *      Author: anaal
 */

#ifndef INCLUDES_I2C_SLAVE_EEPROM_H_
#define INCLUDES_I2C_SLAVE_EEPROM_H_

#include "stm32f103c6_I2C_Driver.h"

#define EEPROM_SlaveAddress		0x2A

void EEPROM_Init(void);
unsigned char EEPROM_WriteNbytes(unsigned int MemoryAddress , unsigned char *bytes , uint8_t DataLen);
unsigned char EEPROM_Readbyte(unsigned int MemoryAddress , uint8_t *DataOut , uint8_t DataLen);



#endif /* INCLUDES_I2C_SLAVE_EEPROM_H_ */
