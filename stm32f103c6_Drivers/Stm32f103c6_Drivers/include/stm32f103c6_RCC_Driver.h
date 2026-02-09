/*
 * stm32f103c6_RCC_Driver.h
 *
 *  Created on: Jan 30, 2026
 *      Author: anaal
 */

#ifndef INCLUDE_STM32F103C6_RCC_DRIVER_H_
#define INCLUDE_STM32F103C6_RCC_DRIVER_H_

#include "stm32f103c6_GPIO_Driver.h"


#define HSI_RC_CLK		(uint32_t)8000000
#define HSE_CLK			(uint32_t)16000000

uint32_t MCAL_RCC_GetSYSCLK_Freq(void);
uint32_t MCAL_RCC_GetHCLK_Freq(void);
uint32_t MCAL_RCC_GetPCLK1_Freq(void);
uint32_t MCAL_RCC_GetPCLK2_Freq(void);


#endif /* INCLUDE_STM32F103C6_RCC_DRIVER_H_ */
