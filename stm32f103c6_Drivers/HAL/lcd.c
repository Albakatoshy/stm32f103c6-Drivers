/*
 * lcd.c - Fixed for Proteus simulation
 */
#include "lcd.h"

// Delay function optimized for Proteus
void wait_ms(uint32_t ms){
    volatile uint32_t i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<100; j++);  // Increased for better Proteus timing
}

// Microsecond delay
void wait_us(uint32_t us){
    volatile uint32_t i;
    for(i=0; i<us*5; i++);  // Increased multiplier
}

// Check if LCD is busy (optional but recommended)
uint8_t LCD_IsBusy(Lcd_Pins_t *LCD){
    uint8_t status = 0;
    uint8_t i;

    // RS = 0, RW = 1 to read status
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RS_PIN.GPIO_PinNumber, 0);
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RW_PIN.GPIO_PinNumber, 1);

    // Configure data pins as input temporarily
    GPIO_PinConfig_t pin;
    pin.GPIO_Mode = GPIO_MODE_FLOATING_INPUT;

    for(i = 0; i < 8; i++){
        MCAL_GPIO_Init(LCD->GPIOx, &(LCD->LCD_PINS[i]));
    }

    wait_us(100);

    // Pulse EN to read
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 1);
    wait_us(100);

    // Read busy flag (bit 7)
    status = MCAL_GPIO_ReadPin(LCD->GPIOx, LCD->LCD_PINS[7].GPIO_PinNumber);

    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 0);

    // Reconfigure data pins as output
    pin.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
    pin.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_2MHz;

    for(i = 0; i < 8; i++){
        MCAL_GPIO_Init(LCD->GPIOx, &(LCD->LCD_PINS[i]));
    }

    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RW_PIN.GPIO_PinNumber, 0);

    return status;
}

// Initialize LCD in 8-bit mode
void LCD_Init(Lcd_Pins_t *LCD){
    uint8_t i;

    // Initialize RS, RW, and EN pins
    MCAL_GPIO_Init(LCD->GPIOx, &(LCD->RS_PIN));
    MCAL_GPIO_Init(LCD->GPIOx, &(LCD->RW_PIN));
    MCAL_GPIO_Init(LCD->GPIOx, &(LCD->EN_PIN));

    // Set initial states
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RW_PIN.GPIO_PinNumber, 0);  // Write mode
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 0);  // EN low
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RS_PIN.GPIO_PinNumber, 0);  // Command mode

    // Initialize 8 data pins
    for(i = 0; i < 8; i++){
        MCAL_GPIO_Init(LCD->GPIOx, &(LCD->LCD_PINS[i]));
        MCAL_GPIO_WritePin(LCD->GPIOx, LCD->LCD_PINS[i].GPIO_PinNumber, 0);
    }

    wait_ms(100);  // CRITICAL: Power-up delay increased

    // Function Set: 8-bit mode, 2 lines, 5x8 font
    LCD_SendCommand(_LCD_8BIT_MODE_2_LINE, LCD);
    wait_ms(10);  // Increased wait time

    LCD_SendCommand(_LCD_8BIT_MODE_2_LINE, LCD);
    wait_ms(5);

    LCD_SendCommand(_LCD_8BIT_MODE_2_LINE, LCD);
    wait_ms(5);

    // Display ON, Cursor OFF
    LCD_SendCommand(_LCD_CURSOR_OFF_DISPLAY_ON, LCD);
    wait_ms(5);

    // Clear display
    LCD_SendCommand(_LCD_CLEAR, LCD);
    wait_ms(10);  // Clear needs longer time

    // Entry mode: increment cursor, no shift
    LCD_SendCommand(_LCD_ENRTY_MODE, LCD);
    wait_ms(5);
}

// Send command to LCD
void LCD_SendCommand(uint8_t cmd, Lcd_Pins_t *LCD){
    uint8_t i;

    // Wait if busy (optional but recommended for Proteus)
    wait_ms(2);  // Simple delay instead of busy check

    // RS = 0 for command
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RS_PIN.GPIO_PinNumber, 0);

    // RW = 0 for write
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RW_PIN.GPIO_PinNumber, 0);

    // Stabilization delay
    wait_us(100);

    // Set data pins
    for(i = 0; i < 8; i++){
        MCAL_GPIO_WritePin(LCD->GPIOx, LCD->LCD_PINS[i].GPIO_PinNumber, (cmd >> i) & 0x1);
    }

    wait_us(100);  // Setup time

    // Pulse EN: Low -> High -> Low
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 0);
    wait_us(50);

    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 1);
    wait_us(100);  // Enable pulse width

    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 0);
    wait_us(100);  // Hold time

    // Command execution time - CRITICAL for Proteus
    if(cmd == _LCD_CLEAR || cmd == _LCD_RETURN_HOME){
        wait_ms(5);  // Clear and Home need more time
    } else {
        wait_ms(2);  // Standard command time
    }
}

// Write data (character) to LCD
void LCD_WriteCharData(uint8_t data, Lcd_Pins_t *LCD){
    uint8_t i;

    // Wait if busy
    wait_ms(1);

    // RS = 1 for data
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RS_PIN.GPIO_PinNumber, 1);

    // RW = 0 for write
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->RW_PIN.GPIO_PinNumber, 0);

    wait_us(100);

    // Set data pins
    for(i = 0; i < 8; i++){
        MCAL_GPIO_WritePin(LCD->GPIOx, LCD->LCD_PINS[i].GPIO_PinNumber, (data >> i) & 0x1);
    }

    wait_us(100);  // Setup time

    // Pulse EN: Low -> High -> Low
    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 0);
    wait_us(50);

    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 1);
    wait_us(100);  // Enable pulse width

    MCAL_GPIO_WritePin(LCD->GPIOx, LCD->EN_PIN.GPIO_PinNumber, 0);
    wait_us(100);  // Hold time

    wait_ms(2);  // Data write time - increased for Proteus
}

void LCD_SetCursor(uint8_t Column, uint8_t Rows, Lcd_Pins_t *LCD){
    switch(Rows){
        case 1: LCD_SendCommand((_LCD_DDRAM_START + Column), LCD); break;
        case 2: LCD_SendCommand((0xC0 + Column), LCD); break;
        case 3: LCD_SendCommand((0x94 + Column), LCD); break;
        case 4: LCD_SendCommand((0xD4 + Column), LCD); break;
        default: LCD_SendCommand(_LCD_RETURN_HOME, LCD); break;
    }
}

void LCD_WriteDataInPosition(uint8_t data, uint8_t Column, uint8_t Rows, Lcd_Pins_t *LCD){
    LCD_SetCursor(Column, Rows, LCD);
    wait_ms(2);  // Wait after cursor set
    LCD_WriteCharData(data, LCD);
}

void LCD_WriteStringData(uint8_t *str, Lcd_Pins_t *LCD){
    while(*str){
        LCD_WriteCharData(*str++, LCD);
        wait_ms(2);  // Increased delay between characters
    }
}

void LCD_WriteStringDataInPosition(uint8_t *str, uint8_t Column, uint8_t Rows, Lcd_Pins_t *LCD){
    LCD_SetCursor(Column, Rows, LCD);
    wait_ms(2);  // Wait after cursor set
    while(*str){
        LCD_WriteCharData(*str++, LCD);
        wait_ms(2);  // Increased delay between characters
    }
}
