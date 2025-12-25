/*
 * stm32f103c6_EXTI_Driver.c
 *
 *  Created on: Dec 25, 2025
 *      Author: Abdalrahman
 */


#include "stm32f103c6_EXTI_Driver.h"


//----------------------------------
//			GLOBAL VARIABLES
//----------------------------------

void(* GP_IRQ_CallBack[16])(void);

//----------------------------------
//Helper MACROS
//----------------------------------
#define AFIO_GPIO_EXTI_MAPPING(x)	(	(x==GPIOA)?0:\
										(x==GPIOB)?1:\
										(x==GPIOC)?2:\
										(x==GPIOD)?3:0 )

//----------------------------------
//Helper Functions
//----------------------------------

void NVIC_EXTI_Enable(uint8_t EXTI_InputLineNumber)
{
    switch (EXTI_InputLineNumber)
    {
        case 0:  NVIC_IRQ_EXTI0_ENABLE;  break;
        case 1:  NVIC_IRQ_EXTI1_ENABLE;  break;
        case 2:  NVIC_IRQ_EXTI2_ENABLE;  break;
        case 3:  NVIC_IRQ_EXTI3_ENABLE;  break;
        case 4:  NVIC_IRQ_EXTI4_ENABLE;  break;

        case 5:  NVIC_IRQ_EXTI5_ENABLE;  break;
        case 6:  NVIC_IRQ_EXTI6_ENABLE;  break;
        case 7:  NVIC_IRQ_EXTI7_ENABLE;  break;
        case 8:  NVIC_IRQ_EXTI8_ENABLE;  break;
        case 9:  NVIC_IRQ_EXTI9_ENABLE;  break;

        case 10: NVIC_IRQ_EXTI10_ENABLE; break;
        case 11: NVIC_IRQ_EXTI11_ENABLE; break;
        case 12: NVIC_IRQ_EXTI12_ENABLE; break;
        case 13: NVIC_IRQ_EXTI13_ENABLE; break;
        case 14: NVIC_IRQ_EXTI14_ENABLE; break;
        case 15: NVIC_IRQ_EXTI15_ENABLE; break;

        default:
            /* invalid EXTI line */
            break;
    }
}

void NVIC_EXTI_Disable(uint8_t EXTI_InputLineNumber)
{
    switch (EXTI_InputLineNumber)
    {
        case 0:  NVIC_IRQ_EXTI0_DISABLE;  break;
        case 1:  NVIC_IRQ_EXTI1_DISABLE;  break;
        case 2:  NVIC_IRQ_EXTI2_DISABLE;  break;
        case 3:  NVIC_IRQ_EXTI3_DISABLE;  break;
        case 4:  NVIC_IRQ_EXTI4_DISABLE;  break;

        case 5:  NVIC_IRQ_EXTI5_DISABLE;  break;
        case 6:  NVIC_IRQ_EXTI6_DISABLE;  break;
        case 7:  NVIC_IRQ_EXTI7_DISABLE;  break;
        case 8:  NVIC_IRQ_EXTI8_DISABLE;  break;
        case 9:  NVIC_IRQ_EXTI9_DISABLE;  break;

        case 10: NVIC_IRQ_EXTI10_DISABLE; break;
        case 11: NVIC_IRQ_EXTI11_DISABLE; break;
        case 12: NVIC_IRQ_EXTI12_DISABLE; break;
        case 13: NVIC_IRQ_EXTI13_DISABLE; break;
        case 14: NVIC_IRQ_EXTI14_DISABLE; break;
        case 15: NVIC_IRQ_EXTI15_DISABLE; break;

        default:
            /* invalid EXTI line */
            break;
    }
}



