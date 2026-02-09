/*
 * stm32f103c6_RCC_Driver.c
 *
 *  Created on: Jan 30, 2026
 *      Author: Abdalrahman
 */



#include "stm32f103c6_RCC_Driver.h"


/*	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16*/

const uint32_t APBPrescalerTable[8U] = {0,0,0,0,1,2,3,4};

/*	0xxx: SYSCLK not divided
	1000: SYSCLK divided by 2
	1001: SYSCLK divided by 4
	1010: SYSCLK divided by 8
	1011: SYSCLK divided by 16
	1100: SYSCLK divided by 64
	1101: SYSCLK divided by 128
	1110: SYSCLK divided by 256
	1111: SYSCLK divided by 512*/
const uint32_t AHBPrescalerTable[16U] = {0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9};

uint32_t MCAL_RCC_GetSYSCLK_Freq(void){
/*
SW: System clock switch
Set and cleared by software to select SYSCLK source.
Set by hardware to force HSI selection when leaving Stop and Standby mode or in case of
failure of the HSE oscillator used directly or indirectly as system clock (if the Clock Security
System is enabled).
00: HSI selected as system clock
01: HSE selected as system clock
10: PLL selected as system clock
11: not allowed*/

/* SWS: System clock switch status
Set and cleared by hardware to indicate which clock source is used as system clock.
00: HSI oscillator used as system clock
01: HSE oscillator used as system clock
10: PLL used as system clock
11: not applicable*/
	switch((RCC->CFGR>>2)&0b11){
		case 0 :
			return HSI_RC_CLK;
			break;
		case 1 :
			return HSE_CLK;
			break;
		case 2 :
			return 16000000;
			break;
	}
}

uint32_t MCAL_RCC_GetHCLK_Freq(void){
/*Bits 7:4
HPRE: AHB prescaler
Set and cleared by software to control the division factor of the AHB clock.
0xxx: SYSCLK not divided
1000: SYSCLK divided by 2
1001: SYSCLK divided by 4
1010: SYSCLK divided by 8
1011: SYSCLK divided by 16
1100: SYSCLK divided by 64
1101: SYSCLK divided by 128
1110: SYSCLK divided by 256
1111: SYSCLK divided by 512*/
	return (MCAL_RCC_GetSYSCLK_Freq()>>AHBPrescalerTable[(RCC->CFGR>>4)&0xF]);

}

uint32_t MCAL_RCC_GetPCLK1_Freq(void){
	/* PPRE1: APB low-speed prescaler (APB1)
	Set and cleared by software to control the division factor of the APB low-speed clock
	(PCLK1).
	Warning: the software has to set correctly these bits to not exceed 36 MHz on this domain.
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16*/

	return (MCAL_RCC_GetHCLK_Freq()>>APBPrescalerTable[(RCC->CFGR>>8)&0b111]);
}

uint32_t MCAL_RCC_GetPCLK2_Freq(void){
	/* PPRE2: APB high-speed prescaler (APB2)
	Set and cleared by software to control the division factor of the APB high-speed clock
	(PCLK2).
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16*/
	return (MCAL_RCC_GetHCLK_Freq()>>APBPrescalerTable[(RCC->CFGR>>11)&0b111]);
}
