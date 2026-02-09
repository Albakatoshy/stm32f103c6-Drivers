/*
 * stm32f103c6_USART_Driver.c
 *
 * Notes:
 *  - Fixes the common bugs:
 *      1) DO NOT overwrite CR1 when enabling interrupts (use |=)
 *      2) Keep a config per USART instance (USART1/2/3) instead of one global pointer
 *      3) Configure GPIO pins inside Init()
 *  - This is a lightweight MCAL-style driver (polling + optional IRQ callback)
 */

#include "stm32f103c6_USART_Driver.h"

/* ============================================================
 *                 Generic (global) data
 * ============================================================ */

/* Keep a separate config pointer for each USART peripheral */
static USART_Config *g_USART_Config[3] = {0};

/* Map USART base address to index (0..2) */
static uint8_t USART_GetIndex(USART_TypeDef *USARTx)
{
	if (USARTx == USART1) return 0;
	if (USARTx == USART2) return 1;
	return 2; // USART3
}

/* ============================================================
 *                 Helper: GPIO pin configuration
 * ============================================================ */
void MCAL_USART_GPIO_SetPins(USART_TypeDef *USARTx)
{
	GPIO_PinConfig_t PinCfg;
	uint8_t idx = USART_GetIndex(USARTx);
	USART_Config *cfg = g_USART_Config[idx];

	/* If init not called yet, cfg may be NULL */
	if (!cfg) return;

	/* -------- USART1 default pins (no remap) --------
	 * TX  -> PA9  (AF Push-Pull)
	 * RX  -> PA10 (Input Pull-up)
	 * CTS -> PA11 (Input Pull-up)  optional
	 * RTS -> PA12 (AF Push-Pull)   optional
	 */
	if (USARTx == USART1)
	{
		// TX: PA9
		PinCfg.GPIO_PinNumber = GPIO_PIN_9;
		PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
		PinCfg.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		// RX: PA10
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_Mode = GPIO_MODE_INPUT_PU;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		// CTS: PA11 (optional)
		if (cfg->HwFlowControl == USART_HwFlowControl_CTS ||
			cfg->HwFlowControl == USART_HwFlowControl_CTS_RTS)
		{
			PinCfg.GPIO_PinNumber = GPIO_PIN_11;
			PinCfg.GPIO_Mode = GPIO_MODE_INPUT_PU;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}

		// RTS: PA12 (optional)
		if (cfg->HwFlowControl == USART_HwFlowControl_RTS ||
			cfg->HwFlowControl == USART_HwFlowControl_CTS_RTS)
		{
			PinCfg.GPIO_PinNumber = GPIO_PIN_12;
			PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
	}

	/* -------- USART2 default pins (no remap) --------
	 * TX  -> PA2
	 * RX  -> PA3
	 * CTS -> PA0  optional
	 * RTS -> PA1  optional
	 */
	else if (USARTx == USART2)
	{
		// TX: PA2
		PinCfg.GPIO_PinNumber = GPIO_PIN_2;
		PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
		PinCfg.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		// RX: PA3
		PinCfg.GPIO_PinNumber = GPIO_PIN_3;
		PinCfg.GPIO_Mode = GPIO_MODE_INPUT_PU;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		// CTS: PA0 (optional)
		if (cfg->HwFlowControl == USART_HwFlowControl_CTS ||
			cfg->HwFlowControl == USART_HwFlowControl_CTS_RTS)
		{
			PinCfg.GPIO_PinNumber = GPIO_PIN_0;
			PinCfg.GPIO_Mode = GPIO_MODE_INPUT_PU;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}

		// RTS: PA1 (optional)
		if (cfg->HwFlowControl == USART_HwFlowControl_RTS ||
			cfg->HwFlowControl == USART_HwFlowControl_CTS_RTS)
		{
			PinCfg.GPIO_PinNumber = GPIO_PIN_1;
			PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
	}

	/* -------- USART3 default pins (no remap) --------
	 * TX  -> PB10
	 * RX  -> PB11
	 * CTS -> PB13 optional
	 * RTS -> PB14 optional
	 */
	else /* USART3 */
	{
		// TX: PB10
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
		PinCfg.GPIO_Output_Speed = GPIO_OUTPUT_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		// RX: PB11
		PinCfg.GPIO_PinNumber = GPIO_PIN_11;
		PinCfg.GPIO_Mode = GPIO_MODE_INPUT_PU;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		// CTS: PB13 (optional)
		if (cfg->HwFlowControl == USART_HwFlowControl_CTS ||
			cfg->HwFlowControl == USART_HwFlowControl_CTS_RTS)
		{
			PinCfg.GPIO_PinNumber = GPIO_PIN_13;
			PinCfg.GPIO_Mode = GPIO_MODE_INPUT_PU;
			MCAL_GPIO_Init(GPIOB, &PinCfg);
		}

		// RTS: PB14 (optional)
		if (cfg->HwFlowControl == USART_HwFlowControl_RTS ||
			cfg->HwFlowControl == USART_HwFlowControl_CTS_RTS)
		{
			PinCfg.GPIO_PinNumber = GPIO_PIN_14;
			PinCfg.GPIO_Mode = GPIO_MODE_AL_OUTPUT_PUSH_PULL;
			MCAL_GPIO_Init(GPIOB, &PinCfg);
		}
	}
}

/* ============================================================
 *                 Init / DeInit
 * ============================================================ */
void MCAL_USART_Init(USART_TypeDef *USARTx, USART_Config *USARTx_Config)
{
	uint32_t Pclk = 0;
	uint32_t BRR  = 0;

	/* Save config pointer (per USART instance) */
	uint8_t idx = USART_GetIndex(USARTx);
	g_USART_Config[idx] = USARTx_Config;

	/* 1) Enable peripheral clock */
	if (USARTx == USART1)      { RCC_USART1_CLK_EN(); }
	else if (USARTx == USART2) { RCC_USART2_CLK_EN(); }
	else                       { RCC_USART3_CLK_EN(); }

	/* 2) Make sure GPIO clocks are enabled (USART pins are on GPIOA/GPIOB) */
	/*    You can keep these always enabled, or enable conditionally. */
	RCC_GPIOA_CLK_EN();
	RCC_GPIOB_CLK_EN();
	RCC_AFIO_CLK_EN(); /* only needed if you later support remap */

	/* 3) Configure GPIO pins for selected USART + Flow Control */
	MCAL_USART_GPIO_SetPins(USARTx);

	/* 4) USART enable (UE bit) */
	USARTx->USART_CR1 |= (1U << 13);

	/* 5) Configure mode (TE/RE), word length, parity */
	USARTx->USART_CR1 |= USARTx_Config->USART_Mode;
	USARTx->USART_CR1 |= USARTx_Config->PayLoad_Length;
	USARTx->USART_CR1 |= USARTx_Config->Parity;

	/* 6) Stop bits */
	USARTx->USART_CR2 |= USARTx_Config->StopBits;

	/* 7) Hardware flow control (CTS/RTS) */
	USARTx->USART_CR3 |= USARTx_Config->HwFlowControl;

	/* 8) Select PCLK source:
	 *    USART1 on APB2 (PCLK2)
	 *    USART2/3 on APB1 (PCLK1)
	 */
	if (USARTx == USART1) { Pclk = MCAL_RCC_GetPCLK2_Freq(); }
	else                  { Pclk = MCAL_RCC_GetPCLK1_Freq(); }

	/* 9) Program baud rate register */
	BRR = MACRO_USART_BRR(Pclk, USARTx_Config->BaudRate);
	USARTx->USART_BRR = BRR;

	/* 10) Enable interrupts (do NOT overwrite CR1, use |=) */
	if (USARTx_Config->Enable_IQR != USART_Enable_IQR_None)
	{
		USARTx->USART_CR1 |= USARTx_Config->Enable_IQR;

		/* Enable NVIC for USART IRQ line */
		if (USARTx == USART1)      { NVIC_IRQ37_USART1_ENABLE; }
		else if (USARTx == USART2) { NVIC_IRQ38_USART2_ENABLE; }
		else                       { NVIC_IRQ39_USART3_ENABLE; }
	}
}

void MCAL_USART_DeInit(USART_TypeDef *USARTx)
{
	/* Disable NVIC + reset peripheral */
	if (USARTx == USART1)
	{
		RCC_USART1_CLK_RESET();
		NVIC_IRQ37_USART1_DISABLE;
		g_USART_Config[0] = 0;
	}
	else if (USARTx == USART2)
	{
		RCC_USART2_CLK_RESET();
		NVIC_IRQ38_USART2_DISABLE;
		g_USART_Config[1] = 0;
	}
	else
	{
		RCC_USART3_CLK_RESET();
		NVIC_IRQ39_USART3_DISABLE;
		g_USART_Config[2] = 0;
	}
}

/* ============================================================
 *                 Transmit / Receive (Polling)
 * ============================================================ */
void MCAL_USART_TransmitData(USART_TypeDef *USARTx, uint16_t *pTxBuffer, enum PollingMechanism PollEn)
{
	uint8_t idx = USART_GetIndex(USARTx);
	USART_Config *cfg = g_USART_Config[idx];
	if (!cfg) return;

	/* TXE (bit 7) = data register empty */
	if (PollEn == Enable)
	{
		while (!(USARTx->USART_SR & (1U << 7)));
	}

	/* Data size depends on word length + parity:
	 *  - 9-bit, no parity  -> 9 data bits
	 *  - 9-bit, parity     -> 8 data bits (parity uses the 9th)
	 *  - 8-bit, no parity  -> 8 data bits
	 *  - 8-bit, parity     -> 7 data bits (parity uses MSB)
	 */
	if (cfg->PayLoad_Length == USART_PayLoad_Length_9DBits)
	{
		if (cfg->Parity == USART_NoParity)
			USARTx->USART_DR = (*pTxBuffer & 0x01FF);
		else
			USARTx->USART_DR = (*pTxBuffer & 0x00FF);
	}
	else
	{
		if (cfg->Parity == USART_NoParity)
			USARTx->USART_DR = (*pTxBuffer & 0x00FF);
		else
			USARTx->USART_DR = (*pTxBuffer & 0x007F);
	}
}

void MCAL_USART_ReceiveData(USART_TypeDef *USARTx, uint16_t *pRxBuffer, enum PollingMechanism PollEn)
{
	uint8_t idx = USART_GetIndex(USARTx);
	USART_Config *cfg = g_USART_Config[idx];
	if (!cfg) return;

	/* RXNE (bit 5) = read data register not empty */
	if (PollEn == Enable)
	{
a		while (!(USARTx->USART_SR & (1U << 5)));
	}

	/* Read DR and mask according to payload/parity */
	if (cfg->PayLoad_Length == USART_PayLoad_Length_9DBits)
	{
		if (cfg->Parity == USART_NoParity)
			*pRxBuffer = (uint16_t)(USARTx->USART_DR & 0x01FF);
		else
			*pRxBuffer = (uint16_t)(USARTx->USART_DR & 0x00FF);
	}
	else
	{
		if (cfg->Parity == USART_NoParity)
			*pRxBuffer = (uint16_t)(USARTx->USART_DR & 0x00FF);
		else
			*pRxBuffer = (uint16_t)(USARTx->USART_DR & 0x007F);
	}
}

void MCAL_USART_Wait_TC(USART_TypeDef *USARTx)
{
	/* TC (bit 6) = transmission complete */
	while (!(USARTx->USART_SR & (1U << 6)));
}

/* ============================================================
 *                 IRQ Handlers (simple callback)
 * ============================================================ */
/* Tip:
 * - RXNE flag is cleared by reading DR
 * - Many error flags are cleared by reading SR then DR
 * Here we do minimal safe handling and then call user's callback.
 */

void USART1_IRQHandler(void)
{
	USART_Config *cfg = g_USART_Config[0];
	if (!cfg) return;

	/* minimal clear sequence (optional but safer) */
	volatile uint32_t sr = USART1->USART_SR;
	(void)sr;
	/* If RXNE set, reading DR clears it */
	if (USART1->USART_SR & (1U << 5))
	{
		volatile uint32_t dr = USART1->USART_DR;
		(void)dr;
	}

	if (cfg->P_IRQ_CallBack) cfg->P_IRQ_CallBack();
}

void USART2_IRQHandler(void)
{
	USART_Config *cfg = g_USART_Config[1];
	if (!cfg) return;

	volatile uint32_t sr = USART2->USART_SR;
	(void)sr;
	if (USART2->USART_SR & (1U << 5))
	{
		volatile uint32_t dr = USART2->USART_DR;
		(void)dr;
	}

	if (cfg->P_IRQ_CallBack) cfg->P_IRQ_CallBack();
}

void USART3_IRQHandler(void)
{
	USART_Config *cfg = g_USART_Config[2];
	if (!cfg) return;

	volatile uint32_t sr = USART3->USART_SR;
	(void)sr;
	if (USART3->USART_SR & (1U << 5))
	{
		volatile uint32_t dr = USART3->USART_DR;
		(void)dr;
	}

	if (cfg->P_IRQ_CallBack) cfg->P_IRQ_CallBack();
}
