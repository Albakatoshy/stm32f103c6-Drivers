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



//					UART1
#define USART1_BASE_ADDRESS								0x40013800UL


//					SPI1
#define SPI1_BASE_ADDRESS								0x40013000UL


#define I2C1_BASE_ADDRESS								0x40005400UL
#define I2C2_BASE_ADDRESS								0x40005800UL


//----------------------------------
//Base Addresses For The APB1 Peripherals
//----------------------------------

//					UART2
#define USART2_BASE_ADDRESS								0x40004400UL


//					UART3
#define USART3_BASE_ADDRESS								0x40004800UL



//					SPI2
#define SPI2_BASE_ADDRESS								0x40003800UL





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


typedef struct{
	volatile uint32_t USART_SR;
	volatile uint32_t USART_DR;
	volatile uint32_t USART_BRR;
	volatile uint32_t USART_CR1;
	volatile uint32_t USART_CR2;
	volatile uint32_t USART_CR3;
	volatile uint32_t USART_GTPR;
}USART_TypeDef;

typedef struct{
	volatile uint32_t SPI_CR1;
	volatile uint32_t SPI_CR2;
	volatile uint32_t SPI_SR;
	volatile uint32_t SPI_DR;
	volatile uint32_t SPI_CRCPR;
	volatile uint32_t SPI_RXCRCR;
	volatile uint32_t SPI_TXCRCR;
	volatile uint32_t SPI_I2SCFGR;
	volatile uint32_t SPI_I2SPR;
}SPI_TypeDef;


typedef struct{
	volatile uint32_t I2C_CR1;
	volatile uint32_t I2C_CR2;
	volatile uint32_t I2C_OAR1;
	volatile uint32_t I2C_OAR2;
	volatile uint32_t I2C_DR;
	volatile uint32_t I2C_SR1;
	volatile uint32_t I2C_SR2;
	volatile uint32_t I2C_CCR;
	volatile uint32_t I2C_TRISE;
}I2C_TypeDef;




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

#define USART1					((USART_TypeDef *)(USART1_BASE_ADDRESS))
#define USART2					((USART_TypeDef *)(USART2_BASE_ADDRESS))
#define USART3					((USART_TypeDef *)(USART3_BASE_ADDRESS))


//					SPI
#define SPI1					((SPI_TypeDef *)(SPI1_BASE_ADDRESS))
#define SPI2					((SPI_TypeDef *)(SPI2_BASE_ADDRESS))

#define I2C1					((SPI_TypeDef *)(I2C1_BASE_ADDRESS))
#define I2C2					((SPI_TypeDef *)(I2C2_BASE_ADDRESS))

//=======================================================

//Clock Enable Macros
#define RCC_GPIOA_CLK_EN()		(RCC->APB2ENR |= (1<<2))
#define RCC_GPIOB_CLK_EN()		(RCC->APB2ENR |= (1<<3))
#define RCC_GPIOC_CLK_EN()		(RCC->APB2ENR |= (1<<4))
#define RCC_GPIOD_CLK_EN()		(RCC->APB2ENR |= (1<<5))
#define RCC_GPIOE_CLK_EN()		(RCC->APB2ENR |= (1<<6))

#define RCC_AFIO_CLK_EN()		(RCC->APB2ENR |= (1<<0))

#define RCC_USART1_CLK_EN()		(RCC->APB2ENR |= (1<<14))
#define RCC_USART2_CLK_EN()		(RCC->APB1ENR |= (1<<17))
#define RCC_USART3_CLK_EN()		(RCC->APB1ENR |= (1<<18))

//Clock Reset Macros
#define RCC_USART1_CLK_RESET()	(RCC->APB2RSTR |= (1<<14))
#define RCC_USART2_CLK_RESET()	(RCC->APB1RSTR |= (1<<17))
#define RCC_USART3_CLK_RESET()	(RCC->APB1RSTR |= (1<<18))

// SPI CLOCK ENABLE
#define RCC_SPI1_CLK_EN()		(RCC->APB2ENR |=(1<<12))
#define RCC_SPI2_CLK_EN()		(RCC->APB1ENR |=(1<<14))

// SPI CLOCK RESET
#define RCC_SPI1_CLK_RESET()		(RCC->APB2RSTR |=(1<<12))
#define RCC_SPI2_CLK_RESET()		(RCC->APB1RSTR |=(1<<14))

// I2C CLOCK ENABLE
#define RCC_I2C1_CLK_EN()		(RCC->APB1ENR |= (1<<21))
#define RCC_I2C2_CLK_EN()		(RCC->APB1ENR |= (1<<22))
// I2C CLOCK RESET
#define RCC_I2C1_CLK_RESET()		(RCC->APB1RSTR |=(1<<21))
#define RCC_I2C2_CLK_RESET()		(RCC->APB1RSTR |=(1<<22))


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


