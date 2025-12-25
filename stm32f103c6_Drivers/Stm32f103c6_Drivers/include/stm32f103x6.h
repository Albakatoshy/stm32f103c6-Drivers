/*
 * stm32f103x6.h
 *
 *  Created on: Dec 15, 2025
 *      Author: Abdalrahman
 */

#ifndef INCLUDE_STM32F103X6_H_
#define INCLUDE_STM32F103X6_H_


//----------------------------------
//Includes
//----------------------------------

#include <stdlib.h>
#include <stdint.h>

//----------------------------------
//Base Addresses For The Memory
//----------------------------------

#define FLASH_MEMORY_BASE_ADDRESS						0x08000000UL
#define SYSTEM_MEMORY_BASE_ADDRESS						0x1FFFF000UL
#define SRAM_MEMORY_BASE_ADDRESS						0x20000000UL

#define PERIPHERAL_BASE_ADDRESS							0x40000000UL

#define CORTEX_M3_INTERNAL_PERIPHERAL_BASE_ADDRESS		0xE0000000UL

//----------------------------------
//Base Addresses For The AHB Peripherals
//----------------------------------

//Reset and clock control RCC
#define RCC_BASE_ADDRESS								0x40021000UL


//----------------------------------
//Base Addresses For The APB2 Peripherals
//----------------------------------

//AFIO
#define AFIO_BASE_ADDRESS								0x40010000UL

//EXTI
#define EXTI_BASE_ADDRESS								0x40010400UL

//GPIO (A,B) Ports is fully included in LQF48 Package

#define GPIOA_BASE_ADDRESS								0x40010800UL
#define GPIOB_BASE_ADDRESS								0x40010C00UL


//GPIO (C,D) Ports is Partially included in LQF48 Package

#define GPIOC_BASE_ADDRESS								0x40011000UL
#define GPIOD_BASE_ADDRESS								0x40011400UL


//GPIO (E)   Ports not included in LQF48 Package
#define GPIOE_BASE_ADDRESS								0x40011800UL

//----------------------------------
//Base Addresses For The APB1 Peripherals
//----------------------------------


//=======================================================

//----------------------------------
//Peripheral Register
//----------------------------------

//GPIO Peripheral Register
typedef struct{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
}GPIO_TypeDef;


//RCC Peripheral Register
typedef struct{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;

}RCC_TypeDef;

//EXTI Peripheral Register
typedef struct{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_TypeDef;

//AFIO Peripheral Register
typedef struct{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	volatile		 uint32_t RESERVED0;
	volatile uint32_t MAPR2;

}AFIO_TypeDef;


//=======================================================

//Peripheral Instant
#define GPIOA					((GPIO_TypeDef *)GPIOA_BASE_ADDRESS)
#define GPIOB					((GPIO_TypeDef *)GPIOB_BASE_ADDRESS)
#define GPIOC					((GPIO_TypeDef *)GPIOC_BASE_ADDRESS)
#define GPIOD					((GPIO_TypeDef *)GPIOD_BASE_ADDRESS)
#define GPIOE					((GPIO_TypeDef *)GPIOE_BASE_ADDRESS)

#define RCC						((RCC_TypeDef  *)RCC_BASE_ADDRESS)

#define EXTI					((EXTI_TypeDef *)EXTI_BASE_ADDRESS)

#define AFIO					((AFIO_TypeDef *)AFIO_BASE_ADDRESS)
//=======================================================

//Clock Enable Macros
#define RCC_GPIOA_CLK_EN()		(RCC->APB2ENR |= (1<<2))
#define RCC_GPIOB_CLK_EN()		(RCC->APB2ENR |= (1<<3))
#define RCC_GPIOC_CLK_EN()		(RCC->APB2ENR |= (1<<4))
#define RCC_GPIOD_CLK_EN()		(RCC->APB2ENR |= (1<<5))
#define RCC_GPIOE_CLK_EN()		(RCC->APB2ENR |= (1<<6))

#define RCC_AFIO_CLK_EN()		(RCC->APB2ENR |= (1<<0))


#endif /* INCLUDE_STM32F103X6_H_ */
