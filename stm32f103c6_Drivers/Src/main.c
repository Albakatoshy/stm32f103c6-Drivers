/*
 * main.c
 * STM32F103C6 Keypad Test Program
 */
#include "stm32f103c6_GPIO_Driver.h"
#include "Keypad.h"
#include "lcd.h"

// Keypad configuration
KEYPAD_PinConfig MyKeypad;
Lcd_Pins_t MyLcd;


// SWAPPED to match your Proteus wiring:
// Rows are connected to PA0-PA3 (left side of keypad: A, B, C, D)
// Columns are connected to PA4-PA7 (bottom of keypad: 1, 2, 3, 4)
// Correct pin assignment for standard keypad orientation

uint16_t RowPins[4] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3 };
uint16_t ColPins[4] = { GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7 };
uint16_t LcdPins[8] = { GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6,
						GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10 };

// Key pressed variable
volatile uint8_t Button;

int main(void)
{
    // 1. Enable GPIO clocks
    RCC_GPIOA_CLK_EN();
    RCC_GPIOB_CLK_EN();

    // 2. Configure Keypad structure
    MyKeypad.GPIOx = GPIOA;
    MyLcd.GPIOx = GPIOB;


    MyLcd.EN_PIN.GPIO_PinNumber = GPIO_PIN_0;
    MyLcd.EN_PIN.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
    MyLcd.EN_PIN.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_2MHz;

    MyLcd.RS_PIN.GPIO_PinNumber = GPIO_PIN_1;
    MyLcd.RS_PIN.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
    MyLcd.RS_PIN.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_2MHz;

    for(int i =0 ; i<8 ; i++){
    	MyLcd.LCD_PINS[i].GPIO_PinNumber = LcdPins[i];
    	MyLcd.LCD_PINS[i].GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
    	MyLcd.LCD_PINS[i].GPIO_Output_Speed = GPIO_OUTPUT_SPEED_2MHz;
    }


    // Columns → outputs push-pull (PA4-PA7)
    for(uint8_t c = 0; c < KEYPAD_COL; c++)
    {
        MyKeypad.KeypadCols[c].GPIO_PinNumber = ColPins[c];
        MyKeypad.KeypadCols[c].GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
        MyKeypad.KeypadCols[c].GPIO_Output_Speed = GPIO_OUTPUT_SPEED_2MHz;
    }

    // Rows → inputs pull-up (PA0-PA3)
    for(uint8_t r = 0; r < KEYPAD_ROWS; r++)
    {
        MyKeypad.KeypadRows[r].GPIO_PinNumber = RowPins[r];
        MyKeypad.KeypadRows[r].GPIO_Mode = GPIO_MODE_INPUT_PU;
    }

    // 3. Initialize keypad
    KEYPAD_Init(&MyKeypad);
    LCD_Init(&MyLcd);

    LCD_WriteCharData('y', &MyLcd);

    // 4. Main loop
    while(1)
    {
        Button = KEYPAD_GetChar(&MyKeypad);

        if(Button != RELEASED_BUTTON)
        {
            // Key was pressed - add breakpoint here to check value
            volatile uint8_t pressed = Button;
            LCD_WriteCharData(pressed, &MyLcd);
        }

        // Small delay
        for(volatile uint32_t i = 0; i < 100; i++);
    }
}
