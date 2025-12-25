/*
 * KeyPad_Port.h
 * Hardware abstraction layer header for keypad GPIO operations
 */

#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "Keypad_CFG.h"

typedef struct{
	GPIO_TypeDef 		*GPIOx;
	GPIO_PinConfig_t	KeypadRows[KEYPAD_ROWS];
	GPIO_PinConfig_t    KeypadCols[KEYPAD_COL];
}KEYPAD_PinConfig;

void KEYPAD_Init(KEYPAD_PinConfig *Keypad);
uint8_t KEYPAD_GetChar(KEYPAD_PinConfig *Keypad);


#endif // _KEYPAD_PORT_H_
