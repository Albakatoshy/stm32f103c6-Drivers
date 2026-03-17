#define MCU_ACT_AS_MASTER
// #define MCU_ACT_AS_SLAVE

#include <stdint.h>
#include "stm32f103x6.h"
#include "stm32f103c6_GPIO_Driver.h"
#include "stm32f103c6_EXTI_Driver.h"
#include "stm32f103c6_RCC_Driver.h"
#include "stm32f103c6_USART_Driver.h"
#include "stm32f103c6_SPI_Driver.h"
#include "stm32f103c6_I2C_Driver.h"
#include "i2c_slave_EEPROM.h"




int main(void)
{
    RCC_GPIOA_CLK_EN();
    RCC_GPIOB_CLK_EN();
    RCC_AFIO_CLK_EN();

    unsigned char ch1[] = {0x1,0x2,0x3,0x4,0x5,0x7,0x8};
    unsigned char ch2[7] = {0};

    EEPROM_Init();
    EEPROM_WriteNbytes(0xAF, ch1, 7);
    EEPROM_Readbyte(0xAF, ch2, 7);

    ch1[0] = 0xA;
    ch1[1] = 0xB;
    ch1[2] = 0xC;
    ch1[3] = 0xD;

    EEPROM_WriteNbytes(0xFFF, ch1, 4);
    EEPROM_Readbyte(0xFFF, ch2, 4);

    while (1)
    {
        /* main loop idle */
    }
}
