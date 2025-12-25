/*
 * stm32f103c6_GPIO_Driver.h
 *
 *  Created on: Dec 22, 2025
 *      Author: Abdalrahman
 */

#ifndef INCLUDE_STM32F103C6_GPIO_DRIVER_H_
#define INCLUDE_STM32F103C6_GPIO_DRIVER_H_

//----------------------------------
//Includes
//----------------------------------

#include "stm32f103x6.h"

//----------------------------------
//Config Structure
//----------------------------------
typedef struct{
	uint16_t GPIO_PinNumber;      // Specifies the GPIO pin to be configured
	                              // This parameter must be a value of @ref GPIO_PINS_DEFINE
	uint8_t GPIO_Mode;            // Specifies the operation mode of the pin
	                              // This parameter must be a value of @ref GPIO_MODE_DEFINE
	uint8_t GPIO_Output_Speed;    // Specifies the speed of the output mode
	                              // This parameter must be a value of @ref GPIO_SPEED_DEFINE
}GPIO_PinConfig_t;

//----------------------------------
//Macros Configuration Reference
//----------------------------------

//@ref GPIO_PIN_STATE
#define GPIO_PIN_STATE_HIGH         1
#define GPIO_PIN_STATE_LOW          0

//@ref GPIO_PORT_STATE
#define GPIO_ALL_PORT_HIGH          0xFFFFFFFFUL
#define GPIO_ALL_PORT_LOW           0x00000000UL

//@ref GPIO_LOCK_RETURN
#define GPIO_LOCK_OK                1
#define GPIO_LOCK_ERROR             0

//@ref GPIO_PINS_DEFINE
#define GPIO_PIN_0                  ((uint16_t)0x0001)
#define GPIO_PIN_1                  ((uint16_t)0x0002)
#define GPIO_PIN_2                  ((uint16_t)0x0004)
#define GPIO_PIN_3                  ((uint16_t)0x0008)
#define GPIO_PIN_4                  ((uint16_t)0x0010)
#define GPIO_PIN_5                  ((uint16_t)0x0020)
#define GPIO_PIN_6                  ((uint16_t)0x0040)
#define GPIO_PIN_7                  ((uint16_t)0x0080)
#define GPIO_PIN_8                  ((uint16_t)0x0100)
#define GPIO_PIN_9                  ((uint16_t)0x0200)
#define GPIO_PIN_10                 ((uint16_t)0x0400)
#define GPIO_PIN_11                 ((uint16_t)0x0800)
#define GPIO_PIN_12                 ((uint16_t)0x1000)
#define GPIO_PIN_13                 ((uint16_t)0x2000)
#define GPIO_PIN_14                 ((uint16_t)0x4000)
#define GPIO_PIN_15                 ((uint16_t)0x8000)
#define GPIO_PIN_ALL                ((uint16_t)0xFFFF)

//@ref GPIO_MODE_DEFINE
/*
In input mode (MODE[1:0]=00):
0: Analog mode
1: Floating input (reset state)
2: Input with pull-up
3: Input with pull-down
In output mode (MODE[1:0] > 00):
4: General purpose output push-pull
5: General purpose output Open-drain
6: Alternate function output Push-pull
7: Alternate function output Open-drain
8: Alternate function input
*/
#define GPIO_MODE_ANALOG_INPUT          0x00000000u // Analog mode
#define GPIO_MODE_FLOATING_INPUT        0x00000001u // Floating input (reset state)
#define GPIO_MODE_INPUT_PU              0x00000002u // Input with pull-up
#define GPIO_MODE_INPUT_PD              0x00000003u // Input with pull-down
#define GPIO_MODE_OUTPUT_PUSH_PULL      0x00000004u // General purpose output push-pull
#define GPIO_MODE_OUTPUT_OPEN_DRAIN     0x00000005u // General purpose output Open-drain
#define GPIO_MODE_AL_OUTPUT_PUSH_PULL   0x00000006u // Alternate function output Push-pull
#define GPIO_MODE_AL_OUTPUT_OPEN_DRAIN  0x00000007u // Alternate function output Open-drain
#define GPIO_MODE_AL_INPUT              0x00000008u // Alternate function input

//@ref GPIO_SPEED_DEFINE
/*
00: Input mode (reset state)
01: Output mode, max speed 10 MHz.
10: Output mode, max speed 2 MHz.
11: Output mode, max speed 50 MHz
*/
#define GPIO_OUTPUT_SPEED_10MHz         0x00000001u // Output mode, max speed 10 MHz.
#define GPIO_OUTPUT_SPEED_2MHz          0x00000002u // Output mode, max speed 2 MHz.
#define GPIO_OUTPUT_SPEED_50MHz         0x00000003u // Output mode, max speed 50 MHz.

//----------------------------------
//APIs supported by "MCAL GPIO DRIVER"
//----------------------------------

void        MCAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_PinConfig_t *PinConfig);
void        MCAL_GPIO_DeInit(GPIO_TypeDef *GPIOx);

uint8_t     MCAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber);
uint16_t    MCAL_GPIO_ReadPort(GPIO_TypeDef *GPIOx);

void        MCAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint8_t Value);
void        MCAL_GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t Value);

void        MCAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber);
void        MCAL_GPIO_TogglePort(GPIO_TypeDef *GPIOx);

uint8_t     MCAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber);

#endif /* INCLUDE_STM32F103C6_GPIO_DRIVER_H_ */
