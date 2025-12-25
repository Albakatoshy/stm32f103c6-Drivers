#include "stm32f103c6_GPIO_Driver.h"
#include "lcd.h"

// Enable clocks for GPIO ports
void ClockInit(void){
    RCC_GPIOA_CLK_EN();
    RCC_GPIOB_CLK_EN();
}

int main(void)
{
    // Enable GPIO clocks
    ClockInit();

    // Configure LCD data pins (D0-D7) on GPIOA pins 0-7
    GPIO_PinConfig_t lcd_pins[8];
    uint16_t pin_numbers[8] = {
        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
        GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7
    };

    for(uint8_t i = 0; i < 8; i++){
        lcd_pins[i].GPIO_PinNumber = pin_numbers[i];
        lcd_pins[i].GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
        lcd_pins[i].GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
    }

    // Configure EN and RS pins
    GPIO_PinConfig_t lcd_en = {GPIO_PIN_9, GPIO_MODE_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_10MHz};
    GPIO_PinConfig_t lcd_rs = {GPIO_PIN_8, GPIO_MODE_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_10MHz};

    // Configure RW pin (tie it LOW for write mode)
    GPIO_PinConfig_t lcd_rw = {GPIO_PIN_10, GPIO_MODE_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_10MHz};

    // Create LCD struct and copy pins
    Lcd_Pins_t MyLcd;
    MyLcd.GPIOx = GPIOA;
    MyLcd.EN_PIN = lcd_en;
    MyLcd.RS_PIN = lcd_rs;
    MyLcd.RW_PIN = lcd_rw;

    for(uint8_t i = 0; i < 8; i++){
        MyLcd.LCD_PINS[i] = lcd_pins[i];
    }

    // Initialize LCD
    LCD_Init(&MyLcd);

    // Display ON, Cursor OFF
    LCD_SendCommand(_LCD_CURSOR_OFF_DISPLAY_ON, &MyLcd);

    // Write test string
    LCD_WriteStringDataInPosition("ABOOD", 5, 1, &MyLcd);
    LCD_WriteDataInPosition('K', 5, 2,  &MyLcd);

    while(1){
        // Main loop
    }
}
