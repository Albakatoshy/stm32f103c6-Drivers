/*
 * lcd.h
 *
 *  Created on: Dec 19, 2025
 *      Author: anaal
 */

#ifndef LCD_DRIVER_LCD_H_
#define LCD_DRIVER_LCD_H_


/* Section : Macro Declarations */

#include "lcd_CFG.h"

#define _LCD_CLEAR                      0x01
#define _LCD_RETURN_HOME                0x02
#define _LCD_ENRTY_MODE                 0x06
#define _LCD_CURSOR_OFF_DISPLAY_ON      0x0C
#define _LCD_CURSOR_OFF_DISPLAY_OFF     0x08
#define _LCD_CURSOR_ON_BLINKING_ON      0x0F
#define _LCD_CURSOR_ON_BLINKING_OFF     0x0E
#define _LCD_DISPLAY_SHIFT_RIGHT        0x1C
#define _LCD_DISPLAY_SHIFT_LEFT         0x18
#define _LCD_8BIT_MODE_2_LINE           0x38
#define _LCD_4BIT_MODE_2_LINE           0x28
#define _LCD_CGRAM_START                0x40
#define _LCD_DDRAM_START                0x80


typedef struct{
	GPIO_TypeDef 		*GPIOx;
	GPIO_PinConfig_t 	RS_PIN;
	GPIO_PinConfig_t 	RW_PIN;
	GPIO_PinConfig_t 	EN_PIN;
	GPIO_PinConfig_t 	LCD_PINS[8];
}Lcd_Pins_t;



void LCD_Init(Lcd_Pins_t *LCD);
void LCD_SendCommand(uint8_t cmd , Lcd_Pins_t *LCD);
void LCD_WriteCharData(uint8_t data , Lcd_Pins_t *LCD);
void LCD_SetCursor(uint8_t Column , uint8_t Rows , Lcd_Pins_t *LCD );
void LCD_WriteDataInPosition(uint8_t data , uint8_t Column , uint8_t Rows , Lcd_Pins_t *LCD);
void LCD_WriteStringData(uint8_t *str , Lcd_Pins_t *LCD);
void LCD_WriteStringDataInPosition(uint8_t *str  , uint8_t Column , uint8_t Rows , Lcd_Pins_t *LCD);

#endif /* LCD_DRIVER_LCD_H_ */