//					USART
#define USART1_IRQ					37
#define USART2_IRQ					38
#define USART3_IRQ					39


//					SPI
#define SPI1_IRQ					35
#define SPI2_IRQ					36

// 					I2C
#define I2C1_EV_IRQ					31
#define I2C1_ER_IRQ					32
#define I2C2_EV_IRQ					33
#define I2C2_ER_IRQ					34


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




//USART
#define NVIC_IRQ37_USART1_ENABLE	 (NVIC_ISER1 |= (1U << (USART1_IRQ-32)))
#define NVIC_IRQ38_USART2_ENABLE	 (NVIC_ISER1 |= (1U << (USART2_IRQ-32)))
#define NVIC_IRQ39_USART3_ENABLE	 (NVIC_ISER1 |= (1U << (USART3_IRQ-32)))

#define NVIC_IRQ37_USART1_DISABLE	 (NVIC_ICER1 |= (1U << (USART1_IRQ-32)))
#define NVIC_IRQ38_USART2_DISABLE	 (NVIC_ICER1 |= (1U << (USART2_IRQ-32)))
#define NVIC_IRQ39_USART3_DISABLE	 (NVIC_ICER1 |= (1U << (USART3_IRQ-32)))

//SPI
#define NVIC_IRQ35_SPI1_ENABLE		(NVIC_ISER1 |= (1U << (SPI1_IRQ-32)))
#define NVIC_IRQ36_SPI2_ENABLE		(NVIC_ISER1 |= (1U << (SPI2_IRQ-32)))

#define NVIC_IRQ35_SPI1_DISABLE	 	(NVIC_ICER1 |= (1U << (SPI1_IRQ-32)))
#define NVIC_IRQ36_SPI2_DISABLE	 	(NVIC_ICER1 |= (1U << (SPI2_IRQ-32)))

//I2C
#define NVIC_IRQ31_I2C1_EV_ENABLE	(NVIC_ISER0 |= (1U << (I2C1_EV_IRQ	 )))
#define NVIC_IRQ32_I2C1_ER_ENABLE	(NVIC_ISER1 |= (1U << (I2C1_ER_IRQ-32)))
#define NVIC_IRQ33_I2C2_EV_ENABLE	(NVIC_ISER1 |= (1U << (I2C2_EV_IRQ-32)))
#define NVIC_IRQ34_I2C2_ER_ENABLE	(NVIC_ISER1 |= (1U << (I2C2_ER_IRQ-32)))

#define NVIC_IRQ31_I2C1_EV_DISABLE	 (NVIC_ICER0 |= (1U << (I2C1_EV_IRQ   )))
#define NVIC_IRQ32_I2C1_ER_DISABLE	 (NVIC_ICER1 |= (1U << (I2C1_ER_IRQ-32)))
#define NVIC_IRQ33_I2C2_EV_DISABLE	 (NVIC_ICER1 |= (1U << (I2C2_EV_IRQ-32)))
#define NVIC_IRQ34_I2C2_ER_DISABLE	 (NVIC_ICER1 |= (1U << (I2C2_ER_IRQ-32)))


/********************************************************/
/********************************************************/
/********************************************************/
/*******************  Bit definition  ********************/
/********************************************************/
/********************************************************/



