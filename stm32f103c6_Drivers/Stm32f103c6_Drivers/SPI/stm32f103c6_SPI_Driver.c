/*
 * stm32f103c6_SPI_Driver.c
 *
 *  Created on: Feb 9, 2026
 *      Author: anaal
 */

#include "stm32f103c6_SPI_Driver.h"

SPI_Config *Global_SPI_Cfg[2] ={NULL,NULL};
SPI_Config Global_SPI_Cfg1;
SPI_Config Global_SPI_Cfg2;


#define SPI_SR_TXE					((uint8_t)0x02)
#define SPI_SR_RXNE					((uint8_t)0x01)

void MCAL_SPI_Init(SPI_TypeDef *SPIx , SPI_Config *SPI_Config){
	//Saftey For Registers
	uint16_t TempReg_Cr1 = 0;
	uint16_t TempReg_Cr2 = 0;


	if(SPIx == SPI1){
		Global_SPI_Cfg1 = *SPI_Config;
		Global_SPI_Cfg[0]=&Global_SPI_Cfg1;
		RCC_SPI1_CLK_EN();
	}
	else if(SPIx == SPI2){
		Global_SPI_Cfg2 = *SPI_Config;
		Global_SPI_Cfg[1]=&Global_SPI_Cfg2;
		RCC_SPI2_CLK_EN();
	}

	//ENABLE SPI
/*SPI control register 1 (SPI_CR1)
 * Bit 6 SPE: SPI enable
0: Peripheral disabled
1: Peripheral enabled*/
	TempReg_Cr1 = (0x1U<<6);
	//master or slave
	TempReg_Cr1 |= SPI_Config->DeviceMode;
	//Communication Mode
	TempReg_Cr1 |= SPI_Config->CommunicationMode;
	//SPI_FRAME_FORMAT
	TempReg_Cr1 |= SPI_Config->FrameFormat;
	//DATA_SIZE
	TempReg_Cr1 |= SPI_Config->DataSize;
	//CLK_POLARITY
	TempReg_Cr1 |= SPI_Config->CLKPolarity;
	//CLK_PHASE
	TempReg_Cr1 |= SPI_Config->CLKPhase;

	//			NSS
	if(SPI_Config->NSS == SPI_NSS_HW_MASTER_SS_OUTPUT_ENABLE){
		TempReg_Cr2 |= SPI_Config->NSS;
	}
	else if (SPI_Config->NSS == SPI_NSS_HW_MASTER_SS_OUTPUT_DISABLE) {
		TempReg_Cr2 &= SPI_Config->NSS;
	}
	else{
		TempReg_Cr1 |= SPI_Config->NSS;
	}

	//BAUDRATE_PRESCALER
	TempReg_Cr1 |= SPI_Config->SpiBaudRatePreScaler;

	if(SPI_Config->IRQ_ENABLE!=SPI_IRQ_NONE){
		TempReg_Cr2 |= SPI_Config->IRQ_ENABLE;
		if(SPIx == SPI1){
			NVIC_IRQ35_SPI1_ENABLE;
		}
		else if(SPIx == SPI2){
			NVIC_IRQ36_SPI2_ENABLE;
		}
	}

	SPIx->SPI_CR1 = TempReg_Cr1;
	SPIx->SPI_CR2 = TempReg_Cr2;

}

void MCAL_SPI_DeInit(SPI_TypeDef *SPIx , SPI_Config *SPI_Config){
	if(SPIx == SPI1){
		RCC_SPI1_CLK_RESET();
	}
	else if(SPIx == SPI2){
		RCC_SPI2_CLK_RESET();
	}
}