void Update_EXTI(EXTI_PinConfig_t *EXTI_Config)
{
	//Configure GPIO to be AFIO ->
	/*For alternate function inputs, the port must be configured in Input mode (floating, pull
	  up or pull-down) and the input pin must be driven externally*/
	GPIO_PinConfig_t PinCfg;
	PinCfg.GPIO_PinNumber = EXTI_Config->EXTI_PIN.GPIO_PIN;
	PinCfg.GPIO_Mode = GPIO_MODE_FLOATING_INPUT;
	MCAL_GPIO_Init((EXTI_Config->EXTI_PIN.GPIOx), &PinCfg);

	// Configure the mask bits of the 20 Interrupt lines
	uint8_t AFIO_EXTICR_Index = (uint8_t)((EXTI_Config->EXTI_PIN.EXTI_InputLineNumber)/4);
	uint8_t AFIO_EXTICR_BitPosition = ((uint8_t)((EXTI_Config->EXTI_PIN.EXTI_InputLineNumber)%4))*4 ;

	//clear 4bits to be set
	AFIO->EXTICR[AFIO_EXTICR_Index] &= ~(0xF<<AFIO_EXTICR_BitPosition);
	AFIO->EXTICR[AFIO_EXTICR_Index] |= (AFIO_GPIO_EXTI_MAPPING(EXTI_Config->EXTI_PIN.GPIOx)&0xF)<<AFIO_EXTICR_BitPosition;


	//Update Rising and falling edge
	EXTI->RTSR &= ~(1<<(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber));
	EXTI->FTSR &= ~(1<<(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber));

	if(EXTI_Config->Trigger_Case == EXTI_TRIGGER_RISING){
		EXTI->RTSR |= (1<<(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber));
	}
	else if(EXTI_Config->Trigger_Case == EXTI_TRIGGER_FALLING){
		EXTI->FTSR |= (1<<(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber));
	}
	else if(EXTI_Config->Trigger_Case == EXTI_TRIGGER_RISING_FALLING){
		EXTI->RTSR |= (1<<(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber));
		EXTI->FTSR |= (1<<(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber));
	}

	//update IRQ Handling Callback
	GP_IRQ_CallBack[EXTI_Config->EXTI_PIN.EXTI_InputLineNumber] = EXTI_Config->P_IRQ_CallBack;


	//Enable Or Disable IRQ  and NVIC
	if(EXTI_Config->IQR_EN == EXTI_IRQ_ENABLE ){
		EXTI->IMR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
		NVIC_EXTI_Enable(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
	}
	else if(EXTI_Config->IQR_EN == EXTI_IRQ_DISABLE ){
		EXTI->IMR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
		NVIC_EXTI_Disable(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
	}




}

//----------------------------------
//APIs Implementation
//----------------------------------


void MCAL_EXTI_GPIO_Init(EXTI_PinConfig_t *EXTI_Config){
	Update_EXTI(EXTI_Config);
}

void MCAL_EXTI_GPIO_DeInit(void){
	EXTI->IMR  = (uint32_t)0x0;
	EXTI->EMR  = (uint32_t)0x0;
	EXTI->RTSR = (uint32_t)0x0;
	EXTI->FTSR = (uint32_t)0x0;
	EXTI->SWIER= (uint32_t)0x0;
	EXTI->PR   = 0xFFFFFFFF;

	//Disable EXTI IRQ from NVIC
    /* EXTI0 → EXTI4 */
    NVIC_IRQ_EXTI0_DISABLE;
    NVIC_IRQ_EXTI1_DISABLE;
    NVIC_IRQ_EXTI2_DISABLE;
    NVIC_IRQ_EXTI3_DISABLE;
    NVIC_IRQ_EXTI4_DISABLE;

    /* EXTI5 → EXTI9 */
    NVIC_IRQ_EXTI5_DISABLE;
    NVIC_IRQ_EXTI6_DISABLE;
    NVIC_IRQ_EXTI7_DISABLE;
    NVIC_IRQ_EXTI8_DISABLE;
    NVIC_IRQ_EXTI9_DISABLE;

    /* EXTI10 → EXTI15 */
    NVIC_IRQ_EXTI10_DISABLE;
    NVIC_IRQ_EXTI11_DISABLE;
    NVIC_IRQ_EXTI12_DISABLE;
    NVIC_IRQ_EXTI13_DISABLE;
    NVIC_IRQ_EXTI14_DISABLE;
    NVIC_IRQ_EXTI15_DISABLE;
}

void MCAL_EXTI_GPIO_Update(EXTI_PinConfig_t *EXTI_Config){
	Update_EXTI(EXTI_Config);
}


//--------------------------------------------------------------------
//--------------------------------------------------------------------
//							ISR FUNCTIONS
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//  .word	EXTI0_IRQHandler          			/* EXTI Line0 interrupt                             */
//	.word	EXTI1_IRQHandler          			/* EXTI Line1 interrupt                             */
//	.word	EXTI2_IRQHandler          			/* EXTI Line2 interrupt                             */
//	.word	EXTI3_IRQHandler          			/* EXTI Line3 interrupt                             */
//	.word	EXTI4_IRQHandler          			/* EXTI Line4 interrupt
//  .word	EXTI9_5_IRQHandler        			/* EXTI Line[9:5] interrupts
//  .word	EXTI15_10_IRQHandler      			/* EXTI Line[15:10] interrupts

void EXTI0_IRQHandler(void){
	//Clear Pending register (EXTI_PR)
	EXTI->PR |= (1<<0);

	//CALL IRQ
	if(GP_IRQ_CallBack[0]){
		GP_IRQ_CallBack[0]();
	}
}

void EXTI1_IRQHandler(void){
	EXTI->PR |= (1<<1);
	GP_IRQ_CallBack[1]();
}

void EXTI2_IRQHandler(void){
	EXTI->PR |= (1<<2);
	GP_IRQ_CallBack[2]();
}

void EXTI3_IRQHandler(void){
	EXTI->PR |= (1<<3);
	GP_IRQ_CallBack[3]();
}

void EXTI4_IRQHandler(void){
	EXTI->PR |= (1<<4);
	GP_IRQ_CallBack[4]();
}

void EXTI9_5_IRQHandler(void){
	if(EXTI->PR & (1<<5)){EXTI->PR |= (1<<5); GP_IRQ_CallBack[5](); }
	if(EXTI->PR & (1<<6)){EXTI->PR |= (1<<6); GP_IRQ_CallBack[6](); }
	if(EXTI->PR & (1<<7)){EXTI->PR |= (1<<7); GP_IRQ_CallBack[7](); }
	if(EXTI->PR & (1<<8)){EXTI->PR |= (1<<8); GP_IRQ_CallBack[8](); }
	if(EXTI->PR & (1<<9)){EXTI->PR |= (1<<9); GP_IRQ_CallBack[9](); }
}

void EXTI15_10_IRQHandler(void){
	if(EXTI->PR & (1<<10)){EXTI->PR |= (1<<10); GP_IRQ_CallBack[10](); }
	if(EXTI->PR & (1<<11)){EXTI->PR |= (1<<11); GP_IRQ_CallBack[11](); }
	if(EXTI->PR & (1<<12)){EXTI->PR |= (1<<12); GP_IRQ_CallBack[12](); }
	if(EXTI->PR & (1<<13)){EXTI->PR |= (1<<13); GP_IRQ_CallBack[13](); }
	if(EXTI->PR & (1<<14)){EXTI->PR |= (1<<14); GP_IRQ_CallBack[14](); }
	if(EXTI->PR & (1<<15)){EXTI->PR |= (1<<15); GP_IRQ_CallBack[15](); }
}




