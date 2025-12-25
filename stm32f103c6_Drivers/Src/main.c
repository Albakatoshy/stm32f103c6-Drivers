#include "stm32f103c6_GPIO_Driver.h"
#include "stm32f103c6_EXTI_Driver.h"
#include "lcd.h"

Lcd_Pins_t MyLcd;
uint8_t IQR_Flag = 0;

void MyWait(uint32_t x){
    for(uint32_t i = 0; i < x; i++){
        for(uint32_t l = 0; l < x; l++);
    }
}

void EXTI9_PB9_CallBack(void){
    IQR_Flag = 1;  // Only set flag
}

int main(void)
{
    RCC_GPIOA_CLK_EN();
    RCC_GPIOB_CLK_EN();
    RCC_AFIO_CLK_EN();

    // LCD data pins
    MyLcd.GPIOx = GPIOA;
    uint16_t data_pins[8] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
                              GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};
    for(uint8_t i = 0; i < 8; i++){
        MyLcd.LCD_PINS[i].GPIO_PinNumber = data_pins[i];
        MyLcd.LCD_PINS[i].GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
        MyLcd.LCD_PINS[i].GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
        MCAL_GPIO_Init(GPIOA, &MyLcd.LCD_PINS[i]);
    }

    // EN and RS
    MyLcd.EN_PIN.GPIO_PinNumber = GPIO_PIN_8;
    MyLcd.EN_PIN.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
    MyLcd.EN_PIN.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
    MCAL_GPIO_Init(GPIOA, &MyLcd.EN_PIN);

    MyLcd.RS_PIN.GPIO_PinNumber = GPIO_PIN_9;
    MyLcd.RS_PIN.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
    MyLcd.RS_PIN.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
    MCAL_GPIO_Init(GPIOA, &MyLcd.RS_PIN);

    // Initialize LCD
    LCD_Init(&MyLcd);

    // Configure EXTI
    EXTI_PinConfig_t EXTI_CFG;
    EXTI_CFG.EXTI_PIN = EXTI_DEFINE_EXTI9_PB9;
    EXTI_CFG.Trigger_Case = EXTI_TRIGGER_RISING;
    EXTI_CFG.P_IRQ_CallBack = EXTI9_PB9_CallBack;
    EXTI_CFG.IQR_EN = EXTI_IRQ_ENABLE;
    MCAL_EXTI_GPIO_Init(&EXTI_CFG);

    while(1){
        if(IQR_Flag){
            LCD_SendCommand(_LCD_CLEAR, &MyLcd);
            LCD_WriteStringData("ABOOOOD", &MyLcd);
            IQR_Flag = 0;
            MyWait(100);
        }
    }
}