void MCAL_SPI_GPIO_SetPins(SPI_TypeDef *SPIx)
{
    GPIO_PinConfig_t pin = {0};

    if (SPIx == SPI1)
    {
        // SPI1: NSS PA4, SCK PA5, MISO PA6, MOSI PA7

        // ---------- SCK (PA5) ----------
        pin.GPIO_PinNumber = GPIO_PIN_5;
        if (Global_SPI_Cfg[0]->DeviceMode == SPI_DEVICE_MODE_MASTER)
            pin.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
        else
            pin.GPIO_Mode = GPIO_MODE_FLOATING_INPUT;
        MCAL_GPIO_Init(GPIOA, &pin);

        // ---------- MOSI (PA7) ----------
        pin.GPIO_PinNumber = GPIO_PIN_7;
        if (Global_SPI_Cfg[0]->DeviceMode == SPI_DEVICE_MODE_MASTER)
            pin.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
        else
            pin.GPIO_Mode = GPIO_MODE_FLOATING_INPUT;
        MCAL_GPIO_Init(GPIOA, &pin);

        // ---------- MISO (PA6) ----------
        pin.GPIO_PinNumber = GPIO_PIN_6;
        if (Global_SPI_Cfg[0]->DeviceMode == SPI_DEVICE_MODE_MASTER)
            pin.GPIO_Mode = GPIO_MODE_FLOATING_INPUT;
        else
            pin.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL; // slave drives MISO (point-to-point)
            // if multi-slave with shared MISO, you may use: GPIO_MODE_OUTPUT_AF_OPEN_DRAIN
        MCAL_GPIO_Init(GPIOA, &pin);

        // ---------- NSS (PA4) ----------
        pin.GPIO_PinNumber = GPIO_PIN_4;

        if (Global_SPI_Cfg[0]->DeviceMode == SPI_DEVICE_MODE_MASTER)
        {
            switch ((uint32_t)Global_SPI_Cfg[0]->NSS)
            {
            case SPI_NSS_HW_MASTER_SS_OUTPUT_DISABLE:
                // use as manual chip select (GPIO)
                pin.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
                break;

            case SPI_NSS_HW_MASTER_SS_OUTPUT_ENABLE:
                // hardware NSS output (SSOE=1)
                pin.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
                break;

            default:
                pin.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
                break;
            }
        }
        else
        {
            // slave: NSS is input (usually pull-up)
            pin.GPIO_Mode = GPIO_MODE_INPUT_PU;  // or GPIO_MODE_FLOATING_INPUT
        }

        MCAL_GPIO_Init(GPIOA, &pin);
    }
    else if (SPIx == SPI2)
    {
        // SPI2: NSS PB12, SCK PB13, MISO PB14, MOSI PB15

        // ---------- SCK (PB13) ----------
        pin.GPIO_PinNumber = GPIO_PIN_13;
        if (Global_SPI_Cfg[1]->DeviceMode == SPI_DEVICE_MODE_MASTER)
            pin.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
        else
            pin.GPIO_Mode = GPIO_MODE_FLOATING_INPUT;
        MCAL_GPIO_Init(GPIOB, &pin);

        // ---------- MOSI (PB15) ----------
        pin.GPIO_PinNumber = GPIO_PIN_15;
        if (Global_SPI_Cfg[1]->DeviceMode == SPI_DEVICE_MODE_MASTER)
            pin.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
        else
            pin.GPIO_Mode = GPIO_MODE_FLOATING_INPUT;
        MCAL_GPIO_Init(GPIOB, &pin);

        // ---------- MISO (PB14) ----------
        pin.GPIO_PinNumber = GPIO_PIN_14;
        if (Global_SPI_Cfg[1]->DeviceMode == SPI_DEVICE_MODE_MASTER)
            pin.GPIO_Mode = GPIO_MODE_FLOATING_INPUT;
        else
            pin.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
        MCAL_GPIO_Init(GPIOB, &pin);

        // ---------- NSS (PB12) ----------
        pin.GPIO_PinNumber = GPIO_PIN_12;

        if (Global_SPI_Cfg[1]->DeviceMode == SPI_DEVICE_MODE_MASTER)
        {
            switch ((uint32_t)Global_SPI_Cfg[1]->NSS)
            {
            case SPI_NSS_HW_MASTER_SS_OUTPUT_DISABLE:
                pin.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;     // manual CS
                break;

            case SPI_NSS_HW_MASTER_SS_OUTPUT_ENABLE:
                pin.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;  // HW NSS
                break;

            default:
                pin.GPIO_Mode = GPIO_MODE_OUTPUT_PUSH_PULL;
                break;
            }
        }
        else
        {
            pin.GPIO_Mode = GPIO_MODE_INPUT_PU; // or GPIO_MODE_FLOATING_INPUT
        }

        MCAL_GPIO_Init(GPIOB, &pin);
    }
}




void MCAL_SPI_SendData(SPI_TypeDef *SPIx , uint16_t *pTxBuffer , enum SpiPollingMechanism PollEn ){
	if(PollEn == PollingEnable){
		while(!((SPIx->SPI_SR) & SPI_SR_TXE));
	}

	SPIx->SPI_DR = *pTxBuffer;

}


void MCAL_SPI_ReceiveData(SPI_TypeDef *SPIx , uint16_t *pRxBuffer , enum SpiPollingMechanism PollEn ){
	if(PollEn == PollingEnable){
		while(!((SPIx->SPI_SR) & SPI_SR_RXNE));
	}

	*pRxBuffer = SPIx->SPI_DR;
}


void MCAL_SPI_TX_RX(SPI_TypeDef *SPIx , uint16_t *pTxBuffer , enum SpiPollingMechanism PollEn ){
	if(PollEn == PollingEnable){
		while(!((SPIx->SPI_SR) & SPI_SR_TXE));
	}
	SPIx->SPI_DR = *pTxBuffer;

	if(PollEn == PollingEnable){
		while(!((SPIx->SPI_SR) & SPI_SR_RXNE));
	}

	*pTxBuffer = SPIx->SPI_DR;

}


/*
 * ==============================================================================
 * 										IRQ
 * ==============================================================================
*/

void SPI1_IRQHandler(void)
{
    S_IRQ_SRC irq_src;

    irq_src.TXE  = (uint8_t)((SPI1->SPI_SR >> 1) & 1U);
    irq_src.RXE  = (uint8_t)((SPI1->SPI_SR >> 0) & 1U);
    irq_src.ERRI = (uint8_t)((SPI1->SPI_SR >> 4) & 1U);

    Global_SPI_Cfg[0]->P_IRQ_CallBack(irq_src);
}

void SPI2_IRQHandler(void)
{
    S_IRQ_SRC irq_src;

    irq_src.TXE  = (uint8_t)((SPI2->SPI_SR >> 1) & 1U);
    irq_src.RXE  = (uint8_t)((SPI2->SPI_SR >> 0) & 1U);
    irq_src.ERRI = (uint8_t)((SPI2->SPI_SR >> 4) & 1U);

    Global_SPI_Cfg[1]->P_IRQ_CallBack(irq_src);
}