/*******************  Bit definition for I2C_CR1 register  ********************/
#define I2C_CR1_PE_Pos                      (0U)
#define I2C_CR1_PE_Msk                      (0x1UL << I2C_CR1_PE_Pos)           /*!< 0x00000001 */
#define I2C_CR1_PE                          I2C_CR1_PE_Msk                     /*!< Peripheral Enable */
#define I2C_CR1_SMBUS_Pos                   (1U)
#define I2C_CR1_SMBUS_Msk                   (0x1UL << I2C_CR1_SMBUS_Pos)        /*!< 0x00000002 */
#define I2C_CR1_SMBUS                       I2C_CR1_SMBUS_Msk                  /*!< SMBus Mode */
#define I2C_CR1_SMBTYPE_Pos                 (3U)
#define I2C_CR1_SMBTYPE_Msk                 (0x1UL << I2C_CR1_SMBTYPE_Pos)      /*!< 0x00000008 */
#define I2C_CR1_SMBTYPE                     I2C_CR1_SMBTYPE_Msk                /*!< SMBus Type */
#define I2C_CR1_ENARP_Pos                   (4U)
#define I2C_CR1_ENARP_Msk                   (0x1UL << I2C_CR1_ENARP_Pos)        /*!< 0x00000010 */
#define I2C_CR1_ENARP                       I2C_CR1_ENARP_Msk                  /*!< ARP Enable */
#define I2C_CR1_ENPEC_Pos                   (5U)
#define I2C_CR1_ENPEC_Msk                   (0x1UL << I2C_CR1_ENPEC_Pos)        /*!< 0x00000020 */
#define I2C_CR1_ENPEC                       I2C_CR1_ENPEC_Msk                  /*!< PEC Enable */
#define I2C_CR1_ENGC_Pos                    (6U)
#define I2C_CR1_ENGC_Msk                    (0x1UL << I2C_CR1_ENGC_Pos)         /*!< 0x00000040 */
#define I2C_CR1_ENGC                        I2C_CR1_ENGC_Msk                   /*!< General Call Enable */
#define I2C_CR1_NOSTRETCH_Pos               (7U)
#define I2C_CR1_NOSTRETCH_Msk               (0x1UL << I2C_CR1_NOSTRETCH_Pos)    /*!< 0x00000080 */
#define I2C_CR1_NOSTRETCH                   I2C_CR1_NOSTRETCH_Msk              /*!< Clock Stretching Disable (Slave mode) */
#define I2C_CR1_START_Pos                   (8U)
#define I2C_CR1_START_Msk                   (0x1UL << I2C_CR1_START_Pos)        /*!< 0x00000100 */
#define I2C_CR1_START                       I2C_CR1_START_Msk                  /*!< Start Generation */
#define I2C_CR1_STOP_Pos                    (9U)
#define I2C_CR1_STOP_Msk                    (0x1UL << I2C_CR1_STOP_Pos)         /*!< 0x00000200 */
#define I2C_CR1_STOP                        I2C_CR1_STOP_Msk                   /*!< Stop Generation */
#define I2C_CR1_ACK_Pos                     (10U)
#define I2C_CR1_ACK_Msk                     (0x1UL << I2C_CR1_ACK_Pos)          /*!< 0x00000400 */
#define I2C_CR1_ACK                         I2C_CR1_ACK_Msk                    /*!< Acknowledge Enable */
#define I2C_CR1_POS_Pos                     (11U)
#define I2C_CR1_POS_Msk                     (0x1UL << I2C_CR1_POS_Pos)          /*!< 0x00000800 */
#define I2C_CR1_POS                         I2C_CR1_POS_Msk                    /*!< Acknowledge/PEC Position (for data reception) */
#define I2C_CR1_PEC_Pos                     (12U)
#define I2C_CR1_PEC_Msk                     (0x1UL << I2C_CR1_PEC_Pos)          /*!< 0x00001000 */
#define I2C_CR1_PEC                         I2C_CR1_PEC_Msk                    /*!< Packet Error Checking */
#define I2C_CR1_ALERT_Pos                   (13U)
#define I2C_CR1_ALERT_Msk                   (0x1UL << I2C_CR1_ALERT_Pos)        /*!< 0x00002000 */
#define I2C_CR1_ALERT                       I2C_CR1_ALERT_Msk                  /*!< SMBus Alert */
#define I2C_CR1_SWRST_Pos                   (15U)
#define I2C_CR1_SWRST_Msk                   (0x1UL << I2C_CR1_SWRST_Pos)        /*!< 0x00008000 */
#define I2C_CR1_SWRST                       I2C_CR1_SWRST_Msk                  /*!< Software Reset */
/*******************  Bit definition for I2C_CR2 register  ********************/
#define I2C_CR2_FREQ_Pos                    (0U)
#define I2C_CR2_FREQ_Msk                    (0x3FUL << I2C_CR2_FREQ_Pos)        /*!< 0x0000003F */
#define I2C_CR2_FREQ                        I2C_CR2_FREQ_Msk                   /*!< FREQ[5:0] bits (Peripheral Clock Frequency) */
#define I2C_CR2_ITERREN_Pos                 (8U)
#define I2C_CR2_ITERREN_Msk                 (0x1UL << I2C_CR2_ITERREN_Pos)      /*!< 0x00000100 */
#define I2C_CR2_ITERREN                     I2C_CR2_ITERREN_Msk                /*!< Error Interrupt Enable */
#define I2C_CR2_ITEVTEN_Pos                 (9U)
#define I2C_CR2_ITEVTEN_Msk                 (0x1UL << I2C_CR2_ITEVTEN_Pos)      /*!< 0x00000200 */
#define I2C_CR2_ITEVTEN                     I2C_CR2_ITEVTEN_Msk                /*!< Event Interrupt Enable */
#define I2C_CR2_ITBUFEN_Pos                 (10U)
#define I2C_CR2_ITBUFEN_Msk                 (0x1UL << I2C_CR2_ITBUFEN_Pos)      /*!< 0x00000400 */
#define I2C_CR2_ITBUFEN                     I2C_CR2_ITBUFEN_Msk                /*!< Buffer Interrupt Enable */
#define I2C_CR2_DMAEN_Pos                   (11U)
#define I2C_CR2_DMAEN_Msk                   (0x1UL << I2C_CR2_DMAEN_Pos)        /*!< 0x00000800 */
#define I2C_CR2_DMAEN                       I2C_CR2_DMAEN_Msk                  /*!< DMA Requests Enable */
#define I2C_CR2_LAST_Pos                    (12U)
#define I2C_CR2_LAST_Msk                    (0x1UL << I2C_CR2_LAST_Pos)         /*!< 0x00001000 */
#define I2C_CR2_LAST                        I2C_CR2_LAST_Msk                   /*!< DMA Last Transfer */
/*******************  Bit definition for I2C_OAR2 register  *******************/
#define I2C_OAR2_ENDUAL_Pos                 (0U)
#define I2C_OAR2_ENDUAL_Msk                 (0x1UL << I2C_OAR2_ENDUAL_Pos)      /*!< 0x00000001 */
#define I2C_OAR2_ENDUAL                     I2C_OAR2_ENDUAL_Msk                /*!< Dual addressing mode enable */
#define I2C_OAR2_ADD2_Pos                   (1U)
/*******************  Bit definition for I2C_SR1 register  ********************/
#define I2C_SR1_SB_Pos                      (0U)
#define I2C_SR1_SB_Msk                      (0x1UL << I2C_SR1_SB_Pos)           /*!< 0x00000001 */
#define I2C_SR1_SB                          I2C_SR1_SB_Msk                     /*!< Start Bit (Master mode) */
#define I2C_SR1_ADDR_Pos                    (1U)
#define I2C_SR1_ADDR_Msk                    (0x1UL << I2C_SR1_ADDR_Pos)         /*!< 0x00000002 */
#define I2C_SR1_ADDR                        I2C_SR1_ADDR_Msk                   /*!< Address sent (master mode)/matched (slave mode) */
#define I2C_SR1_BTF_Pos                     (2U)
#define I2C_SR1_BTF_Msk                     (0x1UL << I2C_SR1_BTF_Pos)          /*!< 0x00000004 */
#define I2C_SR1_BTF                         I2C_SR1_BTF_Msk                    /*!< Byte Transfer Finished */
#define I2C_SR1_ADD10_Pos                   (3U)
#define I2C_SR1_ADD10_Msk                   (0x1UL << I2C_SR1_ADD10_Pos)        /*!< 0x00000008 */
#define I2C_SR1_ADD10                       I2C_SR1_ADD10_Msk                  /*!< 10-bit header sent (Master mode) */
#define I2C_SR1_STOPF_Pos                   (4U)
#define I2C_SR1_STOPF_Msk                   (0x1UL << I2C_SR1_STOPF_Pos)        /*!< 0x00000010 */
#define I2C_SR1_STOPF                       I2C_SR1_STOPF_Msk                  /*!< Stop detection (Slave mode) */
#define I2C_SR1_RXNE_Pos                    (6U)
#define I2C_SR1_RXNE_Msk                    (0x1UL << I2C_SR1_RXNE_Pos)         /*!< 0x00000040 */
#define I2C_SR1_RXNE                        I2C_SR1_RXNE_Msk                   /*!< Data Register not Empty (receivers) */
#define I2C_SR1_TXE_Pos                     (7U)
#define I2C_SR1_TXE_Msk                     (0x1UL << I2C_SR1_TXE_Pos)          /*!< 0x00000080 */
#define I2C_SR1_TXE                         I2C_SR1_TXE_Msk                    /*!< Data Register Empty (transmitters) */
#define I2C_SR1_BERR_Pos                    (8U)
#define I2C_SR1_BERR_Msk                    (0x1UL << I2C_SR1_BERR_Pos)         /*!< 0x00000100 */
#define I2C_SR1_BERR                        I2C_SR1_BERR_Msk                   /*!< Bus Error */
#define I2C_SR1_ARLO_Pos                    (9U)
#define I2C_SR1_ARLO_Msk                    (0x1UL << I2C_SR1_ARLO_Pos)         /*!< 0x00000200 */
#define I2C_SR1_ARLO                        I2C_SR1_ARLO_Msk                   /*!< Arbitration Lost (master mode) */
#define I2C_SR1_AF_Pos                      (10U)
#define I2C_SR1_AF_Msk                      (0x1UL << I2C_SR1_AF_Pos)           /*!< 0x00000400 */
#define I2C_SR1_AF                          I2C_SR1_AF_Msk                     /*!< Acknowledge Failure */
#define I2C_SR1_OVR_Pos                     (11U)
#define I2C_SR1_OVR_Msk                     (0x1UL << I2C_SR1_OVR_Pos)          /*!< 0x00000800 */
#define I2C_SR1_OVR                         I2C_SR1_OVR_Msk                    /*!< Overrun/Underrun */
#define I2C_SR1_PECERR_Pos                  (12U)
#define I2C_SR1_PECERR_Msk                  (0x1UL << I2C_SR1_PECERR_Pos)       /*!< 0x00001000 */
#define I2C_SR1_PECERR                      I2C_SR1_PECERR_Msk                 /*!< PEC Error in reception */
#define I2C_SR1_TIMEOUT_Pos                 (14U)
#define I2C_SR1_TIMEOUT_Msk                 (0x1UL << I2C_SR1_TIMEOUT_Pos)      /*!< 0x00004000 */
#define I2C_SR1_TIMEOUT                     I2C_SR1_TIMEOUT_Msk                /*!< Timeout or Tlow Error */
#define I2C_SR1_SMBALERT_Pos                (15U)
#define I2C_SR1_SMBALERT_Msk                (0x1UL << I2C_SR1_SMBALERT_Pos)     /*!< 0x00008000 */
#define I2C_SR1_SMBALERT                    I2C_SR1_SMBALERT_Msk               /*!< SMBus Alert */

