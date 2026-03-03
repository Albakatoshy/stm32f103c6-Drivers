#define MCU_ACT_AS_MASTER
// #define MCU_ACT_AS_SLAVE

#include <stdint.h>

#include "stm32f103c6_GPIO_Driver.h"
#include "stm32f103c6_EXTI_Driver.h"
#include "stm32f103c6_RCC_Driver.h"
#include "stm32f103c6_USART_Driver.h"
#include "stm32f103c6_SPI_Driver.h"

static volatile uint8_t ch;

static void MyWait(uint32_t x)
{
    for (uint32_t i = 0; i < x; i++)
        for (uint32_t l = 0; l < x; l++);
}

void CALL_BACK(void)
{
#ifdef MCU_ACT_AS_MASTER
    /* Read 1 byte from UART then echo it back */
    MCAL_USART_ReceiveData(USART1, (uint16_t *)&ch, Disable);
    MCAL_USART_TransmitData(USART1, (uint16_t *)&ch, Enable);

    /* Send the same byte over SPI (SPI API uses uint16_t*) */
    uint16_t spi_data = (uint16_t)ch;

    /* Manual CS (PA4) low -> transfer -> high */
    MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    MCAL_SPI_TX_RX(SPI1, &spi_data, PollingEnable);
    MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);

    /* If you want to keep received SPI byte */
    ch = (uint8_t)spi_data;
#endif
}

int main(void)
{
    /* ================= USART1 init ================= */
    USART_Config Usart_cfg;
    Usart_cfg.BaudRate        = USART_BaudRate_115200;
    Usart_cfg.HwFlowControl   = USART_No_HwFlowControl;
    Usart_cfg.Enable_IQR      = USART_Enable_IQR_RXNE;
    Usart_cfg.P_IRQ_CallBack  = CALL_BACK;
    Usart_cfg.Parity          = USART_NoParity;
    Usart_cfg.PayLoad_Length  = USART_PayLoad_Length_8DBits;
    Usart_cfg.StopBits        = USART_StopBits_1;
    Usart_cfg.USART_Mode      = USART_MODE_TX_RX;

    MCAL_USART_Init(USART1, &Usart_cfg);

    /* ================= SPI1 init ================= */
    SPI_Config spi_cfg;

    spi_cfg.CLKPhase              = SPI_CLK_PHASE_2nd_FirstDataCaptureEdge;
    spi_cfg.CLKPolarity           = SPI_CLK_POLARITY_IDLE_HIGH;
    spi_cfg.DataSize              = SPI_DATA_SIZE_8BIT;
    spi_cfg.FrameFormat           = SPI_FRAME_FORMAT_MSB_FIRST;
    spi_cfg.SpiBaudRatePreScaler  = SPI_BAUDRATE_PRESCALER_8;   // assume PCLK2 = 8 MHz
    spi_cfg.CommunicationMode     = SPI_2LINE_UNIDRECTIONAL;

#ifdef MCU_ACT_AS_MASTER
    spi_cfg.DeviceMode     = SPI_DEVICE_MODE_MASTER;
    spi_cfg.IRQ_ENABLE     = SPI_IRQ_NONE;
    spi_cfg.NSS            = SPI_NSS_SW_NSS_SSI_SET;  // software NSS management (CR1)
    spi_cfg.P_IRQ_CallBack = NULL;
#endif

    MCAL_SPI_Init(SPI1, &spi_cfg);
    MCAL_SPI_GPIO_SetPins(SPI1);

    /* ========== Manual CS on PA4 (software NSS) ========== */
    GPIO_PinConfig_t pinCfg;
    pinCfg.GPIO_PinNumber      = GPIO_PIN_4;
    pinCfg.GPIO_Mode           = GPIO_MODE_OUTPUT_PUSH_PULL;     // IMPORTANT: not AF
    pinCfg.GPIO_Output_Speed   = GPIO_OUTPUT_SPEED_10MHz;
    MCAL_GPIO_Init(GPIOA, &pinCfg);

    MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);  // CS idle high

    while (1)
    {
        /* main loop idle */
    }
}
