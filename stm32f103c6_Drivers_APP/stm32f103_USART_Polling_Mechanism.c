#include "stm32f103c6_GPIO_Driver.h"
#include "stm32f103c6_EXTI_Driver.h"
#include "stm32f103c6_RCC_Driver.h"
#include "stm32f103c6_USART_Driver.h"
#include "lcd.h"



void MyWait(uint32_t x){
    for(uint32_t i = 0; i < x; i++){
        for(uint32_t l = 0; l < x; l++);
    }
}

unsigned char ch;


int main(void)
{
	USART_Config Usart_cfg;
	Usart_cfg.BaudRate = USART_BaudRate_115200;
	Usart_cfg.HwFlowControl = USART_No_HwFlowControl;
	Usart_cfg.Enable_IQR = USART_Enable_IQR_None;
	Usart_cfg.P_IRQ_CallBack = NULL;
	Usart_cfg.Parity = USART_NoParity;
	Usart_cfg.PayLoad_Length = USART_PayLoad_Length_8DBits;
	Usart_cfg.StopBits = USART_StopBits_1;
	Usart_cfg.USART_Mode=USART_MODE_TX_RX;

	MCAL_USART_Init(USART1, &Usart_cfg);

    while(1){
    	MCAL_USART_ReceiveData(USART1, &ch, Enable);
    	MCAL_USART_TransmitData(USART1, &ch, Enable);
    }
}