/*******************  Bit definition for I2C_SR2 register  ********************/
#define I2C_SR2_MSL_Pos                     (0U)
#define I2C_SR2_MSL_Msk                     (0x1UL << I2C_SR2_MSL_Pos)          /*!< 0x00000001 */
#define I2C_SR2_MSL                         I2C_SR2_MSL_Msk                    /*!< Master/Slave */
#define I2C_SR2_BUSY_Pos                    (1U)
#define I2C_SR2_BUSY_Msk                    (0x1UL << I2C_SR2_BUSY_Pos)         /*!< 0x00000002 */
#define I2C_SR2_BUSY                        I2C_SR2_BUSY_Msk                   /*!< Bus Busy */
#define I2C_SR2_TRA_Pos                     (2U)
#define I2C_SR2_TRA_Msk                     (0x1UL << I2C_SR2_TRA_Pos)          /*!< 0x00000004 */
#define I2C_SR2_TRA                         I2C_SR2_TRA_Msk                    /*!< Transmitter/Receiver */
#define I2C_SR2_GENCALL_Pos                 (4U)
#define I2C_SR2_GENCALL_Msk                 (0x1UL << I2C_SR2_GENCALL_Pos)      /*!< 0x00000010 */
#define I2C_SR2_GENCALL                     I2C_SR2_GENCALL_Msk                /*!< General Call Address (Slave mode) */
#define I2C_SR2_SMBDEFAULT_Pos              (5U)
#define I2C_SR2_SMBDEFAULT_Msk              (0x1UL << I2C_SR2_SMBDEFAULT_Pos)   /*!< 0x00000020 */
#define I2C_SR2_SMBDEFAULT                  I2C_SR2_SMBDEFAULT_Msk             /*!< SMBus Device Default Address (Slave mode) */
#define I2C_SR2_SMBHOST_Pos                 (6U)
#define I2C_SR2_SMBHOST_Msk                 (0x1UL << I2C_SR2_SMBHOST_Pos)      /*!< 0x00000040 */
#define I2C_SR2_SMBHOST                     I2C_SR2_SMBHOST_Msk                /*!< SMBus Host Header (Slave mode) */
#define I2C_SR2_DUALF_Pos                   (7U)
#define I2C_SR2_DUALF_Msk                   (0x1UL << I2C_SR2_DUALF_Pos)        /*!< 0x00000080 */
#define I2C_SR2_DUALF                       I2C_SR2_DUALF_Msk                  /*!< Dual Flag (Slave mode) */
#define I2C_SR2_PEC_Pos                     (8U)
#define I2C_SR2_PEC_Msk                     (0xFFUL << I2C_SR2_PEC_Pos)         /*!< 0x0000FF00 */
#define I2C_SR2_PEC                         I2C_SR2_PEC_Msk                    /*!< Packet Error Checking Register */



#endif /* INCLUDE_STM32F103X6_H_ */
