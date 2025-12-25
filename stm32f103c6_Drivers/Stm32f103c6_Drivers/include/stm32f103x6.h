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

#define NVIC_BASE_ADDRESS								0xE000E100UL
#define NVIC_ISER0							*(volatile uint32_t*)(NVIC_BASE_ADDRESS + 0x00)
#define NVIC_ISER1							*(volatile uint32_t*)(NVIC_BASE_ADDRESS + 0x04)
#define NVIC_ISER2							*(volatile uint32_t*)(NVIC_BASE_ADDRESS + 0x08)
#define NVIC_ICER0							*(volatile uint32_t*)(NVIC_BASE_ADDRESS + 0x80)
#define NVIC_ICER1							*(volatile uint32_t*)(NVIC_BASE_ADDRESS + 0x84)
#define NVIC_ICER2							*(volatile uint32_t*)(NVIC_BASE_ADDRESS + 0x88)

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
	volatile uint32_t EXTICR[4];
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


//=======================================================

//. Vector table for other STM32F10xxx devices
//					IVT


//EXTI
#define EXTI0_IRQ					6
#define EXTI1_IRQ					7
#define EXTI2_IRQ					8
#define EXTI3_IRQ					9
#define EXTI4_IRQ					10
#define EXTI5_IRQ					23
#define EXTI6_IRQ					23
#define EXTI7_IRQ					23
#define EXTI8_IRQ					23
#define EXTI9_IRQ					23
#define EXTI10_IRQ					40
#define EXTI11_IRQ					40
#define EXTI12_IRQ					40
#define EXTI13_IRQ					40
#define EXTI14_IRQ					40
#define EXTI15_IRQ					40


//=======================================================
//			NVIC IRQ Enable/Disable Macros
//=======================================================


//						NVIC IRQ Enable
//EXTI0 → EXTI9 (IRQ < 32 → ICER0)
#define NVIC_IRQ_EXTI0_ENABLE   (NVIC_ISER0 |= (1U << EXTI0_IRQ))
#define NVIC_IRQ_EXTI1_ENABLE   (NVIC_ISER0 |= (1U << EXTI1_IRQ))
#define NVIC_IRQ_EXTI2_ENABLE   (NVIC_ISER0 |= (1U << EXTI2_IRQ))
#define NVIC_IRQ_EXTI3_ENABLE   (NVIC_ISER0 |= (1U << EXTI3_IRQ))
#define NVIC_IRQ_EXTI4_ENABLE   (NVIC_ISER0 |= (1U << EXTI4_IRQ))

#define NVIC_IRQ_EXTI5_ENABLE   (NVIC_ISER0 |= (1U << EXTI5_IRQ))
#define NVIC_IRQ_EXTI6_ENABLE   (NVIC_ISER0 |= (1U << EXTI6_IRQ))
#define NVIC_IRQ_EXTI7_ENABLE   (NVIC_ISER0 |= (1U << EXTI7_IRQ))
#define NVIC_IRQ_EXTI8_ENABLE   (NVIC_ISER0 |= (1U << EXTI8_IRQ))
#define NVIC_IRQ_EXTI9_ENABLE   (NVIC_ISER0 |= (1U << EXTI9_IRQ))
//EXTI10 → EXTI15 (IRQ ≥ 32 → ICER1)
#define NVIC_IRQ_EXTI10_ENABLE  (NVIC_ISER1 |= (1U << (EXTI10_IRQ - 32)))
#define NVIC_IRQ_EXTI11_ENABLE  (NVIC_ISER1 |= (1U << (EXTI11_IRQ - 32)))
#define NVIC_IRQ_EXTI12_ENABLE  (NVIC_ISER1 |= (1U << (EXTI12_IRQ - 32)))
#define NVIC_IRQ_EXTI13_ENABLE  (NVIC_ISER1 |= (1U << (EXTI13_IRQ - 32)))
#define NVIC_IRQ_EXTI14_ENABLE  (NVIC_ISER1 |= (1U << (EXTI14_IRQ - 32)))
#define NVIC_IRQ_EXTI15_ENABLE  (NVIC_ISER1 |= (1U << (EXTI15_IRQ - 32)))


//							NVIC IRQ Disable
//EXTI0 → EXTI9 (IRQ < 32 → ICER0)
#define NVIC_IRQ_EXTI0_DISABLE   (NVIC_ICER0 |= (1U << EXTI0_IRQ))
#define NVIC_IRQ_EXTI1_DISABLE   (NVIC_ICER0 |= (1U << EXTI1_IRQ))
#define NVIC_IRQ_EXTI2_DISABLE   (NVIC_ICER0 |= (1U << EXTI2_IRQ))
#define NVIC_IRQ_EXTI3_DISABLE   (NVIC_ICER0 |= (1U << EXTI3_IRQ))
#define NVIC_IRQ_EXTI4_DISABLE   (NVIC_ICER0 |= (1U << EXTI4_IRQ))

#define NVIC_IRQ_EXTI5_DISABLE   (NVIC_ICER0 |= (1U << EXTI5_IRQ))
#define NVIC_IRQ_EXTI6_DISABLE   (NVIC_ICER0 |= (1U << EXTI6_IRQ))
#define NVIC_IRQ_EXTI7_DISABLE   (NVIC_ICER0 |= (1U << EXTI7_IRQ))
#define NVIC_IRQ_EXTI8_DISABLE   (NVIC_ICER0 |= (1U << EXTI8_IRQ))
#define NVIC_IRQ_EXTI9_DISABLE   (NVIC_ICER0 |= (1U << EXTI9_IRQ))

//EXTI10 → EXTI15 (IRQ ≥ 32 → ICER1)
#define NVIC_IRQ_EXTI10_DISABLE  (NVIC_ICER1 |= (1U << (EXTI10_IRQ - 32)))
#define NVIC_IRQ_EXTI11_DISABLE  (NVIC_ICER1 |= (1U << (EXTI11_IRQ - 32)))
#define NVIC_IRQ_EXTI12_DISABLE  (NVIC_ICER1 |= (1U << (EXTI12_IRQ - 32)))
#define NVIC_IRQ_EXTI13_DISABLE  (NVIC_ICER1 |= (1U << (EXTI13_IRQ - 32)))
#define NVIC_IRQ_EXTI14_DISABLE  (NVIC_ICER1 |= (1U << (EXTI14_IRQ - 32)))
#define NVIC_IRQ_EXTI15_DISABLE  (NVIC_ICER1 |= (1U << (EXTI15_IRQ - 32)))











#endif /* INCLUDE_STM32F103X6_H_ */
